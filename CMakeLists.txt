cmake_minimum_required(VERSION 3.16)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1
    OVERRIDE_FIND_PACKAGE)
FetchContent_MakeAvailable(SFML)

set(CLIPPER2_TESTS OFF)
# include clipper2
FetchContent_Declare(clipper2
    GIT_REPOSITORY https://github.com/AngusJohnson/Clipper2.git
    GIT_TAG Clipper2_1.3.0
    SOURCE_SUBDIR CPP
    )
FetchContent_MakeAvailable(clipper2)

# include selbaward because it contains a triangulation polygon class
FetchContent_Declare(selbaward
    GIT_REPOSITORY https://github.com/Hapaxia/SelbaWard
    GIT_TAG master)
FetchContent_MakeAvailable(SelbaWard)


add_executable(CMakeSFMLProject src/main.cpp)
target_link_libraries(CMakeSFMLProject PRIVATE sfml-graphics Clipper2 SelbaWard)
target_compile_features(CMakeSFMLProject PRIVATE cxx_std_20)


if(WIN32)
    add_custom_command(
        TARGET CMakeSFMLProject
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:CMakeSFMLProject>
        VERBATIM)
endif()

install(TARGETS CMakeSFMLProject)

add_subdirectory(src/test)