cmake_minimum_required(VERSION 3.15)
project(zxvis)

set(CMAKE_CXX_STANDARD 17)

INCLUDE_DIRECTORIES( BEFORE
  src/3rdparty
  src/app
  src/runtime
  src/runtime/primitive
  src/runtime/math
  src/runtime/pipeline
  src/runtime/entity
  src/runtime/component
  src/runtime/system
  src/loader
E:/vulkansdk/Include
)

set(BASE_LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/libs")
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    SET(GLFW_LIBRARY_PATH "${BASE_LIBRARY_PATH}/glfw/x64")
else()
    SET(GLFW_LIBRARY_PATH "${BASE_LIBRARY_PATH}/glfw/x86")
endif()
LINK_DIRECTORIES("${GLFW_LIBRARY_PATH}")
LINK_DIRECTORIES("E:/vulkansdk/Lib")
link_libraries("vulkan-1.lib" "glfw3.lib")


file(GLOB SOURCE
    ./src/app/*.cpp
    ./src/runtime/*.cpp
    ./src/runtime/primitive/*.cpp
    ./src/runtime/math/*.cpp
    ./src/runtime/pipeline/*.cpp
    ./src/runtime/entity/*.cpp
    ./src/runtime/component/*.cpp
    ./src/runtime/system/*.cpp
    ./src/loader/*.cpp
)

file(GLOB HEADERS
    ./src/app/*.h
    ./src/runtime/*.h
    ./src/runtime/primitive/*.h
    ./src/runtime/math/*.h
    ./src/runtime/pipeline/*.h
    ./src/runtime/entity/*.h
    ./src/runtime/component/*.h
    ./src/runtime/system/*.h
    ./src/loader/*.h
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)

add_subdirectory(./src/3rdparty/OpenXLSX)
SET(EXECUTABLE_OUTPUT_PATH ./build)
add_executable(zxvis ${SOURCE} ${HEADERS})
target_link_libraries(zxvis OpenXLSX::OpenXLSX)