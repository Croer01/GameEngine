# Game Engine (Provisional name)
This is a more refined game engine created after the [first iteration](https://github.com/Croer01/tic-tac-toe).

The idea is create a custom engine to be able to use in my own games.  
Along the time I will develop tools and write a documentation.
Also I will do tests to check all parts of the engine works properly and improve its robustness.

## GameEngine Dependencies
 - SDL2 (core and image)
 - GLEW
 - GLM
 - Yaml cpp
 - Box2D
 - FreeType
 - utf8-cpp 
 - OpenAl-soft (not OpenAl)
 - Libsndfile
 - Gtest (test framework)
 - zlib
 
## Editor Dependencies [Abandonned]
 - GameEngine
 - SDL2 (core and image)
 - GLEW
 - Yaml cpp
 - ImGui
 - TinyFileDialogComponentRegister
 - Boost filesystem
 
The editor is abandonned. I will develop another version "editorV2".  
At the moment I developed some tools to check usability but are not ready to use.  
The [WhishlistEditorV2.md](WhishlistEditorV2.md) contains a list of things I want to develop.
 
## Setup environment
This project use [meson](https://mesonbuild.com/)
Previously I tried [vcpkg](https://github.com/Microsoft/vcpkg) and [Conan](https://conan.io) to manage dependencies.  
The first one it was tricky to manage packages because need to checkout the full repository and checkout the multiple
subfolders to fix the version of the multiple dependencies of this project.
The second one is much better than vcpkg, but it is very tied to CMake and requires to configure the local environment, but as far as
I know it's not possible to share.

install meson and ninja following meson instructions:  
https://mesonbuild.com/Getting-meson.html

### Windows
ensure you have installed Visual Studio C++ tools

### Linux
install gl development libraries required by glew on Linux.
install openal development libraries for Linux. The current meson wrap doesn't support Linux  
install libsndfile.  This recomended to use the libraries provided by your system package supplier
install libxi-dev required by sdl2

eg: for Debian/Ubuntu
~~~bash
sudo apt-get install libgl-dev freeglut3-dev libopenal-dev libsndfile1-dev libxi-dev
~~~

## Build with Meson
After you will set up Meson, execute the next command in the directory root:
~~~ bash
# The argument is the name of the build folder, in this case "builddir"
meson builddir
cd builddir
# you can execute "ninja" but meson recomends to use the "meson compile" command because set up automatically the required environment to build the project
meson compile
# finally to run tests. Meson devenv set up all the dependencies to be able to work without need to install them
meson devenv meson test
~~~

_NOTE: more info related to how to build a project with meson can be found here: https://mesonbuild.com/Quick-guide.html_
