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

# public stuff
set(ENGINE_SOURCES
    public/geGameObject.cpp
    public/geGame.cpp
)

set(ENGINGE_HEADERS
    include/game-engine/geGameObject.hpp
    include/game-engine/geGame.hpp.hpp
)
# private stuff
set(ENGINE_PRIVATE_SOURCES
        private/Game.cpp
        private/GameObject.cpp
        private/Component.cpp
        private/ObjectManager.cpp
        private/SceneManager.cpp
        private/Scene.cpp
        private/utils.cpp
        private/components/SpriteComponent.cpp
        private/components/SpriteAnimatedComponent.cpp
        private/graphics/GraphicsEngine.cpp
        private/graphics/Graphic.cpp
        private/graphics/Shader.cpp
        private/InputManager.cpp
        private/events/Observer.hpp
        private/events/Subject.hpp
        private/graphics/GraphicHolder.cpp
        private/Screen.cpp
        private/physics/PhysicsEngine.cpp
        private/physics/Collider.cpp
        private/components/ColliderComponent.cpp
        private/physics/DebugView.cpp
        private/graphics/font/Font.cpp
        private/graphics/font/Text.cpp
        private/graphics/font/FontManager.cpp
        private/components/TextComponent.cpp
        private/audio/AudioEngine.cpp
        private/audio/AudioSource.cpp
        private/audio/AudioBuffer.cpp
        private/components/AudioComponent.cpp
        private/Random.cpp)

add_library(GameEngine SHARED ${ENGINE_SOURCES} ${ENGINE_PRIVATE_SOURCES})
set_target_properties(GameEngine PROPERTIES
        VERSION 0.0.1
        PUBLIC_HEADER "${ENGINGE_HEADERS}"
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )

target_include_directories(GameEngine PUBLIC include)

target_link_libraries(GameEngine PUBLIC
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

install(TARGETS GameEngine
        ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/GameEngine/lib
        LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/GameEngine/lib
        PUBLIC_HEADER DESTINATION ${CMAKE_BINARY_DIR}/GameEngine/include)