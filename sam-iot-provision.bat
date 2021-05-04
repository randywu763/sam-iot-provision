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
ECHO.
ECHO Provisioning Script STARTED at %TIME% (%DATE%)
set startHH24=%TIME: =0%
set startHH24=%HH:~0,2%
set startMI=%TIME:~3,2%
set startSS=%TIME:~6,2%
set startFF=%TIME:~9,2%

ECHO.
ECHO *** Programming Serial Bridge Firmware into MCU ***
ECHO.
copy .\%DIR_UPGRADER%\sam_iot_serial_bridge.hex %drive%:\

ECHO.
ECHO *** Launching WINC Programming Utilities ***
ECHO.
ECHO *** (Now might be a good time for a coffee break) ***
ECHO.
cd .\%DIR_UPGRADER%\%wincver%
IF %wincver% LSS 19.7.3 (CALL download_all.bat UART SAMD21 3A0 0 %comport%) ELSE (CALL download_all.bat UART 1500 0 %comport%)

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
ECHO Provisioning Script ENDED at %TIME% (%DATE%)
set endHH24=%TIME: =0%
set endHH24=%HH:~0,2%
set endMI=%TIME:~3,2%
set endSS=%TIME:~6,2%
set endFF=%TIME:~9,2%

REM <Calculate duration of provisioning process in minutes>
ECHO.
SET /A minutes = (endMI - startMI)
IF %minutes%==0 (ECHO The total provisioning time took less than a minute which means something went wrong...) ELSE (ECHO The total provisioning time took approximately %minutes% mins)
ECHO.
