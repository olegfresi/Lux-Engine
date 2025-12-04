@echo off
setlocal
echo 🔍 Checking if Git is installed...

git --version >nul 2>&1
IF %ERRORLEVEL% EQU 0 (
    echo ✅ Git is already installed:
    git --version
    pause
    exit /b 0
)

echo ❌ Git not found. Installing...

REM
REM
REM
NET SESSION >nul 2>&1
IF %ERRORLEVEL% NEQ 0 (
    echo ⚠️ Please run this script as ADMINISTRATOR!
    pause
    exit /b 1
)

REM
REM
REM
echo ⬇️ Downloading Git Installer...

powershell -Command ^
"Invoke-WebRequest -Uri https://github.com/git-for-windows/git/releases/latest/download/Git-64-bit.exe -OutFile GitInstaller.exe"

IF NOT EXIST GitInstaller.exe (
    echo ❌ Download failed!
    pause
    exit /b 1
)

REM
REM
REM
echo 🔧 Installing Git silently...

GitInstaller.exe /VERYSILENT /NORESTART

REM
REM
REM
del GitInstaller.exe

REM
REM
REM
git --version >nul 2>&1
IF %ERRORLEVEL% EQU 0 (
    echo ✅ Git installed successfully!
    git --version
) ELSE (
    echo ❌ Git installation failed!
)

pause


where python >nul 2>nul
if %errorlevel% equ 0 (
    echo Python already installed.
) else (
    echo Python not installed


    echo Download of python installer
    powershell -Command "Invoke-WebRequest -Uri https://www.python.org/ftp/python/3.11.5/python-3.11.5-amd64.exe -OutFile python_installer.exe"
    echo Installing Python...
    start /wait python_installer.exe /quiet InstallAllUsers=1 PrependPath=1
    del python_installer.exe


    where python >nul 2>nul
    if %errorlevel% neq 0 (
        echo Error: Python installation failed
        exit /b 1
    )
)

echo Running of setup.py...
python setup.py
