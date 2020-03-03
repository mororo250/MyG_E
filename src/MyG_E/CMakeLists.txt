#Sources
include (../../cmake/include/cmake-precompiled-header.cmake)

set (core_source
    Core/BatchRenderer.h
	Core/FrameBuffer.cpp
    Core/FrameBuffer.h
    Core/IndexBuffer.cpp
    Core/IndexBuffer.h
	Core/ProjectController.cpp
	Core/ProjectController.h
	Core/Material.cpp
	Core/Material.h
    Core/Mesh.cpp
    Core/Mesh.h
    Core/Model3D.cpp
    Core/Model3D.h
    Core/RenderBuffer.cpp
    Core/RenderBuffer.h
	Core/Renderer.cpp
    Core/Renderer.h
	Core/Renderer2D.cpp
	Core/Renderer2D.h
	Core/Renderer3D.cpp
	Core/Renderer3D.h
    Core/Shader.cpp
    Core/Shader.h
    Core/ShadowMap.cpp
    Core/ShadowMap.h
    Core/Sprite.cpp
    Core/Sprite.h
    Core/SkyBox.cpp
    Core/SkyBox.h
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

set (camera_source
    Core/Camera/Camera.cpp
    Core/Camera/Camera.h 
    Core/Camera/EditCamera.cpp
    Core/Camera/EditCamera.h
    Core/Camera/FPSCamera.cpp
    Core/Camera/FPSCamera.h
)

list (APPEND MyG_E_sources
    ${camera_source}
)
source_group ("Core\\Camera" FILES
    ${camera_source}
)

set (light_source
	Core/Light/AmbientLight.cpp
	Core/Light/AmbientLight.h
	Core/Light/Light.cpp
	Core/Light/Light.h
	Core/Light/DirectionalLight.cpp
	Core/Light/DirectionalLight.h
	Core/Light/PointLight.cpp
	Core/Light/PointLight.h
	Core/Light/SpotLight.cpp
	Core/Light/SpotLight.h
)

list (APPEND MyG_E_sources
	${light_source}
)
source_group ("Core\\Light" FILES
	${light_source}
)

set (texture_source
	Core/Texture/BasicTexture2D.cpp
	Core/Texture/BasicTexture2D.h
	Core/Texture/Texture2D.cpp
	Core/Texture/Texture2D.h
	Core/Texture/TextureCubMap.cpp
	Core/Texture/TextureCubMap.h
	Core/Texture/Texture.cpp
	Core/Texture/Texture.h
)

list (APPEND MyG_E_sources
	${texture_source}
)
source_group ("Core\\Texture" FILES
	${texture_source}
)

set (math_source
	Foundation/Math/EulerAngles.cpp
	Foundation/Math/EulerAngles.h
	Foundation/Math/ExtraMathConstant.h
	Foundation/Math/Matrix.cpp
	Foundation/Math/Matrix.h
	Foundation/Math/Quaternion.cpp
	Foundation/Math/Quaternion.h
	Foundation/Math/Vector.cpp
	Foundation/Math/Vector.h
)

list (APPEND MyG_E_sources
    ${math_source}
)
source_group ("Foundation\\Math" FILES
    ${math_source}
)

set (ui_source
	Foundation/UI/FileBrowser.cpp
	Foundation/UI/FileBrowser.h
)

list (APPEND MyG_E_sources
    ${ui_source}
)
source_group ("Foundation\\UI" FILES
    ${ui_source}
)

set (precompiled_source
    Foundation/Precompiled/pch.cpp
)

list (APPEND MyG_E_sources
    ${precompiled_source}
)
source_group ("Foundation\\Precompiled" FILES
    ${precompiled_source}
)

set (project_source
	Foundation/Project/ProjectFileReader.cpp
	Foundation/Project/ProjectFileReader.h
	Foundation/Project/ProjectFileWriter.cpp
	Foundation/Project/ProjectFileWriter.h
)

list (APPEND MyG_E_sources
    ${project_source}
)
source_group ("Foundation\\Project" FILES
    ${project_source}
)

set (foundation_source
    Foundation/Game.cpp
    Foundation/Game.h
    Foundation/Gldebug.cpp
    Foundation/Gldebug.h
	Foundation/ImGuiLayer.cpp
	Foundation/ImGuiLayer.h
    Foundation/Input.cpp
    Foundation/Input.h
	Foundation/Layer.h
	Foundation/LayerCollection.cpp
	Foundation/LayerCollection.h
    Foundation/Main.cpp
    Foundation/Uncopyable.h
	Foundation/Utility.h
)

list (APPEND MyG_E_sources
    ${foundation_source}
)
source_group ("Foundation" FILES
    ${foundation_source}
)

set (shaders_source
    Shaders/Blinn-Phong.frag
    Shaders/GaussianBlur.vert
    Shaders/GaussianBlurHorizontal.frag
    Shaders/GaussianBlurVertical.frag
    Shaders/Light.frag
    Shaders/Light.vert
    Shaders/Normal.frag
    Shaders/Normal.geom
    Shaders/Normal.vert
    Shaders/Phong.frag
    Shaders/Phong.vert
	Shaders/ShadowMapDebug.vert
	Shaders/ShadowMapDebug.frag
    Shaders/ShadowMap.frag
    Shaders/ShadowMap.vert
    Shaders/SkyBox.frag
    Shaders/SkyBox.vert
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
set_property(TARGET MyG_E 
    PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded"
)


#Precompiled
add_precompiled_header(MyG_E Foundation/Precompiled/pch.h SOURCE_CXX Foundation/Precompiled/pch.cpp FORCEINCLUDE)

#Link

target_link_libraries (MyG_E
    glad
    imgui
    imstb
    rapidjson
)

target_link_libraries (MyG_E     
    ${ASSIMP_LIBRARIES}
    ${GLFW3_LIBRARIES}
    ${IRRXML_LIBRARIES}
    ${ZLIB_LIBRARIES}
)

#Opengl
find_package (OpenGL REQUIRED)
target_include_directories (MyG_E PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries (MyG_E ${OPENGL_gl_LIBRARY})

#install
install (TARGETS MyG_E DESTINATION bin)