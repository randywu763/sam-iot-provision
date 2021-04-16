# Procedure for Upgrading the WINC1510 Firmware on the SAM-IoT WG Development Board

## Upgrade the WINC1510 FW using one of the existing `winc1500_X.Y.Z.prog` files (where X.Y.Z = WINC FW version)

1. Connect the SAM-IoT WG Development Board to the Host PC using a standard micro-USB cable

2. Drag & drop (i.e. copy) the `sam_iot_serial_bridge.hex` file to the `CURIOSITY` drive.  This operation programs the flash memory of the MCU on the SAM-IoT board so any application firmware that was previously programmed will be overwritten.  If the drag-and-drop operation fails multiple times, consult the `Drag and Drop Troubleshooting for SAM-IoT` document that is located in the `Docs` folder.

3. Determine the COM port number for your `Curiosity Virtual COM port`.  You can find the COM port number by opening the Windows `Device Manager` &gt; expand `Ports(COM & LPT)` &gt; take note of the specific `(COM<X>)` associated with the Curiosity Virtual COM Port

    <img src=".//media/image_01.png" />

4. Open the `sam_iot_winc_update.bat` file using any text editor.  Edit the file to reflect the Curiosity Virtual COM port number and the desired WINC FW version:

    > winc_programmer -p COM`29` -d winc1500 -i winc1500_`19.6.5`.prog -if prog -w -r -pfw winc1500_`19.6.5`\firmware\programmer_firmware_3A0.bin

5. Save the changes to the file and exit the editor

6. Open a `PowerShell` window > click on `Start` > type `PowerShell` in the Search field > `Open`.  Navigate to the `SAM_IoT_WINC_Upgrader` subfolder and then execute the following command (the update process should take approximately 15 minutes):
    ```
    .\sam_iot_winc_update.bat
    ```

## Creating new `winc1500_X.Y.Z.prog` files

Perform this section if a newer WINC1500 firmware binary file has been released and is not included in this repository.

1. Copy one of the existing `winc1500_X.Y.Z` folders and rename it with the desired/newer WINC FW version

2. Using a File Explorer window, navigate to the `firmware` subfolder inside the newly-created `winc1500_X.Y.Z` folder

3. Replace `wifi_firmware.bin` with the desired/newer WINC binary image file

4. Open a `PowerShell` window > click on `Start` > type `PowerShell` in the Search field > `Open`.  Navigate to the `SAM_IoT_WINC_Upgrader` subfolder and then execute the following command:
    ```
    .\image_tool.exe -c winc1500_X.Y.Z\config.txt -o winc1500_X.Y.Z.prog -of prog
    ```
