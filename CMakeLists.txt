cmake_minimum_required(VERSION 2.8)
project(GemMiner)

set(THIRDPARTY_PATH ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)
