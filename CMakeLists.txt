cmake_minimum_required(VERSION 3.12)
project(hyde)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ZLIB REQUIRED)
find_package(PNG REQUIRED)

add_subdirectory(lib)

file(GLOB LIBJ3D_SRC
    # libj3dconv
    "libj3d/src/*.cpp"
	"libj3d/src/geometry/*.cpp"
	"libj3d/src/materials/*.cpp"
	"libj3d/src/scenegraph/*.cpp"
	"libj3d/src/util/*.cpp"
    "libj3d/include/*.h"
	"libj3d/include/geometry/*.h"
	"libj3d/include/materials/*.h"
	"libj3d/include/scenegraph/*.h"
	"libj3d/include/util/*.h"
    
    # bStream
    "lib/bStream/*.cpp"
	
	# ufbx
	"lib/ufbx/ufbx.h"
	"lib/ufbx/ufbx.c"
)

add_library(libj3d ${LIBJ3D_SRC})
target_include_directories(libj3d PUBLIC libj3d/include lib/bStream lib/glm lib/TriStripper/include lib/ufbx ${ZLIB_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS})
target_link_libraries(libj3d PUBLIC glm TriStripper ZLIB::ZLIB PNG::PNG)

option(HYDE_BUILD_APP "Builds the commandline conversion app" ON)
if (HYDE_BUILD_APP)
  add_executable(hyde hyde/src/main.cpp)
  target_include_directories(hyde PUBLIC include lib/bStream)
  target_link_libraries(hyde PUBLIC libj3d)
endif (HYDE_BUILD_APP)