cmake_minimum_required (VERSION 2.8.10)
PROJECT(nekofeed CXX C)


INCLUDE(FindPkgConfig)
INCLUDE(CheckIncludeFiles)
INCLUDE(CheckIncludeFileCXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMakeModules/")
#include(GetGitRevisionDescription)
#git_describe(LLLLL_GIT_VERSION)

IF("${LLLLL_GIT_VERSION}" STREQUAL "HEAD-HASH-NOTFOUND")
SET(LLLLL_GIT_VERSION "0.0.0")
ENDIF()
#################################################################
# Add strict warning if building under linux
#################################################################

IF (UNIX)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -std=c++14 -ffast-math")
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -std=gnu11 -ffast-math")
	SET(CMAKE_REQUIRED_FLAGS "${CMAKE_CXX_FLAGS}")
ENDIF()


include_directories( ${CMAKE_SOURCE_DIR}/src )
SET (EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
IF(WIN32)
	SET (ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
	SET (RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
	SET (LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
	SET (LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
ENDIF()
#OPTION (BUILD_SHARED_LIBS "Build shared libraries." ON)
#OPTION (YURI_DISABLE_TESTS "Disable unit tests" ON )

#################################################################
# Conditionaly enable testing
#################################################################

IF( NOT YURI_DISABLE_TESTS )
	enable_testing()
ENDIF()

add_subdirectory(src)

