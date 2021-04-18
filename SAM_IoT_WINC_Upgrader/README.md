# Upgrading the WINC1510 Firmware on the SAM-IoT WG Development Board

This process programs the flash memory of the MCU on the SAM-IoT board so any application firmware that was previously programmed will be overwritten.

## Reprogram the WINC1510 FW using one of the existing `winc1500_X.Y.Z.prog` files (where X.Y.Z = WINC FW version)

1. Connect the SAM-IoT WG Development Board to the Host PC using a standard micro-USB cable

2. Open a `File Explorer` window and determine the drive `letter` assigned to `CURIOSITY` 

    <img src=".//media/image_01.png" />

3. Determine the COM port number for your `Curiosity Virtual COM port`.  You can find the COM port number by opening the Windows `Device Manager` &gt; expand `Ports(COM & LPT)` &gt; take note of the specific `(COM<X>)` associated with the Curiosity Virtual COM Port

    <img src=".//media/image_02.png" />

4. Using a `File Explorer` window, navigate to the `CertGen` subfolder

5. Open the `sam_iot_winc_update.bat` file using any text editor

6. Locate the following line in the batch file and modify the drive `letter` corresponding to `CURIOSITY`

    > copy SAM_IoT_CertGen.hex `D`:\

7. Locate the following line in the batch file and modify the highlighted parameters to reflect the correct Curiosity Virtual COM port `number` and the desired WINC FW `version`

    > winc_programmer -p COM`29` -d winc1500 -i winc1500_`19.7.3`.prog -if prog -w -r -pfw winc1500_`19.7.3`\firmware\programmer_firmware_3A0.bin

8. Save the changes to the file and exit the text editor

9. Open a `PowerShell` window > click on `Start` > type `PowerShell` in the Search field > `Open`.  Navigate to the `SAM_IoT_WINC_Upgrader` subfolder and then execute the following command (the update process should take approximately 15 minutes):
    ```
    .\sam_iot_winc_update
    ```
    For example:

    <img src=".//media/image_03.png" />

    Here is an example of the expected output upon successful completion of running the batch file.  If any errors were encountered, confirm that the `CURIOSITY` drive `letter` and COM port `number` of the `Curiosity Virtual COM Port` are all correctly set in the `sam_iot_winc_update.bat` file.  
    
    <img src=".//media/image_04.png" />

## Creating new `winc1500_X.Y.Z.prog` files

Perform this section if a newer WINC1500 firmware binary file has been released and is not included in this repository.

1. Copy one of the existing `winc1500_X.Y.Z` folders and rename it with the desired/newer WINC FW version (i.e. change `X.Y.Z` to reflect the newer FW version)

2. Using a `File Explorer` window, navigate to the `firmware` subfolder inside the newly-created `winc1500_X.Y.Z` folder

3. Replace the existing `wifi_firmware.bin` file with the desired/newer WINC binary image file (maintaining the same file name of `wifi_firmware.bin`)

4. Open a `PowerShell` window > click on `Start` > type `PowerShell` in the Search field > `Open`.  Navigate to the `SAM_IoT_WINC_Upgrader` subfolder and then execute the following command:
    ```
    .\image_tool.exe -c winc1500_X.Y.Z\config.txt -o winc1500_X.Y.Z.prog -of prog
    ```
