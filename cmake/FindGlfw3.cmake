#
# Find glfw3 headers and libraries.
#
# This module defines the following variables:
#
#   GLFW3_FOUND            True if glfw3 was found
#   GLFW3_INCLUDE_DIRS     Where to find glfw3 header files
#   GLFW3_LIBRARIES        List of glfw3 libraries to link against
#

INCLUDE(FindPackageHandleStandardArgs)

find_path (GLFW3_INCLUDE_DIR NAMES GLFW/glfw3.h HINTS ${CMAKE_HOME_DIRECTORY}/dep/glfw/include)
find_library (GLFW3_LIBRARY NAMES glfw3.lib HINTS ${CMAKE_HOME_DIRECTORY}/dep/glfw/lib-vc2017)

# Handle the QUIETLY and REQUIRED arguments and set PYTHON3_FOUND.
find_package_handle_standard_args (GLFW3 DEFAULT_MSG
    GLFW3_INCLUDE_DIR 
    GLFW3_LIBRARY
)

# Set the output variables.
if (GLFW3_FOUND)
    set (GLFW3_INCLUDE_DIRS ${GLFW3_INCLUDE_DIR})
    set (GLFW3_LIBRARIES ${GLFW3_LIBRARY})
else ()
    set (GLFW3_INCLUDE_DIRS)
    set (GLFW3_LIBRARIES)
endif ()
