import os
import sys
import subprocess
import json
import time
import platform
import shutil


PROJECT_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), ".."))
os.chdir(PROJECT_ROOT)
print(f"✅ Project root set to: {PROJECT_ROOT}")


def generate_vscode_project():
    cmake_command = "cmake -G Ninja .."
    try:
        subprocess.run(cmake_command, shell=True, check=True)
        print("CMake project generated.")
    except subprocess.CalledProcessError as e:
        print(f"Error generating CMake project: {e}")


def check_and_install(package, check_cmd, install_cmd):
    try:
        subprocess.run(check_cmd, shell=True, check=True,
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        print(f"{package} is already installed.")
    except subprocess.CalledProcessError:
        print(f"Installing {package}...")
        subprocess.run(install_cmd, shell=True, check=True)


def setup_vscode():
    if not os.path.exists(".vscode"):
        os.makedirs(".vscode")

    system = platform.system()

    if system == "Windows":
        mi_mode = "cppvsdbg"
        compiler_path = "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/Llvm/bin/clang++.exe"
        executable_name = "TestProject.exe"
        build_command = "cmake --build ${workspaceFolder}/build"
        include_paths = [
            "${workspaceFolder}/**",
            "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/**/include",
        ]
        intellisense_mode = "msvc-x64"

    elif system == "Linux":
        mi_mode = "gdb"
        compiler_path = shutil.which("g++") or "/usr/bin/g++"
        executable_name = "TestProject"
        build_command = "cmake --build ${workspaceFolder}/build"
        include_paths = [
            "${workspaceFolder}/**",
            "/usr/include",
            "/usr/local/include",
        ]
        intellisense_mode = "gcc-x64"

    elif system == "Darwin":
        mi_mode = "lldb"
        compiler_path = "/usr/bin/clang++"
        executable_name = "TestProject"
        build_command = "cmake --build ${workspaceFolder}/build"
        include_paths = [
            "${workspaceFolder}/**",
            "/usr/local/include",
            "/opt/homebrew/include",
        ]
        intellisense_mode = "clang-x64"

    else:
        print("⚠️ Sistema operativo non supportato!")
        return

    executable_path = f"${{workspaceFolder}}/build/{executable_name}"

    launch_json = {
        "version": "0.2.0",
        "configurations": [{
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
            "setupCommands": [{
                "description": "Enable pretty-printing",
                "text": "-enable-pretty-printing",
                "ignoreFailures": True
            }],
            "preLaunchTask": "build"
        }]
    }

    tasks_json = {
        "version": "2.0.0",
        "tasks": [{
            "label": "build",
            "type": "shell",
            "command": build_command,
            "group": {"kind": "build", "isDefault": True},
            "presentation": {"reveal": "always"},
            "problemMatcher": ["$gcc", "$clang", "$msCompile"]
        }]
    }

    c_cpp_properties = {
        "configurations": [{
            "name": system,
            "includePath": include_paths,
            "defines": [],
            "compilerPath": compiler_path,
            "cStandard": "c11",
            "cppStandard": "c++20",
            "intelliSenseMode": intellisense_mode
        }],
        "version": 4
    }

    with open(".vscode/launch.json", "w") as f:
        json.dump(launch_json, f, indent=4)

    with open(".vscode/tasks.json", "w") as f:
        json.dump(tasks_json, f, indent=4)

    with open(".vscode/c_cpp_properties.json", "w") as f:
        json.dump(c_cpp_properties, f, indent=4)

    print("✅ VS Code configuration generated successfully!")


def get_mac_architecture():
    try:
        result = subprocess.run(["uname", "-m"], capture_output=True, text=True)
        return result.stdout.strip()
    except Exception:
        return "arm64"


def check_xcode_installation():
    try:
        subprocess.run("xcode-select -p", shell=True, check=True,
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        return True
    except subprocess.CalledProcessError:
        return False


def configure_cmake_for_mac_or_generic():
    os.makedirs("build", exist_ok=True)
    os.chdir("build")

    if sys.platform == "darwin":
        arch = get_mac_architecture()
        print(f"✅ macOS Architecture: {arch}")

        if check_xcode_installation():
            print("✅ Xcode detected → Generating Xcode project")
            cmake_cmd = f'cmake .. -G "Xcode" -DCMAKE_OSX_ARCHITECTURES="{arch}"'
        else:
            cmake_cmd = f'cmake .. -G "Unix Makefiles" -DCMAKE_OSX_ARCHITECTURES="{arch}"'

    elif sys.platform == "win32":
        cmake_cmd = 'cmake .. -G "Visual Studio 17 2022"'

    else:
        cmake_cmd = 'cmake .. -G "Unix Makefiles"'

    subprocess.run(cmake_cmd, shell=True, check=True)
    os.chdir(PROJECT_ROOT)

def configure_cmake_for_mac_or_generic():
    os.makedirs("build", exist_ok=True)
    os.chdir("build")

    if sys.platform == "darwin":
        arch = get_mac_architecture()
        print(f"✅ macOS Architecture: {arch}")

        # ✅ Forziamo la root come working dir per Xcode
        extra_flags = f'-DCMAKE_RUNTIME_OUTPUT_DIRECTORY="{PROJECT_ROOT}/bin"'
        extra_flags += f' -DCMAKE_LIBRARY_OUTPUT_DIRECTORY="{PROJECT_ROOT}/bin"'
        extra_flags += f' -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY="{PROJECT_ROOT}/lib"'

        if check_xcode_installation():
            print("✅ Xcode detected → Generating Xcode project")
            cmake_cmd = (
                f'cmake .. -G "Xcode" '
                f'-DCMAKE_OSX_ARCHITECTURES="{arch}" '
                f'{extra_flags}'
            )
        else:
            cmake_cmd = (
                f'cmake .. -G "Unix Makefiles" '
                f'-DCMAKE_OSX_ARCHITECTURES="{arch}" '
                f'{extra_flags}'
            )

    elif sys.platform == "win32":
        cmake_cmd = 'cmake .. -G "Visual Studio 17 2022"'

    else:
        cmake_cmd = 'cmake .. -G "Unix Makefiles"'

    print(f"🛠️ Running: {cmake_cmd}")
    subprocess.run(cmake_cmd, shell=True, check=True)
    os.chdir(PROJECT_ROOT)

def fix_xcode_working_directory():
    if sys.platform != "darwin":
        return

    xcodeproj_found = False

    for file in os.listdir("build"):
        if file.endswith(".xcodeproj"):
            xcodeproj_found = True
            pbxproj_path = os.path.join("build", file, "project.pbxproj")
            break

    if not xcodeproj_found:
        print("⚠️ No Xcode project found to fix working directory.")
        return

    print("✅ Fixing Xcode working directory for shader loading...")

    with open(pbxproj_path, "r") as f:
        content = f.read()

    if "WORKING_DIRECTORY" in content:
        print("ℹ️ Xcode working directory already set.")
        return

    insert_string = f'''
				WORKING_DIRECTORY = "{PROJECT_ROOT}";
'''

    content = content.replace(
        "DEBUG_INFORMATION_FORMAT = dwarf;",
        "DEBUG_INFORMATION_FORMAT = dwarf;\n" + insert_string
    )

    with open(pbxproj_path, "w") as f:
        f.write(content)

    print("✅ Xcode working directory fixed successfully!")


def build_and_install():
    print("✅ Building project...")
    configure_cmake_for_mac_or_generic()
    fix_xcode_working_directory()

    subprocess.run("cmake --build build", shell=True, check=True)



def open_appropriate_ide():
    if sys.platform == "darwin" and check_xcode_installation():
        subprocess.run("open build/*.xcodeproj", shell=True)

    if shutil.which("code"):
        subprocess.run("code .", shell=True)


if __name__ == "__main__":
    log_file = "setup_log.txt"

    with open(log_file, "w") as f:
        f.write("=== Setup Log Started ===\n")
        f.write(f"Platform: {sys.platform}\n")
        f.write(f"Python Version: {sys.version}\n\n")

    print("✅ Initializing and updating submodules...")
    subprocess.run("git submodule update --init --recursive", shell=True, check=True)

    build_and_install()
    setup_vscode()
    open_appropriate_ide()

    with open(log_file, "a") as f:
        f.write("\n=== Setup Complete ===\n")

    print("\n🎉 SETUP SUCCESSEFULLY COMPLETED!")
