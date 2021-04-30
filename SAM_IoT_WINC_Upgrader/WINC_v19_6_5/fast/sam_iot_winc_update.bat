REM Set the drive letter below corresponding to CURIOSITY
copy ..\sam_iot_serial_bridge.hex D:\
REM Set the last (right-most) number to the Virtual COM port of the board (e.g. '3' = COM3)
download_all UART SAMD21 3A0 0 3