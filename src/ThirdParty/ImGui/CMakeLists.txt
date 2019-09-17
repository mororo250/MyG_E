#ImGui directory CMake file

set (sources
    include/imconfig.h
    include/imgui.h
    include/imgui_impl_glfw.h
    include/imgui_impl_opengl3.h
    include/imgui_internal.h
    include/imstb_rectpack.h
    include/imstb_textedit.h
    include/imstb_truetype.h
    imgui.cpp
    imgui_widgets.cpp
    imgui_demo.cpp
    imgui_draw.cpp
    imgui_impl_glfw.cpp
    imgui_impl_opengl3.cpp
)

add_library (imgui STATIC ${sources})

set_target_properties (imgui PROPERTIES FOLDER "Third Parties")

target_include_directories (imgui PUBLIC include)