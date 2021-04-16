#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wpointer-sign"   // FIXME?
#pragma GCC diagnostic ignored "-Wpragmas"        // to avoid "pragma pack has no effect"
#define UNREACHABLE   __builtin_unreachable();    // to avoid warning
#ifdef __AVR__ // To avoid the below kicking in when porting to PIC
// We are at the limit of RAM here, so implement a simple stack guard.
extern void *_end; // End of data area, for stack guard
#define SET_STACK_GUARD()   (*(uint32_t*)(&_end)) = 0xfadebabe  // set a guard value at end of data (bss)
#define CHECK_STACK_GUARD() (*(uint32_t*)(&_end) != 0xfadebabe) // check if the guard is overwritten
#else  // __AVR__
#define SET_STACK_GUARD()
#define CHECK_STACK_GUARD() false                 // should optimize out conditionals
#endif
#else  // __GNUC__
#define UNREACHABLE   ;                           // howto in IAR?
#define SET_STACK_GUARD()
#define CHECK_STACK_GUARD() false                 // should optimize out conditionals
#endif

#define AVR_LIKE_ECC



#include <atmel_start.h>

#include "winc1500/driver/include/m2m_wifi.h"
#include "winc1500/spi_flash/include/spi_flash.h"
#include "winc1500/spi_flash/include/spi_flash_map.h"
#include "winc1500/programmer/programmer.h"
#include "winc_certs_functions.h"
#include "cert_def_3_device_csr.h"
#include "cert_def_2_device.h"
#include "cert_def_4_device.h"
#include "cert_def_4_signer.h"
#include "winc_certs_functions.h"

#define MAX_COMMAND_SIZE 16
#define CSR_BUFFER_LENGTH 1500
#define THING_NAME_LENGTH 40

#define SIGNATURE_SIZE 64

#define COMMAND_EXECUTED "OK"  // empty positive response.
#define PREFIX_ERROR "ERROR:"    // prefix for error response

static inline void init_stack(uint8_t x) {
#if defined  __GNUC__ && defined __AVR__
    uint8_t *sp = (uint8_t*) (SPL | (SPH << 8));
    uint8_t *p = (uint8_t*) (&_end);
    while (p < sp - 2) *p++ = x;
#endif
}

/*
 * Return status type.
 * Either only one of atca or m2m members being used, the other being zero,
 * or it contains one of the codes defined below. (ATCA use uint8_t status
 * codes, M2M uses negative signed 8-bit error codes).
 */
typedef union {

    struct {
        uint8_t atca; // Return status from cryptoauthlib
        int8_t m2m; // Return status from winc/spi-flash driver
    };
    uint16_t status; // Accumulated status (0 == OK)
} status_t;

// Error codes not related to M2M or ATCA. See comment above.
#define ERR_OK      (uint16_t)0
#define ERR_UNKNOWN (uint16_t)0x7f7f  // Unknown command
#define ERR_NOTIMPL (uint16_t)0x7f7e  // Command not yet implemented
#define ERR_ARGS    (uint16_t)0x7f7d  // Wrong number of arguments
#define ERR_VALUE   (uint16_t)0x7f7c  // Invalid argument value
#define ERR_WDT     (uint16_t)0x7f7b  // Watchdog timeout (@@TODO: activate Fido)
#define ERR_STKOVF  (uint16_t)0x7f7a  // Stack overflow detected
#define ERR_TOOLONG (uint16_t)0x7f79  // Command too long for buffer

static void sleep_ms(volatile uint16_t millisec) {
    // depends on F_CPU being correctly set.
    while (millisec--) delay_ms(1);
}


// Set/unset WINC download mode

