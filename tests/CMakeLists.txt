include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

include(GoogleTest) 

# Create the ome_core_tests executable 
add_executable(ome_core_tests)
target_sources(ome_core_tests PRIVATE OMECoreTests.cpp) 
target_link_libraries(ome_core_tests PRIVATE ome_core GTest::gtest_main) 

# Introduce ome_core_tests to GoogleTest
gtest_discover_tests(ome_core_tests)



