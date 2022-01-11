
#ifndef SERIALBRIDGE_H_INCLUDED
#define SERIALBRIDGE_H_INCLUDED

#define CONF_STDIO_BAUDRATE      115200

#ifdef AT_SB_JOINT
void usart_rx_complete_handler(struct usart_module *const module);
void usart_tx_complete_handler(struct usart_module *const module);

#endif

sint8 wifi_firmware_download_loop(void);
sint8 enter_wifi_firmware_download(void);

#endif
