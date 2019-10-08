cmake_minimum_required (VERSION 3.1 FATAL_ERROR)

#proprieties

project(MyG_E)

set (CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${PROJECT_SOURCE_DIR}/cmake"
)

find_package(GLFW3 REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(irrXML REQUIRED)
find_package(ZLIB REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set (CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MD")

#includes and directories
#Third Party subdirectory
include_directories (
    ${ASSIMP_INCLUDE_DIRS}
    ${GLFW3_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${IRRXML_INCLUDE_DIRS}
    src/ThirdParty/glad/include
    src/ThirdParty/ImGui/include
    src/ThirdParty/imstb/include
    src/TrirdParty/rapidjson/include
)

add_subdirectory (src/ThirdParty/glad)
add_subdirectory (src/ThirdParty/ImGui)
add_subdirectory (src/ThirdParty/imstb)
add_subdirectory (src/ThirdParty/rapidjson)

#Project subdirectory

include_directories (
    src/MyG_E
)

add_subdirectory(src/MyG_E)
