#Glad directory CMake file

set (sources
    include/glad/glad.h
    include/KHR/khrplatform.h
    src/glad.c
)

add_library (glad STATIC ${sources})

set_target_properties (glad PROPERTIES FOLDER "Third Parties")

target_include_directories (glad PUBLIC include)
