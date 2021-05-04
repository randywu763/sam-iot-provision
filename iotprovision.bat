@ECHO off
ECHO.
ECHO Command Line Arguments: arg1 = cloud [aws,azure] arg2 = wincver [19.6.5,19.7.3] arg3 = comport [number] arg4 = CURIOSITY drive [letter]
CALL sam-iot-provision.bat azure 19.7.3 3 D
