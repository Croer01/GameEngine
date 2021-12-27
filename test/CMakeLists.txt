# Link GameEngine.dll directory to test can link with it
link_directories(${CMAKE_BINARY_DIR}/GameEngine)

set(TEST_NAMES "")
# macro to generate test targets more easy
macro(CreateTestTarget name files)
    set(NAME_TARGET "${name}Tests")
    set(TEST_NAMES ${TEST_NAMES} ${NAME_TARGET})
    add_executable(${NAME_TARGET} ${files})
    target_link_libraries(${NAME_TARGET} CONAN_PKG::gtest GameEngine)
    add_test(NAME ${name} COMMAND ${NAME_TARGET} --gtest_catch_exceptions=0)
    add_dependencies(${NAME_TARGET} copyTestData)
endmacro()

CreateTestTarget(GameObject GameObjectTests.cpp)
CreateTestTarget(Game GameTests.cpp)
CreateTestTarget(Properties PropertiesTests.cpp)
CreateTestTarget(Math MathTests.cpp)
CreateTestTarget(Pathfinding PathfindingTests.cpp)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> --verbose
        DEPENDS ${TEST_NAMES})


# Conan modify some output paths. Copy the resources in the same directory as the binaries
if(NOT ARGUMENTS_NO_OUTPUT_DIRS)
    add_custom_target(copyTestData
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data
            )
else()
    add_custom_target(copyTestData
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data
            )
endif()
