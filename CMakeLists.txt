cmake_minimum_required(VERSION 3.12)
project(GameEngine)

set(CMAKE_CXX_STANDARD 11)

add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")


# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to TRUE.
# See https://cmake.org/cmake/help/v3.4/variable/CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS.html
# See https://blog.kitware.com/create-dlls-on-windows-without-declspec-using-new-cmake-export-all-feature/
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

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
        public/geGame.cpp
        public/geComponent.cpp
        public/geEnvironment.cpp
        public/InputManager.cpp
        public/components/SpriteComponent.cpp
        public/components/SpriteAnimatedComponent.cpp
        public/components/ColliderComponent.cpp
        public/components/TextComponent.cpp
        public/components/AudioComponent.cpp
        public/Random.cpp
        public/geAudio.cpp
        public/geIO.cpp)

# private stuff
set(ENGINE_PRIVATE_SOURCES
        private/Game.cpp
        private/GameObject.cpp
        private/ObjectManager.cpp
        private/SceneManager.cpp
        private/Scene.cpp
        private/utils.cpp
        private/graphics/GraphicsEngine.cpp
        private/graphics/Graphic.cpp
        private/graphics/Shader.cpp
        private/graphics/GraphicHolder.cpp
        private/Screen.cpp
        private/physics/PhysicsEngine.cpp
        private/physics/Collider.cpp
        private/physics/DebugView.cpp
        private/graphics/font/Font.cpp
        private/graphics/font/Text.cpp
        private/graphics/font/FontManager.cpp
        private/audio/AudioEngine.cpp
        private/audio/AudioSource.cpp
        private/audio/AudioBuffer.cpp
        private/Data.cpp
        )

add_library(GameEngine ${ENGINE_SOURCES} ${ENGINE_PRIVATE_SOURCES})

include(GenerateExportHeader)
generate_export_header(GameEngine)
set_target_properties(GameEngine PROPERTIES
        VERSION 0.0.1
        )

target_include_directories(GameEngine PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

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

install(TARGETS GameEngine EXPORT GameEngineTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

enable_testing()
add_subdirectory(test)