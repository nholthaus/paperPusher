#-------------------------------------------------------------------------------
#	CMAKE SETTINGS
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.17)
set(QT_MINIMUM_VERSION 5.15.0)

cmake_host_system_information(RESULT BUILD_HOSTNAME QUERY HOSTNAME)

#-------------------------------------------------------------------------------
#	PROJECT SETTINGS
#-------------------------------------------------------------------------------

PROJECT(logerr)

#-------------------------------------------------------------------------------
#	GIT SETTINGS
#-------------------------------------------------------------------------------

# Use git branch as project name if clones from git
find_package(Git)
if(GIT_FOUND)
	execute_process(
	  COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
	  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	  OUTPUT_VARIABLE GIT_BRANCH
	  OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	execute_process(
	  COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
	  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	  OUTPUT_VARIABLE GIT_COMMIT_SHORT
	  OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	execute_process(
	  COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
	  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	  OUTPUT_VARIABLE GIT_COMMIT_LONG
	  OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	execute_process(
	  COMMAND ${GIT_EXECUTABLE} describe --abbrev=0 --tags
	  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	  OUTPUT_VARIABLE GIT_TAG
	  OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	execute_process(
	  COMMAND ${GIT_EXECUTABLE} config --get remote.origin.url
	  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	  OUTPUT_VARIABLE GIT_ORIGIN
	  OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	execute_process(
	  COMMAND ${GIT_EXECUTABLE} rev-parse --show-toplevel
	  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	  OUTPUT_VARIABLE GIT_DIRECTORY
	  OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	execute_process(
	  COMMAND basename ${GIT_DIRECTORY}
	  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	  OUTPUT_VARIABLE GIT_REPO
	  OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	execute_process(
	  COMMAND ${GIT_EXECUTABLE} config --get user.name
	  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	  OUTPUT_VARIABLE GIT_USER
	  OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	execute_process(
	  COMMAND ${GIT_EXECUTABLE} config --get user.email
	  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	  OUTPUT_VARIABLE GIT_EMAIL
	  OUTPUT_STRIP_TRAILING_WHITESPACE
	)
	execute_process(
	  COMMAND ${GIT_EXECUTABLE} status --porcelain
	  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
	  OUTPUT_VARIABLE GIT_DIRTY
	  OUTPUT_STRIP_TRAILING_WHITESPACE
	)

	set(APP_NAME ${GIT_REPO})
	set(APP_VERSION ${GIT_TAG})

	if(${GIT_DIRTY} STREQUAL "")
		set(GIT_DIRTY_STRING "")
		set(GIT_DIRTY_FLAG "no")

	else()
		set(GIT_DIRTY_STRING " (dirty)")
		set(GIT_DIRTY_FLAG "yes")
	endif()

	message(STATUS "REPO:          " ${GIT_REPO})
	message(STATUS "BRANCH:        " ${GIT_BRANCH})
	message(STATUS "LOCAL CHANGES: " ${GIT_DIRTY_FLAG})
else()
	set(GIT_BRANCH "N/A")
	set(GIT_COMMIT_SHORT "N/A")
	set(GIT_COMMIT_LONG "N/A")
	set(GIT_TAG "N/A")
	set(GIT_ORIGIN "N/A")
	set(GIT_DIRECTORY "N/A")
	set(GIT_REPO "N/A")
	set(GIT_USER "N/A")
	set(GIT_EMAIL "N/A")
endif()



#-------------------------------------------------------------------------------
#	Qt SETTINGS
#-------------------------------------------------------------------------------

find_program(QMAKE_EXECUTABLE NAMES qmake HINTS ${QTDIR} ENV QTDIR PATH_SUFFIXES bin)
if(QMAKE_EXECUTABLE_FOUND)
	execute_process(
		COMMAND ${QMAKE_EXECUTABLE} -query QT_VERSION 
		OUTPUT_VARIABLE QT_VERSION 
		OUTPUT_STRIP_TRAILING_WHITESPACE
	)
else()
	set(QT_VERSION, "N/A")
endif()

#-------------------------------------------------------------------------------
#	CONFIGURATION
#-------------------------------------------------------------------------------

set(APPLICATION_NAME ${TARGET_NAME})
set(APPLICATION_VERSION ${VERSION})
set(APPLICATION_ORGANIZATION "Menari Softworks")
set(APPLICATION_ORGANIZATION_DOMAIN ${GIT_ORIGIN})

configure_file(appinfo.h.in appinfo.h @ONLY NEWLINE_STYLE UNIX)

#-------------------------------------------------------------------------------
#	SOURCE CODE
#-------------------------------------------------------------------------------

# .cpp files
list(APPEND SOURCE_FILES
appinfo.h.in
logerr.h
logerr.cpp
logerrMacros.h
logerrMacros.cpp
StackTrace.h
StackTrace.cpp
StackTraceSigSev.h
StackTraceSigSev.cpp
)

#-------------------------------------------------------------------------------
#	ADDITIONAL LIBRARIES
#-------------------------------------------------------------------------------

list(APPEND LIBRARIES
Qt5::Core
Qt5::Gui
Qt5::Widgets
Threads::Threads
)

# Remove duplicate library dependencies. 
list(REMOVE_DUPLICATES LIBRARIES)

#-------------------------------------------------------------------------------
#	GENERATE EXECUTABLE
#-------------------------------------------------------------------------------

# Generate the executable
add_library(${PROJECT_NAME} ${SOURCE_FILES} ${RESOURCES})

# link libraries
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

# include directories
set_target_properties(${PROJECT_NAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})