status_t winc_download_mode(bool set) {
    status_t result = {.status = ERR_OK};
    if (set)
    {
        nm_spi_deinit();
    
        result.m2m = m2m_wifi_download_mode();
    }
    else {
        tstrWifiInitParam wifi_paramaters;
        m2m_memset((uint8*) & wifi_paramaters, 0, sizeof (wifi_paramaters));
        result.m2m = m2m_wifi_init(&wifi_paramaters);
    }
    sleep_ms(250);
//    sleep_ms(2);
    return result;
}


// Format error message into buffer

static void fmt_error(uint8_t *command, status_t result) {
    uint8_t *p;
    memcpy(command, PREFIX_ERROR, sizeof (PREFIX_ERROR));
    p = command + sizeof (PREFIX_ERROR) - 1;
    p += convert_itohex(result.status, p);
}

// Transmit a character

void USART_0_write(void *str) {
    struct io_descriptor *io;
    usart_sync_get_io_descriptor(&USART_0, &io);
    usart_sync_enable(&USART_0);

    io_write(io, (uint8_t *) str, 1);
}
extern struct usart_sync_descriptor USART_0;

uint8_t USART_0_read(void) {
    struct io_descriptor *io;
    uint8_t value;

    usart_sync_get_io_descriptor(&USART_0, &io);
    usart_sync_enable(&USART_0);

    io_read(io, &value, 1);

    return value;

}

static inline void com_putchar(char c) {
    USART_0_write(&c);
}

// Tramsmit a string, including terminating \0 character.

static inline void com_puts(char *str) {
    do {
        USART_0_write(str);
    } while (*str++);
}


// Receive a character, blocking

static inline char com_getchar() {
    return (char) USART_0_read();
}


// Check if a character is available, non-blocking.

static inline bool com_havedata() {
    return USART_0_is_rx_ready();
}

// Read hex digits from host, convert to binary on the fly.
// Return: Length of binary data

static inline size_t com_read_hexbytes(uint8_t *loc) {
    char c;
    size_t len;
    for (len = 0; is_hexdigit(c = com_getchar()); len++) {
        loc[len] = hex2nibble(c) << 4;
        if (!is_hexdigit(c = com_getchar())) return len;
        loc[len] |= hex2nibble(c);
    }
    return len;
}

// Write hex data to host, convert on the fly from binary source loc.
// Return: Length of string written.

static inline size_t com_write_hexbytes(uint8_t *loc, size_t len) {
    for (int i = 0; i < len; i++) {
        com_putchar(nibble2hex(loc[i] >> 4));
        com_putchar(nibble2hex(loc[i] & 0xf));
    }
    com_putchar('\0');
    return 2 * len;
}



/*****************************************************************************
 * COMMAND INTERPRETER DEFINITIONS
 *****************************************************************************/

/* Command handler function type */
typedef status_t CmdHandler(uint8_t *command, uint8_t argc);


/*****************************************************************************
 * COMMAND IMPLEMENTATIONS
 *****************************************************************************/

/* Receive CA signer certificate from host. */
static status_t rcv_ca_cert(uint8_t *command, uint8_t argc) {
    status_t result = {.status = ERR_OK};
    signer_cert_size = com_read_hexbytes(signer_cert);
    if (signer_cert_size > 0)
        return result;
    else {
        result.status = ERR_VALUE;
        return result;
    }
}

/* Receive device certificate from host. */
static status_t rcv_dev_cert(uint8_t *command, uint8_t argc) {
    status_t result = {.status = ERR_OK};
    device_cert_size = com_read_hexbytes(device_cert);
    if (device_cert_size > 0)
        return result;
    else {
        result.status = ERR_VALUE;
        return result;
    }
}

