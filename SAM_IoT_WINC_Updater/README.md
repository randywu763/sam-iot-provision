# Procedure for Updating the WINC1510 Firmware on a SAM-IoT Development Board

## Upgrade the WINC1510 FW using one of the existing **winc1500_X.Y.Z.prog** files (where X.Y.Z = FW version)

> 1. Drag-and-drop (copy) the **sam_iot_serial_bridge.hex** file (located in the **sam_iot_serial_bridge** project folder) to the SAM-IoT **CURIOSITY** drive
> 2. Edit the **sam_iot_winc_update.bat** file to reflect the desired WINC FW version and COM port for your SAM-IoT board (remember to save changes)
> 3. Execute the **sam_iot_winc_update.bat** file (update process should take approximately 15 minutes)

## Creating new **winc1500_X.Y.Z.prog** files

Perform this section if a newer WINC1500 firmware binary file has been released and is not included in this repository.

> 1. Copy one of the existing **winc1500_X.Y.Z** folders and rename it with the desired/newer WINC FW version
> 2. Navigate to the **firmware** subfolder inside the newly-created **winc1500_X.Y.Z** folder
> 3. Replace **wifi_firmware.bin** with the desired/newer WINC binary image file
> 4. Navigate back to the **SAM_IoT_WINC_Updater** folder
> 5. Execute the following command: **image_tool.exe -c winc1500_X.Y.Z\config.txt -o winc1500_X.Y.Z.prog -of prog**

