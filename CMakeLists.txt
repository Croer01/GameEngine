cmake_minimum_required(VERSION 3.12)
project(SpaceInvaders)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)

set(GAME_SOURCES
        main.cpp
        EnemyComponent.cpp
        EnemyManagerComponent.cpp
        PlayerComponent.cpp
        BulletComponent.cpp
        HUDComponent.cpp
        StartMenuComponent.cpp
        MotherShipComponent.cpp
        BlinkTextComponent.cpp
        EnemyExplosionComponent.cpp
        GameManagerComponent.cpp
        ShelterComponent.cpp)

# auto-generated files
include(FindPythonInterp)
set(COMPONENTS_REGISTERED_CPP ${CMAKE_SOURCE_DIR}/componentsRegistered.cpp)
file(GLOB_RECURSE COMPONENTS_REGISTERED_HEADERS "*.hpp")
add_custom_command(
        COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/engine_components_generator.py > ${COMPONENTS_REGISTERED_CPP}
        DEPENDS ${COMPONENTS_REGISTERED_HEADERS}
        OUTPUT ${COMPONENTS_REGISTERED_CPP}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Generating code to register components into the object manager"
)
set(GAME_SOURCES ${GAME_SOURCES} ${COMPONENTS_REGISTERED_CPP})

find_package(SDL2 CONFIG REQUIRED)

add_executable(SpaceInvaders ${ENGINE_SOURCES} ${GAME_SOURCES})
target_link_libraries(SpaceInvaders PRIVATE
        SDL2::SDL2main
        GameEngine)

ADD_CUSTOM_TARGET(copyData ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/data
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data
        )
ADD_CUSTOM_TARGET(copyConf ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/conf
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/conf ${CMAKE_BINARY_DIR}/conf
        )
ADD_CUSTOM_TARGET(copyAssets ALL
        COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
        )

enable_testing()
add_subdirectory(test)