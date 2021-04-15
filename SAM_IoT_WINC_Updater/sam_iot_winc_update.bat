REM Execute the following command line to provision the WINC1500 device via serial port (make sure to specify the correct Virtual COM port number)

winc_programmer -p COM4 -d winc1500 -i winc1500_19.6.5.prog -if prog -w -r -pfw winc1500_19.6.5\firmware\programmer_firmware_3A0.bin

REM If the winc1500_19.6.5.prog file does not exist, execute the following command line:
REM image_tool.exe -c winc1500_19.6.5\config.txt -o winc1500_19.6.5.prog -of prog
