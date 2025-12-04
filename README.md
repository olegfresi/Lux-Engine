# Lux Engine
![](/images/test.png "")
#### Lux Engine is a C++20 graphics engine built to explore modern rendering pipelines with a strong focus on subsystem modularity.


## Key Features
- **Configurable Rendering Pipeline**
- **Instanced Rendering**
- **Forward and Deferred Rendering Pipeline**
- **Shader System**
- **Material System**
- **Scene Editor** 
- **Terrain Rendering**
- **Test Suite**
- **Multi-platform support**

## Platforms Supported  
  - **Windows**
  - **macOS**
  - **Linux**

## Rendering API Supported
  - OpenGL 4.1 Core Profile
## Next to come
 - Metal

## Requirements
- CMake 3.27 or higher
- C++20 compiler
- Python 3.10
- Git

# Automatic Build Instructions
The only thing you need to have installed is git, everything else will be configured automatically
Open your terminal on Linux or MacOS or powershell on Windows then copy and paste the following commands on MacOS or Linux
```bash 
cd Desktop &&
git clone https://github.com/olegfresi/Lux-Engine.git "Lux Engine" --recursive &&
chmod +x setup.sh &&
cd Lux\ Engine/scripts &&
./setup.sh
```
On Windows open cmd and navigate to the Lux Engine directory and run the following command
```powershell
cd Desktop &&
git clone https://github.com/olegfresi/Lux-Engine.git "Lux Engine" --recursive &&
cd Lux\ Engine/scripts &&
./windowsSetup.bat
```

## Enjoy the engine!

# Manual Build Instructions

### Build (macOS & Linux)
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
```
```

## Build (Windows)
```powershell
cmake -S . -B build `
      -G "Visual Studio 17 2022" `
      -A x64 `
      -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

## Useful CMake Options
- `-DENABLE_TESTING=ON|OFF` toggles the GoogleTest targets (default: ON in Debug). <br><br>
- `-DRUN_TESTS=ON|OFF` runs automated tests before launching the app (default: ON in Debug). <br><br>
- `-DCMAKE_BUILD_TYPE=Debug|Release|RelWithDebInfo` selects the build profile for single-config generators. <br><br>

Debug example with tests:
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DENABLE_TESTING=ON -DRUN_TESTS=ON
cmake --build build
ctest --test-dir build
```

## Setup Scripts
- `scripts/setup.sh` – automatic MacOS/Linux setup <br><br>
- `scripts/windowsSetup.bat` – automatic windows setup <br><br>
- `scripts/setup.py` – Python setup script <br><br>

## Assets
3D assets,shaders, materials and textures are stored in `assets/` and can be replaced with your own.

## Known Issues
- Feel free to write there! 

## Contributing
You MUST read Coding Conventions established for this C++ project
<a href="/Coding Conventions.html" target="_blank"> Coding Conventions </a>
Try to be consistent along your work


## License
The project is distributed under the terms described in `LICENSE.md`.