/* Copy certificates from RAM buffer to WINC flash. */
static status_t copy_certs(uint8_t *command, uint8_t argc) {
    status_t result = {.status = ERR_OK};
    uint8_t arg[2];
    // We expect a single digit number identifying the device cert template to use.
    for (int i = 0; i < sizeof (arg); i++)
        arg[i] = com_getchar();
    switch (*arg) {
        case '2':
            //winc_download_mode(true);
            result.m2m = WINC_CERTS_transfer(&g_cert_def_1_signer, &g_cert_def_2_device);
            //winc_download_mode(false);
            break;
        case '4':
        #ifdef AVR_LIKE_ECC
            result.m2m = WINC_CERTS_transfer(&g_cert_def_1_signer, &g_cert_def_2_device);
        #else
            result.m2m = WINC_CERTS_transfer(&g_cert_def_4_signer, &g_cert_def_4_device);
        #endif
            
            break;
        default:
            result.status = ERR_VALUE;
            break;
    }
    sleep_ms(250); // allow things to settle
    return result;
}

/* Receive thing name (SKI from device cert) form host, save to WINC flash. */
static status_t rcv_thingname(uint8_t *command, uint8_t argc) {
    status_t result = {.status = ERR_OK};
    char thing_name[THING_NAME_LENGTH + 2];
    size_t length = 0;
    char c;

    do {
        c = com_getchar();
        thing_name[length++] = c;
    } while (c);

    if (length != THING_NAME_LENGTH + 1) {
        result.status = ERR_VALUE;
        return result;
    }
    thing_name[length++] = '\0'; // include \0 termimation in copy to WINC
    result = winc_download_mode(true);
    if (result.m2m != M2M_SUCCESS)
        return result;
    result.m2m = spi_flash_write(thing_name, THING_NAME_FLASH_OFFSET, length);
    if (result.m2m == M2M_SUCCESS)
        sleep_ms(250);
    result = winc_download_mode(false);
    return result;
}
/* Receive AWS IoT endPoint, save to WINC flash. */
static status_t rcv_endpoint(uint8_t *command, uint8_t argc) {
    status_t result = {.status = ERR_OK};
    char endPoint[80];
    size_t length = 0;
    char c;

    do {
        c = com_getchar();
        endPoint[length++] = c;
    } while (c);

    endPoint[length++] = '\0'; // include \0 termimation in copy to WINC
    result = winc_download_mode(true);
    if (result.m2m != M2M_SUCCESS)
        return result;
    result.m2m = spi_flash_write(endPoint, ENDPOINT_FLASH_OFFSET, length);
    if (result.m2m == M2M_SUCCESS)
        sleep_ms(250);
    result = winc_download_mode(false);
    return result;
}

/* Generate CSR and send it to host. */
static status_t send_csr(uint8_t *command, uint8_t argc) {
    uint8_t *csr_buffer = (uint8_t*) sector_buffer;
    size_t csr_buffer_length = CSR_BUFFER_LENGTH;
    status_t result = {.status = ERR_OK};

    WINC_CERTS_initBuffer();

    *csr_buffer = 0;
    result.atca = atcacert_create_csr(&g_csr_def_3_device, csr_buffer, &csr_buffer_length);
    if (result.status != ERR_OK)
        return result;

    com_write_hexbytes(csr_buffer, csr_buffer_length);

    // Certificate buffer space is also used to generate the CSR 
    // so only initialize it after the CSR was generated 
    WINC_CERTS_initBuffer();
    return result;
}

/* Get signature from ECC slot and send it to host. */
static status_t send_ecc_slot_sig(uint8_t *command, uint8_t argc) {
    status_t result = {.status = ERR_NOTIMPL};
    // result.atca = atcab_read_sig(slot_numb, buffer);
    return result;
}

/* Get key from ECC slot and send it to host. */
static status_t send_ecc_slot_key(uint8_t *command, uint8_t argc) {
    status_t result = {.status = ERR_NOTIMPL};
    // result.atca = atcab_read_pubkey(slot_numb, buffer);
    return result;
}

/* Get (generate) device public key from ECC and send it to host. */
static status_t send_dev_pubkey(uint8_t *command, uint8_t argc) {
    status_t result = {.status = ERR_OK};
    uint8_t buf[ATCA_PUB_KEY_SIZE];

    result.atca = atcab_get_pubkey(/*device_private_key_slot*/ 0, buf);
    if (result.atca == ATCA_SUCCESS)
        com_write_hexbytes(buf, ATCA_PUB_KEY_SIZE);
    return result;
}

