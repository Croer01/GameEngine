cmake_minimum_required(VERSION 3.12)
project(SpaceInvaders)

set(CMAKE_CXX_STANDARD 11)
# dependencies
#SDL2_image
find_path(SDL2_IMAGE_INCLUDE SDL2/SDL_image.h)
find_library(SDL2_IMAGE_LIBRARY SDL2_image)


find_package(SDL2 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE} ${GLEW_INCLUDE_DIR} ${glm_INCLUDE_DIR})
include_directories(${SDL2_ROOT}/include)

# source files
set(ENGINE_SOURCES src/Game.cpp
        src/GameObject.cpp
        src/Component.cpp
        src/ObjectManager.cpp
        src/Builder.cpp
        src/SceneManager.cpp
        src/Scene.cpp
        src/utils.cpp
        src/components/SpriteComponent.cpp
        src/graphics/GraphicsEngine.cpp
        src/graphics/Graphic.cpp
        src/graphics/Shader.cpp
        src/InputManager.cpp
        src/events/Subject.hpp
        src/graphics/GraphicHolder.cpp
        src/Screen.cpp)

set(GAME_SOURCES main.cpp EnemyComponent.cpp EnemyManagerComponent.cpp)

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
        yaml-cpp
        GLEW::GLEW
        glm
        ${OPENGL_LIBRARIES})

ADD_CUSTOM_TARGET(copyData ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/data
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data
        )
ADD_CUSTOM_TARGET(copyConf ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/conf
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/conf ${CMAKE_BINARY_DIR}/conf
        )