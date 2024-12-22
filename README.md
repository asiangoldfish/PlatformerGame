# Platformer Game

![render-engine.jpeg](docs%2Fimages%2Frender-engine.jpeg)
![3d_viewport_with_grid.jpeg](docs%2Fimages%2F3d_viewport_with_grid.jpeg)

A render engine based on the [Learn OpenGL](https://learnopengl.com/) book.

## Dependencies
**Ubuntu:**
```
sudo apt install libxinerama-dev x11-xserver-utils libxss-dev x11proto-xinerama-dev libxcb-xinerama0-dev libxrandr-dev libxcursor-dev libxi-dev
```

## Usage
### Linux
Execute the included bash script to get started:
```
bash run.sh
```

### Build it yourself
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

## Support
The engine aims to support the following platforms:
- Linux (primary):
  - Arch
  - Debian (not tested)
  - Gentoo (not tested)
- Windows 11 (not tested)
- Windows 10 (not tested)
- BSD (not tested)

Due to the deprecation of OpenGL on MacOS, the said operating system is not supported.

## Testing
We test the Engine using [Doctest](https://github.com/doctest/doctest). It is enabled by default. If the project is built in Release mode, then the test is disabled.

**Building in Release mode:**
```shell
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cd build && make
```

**Building without testing:**<br>
You can also build explicitly without testing, even in Debug mode.
```shell
cmake -S . -B build -DDISABLE_TEST=true
cd build && make
```
Please note if you are not using make, you should replace `make` with the appropriate commands.

If the test is disabled in build time, then the test code is all stripped out of the binary executable.

Once the Engine is built with testing enabled, it embeds doctest's command-line arguments. For starters, the program will run even if the test fails. To disbale this behaviour:

```shell
/path/to/executable --exit
```

This will stop the test even if the test succeeds. To see the rest of the available commands, execute the following command:

```shell
/path/to/executable --help
```

As doctest is embedded into the Engine itself, all targets inherit it. This means if you create your own project and include the Engine's Framework, then you are still able to test your project.

## Dependencies
- [glad](https://github.com/Dav1dde/glad)
- [glfw](https://github.com/glfw/glfw)
- [glm](https://github.com/g-truc/glm)
- [spdlog](https://github.com/gabime/spdlog)
- [stb](https://github.com/nothings/stb)
- [Dear ImGui](https://github.com/ocornut/imgui)
- [Doctest](https://github.com/doctest/doctest)