REM Command Line Arguments:
REM arg1 = Cloud Service [aws, azure, gcp]
REM arg2 = WINC FW Version [XX.Y.Z (or 0 to skip upgrade)]
REM arg3 = Curiosity Virtual COM Port [number]
REM arg4 = CURIOSITY Drive [letter]
REM For example:
REM >sam-iot-provision.bat azure 19.7.3 3 F
CALL sam-iot-provision.bat <cloud_service> <winc_ver> <com_port> <drive_letter>
