@ECHO off
ECHO.
ECHO Command Line Arguments: arg1 = cloud [aws,azure,gcp] arg2 = wincver [XX.Y.Z or 0] arg3 = comport [number] arg4 = CURIOSITY drive [letter]
CALL sam-iot-provision.bat azure 19.7.3 3 D