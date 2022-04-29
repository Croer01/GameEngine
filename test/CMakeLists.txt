set(TEST_NAMES "")

# Conan modify some output paths. Copy the resources in the same directory as the binaries
set(BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})
if(NOT ARGUMENTS_NO_OUTPUT_DIRS)
    set(BIN_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

# macro to generate test targets more easy
macro(CreateTestTarget name files)
    set(NAME_TARGET "${name}Tests")
    set(TEST_NAMES ${TEST_NAMES} ${NAME_TARGET})
    add_executable(${NAME_TARGET} ${files})
    target_link_libraries(${NAME_TARGET} PRIVATE CONAN_PKG::gtest GameEngine)
    target_link_directories(${NAME_TARGET} PRIVATE ${CMAKE_BINARY_DIR}/GameEngine)
    add_test(NAME ${name} COMMAND ${NAME_TARGET} --gtest_catch_exceptions=0 WORKING_DIRECTORY ${BIN_DIR})
    add_dependencies(${NAME_TARGET} copyTestData)
endmacro()

CreateTestTarget(GameObject GameObjectTests.cpp)
CreateTestTarget(Game GameTests.cpp)
CreateTestTarget(Properties PropertiesTests.cpp)
CreateTestTarget(Math MathTests.cpp)
CreateTestTarget(Pathfinding PathfindingTests.cpp)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} -C $<CONFIGURATION> --verbose
        DEPENDS ${TEST_NAMES})


add_custom_target(copyTestData
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${BIN_DIR}/data
        )
