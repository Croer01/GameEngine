# Game Engine (Provisional name)
This is a more refined game engine created after the [first iteration](https://github.com/Croer01/tic-tac-toe).

The idea is create a custom engine to be able to use in my own games.  
Along the time I will develop tools and write a documentation.
Also I will do test to check all parts of the engine works properly and improve its robustness.

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
 - Boost filesystem
 - zlib
 
## Editor Dependencies
 - GameEngine
 - SDL2 (core and image)
 - GLEW
 - Yaml cpp
 - ImGui
 - TinyFileDialogComponentRegister
 - Boost filesystem
 
## Environment
This project use [Conan](https://conan.io) to manage dependencies.  
Previously, I used [vcpkg](https://github.com/Microsoft/vcpkg) to manage dependencies but finally I prefer to move to Conan.  
The main problem I see in vcpkg is that it is tricky to manage the versions of packages (the main goal of a package manager)
because vcpkg requires to use ```git checkout``` to choose the recipes to build the desired version for each dependency.
Also in some cases, It is required to have multiple instances of vcpkgs to preserve some configuration and versions.

Follow the conan instructions to install it in your development platform: https://docs.conan.io/en/latest/installation.html

After you will set up Conan, execute the next command in the directory root:

First export custom packages to build game engine.
~~~ bash
conan export conan-deps/tinyfiledialogs
conan export conan-deps/sdl2-image
~~~

After export, install all the dependencies.  
__Important: the install folder is the same you will build CMake project__
~~~ bash
# Install Debug dependencies
conan install . --install-folder build-debug -s build_type=Debug --build=missing -pr [your conan profile]
# Or install Release dependencies
conan install . --install-folder build-release -s build_type=Release --build=missing -pr [your conan profile]
~~~

_Note: Maybe you will need to tweak Conan's profile to build correctly in your platform.  
Alternatively, there is a script to install dependencies:_ ```install_deps.sh```

Finally, you only need to execute CMake and build:

~~~ bash
# In project root

# Debug build
cd build-debug
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build .

# Or release build
cd build-release
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
~~~
