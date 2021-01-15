# This CMakeLists is based on vcpkg implementation https://github.com/microsoft/vcpkg/blob/ff9e982d26a5041670d700adb3fd26eb5139cb43/ports/tinyfiledialogs/portfile.cmake
CMAKE_MINIMUM_REQUIRED(VERSION 3.4)
PROJECT(tinyfiledialogs C)
# This small hack might be useful to guarantee proper /MT /MD linkage
# in MSVC if the packaged project doesn't have variables to set it properly
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

ADD_LIBRARY(tinyfiledialogs tinyfiledialogs.c)

INSTALL(
    TARGETS tinyfiledialogs
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)
