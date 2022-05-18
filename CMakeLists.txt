cmake_minimum_required(VERSION 3.12)
project(GameEngine)

set(CMAKE_CXX_STANDARD 11)

add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")


# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to TRUE.
# See https://cmake.org/cmake/help/v3.4/variable/CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS.html
# See https://blog.kitware.com/create-dlls-on-windows-without-declspec-using-new-cmake-export-all-feature/
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(MSVC)
    set(Boost_USE_STATIC_LIBS        ON)
endif ()

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup(TARGETS)
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

# public stuff
set(ENGINE_SOURCES
        engine/Game.cpp
        engine/geComponent.cpp
        engine/geEnvironment.cpp
        engine/InputManager.cpp
        engine/components/SpriteComponent.cpp
        engine/components/SpriteAnimatedComponent.cpp
        engine/components/ColliderComponent.cpp
        engine/components/TextComponent.cpp
        engine/components/AudioComponent.cpp
        engine/components/GeometryComponent.cpp
        engine/components/ui/UIControlComponent.cpp
        engine/components/ui/UIButtonComponent.cpp
        engine/components/ui/UITextComponent.cpp
        engine/components/ui/UITextInputComponent.cpp
        engine/components/ui/UIPanelComponent.cpp
        engine/components/ui/UIImageComponent.cpp
        engine/Random.cpp
        engine/geAudio.cpp
        engine/geIO.cpp
        engine/components/ParticleEmitterComponent.cpp
        engine/components/ParticlesPropertiesComponent.cpp
        engine/components/SpawnerComponent.cpp
        engine/Range.cpp
        engine/components/CarColliderComponent.cpp)

# internal stuff
set(ENGINE_PRIVATE_SOURCES
        engine/internal/GameObject.cpp
        engine/internal/ObjectManager.cpp
        engine/internal/SceneManager.cpp
        engine/internal/Scene.cpp
        engine/internal/utils.cpp
        engine/internal/graphics/GraphicsEngine.cpp
        engine/internal/graphics/Shader.cpp
        engine/internal/graphics/GraphicHolder.cpp
        engine/internal/Screen.cpp
        engine/internal/physics/PhysicsEngine.cpp
        engine/internal/physics/Collider.cpp
        engine/internal/physics/DebugView.cpp
        engine/internal/graphics/font/Font.cpp
        engine/internal/graphics/font/Text.cpp
        engine/internal/graphics/font/FontManager.cpp
        engine/internal/audio/AudioEngine.cpp
        engine/internal/audio/AudioSource.cpp
        engine/internal/audio/AudioBuffer.cpp
        engine/internal/Data.cpp
        engine/internal/graphics/Camera.cpp
        engine/internal/graphics/MeshData.cpp
        engine/internal/graphics/GraphicSprite.cpp
        engine/internal/graphics/GraphicGeometry.cpp
        engine/internal/Environment.cpp
        engine/internal/graphics/FBO.cpp
        engine/internal/TimeManager.cpp
        engine/internal/pathfinding/AStarPathfinding.cpp
        engine/internal/pathfinding/Cell.cpp
        engine/internal/pathfinding/Agent.cpp
        engine/internal/pathfinding/Path.cpp
        engine/internal/graphics/Particle.cpp
        engine/internal/WritableData.cpp)

add_library(GameEngine ${ENGINE_SOURCES} ${ENGINE_PRIVATE_SOURCES})

if(MSVC)
    set_target_properties(GameEngine PROPERTIES LINK_FLAGS "/ignore:4099")
endif ()

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

enable_testing()
add_subdirectory(test)

### Editor
#add_executable(GameEngineEditor
#        editor/imgui_backend/imgui_impl_opengl3.cpp
#        editor/imgui_backend/imgui_impl_sdl.cpp
#        editor/imgui_backend/imgui_stdlib.cpp
#        editor/editorMain.cpp
#        editor/Editor.cpp
#        editor/ViewModels.cpp
#        editor/CreateProjectEditor.cpp
#        editor/GameComponentsProvider.cpp
#        editor/ProjectFileDataProvider.cpp
#        editor/dialogs/CreateDataFileDialog.cpp
#        editor/dialogs/DeleteFileDialog.cpp
#        editor/dialogs/SaveAllDialog.cpp
#        editor/ProjectDirectory.cpp
#        editor/DataFile.cpp
#        editor/dialogs/ErrorDialog.cpp
#        editor/DataDirectory.cpp
#        editor/GameEditor.cpp)
#
#target_compile_definitions(GameEngineEditor PUBLIC -DIMGUI_IMPL_OPENGL_LOADER_GLEW)
#
#if(MSVC)
#    target_compile_options(GameEngineEditor PUBLIC /bigobj)
#    set_target_properties(GameEngineEditor PROPERTIES LINK_FLAGS "/ignore:4099")
#endif ()
#
#target_link_libraries(GameEngineEditor PRIVATE GameEngine
#        CONAN_PKG::imgui
#        CONAN_PKG::sdl2
#        CONAN_PKG::glew
#        CONAN_PKG::boost
#        CONAN_PKG::yaml-cpp
#        CONAN_PKG::opengl
#        CONAN_PKG::tinyfiledialogs
#        )

### Editor V2
add_executable(GameEngineEditorV2
        editorV2/imgui_backend/imgui_impl_opengl3.cpp
        editorV2/imgui_backend/imgui_impl_sdl.cpp
        editorV2/imgui_backend/imgui_stdlib.cpp
        editorV2/editorMain.cpp
        editorV2/FileExplorer.cpp
        editorV2/DataDirectory.cpp
        editorV2/DataFile.cpp
        editorV2/GameObjectEditor.cpp)

target_compile_definitions(GameEngineEditorV2 PUBLIC -DIMGUI_IMPL_OPENGL_LOADER_GLEW)

if(MSVC)
    set_target_properties(GameEngineEditorV2 PROPERTIES LINK_FLAGS "/ignore:4099")
endif ()

target_link_libraries(GameEngineEditorV2 PRIVATE GameEngine
        CONAN_PKG::imgui
        CONAN_PKG::sdl2
        CONAN_PKG::glew
        CONAN_PKG::boost
        CONAN_PKG::yaml-cpp
        CONAN_PKG::opengl
        CONAN_PKG::tinyfiledialogs
        )
