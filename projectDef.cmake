#/**********************************************************\ 
# Auto-generated X11 project definition file for the
# Plugin Email Client project
#\**********************************************************/

# X11 template platform definition CMake file
# Included from ../CMakeLists.txt

# remember that the current source dir is the project root; this file is in X11/
file (GLOB PLATFORM RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    X11/[^.]*.cpp
    X11/[^.]*.h
    X11/[^.]*.cmake
    )

SOURCE_GROUP(X11 FILES ${PLATFORM})

# use this to add preprocessor definitions
add_definitions(
)

set (SOURCES
    ${SOURCES}
    ${PLATFORM}
    )

add_x11_plugin(${PROJECT_NAME} SOURCES)

# add library dependencies here; leave ${PLUGIN_INTERNAL_DEPS} there unless you know what you're doing!
target_link_libraries(${PROJECT_NAME}
    ${PLUGIN_INTERNAL_DEPS}
    )

# Use the package PkgConfig to detect GTK+ headers/library files
find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK2 REQUIRED gtk+-2.0)

# Setup CMake to use GTK+, tell the compiler where to look for headers
# and to the linker where to look for libraries
include_directories(${GTK2_INCLUDE_DIRS})

link_directories(${GTK2_LIBRARY_DIRS})

# Add other flags to the compiler
add_definitions(${GTK2_CFLAGS_OTHER})

# Link the target to the GTK+ libraries
target_link_libraries(${PROJECT_NAME} ${GTK2_LIBRARIES})

find_package(CURL)

if(CURL_FOUND)
	include_directories(${CURL_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARY})
else(CURL_FOUND)
	message(FATAL_ERROR "curl not found!")
endif(CURL_FOUND)
