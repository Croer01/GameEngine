cmake_minimum_required(VERSION 3.12)
project(SpaceInvaders)

set(CMAKE_CXX_STANDARD 11)
# dependencies
#SDL2_image
find_path(SDL2_IMAGE_INCLUDE SDL2/SDL_image.h)
find_library(SDL2_IMAGE_LIBRARY SDL2_image)
#Inih
find_path(INIH_INCLUDE ini.h)
find_library(INIH_LIBRARY inih)

find_package(SDL2 CONFIG REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE} ${INIH_INCLUDE})
include_directories(${SDL2_ROOT}/include)

# source files
set(ENGINE_SOURCES src/Game.cpp
        src/GameObject.cpp
        src/Component.cpp src/ObjectManager.cpp src/Builder.cpp src/SceneManager.cpp src/Scene.cpp)

set(GAME_SOURCES main.cpp PrintTextComponent.cpp)

# auto-generated files
set(COMPONENTS_REGISTERED_CPP ${CMAKE_SOURCE_DIR}/componentsRegistered.cpp)
file(GLOB_RECURSE COMPONENTS_REGISTERED_HEADERS "*.hpp")
add_custom_command(
        COMMAND sh ${CMAKE_SOURCE_DIR}/engine_components_gen.sh > ${COMPONENTS_REGISTERED_CPP}
        DEPENDS ${COMPONENTS_REGISTERED_HEADERS}
        OUTPUT ${COMPONENTS_REGISTERED_CPP}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating code to register components into the object manager"
)
set(ENGINE_SOURCES ${ENGINE_SOURCES} ${COMPONENTS_REGISTERED_CPP})

add_executable(SpaceInvaders ${ENGINE_SOURCES} ${GAME_SOURCES})
target_link_libraries(SpaceInvaders PRIVATE
        SDL2::SDL2main
        SDL2::SDL2
        ${SDL2_IMAGE_LIBRARY}
        ${INIH_LIBRARY})

ADD_CUSTOM_TARGET(do_always ALL COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data)