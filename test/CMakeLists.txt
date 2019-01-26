find_package(GTest MODULE REQUIRED)
add_executable(example_test GameObjectTests.cpp ${ENGINE_SOURCES})
target_link_libraries(example_test PRIVATE GTest::GTest GTest::Main GameEngine)
set_target_properties(example_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
add_test(AllTests example_test)