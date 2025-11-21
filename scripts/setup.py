import os
import sys
import subprocess
import json
import time
import platform
import shutil
    
    
def generate_vscode_project():
    # Esegui CMake per generare il progetto
    cmake_command = "cmake -G 'Ninja' .."  # O usa "Unix Makefiles" o "Ninja" a seconda del tuo setup
    try:
        subprocess.run(cmake_command, shell=True, check=True)
        print("CMake project generated.")
    except subprocess.CalledProcessError as e:
        print(f"Error generating CMake project: {e}")

def check_and_install(package, check_cmd, install_cmd):
    try:
        subprocess.run(check_cmd, shell=True, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        print(f"{package} is already installed.")
    except subprocess.CalledProcessError:
        print(f"Installing {package}...")
        subprocess.run(install_cmd, shell=True, check=True)
        
def setup_vscode():
    # Crea la cartella .vscode se non esiste
    if not os.path.exists(".vscode"):
        os.makedirs(".vscode")

    # Determina il sistema operativo
    system = platform.system()

    # Configurazioni specifiche per ogni sistema
    if system == "Windows":
        mi_mode = "cppvsdbg"  # Debugger per MSVC
        compiler_path = "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/Llvm/bin/clang++.exe"
        executable_name = "TestProject.exe"
        build_command = "msbuild ${workspaceFolder}/build/TestProject.sln /p:configuration=Debug"
        include_paths = [
            "${workspaceFolder}/**",
            "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/**/include",
            "C:/mingw64/include"
        ]
        intellisense_mode = "msvc-x64"

    elif system == "Linux":
        mi_mode = "gdb"
        compiler_path = shutil.which("g++") or "/usr/bin/g++"
        executable_name = "TestProject"
        build_command = "cd ${workspaceFolder}/build && make"
        include_paths = [
            "${workspaceFolder}/**",
            "/usr/include",
            "/usr/local/include",
            "/usr/include/c++/11"
        ]
        intellisense_mode = "gcc-x64"

    elif system == "Darwin":  # macOS
        mi_mode = "lldb"
        compiler_path = "/usr/bin/clang++"
        executable_name = "TestProject"
        build_command = "cd ${workspaceFolder}/build && ninja" if shutil.which("ninja") else "cd ${workspaceFolder}/build && make"
        include_paths = [
            "${workspaceFolder}/**",
            "/usr/local/include",
            "/opt/homebrew/include",  # Per Homebrew su macOS ARM
            "/Library/Developer/CommandLineTools/usr/include",
            "/usr/include/c++/v1"
        ]
        intellisense_mode = "clang-x64"

    else:
        print("⚠️ Sistema operativo non supportato!")
        return

    executable_path = f"${{workspaceFolder}}/build/{executable_name}"

    # Configurazione per il debug (launch.json)
    launch_json = {
        "version": "0.2.0",
        "configurations": [
            {
                "name": "Debug",
                "type": "cppdbg",
                "request": "launch",
                "program": executable_path,
                "args": [],
                "stopAtEntry": False,
                "cwd": "${workspaceFolder}",
                "environment": [],
                "externalConsole": False,
                "MIMode": mi_mode,
                "setupCommands": [
                    {
                        "description": "Enable pretty-printing",
                        "text": "-enable-pretty-printing",
                        "ignoreFailures": True
                    }
                ],
                "preLaunchTask": "build"
            }
        ]
    }

    # Configurazione per la build (tasks.json)
    tasks_json = {
        "version": "2.0.0",
        "tasks": [
            {
                "label": "build",
                "type": "shell",
                "command": build_command,
                "group": {
                    "kind": "build",
                    "isDefault": True
                },
                "presentation": {
                    "reveal": "always"
                },
                "problemMatcher": ["$gcc", "$clang", "$msCompile"]
            }
        ]
    }

    # Configurazione IntelliSense (c_cpp_properties.json)
    c_cpp_properties = {
        "configurations": [
            {
                "name": system,
                "includePath": include_paths,
                "defines": [],
                "compilerPath": compiler_path,
                "cStandard": "c11",
                "cppStandard": "c++17",
                "intelliSenseMode": intellisense_mode
            }
        ],
        "version": 4
    }

    # Salva i file JSON
    with open(".vscode/launch.json", "w") as f:
        json.dump(launch_json, f, indent=4)

    with open(".vscode/tasks.json", "w") as f:
        json.dump(tasks_json, f, indent=4)

    with open(".vscode/c_cpp_properties.json", "w") as f:
        json.dump(c_cpp_properties, f, indent=4)

    print("✅ VS Code configuration generated successfully!")


def log_installed_path(package, check_cmd, log_file):
    try:
        # Log prefix
        prefix_result = subprocess.run(f"pkg-config --variable=prefix {package}", shell=True, check=True, capture_output=True, text=True)
        with open(log_file, "a") as log:
            log.write(f"{package} prefix: {prefix_result.stdout.strip()}\n")
        
        # Log cflags
        cflags_result = subprocess.run(f"pkg-config --cflags {package}", shell=True, check=True, capture_output=True, text=True)
        with open(log_file, "a") as log:
            log.write(f"{package} cflags: {cflags_result.stdout.strip()}\n")
        
        # Log libs
        libs_result = subprocess.run(f"pkg-config --libs {package}", shell=True, check=True, capture_output=True, text=True)
        with open(log_file, "a") as log:
            log.write(f"{package} libs: {libs_result.stdout.strip()}\n")
        
        # Log version
        version_result = subprocess.run(f"pkg-config --modversion {package}", shell=True, check=True, capture_output=True, text=True)
        with open(log_file, "a") as log:
            log.write(f"{package} version: {version_result.stdout.strip()}\n")
        
        print(f"{package} paths logged.")
    except subprocess.CalledProcessError as e:
        print(f"Failed to retrieve {package} path: {e}")
        with open(log_file, "a") as log:
            log.write(f"Failed to retrieve {package} path: {e}\n")

def get_mac_architecture():
    try:
        result = subprocess.run(["sysctl", "-n", "machdep.cpu.brand_string"], capture_output=True, text=True)
        cpu_info = result.stdout.strip().lower()

        if "apple" in cpu_info:
            return "arm64"
        else:
            return platform.machine()
    except Exception:
        return platform.machine()

def install_dependencies():
    if sys.platform == "darwin":
        # Check and install dependencies for macOS
        try:
            subprocess.run("brew --version", shell=True, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            print("Homebrew is already installed.")
        except subprocess.CalledProcessError:
            print("Installing Homebrew...")
            subprocess.run("/bin/bash -c \"$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)\"", shell=True, check=True)
            os.environ["PATH"] += ":/opt/homebrew/bin"

def check_xcode_installation():
    try:
        result = subprocess.run("xcode-select -p", shell=True, check=True, capture_output=True, text=True)
        return True
    except subprocess.CalledProcessError:
        return False

def check_visual_studio_installation():
    try:
        # Controlla le versioni di Visual Studio installate
        result = subprocess.run("vswhere -all -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath", shell=True, check=True, capture_output=True, text=True)
        return result.stdout.strip().split('\n')[0] if result.stdout.strip() else None
    except subprocess.CalledProcessError:
        return None

def get_xcode_version():
    try:
        # Ottieni il percorso di Xcode
        xcode_path = subprocess.run("xcode-select -p", shell=True, check=True, capture_output=True, text=True).stdout.strip()
        
        # Ottieni la versione di Xcode
        version_result = subprocess.run(f"{xcode_path}/usr/bin/xcodebuild -version", shell=True, check=True, capture_output=True, text=True)
        version_line = version_result.stdout.strip().split('\n')[0]
        
        # Estrai il numero di versione
        version = version_line.split()[-1]
        major_version = version.split('.')[0]
        
        return major_version
    except subprocess.CalledProcessError:
        return None

def configure_cmake_for_mac():
    if sys.platform == "darwin":
        arch = get_mac_architecture()  
        print(f"Using architecture: {arch}")
        
        # Se Xcode è installato, genera un progetto Xcode
        if check_xcode_installation():
            xcode_version = get_xcode_version()
            if xcode_version:
                print(f"Xcode {xcode_version} detected, generating Xcode project...")
                try:
                    cmake_command = f'cmake .. -G "Xcode" -DCMAKE_OSX_ARCHITECTURES="{arch}" -DCMAKE_CXX_FLAGS=-Wno-error=deprecated-anon-enum-enum-conversion -DCMAKE_XCODE_GENERATOR_VERSION={xcode_version}'
                    subprocess.run(cmake_command, shell=True, check=True)
                except subprocess.CalledProcessError:
                    print(f"Failed to generate Xcode {xcode_version} project, trying with Unix Makefiles...")
                    cmake_command = f'cmake .. -G "Unix Makefiles" -DCMAKE_OSX_ARCHITECTURES="{arch}" -DCMAKE_CXX_FLAGS=-Wno-error=deprecated-anon-enum-enum-conversion'
                    subprocess.run(cmake_command, shell=True, check=True)
            else:
                print("Could not determine Xcode version, using Unix Makefiles...")
                cmake_command = f'cmake .. -G "Unix Makefiles" -DCMAKE_OSX_ARCHITECTURES="{arch}" -DCMAKE_CXX_FLAGS=-Wno-error=deprecated-anon-enum-enum-conversion'
                subprocess.run(cmake_command, shell=True, check=True)
        else:
            print("Xcode not detected, using Unix Makefiles...")
            cmake_command = f'cmake .. -G "Unix Makefiles" -DCMAKE_OSX_ARCHITECTURES="{arch}" -DCMAKE_CXX_FLAGS=-Wno-error=deprecated-anon-enum-enum-conversion'
            subprocess.run(cmake_command, shell=True, check=True)
    else:
        # Per Windows, controlla Visual Studio
        vs_path = check_visual_studio_installation()
        if vs_path:
            print("Visual Studio detected, generating Visual Studio solution...")
            # Usa la versione di Visual Studio rilevata
            cmake_command = 'cmake .. -G "Visual Studio"'
        else:
            print("Visual Studio not detected, using default generator...")
            cmake_command = "cmake .. -G \"MinGW Makefiles\""
        subprocess.run(cmake_command, shell=True, check=True)

def check_vscode_installation():
    try:
        # Prova a eseguire il comando 'code' per verificare se VSCode è installato
        subprocess.run("code --version", shell=True, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        return True
    except subprocess.CalledProcessError:
        return False

def build_and_install():
    print("Building project...")
    os.makedirs("build", exist_ok=True)
    os.chdir("build")
    
    with open("../setup_log.txt", "a") as f:
        f.write("\n=== CMake Configuration ===\n")
        configure_cmake_for_mac()
        f.write("\n=== Build Process ===\n")

        # Primo comando per il build
        try:
            subprocess.run(f"xcodebuild -project TestProject.xcodeproj -scheme TestProject -verbose", shell=True, check=True, stdout=f)
            print("Build succeeded with the default scheme.")
        except subprocess.CalledProcessError:
            print("Build failed with the default scheme. Trying Debug configuration...")
            
            # Se fallisce, prova con la configurazione Debug
            subprocess.run(f"xcodebuild -project TestProject.xcodeproj -scheme TestProject -configuration Debug build", shell=True, check=True, stdout=f)
        
        # Apri il progetto Xcode
        print("Opening Xcode project...")
        subprocess.run("open TestProject.xcodeproj", shell=True, check=True)

        # Imposta lo schema sul target TestProject (Xcode gestirà questo automaticamente quando apri il progetto)
        print("Setting scheme to 'TestProject' and preparing to run the project...")
        
        # Esegui l'apertura del progetto con Xcode (la configurazione del target sarà impostata tramite l'interfaccia)
        subprocess.run("open -a Xcode TestProject.xcodeproj", shell=True, check=True)
        
        print("Project is ready to be run in Xcode.")


def open_appropriate_ide():
    # Open the appropriate IDE depending on the platform
    if sys.platform == "win32":
        vs_path = check_visual_studio_installation()
        if vs_path:
            print("Opening Visual Studio solution...")
            subprocess.run("explorer *.sln", shell=True)
    elif sys.platform == "darwin":
        if check_xcode_installation():
            print("Opening Xcode project...")
            subprocess.run("open *.xcodeproj", shell=True)
    
    # Apri VSCode solo se è installato
    if check_vscode_installation():
        print("Opening VSCode...")
        subprocess.run("code .", shell=True)
    else:
        print("VSCode is not installed. Skipping VSCode launch...")

if __name__ == "__main__":
    log_file = "setup_log.txt"
    
    # Apriamo il file in modalità write e append
    with open(log_file, "w") as f:
        f.write("=== Setup Log Started ===\n")
        f.write(f"Platform: {sys.platform}\n")
        f.write(f"Python Version: {sys.version}\n\n")
    
    # Install dependencies (if macOS, install glfw and glew)
    install_dependencies()
    
    # Initialize and update git submodules
    print("Initializing and updating submodules...")
    with open(log_file, "a") as f:
        f.write("\n=== Git Submodules ===\n")
    subprocess.run("git submodule update --init --recursive", shell=True, check=True, stdout=open(log_file, "a"))
    
    # Build the project
    with open(log_file, "a") as f:
        f.write("\n=== Build Process ===\n")
    build_and_install()
    
    # Setup VSCode configuration
    setup_vscode()
    
    # Open the appropriate IDE
    open_appropriate_ide()
    
    with open(log_file, "a") as f:
        f.write("\n=== Setup Complete ===\n")
