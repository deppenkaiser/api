cmake_minimum_required(VERSION 3.10)
include_guard(GLOBAL)

project(api LANGUAGES C)

add_library(api INTERFACE)

target_include_directories(api INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}
)
