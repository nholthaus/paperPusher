#-------------------------------------------------------------------------------
#	CMAKE SETTINGS
#-------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.17)
set(QT_MINIMUM_VERSION 5.15.0)
include(capitalize)

#-------------------------------------------------------------------------------
#	PROJECT SETTINGS
#-------------------------------------------------------------------------------

PROJECT(paperPusher)
set(APPLICATION_NAME capitalize(${PROJECT_NAME}))

#-------------------------------------------------------------------------------
#	SOURCE CODE
#-------------------------------------------------------------------------------

# .cpp files
list(APPEND SOURCE_FILES
main.cpp
)

#-------------------------------------------------------------------------------
#	ADDITIONAL LIBRARIES
#-------------------------------------------------------------------------------

list(APPEND LIBRARIES
logerr
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

# Set additional include directories
include_directories(${INCLUDE_DIRS})

# Generate the executable
add_executable(${PROJECT_NAME} ${GUI_TYPE} ${SOURCE_FILES} ${RESOURCES})

# link libraries
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

# Use the automatically determined RPATH values
set_target_properties(${PROJECT_NAME} PROPERTIES INSTALL_RPATH_USE_LINK_PATH TRUE)

INSTALL(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
INCLUDE(InstallRequiredSystemLibraries)

#-------------------------------------------------------------------------------
#	DEPLOY QT DLLS
#-------------------------------------------------------------------------------

include(windeployqt)
windeployqt(${PROJECT_NAME} bin)

#-------------------------------------------------------------------------------
#	INSTALLER
#-------------------------------------------------------------------------------

set(CPACK_GENERATOR NSIS)
set(CPACK_PACKAGE_NAME "Paper Pusher")
set(CPACK_PACKAGE_VENDOR "Menari Softworks")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Report Automation Tool")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR "1")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "0")

INCLUDE(CPack)