# Raytracing Render Engine with Vulkan viewport

## Pre-requistes
1. This project uses git-lfs. Make sure git-lfs is installed, before pulling the repository.
2. VS Code.
3. mingw compiler.
4. Vulkan SDK with VULKAN_SDK environment variable set.

## Running
1. Under `main.cpp` set `RENDER_SILENT` to `1`, to test out offline render engine. Set it to `0` to test vulkan viewport.
2. When in vulkan viewport, press `ENTER` key to start rendering. (Currently the viewport freezes, since I am just waiting for thread join. Will fix eventually).
2. You can change `RENDER_IMAGE` and `MODEL_FILE` to render out to different image file and use different obj model file respectively.
3. `teddy.obj` takes around `15` seconds to render with samples per pixel of `16` and resolution of `640x360`.
4. `bunny.obj` takes around `180` seconds to render with samples per pixel of `16` and resolution of `640x360`.


## Implemented Features
<ul>
<li> Basic raytracer.</li>
<li> Generate rendered png images.</li>
<li> Multi-threaded rendering.</li>
<li> Load and render 3D mesh objects from file.</li>
<li> Basic vulkan viewport.</li>
</ul>

## WIP
<ul>
<li> Render progress indicator. </li>
<li> Show 3D model in vulkan viewport. </li>
<li> Interactive placement of camera and 3D object in vulkan viewport.</li>
<li> Live preview of raytracing.</li>
</ul>

## RENDERS
![Teddy](/renders/teddy_render_01.png "Teddy")
![Bunny](/renders/bunny_render_01.png "Bunny")