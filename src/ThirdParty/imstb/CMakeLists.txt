#imstb directory CMake file

set (sources
    include/stb_image.h    
    stb_image.cpp
)

add_library (imstb STATIC ${sources})

set_target_properties (imstb PROPERTIES FOLDER "Third Parties")

target_include_directories (imstb PUBLIC include)