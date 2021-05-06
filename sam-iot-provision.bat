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
SET DIR_DEMOAPPS=CloudDemoApps
SET FILE_SERIALBRIDGE_HEX=sam_iot_serial_bridge.hex
SET FILE_CERTGEN_HEX=SAM_IoT_CertGen.hex

REM <Display the current date and time>
ECHO.
ECHO Provisioning Script STARTED at %TIME% (%DATE%)
REM <Convert start and end times to hundredths of a second>
Call :s_calc_timecode %TIME%
Set _start_timecode=%errorlevel%

REM <If desired WINC firmware version is set to 0 then skip the WINC programming process>
IF %wincver% EQU 0 (GOTO :WINC_Provision_Start)

:WINC_Update_Start

ECHO.
ECHO *** Programming Serial Bridge Firmware into MCU ***
ECHO.
copy .\%DIR_UPGRADER%\%FILE_SERIALBRIDGE_HEX% %drive%:\

ECHO.
ECHO *** Launching WINC Programming Utilities ***
ECHO.
ECHO *** (Now might be a good time for a coffee break) ***
ECHO.
cd .\%DIR_UPGRADER%\%wincver%
IF %wincver% LSS 19.7.3 (CALL download_all.bat UART SAMD21 3A0 0 %comport%) ELSE (CALL download_all.bat UART 1500 0 %comport%)
cd ..\..

:WINC_Provision_Start

ECHO.
ECHO *** Programming Provisioning Firmware into MCU ***
ECHO.
cd %DIR_CERTGEN%
copy %FILE_CERTGEN_HEX% %drive%:\

ECHO.
ECHO *** Updating Python Modules ***
ECHO *** (if needed - requirements may already be satisfied) ***
ECHO.
python -m pip install --upgrade pip
pip install -r py_modules.txt

ECHO.
ECHO *** Preparing Chain of Trust ***
cd cert

ECHO.
ECHO *** Deleting Existing Certificates from Working Directory ***
erase *.crt
erase *.pem
ECHO.
ECHO *** Deleting Existing Keys from Working Directory ***
erase *.key
ECHO.
ECHO *** Deleting Existing CSRs from Working Directory ***
erase *.csr

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
copy /Y device.crt ..\..\%DIR_CREDENTIALS%
copy /Y device.crt ..\..\%DIR_CREDENTIALS%\device.pem

ECHO.
ECHO *** Programming the MCU: Cloud Service Demo Application for %cloud% ***
ECHO *** (Please be patient; this could take up to a minute or more...) ***
ECHO.
erase %drive%:\*.hex
cd ..\..\..\%DIR_DEMOAPPS%
copy samiot_demoapp_%cloud%.hex %drive%:\
cd ..

ECHO.
ECHO *** Provisioning SAM-IoT Development Board for %cloud% COMPLETED ***
ECHO.

REM <Display the current date and time>
ECHO Provisioning Script ENDED at %TIME% (%DATE%)

REM <Convert end time to hundredths of a second>
Call :s_calc_timecode %TIME%
Set _stop_timecode=%errorlevel%

REM <Calculate the difference in hundredths of a second>
Set /a _diff_timecode=_stop_timecode - _start_timecode

REM <Account for midnight rollover>
if %_diff_timecode% LSS 0 set /a _diff_timecode+=(24 * 60 * 60 * 100)

REM <Split out hours/mins/secs and calculate the time difference>
set /a hs=_diff_timecode %% 100
set /a _diff_timecode/=100
set /a ss=_diff_timecode %% 60
set /a _diff_timecode/=60
set /a mm=_diff_timecode %% 60
set /a _diff_timecode/=60
set /a hh=_diff_timecode
set hh=0%hh%
set mm=0%mm%
set ss=0%ss%
set hs=0%hs%
set _tdiff=%hh:~-2%:%mm:~-2%:%ss:~-2%.%hs:~-2%

ECHO.
ECHO The total provisioning time was %_tdiff% [HH:MM:SS.HS]
ECHO.

goto :EOF

:s_calc_timecode
   :: Remove delimiters and convert to hundredths of a second.
  setlocal

  For /f "usebackq tokens=1,2,3,4 delims=:." %%a in ('%1') Do (
      set hh=00%%a
      set mm=00%%b
      set ss=00%%c
      set hs=00%%d
  )
   set hh=%hh:~-2%
   set mm=%mm:~-2%
   set ss=%ss:~-2%
   set hs=%hs:~-2%
   set /a hh=((%hh:~0,1% * 10) + %hh:~1,1%) * 60 * 60 * 100
   set /a mm=((%mm:~0,1% * 10) + %mm:~1,1%) * 60 * 100
   set /a ss=((%ss:~0,1% * 10) + %ss:~1,1%) * 100
   set /a hs=((%hs:~0,1% * 10) + %hs:~1,1%)
   
   set /a _timecode=hh + mm + ss + hs
Exit /b %_timecode%