/* check if slot is locked. If not then lock it */
static status_t lock_if_not_locked(uint16_t slot) {
    status_t result = {.status = ERR_OK};
    uint8_t n;
    bool is_locked;

    // Check if slot  is locked
    result.atca = atcab_is_slot_locked(slot, &is_locked);
    if (result.atca != ATCA_SUCCESS)
        return result;

    // Lock slots if not already locked
    if (!is_locked)
        result.atca = atcab_lock_data_slot(slot);

    return result;
}

/* Lock slots 10-12 on ATECC608A-MAH1H */
static status_t lock_slots(uint8_t *command, uint8_t argc) {
    status_t result = {.status = ERR_OK};
    bool is_locked;

    
    //PREVENT LOCKING THE ECC608A;
    return result;
        
    // Lock slot 10,  :Device certificate in special compressed/partial cert form
    result = lock_if_not_locked(10);
    if (result.atca != ATCA_SUCCESS)
        return result;

    // Lock slot 11,  :Signer device CA public key 
    result = lock_if_not_locked(11);
    if (result.atca != ATCA_SUCCESS)
        return result;

    // Lock slot 12,  :Signer certificate in a compressed/partial cert form
    result = lock_if_not_locked(12);
    if (result.atca != ATCA_SUCCESS)
        return result;

    return result;
}

/* Read serial number from kit */
static status_t send_serial_number(uint8_t *command, uint8_t argc) {
    status_t result = {.status = ERR_OK};
    uint8_t buf[ATCA_SERIAL_NUM_SIZE];

    result.atca = atcab_read_serial_number(buf);
    if (result.atca == ATCA_SUCCESS)
        com_write_hexbytes(buf, ATCA_SERIAL_NUM_SIZE);
    return result;
}

/* Generate device certificate from ECC and send it to host. */
static status_t send_ecc_dev_cert(uint8_t *command, uint8_t argc) {
    status_t result = {.status = ERR_OK};
    uint8_t ca_pubkey[ATCA_PUB_KEY_SIZE];

    WINC_CERTS_initBuffer(); // Will destroy sector buffer

    device_cert_size = DEVICE_CERT_MAX_LEN;
    result.atca = atcab_read_pubkey(11, ca_pubkey);
    if (result.atca != ATCA_SUCCESS)
        return result;

#ifdef AVR_LIKE_ECC
    result.atca = atcacert_read_cert(&g_cert_def_2_device, ca_pubkey, device_cert, &device_cert_size);
#else
    result.atca = atcacert_read_cert(&g_cert_def_4_device, ca_pubkey, device_cert, &device_cert_size);
#endif
    if (result.atca == ATCA_SUCCESS)
        com_write_hexbytes(device_cert, device_cert_size);

    // We expect the host to send back the device cert before transfer to WINC,
    // hence re-init the sector buffer here.
    WINC_CERTS_initBuffer();
    return result;
}

