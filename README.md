# Platformer Game

![render-engine.jpeg](docs%2Fimages%2Frender-engine.jpeg)
![3d_viewport_with_grid.jpeg](docs%2Fimages%2F3d_viewport_with_grid.jpeg)

A render engine based on the [Learn OpenGL](https://learnopengl.com/) book.

# Dependencies
**Ubuntu:**
```
sudo apt install libxinerama-dev x11-xserver-utils libxss-dev x11proto-xinerama-dev libxcb-xinerama0-dev libxrandr-dev libxcursor-dev libxi-dev
```

# Usage
## Linux
Execute the included bash script to get started:
```
bash run.sh
```

## Build it yourself
Execute the following CMake commands:
```
mkdir build
cmake -S . -B build
cmake --build build
```

Build files are generated in the *build* directory. On Windows, open the Visual Studio solution and build and run the
executable.

On Linux, use the following commands:
```
cd build
make
cd bin
./MyApplication
```

# Support
The engine aims to support the following platforms:
- Linux (primary):
  - Arch
  - Debian (not tested)
  - Gentoo (not tested)
- Windows 11 (not tested)
- Windows 10 (not tested)
- BSD (not tested)

Due to the deprecation of OpenGL on MacOS, the said operating system is not supported.

# Dependencies
- [glad](https://github.com/Dav1dde/glad)
- [glfw](https://github.com/glfw/glfw)
- [glm](https://github.com/g-truc/glm)
- [spdlog](https://github.com/gabime/spdlog)
- [stb](https://github.com/nothings/stb)
- [Dear ImGui](https://github.com/ocornut/imgui)
