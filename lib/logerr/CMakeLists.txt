#-------------------------------------------------------------------------------
#	CMAKE SETTINGS
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)
cmake_host_system_information(RESULT BUILD_HOSTNAME QUERY HOSTNAME)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

#-------------------------------------------------------------------------------
#	CMAKE OPTIONS
#-------------------------------------------------------------------------------

option(BUILD_WITH_QT "Build the Qt classes" OFF)

#-------------------------------------------------------------------------------
#	SUBDIRECTORIES
#-------------------------------------------------------------------------------

add_subdirectory(logerr)

if (BUILD_WITH_QT)
    add_subdirectory(qlogerr)
endif (BUILD_WITH_QT)

project(logerr-dummy)