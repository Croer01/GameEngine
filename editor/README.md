#GameEngine Editor

## Dependencies
 - SDL2 (core and image)
 - GLEW
 - Yaml cpp
 - ImGui
 - TinyFileDialog
 
 ## Environment
 Currently, I use [vcpkg](https://github.com/Microsoft/vcpkg) to manage dependencies, It's a cross platform package manager (developed by Microsoft).  
 To use and install follow the vcpkg instructions on its repository.
 
 __Important: The current version I use to get dependencies is from git commit ff9e982d26a5041670d700adb3fd26eb5139cb43.__
 
 After you will have set up vcpkg, execute the next command 
 
 ~~~ bash
 vcpkg install --triplet x64-windows glew sdl2 yaml-cpp imgui tinyfiledialogs 
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