/* Generate signer certificate from ECC and send it to host. */
static status_t send_ecc_sign_cert(uint8_t *command, uint8_t argc) {
    status_t result = {.status = ERR_OK};
    /* uint8_t ca_pubkey[ATCA_PUB_KEY_SIZE]; */

    WINC_CERTS_initBuffer(); // Will destroy sector buffer
    signer_cert_size = SIGNER_CERT_MAX_LEN;

#ifdef AVR_LIKE_ECC
    int atca_status = ATCACERT_E_SUCCESS;

    uint8_t signer_1_ca_public_key[64] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    // Get the Signer's CA public key from the ATECCx08A
    signer_ca_public_key_size = SIGNER_PUBLIC_KEY_MAX_LEN;
    atca_status = provisioning_get_signer_ca_public_key(&signer_ca_public_key_size, signer_1_ca_public_key);
    if (atca_status != ATCA_SUCCESS) {
        // Break the do/while loop
        return result;
    }

    result.atca = atcacert_read_cert(&g_cert_def_1_signer, signer_1_ca_public_key, signer_cert, &signer_cert_size);
#else
    result.atca = atcacert_read_cert(&g_cert_def_4_signer, g_cert_ca_public_key_4_signer, signer_cert, &signer_cert_size);
#endif
    if (result.atca == ATCA_SUCCESS)
        com_write_hexbytes(signer_cert, signer_cert_size);

    // We expect the host to send back the signer cert before transfer to WINC,
    // hence re-init the sector buffer here.
    WINC_CERTS_initBuffer();

    return result;
}

/* Read TLS certificates from WINC */
static status_t winc_read_certs(uint8_t *command, uint8_t argc) {
    status_t result = {.status = ERR_OK};
    winc_download_mode(true);
    result.status = ERR_NOTIMPL; // @@@FIXME!!!!
    winc_download_mode(false);
    return result;
}

/* Erase TLS certificates in WINC */
static status_t winc_erase_certs(uint8_t *command, uint8_t argc) {
    status_t result = {.status = ERR_OK};
    winc_download_mode(true);
    result.m2m = programmer_erase_tls_cert_store();
    winc_download_mode(false);
    return result;
}

/* Do a softwre reset. */
static status_t sw_reset(uint8_t *command, uint8_t argc) {
    //    RSTCTRL_reset();
    //    UNREACHABLE
}

/* Ping, does nothing. Used to test communication. */
static status_t ping(uint8_t *command, uint8_t argc) {
    status_t result = {.status = ERR_OK};
    return result;
}

/*****************************************************************************
 * COMMAND MESSAGE INTERPRETER & EXECUTIONER
 *
 * Command syntax: <cmd>:<payload>
 *                 <payload> is hex-encoded blob
 *
 * Response: <hex> | "OK" | "ERROR:"<xxxx>
 *           <hex> is hex-encoded blob, <xxxx> 16-bit hex error code
 *                                             representing status_t.
 *****************************************************************************/
static void cmd_handler(uint8_t *command) {

    typedef struct Cmd {
        unsigned char *cmd;
        CmdHandler *handler;
        uint8_t argc; // Argument count (only 1 supported
        uint8_t rdata; // Returned data (0 => no data)
    } Command;

    static const Command commands[] = {
        /* Naming convention: '>': data host -> kit, '<': data host <- kit,
         * else action without data transfer
         */
        {"xferCerts", copy_certs, 1, 0},
        {">thingName", rcv_thingname, 1, 0},
        {">endPoint", rcv_endpoint, 1, 0},
        {">caCert", rcv_ca_cert, 1, 0},
        {">devCert", rcv_dev_cert, 1, 0},
        {"<genCsr", send_csr, 0, 1},
        {"<eccSig", send_ecc_slot_sig, 1, 1},
        {"<eccKey", send_ecc_slot_key, 1, 1},
        {"<devKey", send_dev_pubkey, 0, 1},
        {"lock", lock_slots, 0, 0},
        {"<serNum", send_serial_number, 0, 1},
        {"<devCert", send_ecc_dev_cert, 0, 1},
        {"<signCert", send_ecc_sign_cert, 0, 1},
        {"wincErs", winc_erase_certs, 0, 0},
        {"<wincRead", winc_read_certs, 0, 1},

        // debug/test commands        
        {"reset", sw_reset, 0, 0},
        {"ping", ping, 0, 0},
        {NULL, NULL, 0}
    };
    const Command *cmd;
    status_t result = {.status = ERR_OK};

    if (CHECK_STACK_GUARD()) {
        result.status = ERR_STKOVF;
        fmt_error(command, result);
        return;
    }

    for (cmd = commands; cmd->handler != NULL; cmd++) {
        if (strcmp(cmd->cmd, command) == 0) {
            if (cmd->argc == 0) {
                // Need to consume the terminating '\0' character if no arg
                char c = com_getchar();
                if (c) {
                    result.status = ERR_ARGS;
                    // Consume remaining chars to bring protocol in sync
                    while (c) c = com_getchar();
                }
            }
            if (!cmd->rdata)
                strcpy(command, COMMAND_EXECUTED);
            else
                *command = '\0';
            result = cmd->handler(command, cmd->argc);
            if (result.status != ERR_OK)
                fmt_error(command, result);
            return;
        }
    }

    if (cmd->handler == NULL) // bottomed out
    {
        result.status = ERR_UNKNOWN;
        fmt_error(command, result);
        return;
    }
}

