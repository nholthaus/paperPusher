#-------------------------------------------------------------------------------
#	CMAKE SETTINGS
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.17)
set(QT_MINIMUM_VERSION 5.15.0)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

#-------------------------------------------------------------------------------
#	PROJECT SETTINGS
#-------------------------------------------------------------------------------

PROJECT(PaperPusher)

#-------------------------------------------------------------------------------
#	Qt Settings
#-------------------------------------------------------------------------------

enable_testing(true)

# Required Qt settings
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# disable warnings on the Qt sources
add_compile_options("/experimental:external")
add_compile_options("/external:env:QTDIR")
add_compile_options("/external:W0")

# Setup the cmake prefix path so Qt can be found. Use the QTDIR variable if defined
if(NOT DEFINED ENV{QTDIR})
	MESSAGE(STATUS "QTDIR environment variable not set. Be sure to set the QTDIR
	cmake cache variable to the Qt root install location")
endif()

set(QTDIR $ENV{QTDIR} CACHE STRING "Qt install path")
list(APPEND CMAKE_PREFIX_PATH ${QTDIR})

# Find the necessary Qt5 modules
find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

# Test for supported Qt version
find_program(QMAKE_EXECUTABLE NAMES qmake HINTS ${QTDIR} ENV QTDIR PATH_SUFFIXES bin)
execute_process(COMMAND ${QMAKE_EXECUTABLE} -query QT_VERSION OUTPUT_VARIABLE QT_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
if(QT_VERSION VERSION_LESS QT_MINIMUM_VERSION)
	MESSAGE(FATAL_ERROR "Minimum supported Qt version: ${QT_MINIMUM_VERSION}. Installed version: ${QT_VERSION}")
endif()

#-------------------------------------------------------------------------------
#	THREAD SETTINGS
#-------------------------------------------------------------------------------

find_package(Threads REQUIRED)

#-------------------------------------------------------------------------------
#	COMPILER SETTINGS
#-------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)

if(MSVC)
	set(GUI_TYPE WIN32)
	add_compile_options(/MP)
elseif(CMAKE_COMPILER_IS_GNUCXX)
	add_compile_options(-std=c++17)
endif(MSVC)

#-------------------------------------------------------------------------------
#	SUBDIRECTORIES
#-------------------------------------------------------------------------------

add_subdirectory(lib)
add_subdirectory(src)