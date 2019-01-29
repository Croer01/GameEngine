# Find GTest dependencies
find_package(GTest MODULE REQUIRED)
# Link GameEngine.dll directory to test can link with it
link_directories(${CMAKE_BINARY_DIR}/GameEngine)

add_executable(game_object_test GameObjectTests.cpp)
target_link_libraries(game_object_test GTest::GTest GTest::Main GameEngine)
add_test(GameObjectTests game_object_test)

add_custom_target(copyTestData ALL
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/data
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data
)