/*
 *  Check if we came here out of a self-inflicted software reset, in case
 *  clear the SWRF flag and send empty response to host.
 */
static inline void check_reset(char *command) {
#ifdef __AVR__
    if (RSTCTRL_get_reset_cause() & RSTCTRL_SWRF_bm) {
        com_puts(COMMAND_EXECUTED);
    } else if (RSTCTRL_get_reset_cause() & RSTCTRL_WDRF_bm) {
        status_t status = {.status = ERR_WDT};
        fmt_error(command, status);
        com_puts(command);

    }

    RSTCTRL_clear_reset_cause();
#endif
}

#include "driver/include/m2m_periph.h"
static void aws_wifi_disable_pullups(void) {
    uint32 pin_mask =
            (
            M2M_PERIPH_PULLUP_DIS_HOST_WAKEUP |
            M2M_PERIPH_PULLUP_DIS_SD_CMD_SPI_SCK |
            M2M_PERIPH_PULLUP_DIS_SD_DAT0_SPI_TXD
            );

    m2m_periph_pullup_ctrl(pin_mask, 0);
}

static sint8 aws_wifi_init(void) {
    sint8 wifi_status = M2M_SUCCESS;
    tstrWifiInitParam wifi_paramaters;
    gpio_set_pin_level(WINC_WAKE,0);
    gpio_set_pin_level(RESET_PIN, 0);
    delay_ms(10);
    gpio_set_pin_level(RESET_PIN, 1);
    gpio_set_pin_level(WINC_WAKE,1);

    // Set the AWS WIFI configuration attributes
    m2m_memset((uint8*) & wifi_paramaters, 0, sizeof (wifi_paramaters));

    // Initialize the WINC1500 WIFI module for SPI Instance
    set_winc_spi_descriptor(&SPI_INSTANCE);
    nm_bsp_init();


    return wifi_status;
}

int main(void) {
    static char message[MAX_COMMAND_SIZE];

    /* Initializes MCU, drivers and middleware */
    atmel_start_init();

    for (int i =0; i <10; i++)
    {
        gpio_toggle_pin_level(LED_YELLOW);
        delay_ms(100);
    }
    
    cryptoauthlib_init();
    aws_wifi_init();

    // Need to initialize the WIFI for the certificate transfer to work.
     winc_download_mode(false);
     check_reset(message);
    WINC_CERTS_initBuffer();

    /* Replace with your application code */
    while (1) {
        while (1) {
            char c;
            int i;

            for (i = 0; i < sizeof (message); i++) {
                c = com_getchar();
                if (i == 0 && c == '\0') // Discard leading '\0' chars
                    break;
                if (c == ':') {
                    message[i] = '\0';
                    cmd_handler((uint8_t*) message);
                    if (*message) com_puts(message);
                    break;
                } else
                    message[i] = c;
            }
            if (i >= sizeof (message)) {
                status_t status = {.status = ERR_TOOLONG};
                while (c != '\0') c = com_getchar(); // discard remaining incoming characters
                fmt_error(message, status);
                com_puts(message);
            }
        }
    }
}
