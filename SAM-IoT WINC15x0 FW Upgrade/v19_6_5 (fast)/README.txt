*** Procedure to Update WINC1510 Firmware on the SAM-IoT Board ***

NOTE: Before starting this process, please open the document "SAM-IoT Debugger FW Version" (located in the folder containing this one) to make sure the Debugger firmware version is correct on your board.  If the Debugger firmware version is not set to the correct one, this procedure will fail...

1.  Connect SAM-IoT board to PC via micro-USB cable (a disk drive named "Curiosity" should appear)
2.  Drag and drop (i.e. copy) "SAM-IoT-WINC15x0_SerialBridge" HEX file to the "Curiosity" disk drive 
3.  Edit the "winc_update" batch file and change the COM port number corresponding to your board
4.  From a command line window, run the "winc_update" batch file
5.  Launch a Terminal Emulator program
6.  Open a serial port connection to the Virtual COM port corresponding to the board's connection via USB
7.  Set Terminal Emulator serial port for 9600/8-N-1
8.  Drag and drop (i.e. copy) "sam_d21_iot_wg.X.production" HEX file to the "Curiosity" disk drive 
9. After the HEX file has finished copying, confirm the WINC1510's FW version displayed in the Terminal Emulator window

