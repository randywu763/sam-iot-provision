@ECHO off
ECHO.
ECHO Command Line Arguments: arg1 = cloud [azure,aws] arg2 = wincver [XX.Y.Z] arg3 = comport [number] arg4 = CURIOSITY drive [letter]
CALL sam-iot-provision.bat azure 19.6.5 3 D
