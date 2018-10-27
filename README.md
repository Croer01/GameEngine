# Game Engine (Provisional name)

This is a more refined game engine created after the [first iteration](https://github.com/Croer01/tic-tac-toe).

The idea is create a custom engine to be able to use in my own games.  
Along the time I will develop tools and write a documentation to work easy with it.
Also I will do test to check all parts of the engine works properly and it will be robust.

## Dependencies
 - SDL2 (core and image)
 - GLEW
 - GLM
 - Yaml cpp
 
 ## Environment
 Currently, I use vcpkg to manage dependencies, It's a cross platform package manager (developed by Microsoft).  
 To use and install follow the vcpkg instructions on its repository.
 
 After you will have set up vcpkg, execute the next command 
 
 ~~~ bash
 vcpkg install --triplet x64-windows glew glm sdl2-image sdl2 yaml-cpp
 ~~~
 
 _Note: Before you execute this command, you must check your target architecture to ensure install proper version of packages (the --triplet option)_
 
 Finally, you only need to execute CMake and build:
 
 ~~~ bash
 # inside project root
 mkdir outd && cd outd
 cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=[root_vcpkg_directory]/scripts/buildsystems/vcpkg.cmake
 cmake --build .
 ~~~
 
 the CMAKE_TOOLCHAIN_FILE is required to vcpkg so that cmake will be able to looking for all project dependencies installed by vcpkg.