cmake_minimum_required(VERSION 3.12)
project(GameEngine)

set(CMAKE_CXX_STANDARD 11)

add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

# dependencies
# All the dependencies are managed by vcpkg

#SDL2_image
find_path(SDL2_IMAGE_INCLUDE SDL2/SDL_image.h)
find_library(SDL2_IMAGE_LIBRARY SDL2_image)


find_package(SDL2 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)
find_package(unofficial-box2d CONFIG REQUIRED)
find_package(Freetype REQUIRED)
find_package(LibSndFile CONFIG REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE} ${GLEW_INCLUDE_DIR} ${glm_INCLUDE_DIR})
include_directories(${SDL2_ROOT}/include)

# source files
set(ENGINE_SOURCES Game.cpp
        GameObject.cpp
        Component.cpp
        ObjectManager.cpp
        SceneManager.cpp
        Scene.cpp
        utils.cpp
        components/SpriteComponent.cpp
        components/SpriteAnimatedComponent.cpp
        graphics/GraphicsEngine.cpp
        graphics/Graphic.cpp
        graphics/Shader.cpp
        InputManager.cpp
        events/Observer.hpp
        events/Subject.hpp
        graphics/GraphicHolder.cpp
        Screen.cpp
        physics/PhysicsEngine.cpp
        physics/Collider.cpp
        components/ColliderComponent.cpp
        physics/DebugView.cpp
        graphics/font/Font.cpp
        graphics/font/Text.cpp
        graphics/font/FontManager.cpp
        components/TextComponent.cpp
        audio/AudioEngine.cpp
        audio/AudioSource.cpp
        audio/AudioBuffer.cpp
        components/AudioComponent.cpp
        Random.cpp)

add_library(GameEngine SHARED ${ENGINE_SOURCES})
set_target_properties(GameEngine PROPERTIES
        VERSION 0.0.1
        PUBLIC_HEADER include/api.h
        )

target_include_directories(GameEngine PRIVATE include)

target_link_libraries(GameEngine
        SDL2::SDL2
        ${SDL2_IMAGE_LIBRARY}
        yaml-cpp
        GLEW::GLEW
        glm
        ${OPENGL_LIBRARIES}
        unofficial::box2d::Box2D
        Freetype::Freetype
        OpenAL::OpenAL
        sndfile-shared)