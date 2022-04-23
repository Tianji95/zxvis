project(zxvis)

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
E:/vulkansdk/Include
)

LINK_DIRECTORIES("E:/vulkansdk/Lib")
link_libraries("vulkan-1.lib")

file(GLOB SOURCE
    ./src/app/*.cpp
    ./src/runtime/*.cpp
    ./src/runtime/primitive/*.cpp
    ./src/runtime/math/*.cpp
    ./src/runtime/pipeline/*.cpp
    ./src/runtime/entity/*.cpp
    ./src/runtime/component/*.cpp
    ./src/runtime/system/*.cpp
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
)
SET(EXECUTABLE_OUTPUT_PATH ./build)
add_executable(zxvis ${SOURCE} ${HEADERS})