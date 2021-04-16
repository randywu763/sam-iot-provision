"""
Error status messages from provisioning firmware.
The 16-bit status word contains one of the following:
- 8-bit cryptoauthlib (ATCA) error code in LSB, MSB being zero
- 8-bit WINC/SPI flash error codes (M2M) in MSB, LSB being zero
- 16-bit error code from main (both MSB and LSB nonzero)
"""


# Having trouble with ctryptoauthlib when buiding with pyinstaller, drop it
# in this case. This is our only use of cryptoauthlib currently.
try:
    from cryptoauthlib import Status

    CRYPTOAUTHLIB_ERRORS = {
        # See cryptoauthlib/lib/atca_status.h
        # @@@FIXME: actacert has its own status codes, found in atcacert/atcacert.h,
        # partially overlapping the below set (sigh).
        int(Status.ATCA_SUCCESS): "Success",
        int(Status.ATCA_CONFIG_ZONE_LOCKED): "Config Zone Locked",
        int(Status.ATCA_DATA_ZONE_LOCKED): "Data Zone Locked",
        int(Status.ATCA_WAKE_FAILED): "Wake Failed Error",
        int(Status.ATCA_CHECKMAC_VERIFY_FAILED): "Check Mac Verify Failed",
        int(Status.ATCA_PARSE_ERROR): "Parse Error",
        int(Status.ATCA_STATUS_CRC): "CRC Error",
        int(Status.ATCA_STATUS_UNKNOWN): "Status Unknown Error",
        int(Status.ATCA_STATUS_ECC): "ECC Fault",
        0xd7: "Self Test Error",        # ATCA_STATUS_SELFTEST_ERROR missing in cryptoauthlib/status.py
        int(Status.ATCA_FUNC_FAIL): "Function Error due to incorrect condition / state",
        int(Status.ATCA_GEN_FAIL): "Generic Error",
        int(Status.ATCA_BAD_PARAM): "Bad Argument (out of range, null pointer, etc.)",
        int(Status.ATCA_INVALID_ID): "Invalid device Identifier, id not set",
        int(Status.ATCA_INVALID_SIZE): "Invalid Size (of range or greater than buffer size)",
        int(Status.ATCA_BAD_CRC): "Bad Crc in data received from device",
        int(Status.ATCA_RX_FAIL): "Receive timeout Error",
        int(Status.ATCA_RX_NO_RESPONSE): "RX No Response",
        int(Status.ATCA_RESYNC_WITH_WAKEUP): "Resync With Wakeup",
        int(Status.ATCA_PARITY_ERROR): "Parity Error",
        int(Status.ATCA_TX_TIMEOUT): "Transmission Timeout",
        int(Status.ATCA_RX_TIMEOUT): "Receive TimeoutError",
        int(Status.ATCA_COMM_FAIL): "Communication Error",
        0xec: "Too Many Comm Retries",  # ATCA_TOO_MANY_COMM_RETRIES  missing in cryptoauthlib/status.py
        0xed: "Too Small Buffer",       # ATCA_SMALL_BUFFER  missing in cryptoauthlib/status.py
        int(Status.ATCA_TIMEOUT): "TimeOut",
        int(Status.ATCA_BAD_OPCODE): "Bad Opcode",
        int(Status.ATCA_WAKE_SUCCESS): "Wake Success",
        int(Status.ATCA_EXECUTION_ERROR): "Execution Error, chip in a state where it" \
        " could not execute the command",
        int(Status.ATCA_UNIMPLEMENTED): "Not yet Implemented ",
        int(Status.ATCA_ASSERT_FAILURE): "Assertion Failure",
        int(Status.ATCA_TX_FAIL): "Transmission Error",
        int(Status.ATCA_NOT_LOCKED): "Zone Not Locked",
        int(Status.ATCA_NO_DEVICES): "No Devices Found ",
        int(Status.ATCA_HEALTH_TEST_ERROR): "Random number generator Health Test Error",
        int(Status.ATCA_ALLOC_FAILURE): "Couldn't allocate required memory",
        int(Status.ATCA_NOT_INITIALIZED): "Library Not Initialized"
    }
except:
    # If cryptoauthlib import failed, the error codes above will be presented
    # numerically only.
    CRYPTOAUTHLIB_ERRORS = {}


ERROR_MESSAGES = {
    # See main.c
    0: "No error",
    0x7f7f: "Unknown command",
    0x7f7e: "Not yet implemented",
    0x7f7d: "Wrong number of arguments",
    0x7f7c: "Invalid argument value",
    0x7f7b: "Watchdog timeout",
    0x7f7a: "FATAL: Stack overflow",
    0x7f79: "Message buffer exceeded",

    # M2M status is in MSB of status. See winc/common/include/nm_common.h
    0xff << 8: "M2M_ERR_SEND",
    0xfe << 8: "M2M_ERR_RCV",
    0xfd << 8: "M2M_ERR_MEM_ALLOC",
    0xfc << 8: "M2M_ERR_TIME_OUT",
    0xfb << 8: "M2M_ERR_INIT",
    0xfa << 8: "M2M_ERR_BUS_FAIL",
    0xf9 << 8: "M2M_NOT_YET",
    0xf8 << 8: "M2M_ERR_FIRMWARE",
    0xf7 << 8: "M2M_SPI_FAIL",
    0xf6 << 8: "M2M_ERR_FIRMWARE_bURN",
    0xf5 << 8: "M2M_ACK",
    0xf4 << 8: "M2M_ERR_FAIL",
    0xf3 << 8: "M2M_ERR_FW_VER_MISMATCH ",
    0xf2 << 8: "M2M_ERR_SCAN_IN_PROGRESS",
    0xf1 << 8: "M2M_ERR_INVALID_ARG",
    0xf0 << 8: "M2M_ERR_INVALID",
    # Don't know if we can get the below also
    0xe5 << 8: "ERR_FIRMEWARE_EXCEED_SIZE",
    0xe4 << 8: "ERR_PREFIX_NMIS",
    0xe3 << 8: "I2C_ERR_OVER_SIZE",
    0xe2 << 8: "I2C_ERR_TX_ABRT",
    0xe1 << 8: "I2C_ERR_LARGE_ADDRESS"
}.update(CRYPTOAUTHLIB_ERRORS)


def format_error(error, command=""):
    """
    Format error message received from kit.
    Parameters: error message from kit, command sent to kit (if available)
    Return: formatted error message if found, else raw error sttring
    """
    try:
        (_, code) = error.split(":")
        status = int(code, 16)
        message = ERROR_MESSAGES.get(status, "Unknown error code")
        return "Error {} in {}: {}".format(hex(status), command, message)
    except ValueError:
        return error
