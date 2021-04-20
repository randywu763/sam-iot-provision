# Upgrading the WINC1510 Firmware on the SAM-IoT WG Development Board

## Overview

This process requires programming the flash memory of the MCU (SAMD21) on the SAM-IoT board so any application firmware that was previously programmed will be overwritten.  The SAMD21 is basically programmed to act as a "serial bridge" between the Host PC updater utility (WINC Programming Tool) and the WINC1510 Wi-Fi network controller.

## Reprogram the WINC1510 FW using One of the Existing `winc1500_X.Y.Z.prog` Device Image Files (where X.Y.Z = WINC FW version)

1. Connect the SAM-IoT WG Development Board to the Host PC using a standard micro-USB cable

2. Launch the MPLAB X IDE and open the provided project `SAM_IoT_nEDBG_Updater.X` (MPLAB X `Main` menu > click `File` > select `Open Project`)

3. Launch the MPLAB `Pack Manager` (MPLAB X `Main` menu > click `Tools` > select `Packs`)

    <img src=".//media/image_01.png" />

4. In the drop-down menu to the left of the `Search` field, select `Show All Packs`

    <img src=".//media/image_02.png" />

5. Click on `Check for Updates`

    <img src=".//media/image_03.png" />

6. If the `Update Packs` button shows up, click on it (otherwise skip this step)

7. Using the MPLAB X Toolbar, click on the icon for `Make and Program Device` (green arrow pointing downwards)

    <img src=".//media/image_04.png" />

8. Close the project (right-click `SAM_IoT_nEDBG_Updater` > select `Close`)

9. Close the MPLAB X IDE (MPLAB X `Main` menu > select `MPLAB X IDE` > select `Quit MPLAB X IDE`)

10. Open a `File Explorer` window and determine the drive `letter` assigned to `CURIOSITY` 

    <img src=".//media/image_05.png" />

11. Determine the COM port number for your `Curiosity Virtual COM port`.  You can find the COM port number by opening the Windows `Device Manager` &gt; expand `Ports(COM & LPT)` &gt; take note of the specific `(COM<X>)` associated with the Curiosity Virtual COM Port

    <img src=".//media/image_06.png" />

12. Using a `File Explorer` window, navigate to the `SAM_IoT_WINC_Upgrader` subfolder (located in the `sam-iot-provision-main` folder)

13. Open the `sam_iot_winc_update.bat` file using any text editor

14. Locate the following line in the batch file and modify the drive `letter` corresponding to `CURIOSITY`

    > copy sam_iot_serial_bridge.hex `D`:\

15. Locate the following line in the batch file and modify the highlighted parameters to reflect the correct Curiosity Virtual COM port `number` and the desired WINC FW `version`

    > winc_programmer -p COM`29` -d winc1500 -i winc1500_`19.7.3`.prog -if prog -w -r -pfw winc1500_`19.7.3`\firmware\programmer_firmware_3A0.bin

16. Save the changes to the batch file and exit the text editor

17. Open a `PowerShell` window (click on `Start` > type `PowerShell` in the Search field > `Open`).  Navigate to the `SAM_IoT_WINC_Upgrader` subfolder and then execute the following command (once the batch file begins executing, the entire update process should take approximately 15 minutes to complete):
    ```
    .\sam_iot_winc_update
    ```
    For example, the `PowerShell` commands may look something similar to the following:

    <img src=".//media/image_07.png" />

    The below screenshot shows an example of the expected output upon successful completion of running the batch file.  When the firmware upgrade process has completed, the final output message displayed will be `"verify passed"`.  If any errors were encountered, confirm that the `CURIOSITY` drive `letter` and COM port `number` of the `Curiosity Virtual COM Port` are all correctly set in the `sam_iot_winc_update.bat` file.  
    
    <img src=".//media/image_08.png" />

18. To confirm that the correct WINC firmware version was programmed successfully, open a terminal emulator window (e.g. TeraTerm, PuTTY, Cmder, ZOC, ConEmu, Mintty, MobaXterm, Babun, KiTTY, Xshell, ConsoleZ, Console2, Hyper, FireCMD, Terminus, etc.) and connect to the Curiosity Virtual COM port at 9600 baud.

19. Drag-and-drop (i.e. copy) the `sam_iot_winc_version_verify.hex` file to the `CURIOSITY` drive.  Note the `Firmware Ver` message that is displayed in the terminal window to confirm the firmware version programmed inside the WINC1510 Wi-Fi network controller

    <img src=".//media/image_09.png" />
 
## Creating New `winc1500_X.Y.Z.prog` Device Image Files

Perform this section if a newer WINC1500 firmware binary file has been released and is not included in this repository.  A device image file (*.prog) needs to be created for every specific WINC FW version binary file (typically released as files named `wifi_firmware.bin`).  The device image file is used by the WINC Programming Tool.

1. Copy one of the existing `winc1500_X.Y.Z` folders and rename it using the desired/newer WINC FW version (i.e. change `X.Y.Z` to reflect the newer FW version)

2. Using a `File Explorer` window, navigate to the `firmware` subfolder inside the newly-created `winc1500_X.Y.Z` folder

3. Replace the existing `wifi_firmware.bin` file with the desired/newer WINC binary image file (maintaining the same file name of `wifi_firmware.bin`)

4. Move one directory upwards (into the newly-created `winc1500_X.Y.Z` parent folder) and open the `config.txt` file using any text editor.  Throughout the entire file, modify every instance of `X.Y.Z` to match the version of the newly-created `winc1500_X.Y.Z` folder

    <img src=".//media/image_10.png" />

5. Save your changes to the `config.txt` file and exit the text editor

6. Open a `PowerShell` window (click on `Start` > type `PowerShell` in the Search field > `Open`).  Navigate to the `SAM_IoT_WINC_Upgrader` subfolder and then execute the following command:
    ```
    .\image_tool.exe -c winc1500_X.Y.Z\config.txt -o winc1500_X.Y.Z.prog -of prog
    ```
    <img src=".//media/image_11.png" />