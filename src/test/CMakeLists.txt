
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  OVERRIDE_FIND_PACKAGE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
add_executable(CMakeSFMLProjectTests main_test.cpp)
target_link_libraries(CMakeSFMLProjectTests PRIVATE sfml-graphics Clipper2 SelbaWard GTest::gtest_main)
target_compile_features(CMakeSFMLProjectTests PRIVATE cxx_std_20)

include(GoogleTest)
gtest_discover_tests(CMakeSFMLProjectTests)