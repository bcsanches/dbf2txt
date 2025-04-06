cmake_minimum_required (VERSION 3.14)

#main project
project (dbf2txt)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	set (DBF2TXT_MSVC true)
	add_compile_options(/utf-8)
else()
	set (DBF2TXT_MSVC false)
endif()

set( CMAKE_EXPORT_COMPILE_COMMANDS on)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

set_property( GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(src)

include(InstallRequiredSystemLibraries)
