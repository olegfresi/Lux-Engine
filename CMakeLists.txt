cmake_minimum_required(VERSION 3.27)

project(LuxEngine)
set(CMAKE_CXX_STANDARD 20)
enable_language(CXX)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_TESTING "Enable testing" ON)
option(RUN_TESTS "Run tests before application" ON)
 
# Os detection and graphics API setting
if(WIN32)
    set(OS_TYPE "WINDOWS")
    add_definitions(-DOS_TYPE_WINDOWS)
elseif(APPLE)
    set(OS_TYPE "MACOS")
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
    add_definitions(-DOS_TYPE_MACOS)
elseif(UNIX AND NOT APPLE)
    set(OS_TYPE "LINUX")
    add_definitions(-DOS_TYPE_LINUX)
else()
    set(OS_TYPE "UNKNOWN")
endif()

if(NOT APPLE)
    find_package(Vulkan QUIET)
    if(Vulkan_FOUND)
        set(GRAPHICS_API "VULKAN")
        add_definitions(-DGRAPHICS_API_VULKAN)
    endif()
endif()

set(GRAPHICS_API "OPENGL")
add_definitions(-DGRAPHICS_API_OPENGL)

if(APPLE)
    find_library(METAL_FRAMEWORK Metal)
    if(METAL_FRAMEWORK AND NOT GRAPHICS_API STREQUAL "src/OpenGL")
        set(GRAPHICS_API "METAL")
        add_definitions(-DGRAPHICS_API_METAL)
    endif()
endif()

if(WIN32)
    set(DIRECTX12_PATH "C:/Program Files (x86)/Windows Kits/10/Include")
    if(EXISTS "${DIRECTX12_PATH}/d3d12.h" AND GRAPHICS_API STREQUAL "UNKNOWN")
        set(GRAPHICS_API "DIRECTX12")
        add_definitions(-DGRAPHICS_API_DIRECTX)
    endif()
endif()

set(CMAKE_XCODE_SCHEME_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}" CACHE STRING "Working Directory" FORCE)

# Retrieve all source and header files
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.hpp)
file(GLOB_RECURSE TEST_SOURCES ${CMAKE_SOURCE_DIR}/src/Test/*.cpp)

# Configure spirv ang glslang related stuff
set(SPIRV_SKIP_TESTS ON)
set(SPIRV_SKIP_GOOGLETEST ON)
set(BUILD_GMOCK ON)
set(INSTALL_GTEST OFF)
set(ENABLE_GLSLANG_BINARIES OFF)
set(ENABLE_SPVREMAPPER OFF)
set(ENABLE_HLSL OFF)
set(BUILD_TESTING OFF)

# Configure spdlog
set(SPDLOG_INSTALL OFF)
set(SPDLOG_BUILD_EXAMPLES OFF)
set(SPDLOG_BUILD_TESTS OFF)

# Add all dependencies
set(SPIRV_HEADERS_EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/thirdparty/spirv-tools/external/spirv-headers)
set(SPIRV_HEADERS_SOURCE_DIR ${CMAKE_SOURCE_DIR}/thirdparty/spirv-headers)
if(NOT EXISTS ${SPIRV_HEADERS_EXTERNAL_DIR})
    if(UNIX OR APPLE)
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E create_symlink ${SPIRV_HEADERS_SOURCE_DIR} ${SPIRV_HEADERS_EXTERNAL_DIR}
            RESULT_VARIABLE SYMLINK_RESULT
        )
        if(SYMLINK_RESULT)
            message(WARNING "Failed to create symlink for spirv-headers. Creating directory copy instead.")
            execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${SPIRV_HEADERS_SOURCE_DIR} ${SPIRV_HEADERS_EXTERNAL_DIR})
        endif()
    else()
        # Windows: copy directory instead of symlink
        execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${SPIRV_HEADERS_SOURCE_DIR} ${SPIRV_HEADERS_EXTERNAL_DIR})
    endif()
endif()
set(SPIRV-Headers_SOURCE_DIR ${SPIRV_HEADERS_EXTERNAL_DIR} CACHE PATH "SPIRV-Headers source directory" FORCE)
add_subdirectory(thirdparty/googletest)
add_subdirectory(thirdparty/spirv-tools)
add_subdirectory(thirdparty/glslang)
add_subdirectory(thirdparty/spdlog)
add_subdirectory(thirdparty/glfw)
#add_subdirectory(thirdparty/glm)


# Check useful also for multi-config generators
if(NOT CMAKE_CONFIGURATION_TYPES)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(ENABLE_TESTING ON CACHE BOOL "Enable unit tests in Debug" FORCE)
        set(RUN_TESTS ON CACHE BOOL "Run unit tests in Debug" FORCE)
    endif()
endif()

# Checking system's endianness
include(TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)

# Add test files to sources
if(ENABLE_TESTING)
    list(APPEND TEST_SOURCES)
    
    # Add definitions
    add_definitions(-DENABLE_TESTING)
    if(RUN_TESTS)
        add_definitions(-DRUN_TESTS)
    endif()
    
    enable_testing()
endif()

# Main target
add_executable(LuxEngine ${SOURCE_FILES} ${HEADER_FILES})

add_library(glew STATIC thirdparty/glew/src/glew.c)

target_include_directories(glew PUBLIC
    ${CMAKE_SOURCE_DIR}/thirdparty/glew/include
)

target_compile_definitions(glew PUBLIC GLEW_STATIC)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/thirdparty
        ${CMAKE_SOURCE_DIR}/thirdparty/spirv-tools/include
        ${CMAKE_SOURCE_DIR}/thirdparty/spirv-tools/external/spirv-headers/include
    ${CMAKE_SOURCE_DIR}/thirdparty/glslang
    ${CMAKE_SOURCE_DIR}/thirdparty/spdlog/include
    ${CMAKE_SOURCE_DIR}/thirdparty/glew/include)
    #${CMAKE_SOURCE_DIR}/thirdparty/glm)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        glew
        glslang
        SPIRV-Tools-static
        SPIRV-Tools-opt
        spdlog
        gtest
        gtest_main
        "-framework OpenGL"
    )
elseif(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        glew
        glslang
        SPIRV-Tools-static
        SPIRV-Tools-opt
        spdlog
        gtest
        gtest_main
        opengl32
    )
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        glfw
        glew
        glslang
        SPIRV-Tools-static
        SPIRV-Tools-opt
        spdlog
        gtest
        gtest_main
        GL
    )
endif()