# BGFX_QML

I want to draw a 3D scene with BGFX and render my QML gui on top of it using the RenderAPI abstraction layer provided by Qt. 
I took the **[OpenGL under QML](https://doc-snapshots.qt.io/qt5-dev/qtquick-scenegraph-openglunderqml-example.html)** and **[Vulkan under QML](https://doc-snapshots.qt.io/qt5-dev/qtquick-scenegraph-vulkanunderqml-example.html)** 
examples shipped with Qt as a starting point and used the **[bgfx-qt5-win](https://github.com/PetrPPetrov/bgfx-qt5-win)** example by PetrPPetrov as insipiration on how to build and initialize BGFX.

Before BGFX initialization I query the OpenGL context, DX11 device or VkDevice resource used by Qt and pass it to the bgfx::Init struct via the platformData.context field and call bgfx::renderFrame() prior to the bgfx::init() call.

Clearly I am doing something wrong, because I get exceptions with the 3 rendering backends i tried so far (OpenGL, DX11 and Vulkan).

 **OpenGL** seems to draw the first frame ok but crashes on the second frame with 
 \src\renderer_gl.cpp (6944): BGFX CHECK glBindVertexArray(defaultVao); GL error 0x502: GL_INVALID_OPERATION
 
Using **Direct3D11** leads to an exception during initialization in bgfx::DXgi::createSwapChain with 
 *Exception at 0x7ff991869709, code: 0xe06d7363: C++ exception, flags=0x1 (execution cannot be continued) (first chance) in dxgi!DXGIDeclareAdapterRemovalSupport*
 
Similarly **Vulkan** stops during initialization with 
\bgfx\src\renderer_vk.cpp (967): BGFX Create swapchain error: vkCreateSwapchainKHR failed -1000011001: VK_ERROR_VALIDATION_FAILED_EXT.
\bgfx\src\renderer_vk.cpp (2070): BGFX Init error: creating swapchain and image view failed -1000011001: VK_ERROR_VALIDATION_FAILED_EXT
\bgfx\src\renderer_vk.cpp (2287): BGFX errorState 4
