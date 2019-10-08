# - Try to find ZLIB
# Once done, this will define
#
#  ZLIB_FOUND - system has ZLIB
#  ZLIB_INCLUDE_DIRS - the ZLIB include directories
#  ZLIB_LIBRARIES - link these to use ZLIB
#

INCLUDE(FindPackageHandleStandardArgs)

find_path (ZLIB_INCLUDE_DIR NAMES zlib.h HINTS ${CMAKE_HOME_DIRECTORY}/dep/zlib/include)
find_library (ZLIB_LIBRARY NAMES zlibstatic.lib HINTS ${CMAKE_HOME_DIRECTORY}/dep/zlib/lib-vc2019)

# Handle the QUIETLY and REQUIRED arguments and set PYTHON3_FOUND.
find_package_handle_standard_args (ZLIB DEFAULT_MSG
    ZLIB_INCLUDE_DIR 
    ZLIB_LIBRARY
)

# Set the output variables.
if (ZLIB_FOUND)
    set (ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIR})
    set (ZLIB_LIBRARIES ${ZLIB_LIBRARY})
else ()
    set (ZLIB_INCLUDE_DIRS)
    set (ZLIB_LIBRARIES)
endif ()

