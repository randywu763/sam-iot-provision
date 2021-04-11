@ECHO Off

set varPath=%PROGRAMFILES%

:CheckOS
IF EXIST "%PROGRAMFILES(X86)%" (GOTO 64BIT) ELSE (GOTO RUN)
:64BIT
set varPath=%PROGRAMFILES(X86)%
:RUN


set /A edbgCnt=0
set SN=0
for /f "tokens=1-2" %%i in ('"%varPath%\Atmel\Studio\7.0\atbackend\atprogram.exe" list') do (
	if "%%i" == "edbg" (
		set SN=%%j
		set /A edbgCnt+=1
	)
)	

if %edbgCnt%==0 (
	echo Cannot find and EDBG boards?
	echo see  '"%varPath%\Atmel\Studio\7.0\atbackend\atprogram.exe" list'
	exit /b 1
)

if %edbgCnt% GTR 1 (
	echo This batch file is unsuitable if more than one EDBG based development board is installed, found %edbgCnt%
	echo Use download_all_sb.bat with options
	echo		edbg 
	echo		ATSAM4SD32C 
	echo		Tools\serial_bridge\sam4s_xplained_pro_serial_bridge.elf 
	echo		3400 or 3A0 [1500/1510]
	echo		serial number of the dev board attached to the board you wish to program - see '"%varPath%\Atmel\Studio\7.0\atbackend\atprogram.exe" list'
	echo		com port number assigned to the dev board attached to the board you wish to program by the OS
	exit /b 1
)

:: On the apps system the files are in a firmware folder, for dev they are at repo root.
if exist download_all_sb.bat (
  pushd .
) else (
  pushd firmware
)

if not exist download_all_sb.bat (
  echo File layout error
  popd
  pause
  exit /b 1
)

set CHPFAM=error
if exist firmware\m2m_image_3400.bin (
	if not exist firmware\m2m_image_3A0.bin set CHPFAM=3400
)
if exist firmware\m2m_image_3A0.bin (
	if not exist firmware\m2m_image_3400.bin set CHPFAM=3A0
)

if %CHPFAM%==error (
	echo Must have one of firmware\m2m_image_3A0.bin and firmware\m2m_image_3400.bin
	echo Try running Prepare_image.cmd
	popd
	pause
	exit /b 1
)

call download_all_sb.bat edbg ATSAM4SD32C Tools\serial_bridge\sam4s_xplained_pro_serial_bridge.elf %CHPFAM% %SN% 0

popd
pause
