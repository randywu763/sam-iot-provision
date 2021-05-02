SET cloud=%1
SET wincver=%2
SET comport=%3
SET drive=%4
SET DIR_UPGRADER=SAM_IoT_WINC_Upgrader
SET DIR_CERTGEN=SAM_IoT_Certs_Generator\CertGen
SET DIR_CERTIFICATES=Certificates

REM *** Programming Serial Bridge Firmware ***
copy .\%DIR_UPGRADER%\sam_iot_serial_bridge.hex %drive%:\

REM *** Launching WINC Programming Tool ***
cd .\%DIR_UPGRADER%\%wincver%
CALL download_all.bat UART SAMD21 3A0 0 %comport%

@ECHO on

REM *** Programming Provisioning Firmware ***
cd ..\..\%DIR_CERTGEN%
copy SAM_IoT_CertGen.hex %drive%:\

REM *** Updating Python Modules ***
python -m pip install --upgrade pip
pip install -r py_modules.txt

REM *** Generating Root/Signer Certificate Files ***
cd cert
CALL python ca_create_root.py
CALL python ca_create_signer_csr.py
CALL python ca_create_signer.py
copy root-ca.crt root-ca.pem
copy signer-ca.crt signer-ca.pem
copy signer-ca-verification.crt signer-ca-verification.pem
copy device.crt device.pem

REM *** Generating Device Certificate and Writing to WINC1510 ***
cd ..
CALL python provision_samiot.py com%comport%

REM *** Populating Certificates Folder ***
cd cert
copy /Y root-ca.crt ..\..\..\%DIR_CERTIFICATES%
copy /Y root-ca.key ..\..\..\%DIR_CERTIFICATES%
move /Y root-ca.pem ..\..\..\%DIR_CERTIFICATES%
copy /Y signer-ca.key ..\..\..\%DIR_CERTIFICATES%
move /Y signer-ca.pem ..\..\..\%DIR_CERTIFICATES%
move /Y signer-ca-verification.pem ..\..\..\%DIR_CERTIFICATES%
copy /Y device.key ..\..\..\%DIR_CERTIFICATES%
move /Y device.pem ..\..\..\%DIR_CERTIFICATES%

REM *** Programming Demo Application ***
cd ..\..\..
IF %cloud%==azure copy AzureIotPnpDps.X.production.hex %drive%:\