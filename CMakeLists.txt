cmake_minimum_required(VERSION 3.13)
project(mstch)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set(Boost_USE_STATIC_LIBS OFF)
#set(Boost_USE_MULTITHREADED OFF)
#set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.54.0 COMPONENTS variant)
include_directories(${Boost_INCLUDE_DIRS})

add_subdirectory(mstch)
include_directories(${mstch_INCLUDE_DIR})

add_executable(mst mst.cxx)
target_link_libraries(mst mstch)
target_link_libraries(mst ${Boost_LIBRARIES})

