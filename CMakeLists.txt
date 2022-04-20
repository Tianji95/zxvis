project(zxvis)

INCLUDE_DIRECTORIES( BEFORE
  src/3rdparty
  src/app
  src/runtime
E:/vulkansdk/Include
)

LINK_DIRECTORIES("E:/vulkansdk/Lib")
link_libraries("vulkan-1.lib")

file(GLOB SOURCE
    ./src/app/*.cpp
    ./src/runtime/*.cpp
)

file(GLOB HEADERS
    ./src/app/*.h
    ./src/runtime/*.h
)
SET(EXECUTABLE_OUTPUT_PATH ./build)
add_executable(zxvis ${SOURCE} ${HEADERS})