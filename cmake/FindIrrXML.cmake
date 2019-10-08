# Find IrrXMl from irrlicht project
#
# Find LibIrrXML headers and library
#
#   IRRXML_FOUND          - IrrXML found
#   IRRXML_INCLUDE_DIR    - Headers location
#   IRRXML_LIBRARY        - IrrXML main library
#

INCLUDE(FindPackageHandleStandardArgs)

find_path(IRRXML_INCLUDE_DIR NAMES irrXML.h HINTS ${CMAKE_HOME_DIRECTORY}/dep/irrXML/include)
find_library(IRRXML_LIBRARY NAMES irrXML.lib HINTS ${CMAKE_HOME_DIRECTORY}/dep/irrXML/lib-vc2019)

find_package_handle_standard_args(IrrXML DEFAULT_MSG 
    IRRXML_INCLUDE_DIR 
    IRRXML_LIBRARY
)

if (IrrXML_FOUND)
    set (IRRXML_INCLUDE_DIRS ${IRRXML_INCLUDE_DIR})
    set (IRRXML_LIBRARIES ${IRRXML_LIBRARY})
else ()
    set (IRRXML_INCLUDE_DIRS)
    set (IRRXML_LIBRARIES)
endif ()
