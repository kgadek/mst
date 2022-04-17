cmake_minimum_required(VERSION 3.13)
project(mstch)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED OFF)
#set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.54.0 COMPONENTS variant)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(mstch)
include_directories(${mstch_INCLUDE_DIR})

option(WITH_CMD "Allow running arbitrary commands via templates" ON)
option(WITH_INCLUDE "Allow including arbitrary files via templates" ON)
if(WITH_CMD)
    add_definitions(-DMST_WITH_CMD)
endif()
if(WITH_INCLUDE)
    add_definitions(-DMST_WITH_INCLUDE)
endif()

set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-format-style=file;-checks=*,-llvmlibc-restrict-system-libc-headers,-llvmlibc-implementation-in-namespace,-llvmlibc-callee-namespace,-fuchsia-default-arguments-calls,-modernize-use-trailing-return-type")
add_executable(mst mst.cxx)
target_link_libraries(mst mstch)
target_link_libraries(mst ${Boost_LIBRARIES})

