@ECHO off

REM <Clear the screen>
cls

SET cloud=%1
SET wincver=%2
SET comport=%3
SET drive=%4
SET DIR_UPGRADER=SAM_IoT_WINC_Upgrader
SET DIR_CERTGEN=SAM_IoT_Certs_Generator\CertGen
SET DIR_CREDENTIALS=ChainOfTrust

REM <Display the current date and time>
ECHO.
ECHO Provisioning Script STARTED on
date /T
echo at
time /T
ECHO.

ECHO.
ECHO *** Programming Serial Bridge Firmware into MCU ***
ECHO.
copy .\%DIR_UPGRADER%\sam_iot_serial_bridge.hex %drive%:\

ECHO.
ECHO *** Launching WINC Programming Utilities ***
ECHO.
ECHO *** (Now might be a good time for a 15-minute coffee break) ***
ECHO.
cd .\%DIR_UPGRADER%\%wincver%
IF %wincver%==19.6.5 (CALL download_all.bat UART SAMD21 3A0 0 %comport%)
IF %wincver%==19.7.3 (CALL download_all.bat UART 1500 0 %comport%)

ECHO.
ECHO *** Programming Provisioning Firmware into MCU ***
ECHO.
cd ..\..\%DIR_CERTGEN%
copy SAM_IoT_CertGen.hex %drive%:\

ECHO.
ECHO *** Updating Python Modules (if needed) ***
ECHO.
python -m pip install --upgrade pip
pip install -r py_modules.txt

ECHO.
ECHO *** Preparing Chain of Trust ***
ECHO.
cd cert

ECHO.
ECHO *** Deleting Existing Certificates ***
erase *.crt
erase *.pem
ECHO.
ECHO *** Deleting Existing Keys ***
erase *.key
ECHO.
ECHO *** Deleting Existing CSRs ***
erase *.csr
ECHO.

ECHO.
ECHO *** Generating Root Certificate and Key ***
CALL python ca_create_root.py
ECHO.
ECHO *** Generating Signer Key and Certificate Signing Request (CSR) ***
CALL python ca_create_signer_csr.py
ECHO.
ECHO *** Generating Signer Certificate ***
CALL python ca_create_signer.py

ECHO.
ECHO *** Loading Desired Chain of Trust ***
ECHO *** (if none found, the newly-created mock chain will be used) ***
ECHO.
copy /Y ..\..\%DIR_CREDENTIALS%\root-ca.crt
copy /Y ..\..\%DIR_CREDENTIALS%\root-ca.key
copy /Y ..\..\%DIR_CREDENTIALS%\signer-ca.crt
copy /Y ..\..\%DIR_CREDENTIALS%\signer-ca.key
copy /Y ..\..\%DIR_CREDENTIALS%\signer-ca.csr

ECHO.
ECHO *** Generating Device Certificate and Writing Signer/Device Certs into WINC1510 ***
cd ..
CALL python provision_samiot.py com%comport%

ECHO.
ECHO *** Saving Current Chain of Trust ***
ECHO.
cd cert
copy /Y root-ca.crt ..\..\%DIR_CREDENTIALS%
copy /Y root-ca.crt ..\..\%DIR_CREDENTIALS%\root-ca.pem
copy /Y root-ca.key ..\..\%DIR_CREDENTIALS%
copy /Y signer-ca.crt ..\..\%DIR_CREDENTIALS%
copy /Y signer-ca.crt ..\..\%DIR_CREDENTIALS%\signer-ca.pem
copy /Y signer-ca.key ..\..\%DIR_CREDENTIALS%
copy /Y signer-ca.csr ..\..\%DIR_CREDENTIALS%

ECHO.
ECHO ***  Programming Cloud Service Demo Application into MCU  ***
ECHO *** (Please be patient; this could take up to 60 seconds) ***
ECHO.
cd ..\..\..
IF %cloud%==azure (copy AzureIotPnpDps.X.production.hex %drive%:\)

ECHO.
ECHO *** Provisioning SAM-IoT Development Board COMPLETED ***
ECHO.

REM <Display the current date and time>
ECHO Provisioning Script ENDED on
date /T
echo at
time /T
ECHO.