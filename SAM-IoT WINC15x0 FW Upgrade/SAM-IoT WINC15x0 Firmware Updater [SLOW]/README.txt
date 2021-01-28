*** Procedure to Update WINC1510 Firmware on the SAM-IoT Board ***

1.  In the "firmware" folder, duplicate the desired FW image file and rename it to "m2m_aio_3a0.bin"
2.  Connect SAM-IoT board to PC via micro-USB cable (a disk drive named "Curiosity" should appear)
3.  Drag and drop (i.e. copy) "SAM-IoT-WINC15x0_SerialBridge" HEX file to the "Curiosity" disk drive 
4.  Edit the "winc_update" batch file and confirm the COM port number is correct
5.  From a command line window, run the "winc_update" batch file (*** Update process will take at least 15 minutes ***)
6.  Launch a Terminal Emulator program
7.  Open a serial port connection to the Virtual COM port corresponding to the board's connection via USB
8.  Set Terminal Emulator serial port for 9600/8-N-1
9.  Drag and drop (i.e. copy) "sam_d21_iot_wg.X.production" HEX file to the "Curiosity" disk drive 
10. After the HEX file has finished copying, confirm the WINC1510's FW version displayed in the Terminal Emulator window

