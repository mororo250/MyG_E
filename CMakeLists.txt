cmake_minimum_required (VERSION 3.1 FATAL_ERROR)

#proprieties

project(MyG_E)

set (CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    "${PROJECT_SOURCE_DIR}/cmake"
)

find_package(GLFW3 REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set (CMAKE_CXX_STANDARD 17)

#includes and directories
#Third Party subdirectory
include_directories (
    ${GLFW3_INCLUDE_DIRS}
    src/ThirdParty/glad/include
    src/ThirdParty/ImGui/include
    src/ThirdParty/imstb/include
)

add_subdirectory (src/ThirdParty/glad)
add_subdirectory (src/ThirdParty/ImGui)
add_subdirectory (src/ThirdParty/imstb)

#Project subdirectory

include_directories (
    src/MyG_E
)

add_subdirectory(src/MyG_E)
