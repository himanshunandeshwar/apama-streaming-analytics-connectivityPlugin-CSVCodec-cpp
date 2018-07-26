rem
rem    Author: Himanshu Nandeshwar
rem    Date: 26-Jul-2018
rem
@echo off

setlocal

set _CSVCODEC_SOLUTION_FILE=CSVCodec.sln

if not defined APAMA_HOME (
	echo Cannot find required APAMA environment variables. 
	echo Please run this script from an Apama Command Prompt. 
	exit /b 100
	goto END
)

if not defined FrameworkDir (
	echo Cannot find required Visual Studio environment variables. 
	echo Please execute the vcvarsall.bat script with the amd64 
	echo argument before running make, e.g. 
	echo.
	echo "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" amd64
	exit /b 100
)
if /I "%Platform%" == "X64" (
	set VSARCH=x64
) else (
	echo Only 64-bit platforms are supported.
	exit /b 100
)

echo Selected build configuration is: %VSARCH%

if "%1"=="clean" goto CLEAN
if "%1"=="release" goto RELEASE
if "%1"=="ide" goto IDE
goto USAGE

rem --------------------------------------------------------------------------
:IDE
echo Starting Visual Studio IDE...
devenv %_CSVCODEC_SOLUTION_FILE%
goto END

rem --------------------------------------------------------------------------
:CLEAN
echo Cleaning...
msbuild /p:platform=%VSARCH% /t:clean %_CSVCODEC_SOLUTION_FILE%
rmdir /s /q Release
goto END

rem --------------------------------------------------------------------------
:RELEASE
echo Starting release build...
msbuild /p:platform=%VSARCH% %_CSVCODEC_SOLUTION_FILE%
goto END

rem --------------------------------------------------------------------------
:USAGE
echo.
echo This script is for building samples on Microsoft Windows using the 
echo version of Microsoft Visual Studio supported by this Apama release. 
echo.
echo Start this script from an Apama Command Prompt, after running the 
echo Visual Studio vcvarsall.bat script with the amd64 argument. 
echo.
echo make [ help ^| ide ^| release ^| clean ]
echo   help    - Show this message
echo   ide     - Open in the Visual Studio IDE for interactive editing
echo   release - Perform a release build
echo   clean   - Remove intermediate files and binaries from a previous build
goto END


rem -------------------------------------------------------------------------
:END
