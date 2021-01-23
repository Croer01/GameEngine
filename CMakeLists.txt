cmake_minimum_required(VERSION 3.12)
project(GameEngine)

set(CMAKE_CXX_STANDARD 11)

add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")


# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to TRUE.
# See https://cmake.org/cmake/help/v3.4/variable/CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS.html
# See https://blog.kitware.com/create-dlls-on-windows-without-declspec-using-new-cmake-export-all-feature/
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS)
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

# public stuff
set(ENGINE_SOURCES
        engine/public/Game.cpp
        engine/public/geComponent.cpp
        engine/public/geEnvironment.cpp
        engine/public/InputManager.cpp
        engine/public/components/SpriteComponent.cpp
        engine/public/components/SpriteAnimatedComponent.cpp
        engine/public/components/ColliderComponent.cpp
        engine/public/components/TextComponent.cpp
        engine/public/components/AudioComponent.cpp
        engine/public/components/GeometryComponent.cpp
        engine/public/components/ui/UIControlComponent.cpp
        engine/public/components/ui/UIButtonComponent.cpp
        engine/public/components/ui/UITextComponent.cpp
        engine/public/components/ui/UITextInputComponent.cpp
        engine/public/components/ui/UIPanelComponent.cpp
        engine/public/components/ui/UIImageComponent.cpp
        engine/public/Random.cpp
        engine/public/geAudio.cpp
        engine/public/geIO.cpp
        engine/public/components/ParticleEmitterComponent.cpp
        engine/public/components/ParticlesPropertiesComponent.cpp
        engine/public/components/SpawnerComponent.cpp
        engine/public/Range.cpp
        engine/public/components/CarColliderComponent.cpp)

# private stuff
set(ENGINE_PRIVATE_SOURCES
        engine/private/GameObject.cpp
        engine/private/ObjectManager.cpp
        engine/private/SceneManager.cpp
        engine/private/Scene.cpp
        engine/private/utils.cpp
        engine/private/graphics/GraphicsEngine.cpp
        engine/private/graphics/Shader.cpp
        engine/private/graphics/GraphicHolder.cpp
        engine/private/Screen.cpp
        engine/private/physics/PhysicsEngine.cpp
        engine/private/physics/Collider.cpp
        engine/private/physics/DebugView.cpp
        engine/private/graphics/font/Font.cpp
        engine/private/graphics/font/Text.cpp
        engine/private/graphics/font/FontManager.cpp
        engine/private/audio/AudioEngine.cpp
        engine/private/audio/AudioSource.cpp
        engine/private/audio/AudioBuffer.cpp
        engine/private/Data.cpp
        engine/private/graphics/Camera.cpp
        engine/private/graphics/MeshData.cpp
        engine/private/graphics/GraphicSprite.cpp
        engine/private/graphics/GraphicGeometry.cpp
        engine/private/Environment.cpp
        engine/private/graphics/FBO.cpp
        engine/private/TimeManager.cpp
        engine/private/pathfinding/AStarPathfinding.cpp
        engine/private/pathfinding/Cell.cpp
        engine/private/pathfinding/Agent.cpp
        engine/private/pathfinding/Path.cpp
        engine/private/graphics/Particle.cpp
        engine/include/game-engine/FiniteStateMachine.hpp
        engine/private/WritableData.cpp)

add_library(GameEngine ${ENGINE_SOURCES} ${ENGINE_PRIVATE_SOURCES})

include(GenerateExportHeader)
generate_export_header(GameEngine)
set_target_properties(GameEngine PROPERTIES
        VERSION 0.0.1
        )

target_include_directories(GameEngine PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/engine/include>
        $<INSTALL_INTERFACE:include>
        )

target_link_libraries(GameEngine PUBLIC
        CONAN_PKG::sdl2
        CONAN_PKG::sdl2_image
        CONAN_PKG::yaml-cpp
        CONAN_PKG::glew
        CONAN_PKG::glm
        CONAN_PKG::opengl
        CONAN_PKG::box2d
        CONAN_PKG::freetype
        CONAN_PKG::openal
        CONAN_PKG::libsndfile
        CONAN_PKG::boost
        CONAN_PKG::utfcpp
        CONAN_PKG::zlib)

install(TARGETS GameEngine EXPORT GameEngineTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

enable_testing()
add_subdirectory(test)

### Editor
# check if tools is already added
if (NOT COMMAND configure_game_directories)
    add_subdirectory(tools)
endif ()

add_executable(GameEngineEditor
        editor/imgui_backend/imgui_impl_opengl3.cpp
        editor/imgui_backend/imgui_impl_sdl.cpp
        editor/imgui_backend/imgui_stdlib.cpp
        editor/editorMain.cpp
        editor/Editor.cpp
        editor/ViewModels.cpp
        editor/CreateProjectEditor.cpp
        editor/GameComponentsProvider.cpp
        editor/ProjectFileDataProvider.cpp
        editor/dialogs/CreateDataFileDialog.cpp
        editor/dialogs/DeleteFileDialog.cpp
        editor/dialogs/SaveAllDialog.cpp
        editor/ProjectDirectory.cpp
        editor/DataFile.cpp
        editor/dialogs/ErrorDialog.cpp
        editor/DataDirectory.cpp
        editor/GameEditor.cpp)

target_compile_definitions(GameEngineEditor PUBLIC -DIMGUI_IMPL_OPENGL_LOADER_GLEW)

if(MSVC)
    target_compile_options(GameEngineEditor PUBLIC /bigobj)
endif ()

target_link_libraries(GameEngineEditor PRIVATE GameEngine
        CONAN_PKG::imgui
        CONAN_PKG::sdl2
        CONAN_PKG::glew
        CONAN_PKG::boost
        CONAN_PKG::yaml-cpp
        CONAN_PKG::opengl
        CONAN_PKG::tinyfiledialogs
        )
