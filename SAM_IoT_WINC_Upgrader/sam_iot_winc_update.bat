REM *** Programming Serial Bridge Firmware ***
copy sam_iot_serial_bridge.hex D:\
REM *** Begin Updating WINC1510 Firmware ***
.\winc_programmer.exe -p COM29 -d winc1500 -i winc1500_19.6.5.prog -if prog -w -r -pfw winc1500_19.6.5\firmware\programmer_firmware_3A0.bin

