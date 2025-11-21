@echo off
setlocal

:: Controlla se Python è installato
where python >nul 2>nul
if %errorlevel% equ 0 (
    echo Python è installato.
) else (
    echo Python non è installato. Procedo con l'installazione.

    :: Scarica ed esegui l'installer di Python
    echo Download dell'installer di Python...
    powershell -Command "Invoke-WebRequest -Uri https://www.python.org/ftp/python/3.11.5/python-3.11.5-amd64.exe -OutFile python_installer.exe"
    echo Installazione di Python...
    start /wait python_installer.exe /quiet InstallAllUsers=1 PrependPath=1
    del python_installer.exe

    :: Verifica se Python è stato installato correttamente
    where python >nul 2>nul
    if %errorlevel% neq 0 (
        echo Errore: Python non è stato installato correttamente.
        exit /b 1
    )
)

:: Esegui setup.py con Python
echo Esecuzione di setup.py...
python setup.py
