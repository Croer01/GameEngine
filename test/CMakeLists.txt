# Find GTest dependencies
find_package(GTest MODULE REQUIRED)
# Link GameEngine.dll directory to test can link with it
link_directories(${CMAKE_BINARY_DIR}/GameEngine)

# macro to generate test targets more easy
macro(CreateTestTarget name files)
    set(NAME_TARGET "${name}Tests")
    add_executable(${NAME_TARGET} ${files})
    target_link_libraries(${NAME_TARGET} GTest::GTest GTest::Main GameEngine)
    add_test(${name} ${NAME_TARGET})
    add_dependencies(${NAME_TARGET} copyTestData)
endmacro()

CreateTestTarget(GameObject GameObjectTests.cpp)
CreateTestTarget(Game GameTests.cpp)
CreateTestTarget(Properties PropertiesTests.cpp)
CreateTestTarget(Math MathTests.cpp)

add_custom_target(copyTestData
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/data
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data
)