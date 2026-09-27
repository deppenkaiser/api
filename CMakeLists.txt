cmake_minimum_required(VERSION 3.10)
include_guard(GLOBAL)

project(api LANGUAGES C)

add_library(api STATIC)

target_sources(api PRIVATE
	api.c
)

target_include_directories(api PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}
)
