#Sources

set (core_source
    Core/BatchRenderer.h
    Core/Camera.cpp
    Core/Camera.h 
    Core/EditCamera.cpp
    Core/EditCamera.h
    Core/FPSCamera.cpp
    Core/FPSCamera.h
    Core/IndexBuffer.cpp
    Core/IndexBuffer.h
    Core/Light.cpp
    Core/Light.h
    Core/Mesh.cpp
    Core/Mesh.h
    Core/Model3D.cpp
    Core/Model3D.h
    Core/Renderer.cpp
    Core/Renderer.h
    Core/Shader.cpp
    Core/Shader.h
    Core/Sprite.cpp
    Core/Sprite.h
    Core/Texture.cpp
    Core/Texture.h
    Core/VertexArray.cpp
    Core/VertexArray.h
    Core/VertexBuffer.cpp
    Core/VertexBuffer.h
)

list (APPEND MyG_E_sources
    ${core_source}
)
source_group ("Core" FILES
    ${core_source}
)

set (foundation_source
    Foundation/Game.cpp
    Foundation/Game.h
    Foundation/Gldebug.cpp
    Foundation/Gldebug.h 
    Foundation/Input.cpp
    Foundation/Input.h
    Foundation/Main.cpp
    Foundation/Math.cpp
    Foundation/Math.h
)

list (APPEND MyG_E_sources
    ${foundation_source}
)
source_group ("Foundation" FILES
    ${foundation_source}
)

set (scenes_source
    Scenes/BatchRenderScene.cpp
    Scenes/BatchRenderScene.h
    Scenes/Menu.cpp
    Scenes/Menu.h
    Scenes/Render3DScene.cpp
    Scenes/Render3DScene.h
    Scenes/Scene.cpp
    Scenes/Scene.h
    Scenes/Texture2D.cpp
    Scenes/Texture2D.h
)

list (APPEND MyG_E_sources
    ${scenes_source}
)
source_group ("Scenes" FILES
    ${scenes_source}
)

set (shaders_source
    Shaders/Color.shader
    Shaders/Color3D.shader
    Shaders/Shader.shader
)

list (APPEND MyG_E_sources
    ${shaders_source}
)
source_group ("Shaders" FILES
    ${shaders_source}
)

#Executable

add_executable (MyG_E
    ${MyG_E_sources}
)

set_target_properties (MyG_E PROPERTIES FOLDER "Core")

#Link

target_link_libraries (MyG_E
    glad
    imgui
    imstb
)

target_link_libraries (MyG_E     
    ${GLFW3_LIBRARIES}
)

#Opengl
find_package (OpenGL REQUIRED)
target_include_directories (MyG_E PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries (MyG_E ${OPENGL_gl_LIBRARY})

#install

install (TARGETS MyG_E DESTINATION bin)