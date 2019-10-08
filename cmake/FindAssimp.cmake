#
# Find assimp headers and libraries.
#
# This module defines the following variables:
#
#   ASSIMP_FOUND            True if assimp was found
#   ASSIMP_INCLUDE_DIRS     Where to find assimp header files
#   ASSIMP_LIBRARIES        List of assimp libraries to link against
#

INCLUDE(FindPackageHandleStandardArgs)

find_path (ASSIMP_INCLUDE_DIR NAMES assimp/anim.h HINTS ${CMAKE_HOME_DIRECTORY}/dep/assimp-5.0.0/include)
find_library (ASSIMP_LIBRARY NAMES assimp-vc141-mt.lib HINTS ${CMAKE_HOME_DIRECTORY}/dep/assimp-5.0.0/lib-vc2019)

# Handle the QUIETLY and REQUIRED arguments and set PYTHON3_FOUND.
find_package_handle_standard_args (ASSIMP DEFAULT_MSG
    ASSIMP_INCLUDE_DIR 
    ASSIMP_LIBRARY
)

# Set the output variables.
if (ASSIMP_FOUND)
    set (ASSIMP_INCLUDE_DIRS ${ASSIMP_INCLUDE_DIR})
    set (ASSIMP_LIBRARIES ${ASSIMP_LIBRARY})
else ()
    set (ASSIMP_INCLUDE_DIRS)
    set (ASSIMP_LIBRARIES)
endif ()
