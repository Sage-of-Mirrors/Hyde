cmake_minimum_required(VERSION 3.12)
project(hyde)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)

add_subdirectory(lib)

file(GLOB LIBJ3DCONV_SRC
    # libj3dconv
    "libj3dconv/src/*.cpp"
    "libj3dconv/include/*.hpp"
    
    # bStream
    "lib/bStream/*.cpp"
)

add_library(libj3dconv ${LIBJ3DCONV_SRC})
target_include_directories(libj3dconv PUBLIC libj3dconv/include lib/bStream lib/tinygltf lib/glm lib/TriStripper/include ${ZLIB_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS})
target_link_libraries(libj3dconv PUBLIC tinygltf glm TriStripper ZLIB::ZLIB PNG::PNG)

option(HYDE_BUILD_APP "Builds the commandline conversion app" ON)
if (HYDE_BUILD_APP)
  add_executable(hyde hyde/src/main.cpp)
  target_include_directories(hyde PUBLIC include lib/bStream lib/tinygltf)
  target_link_libraries(hyde PUBLIC libj3dconv tinygltf)
endif (HYDE_BUILD_APP)