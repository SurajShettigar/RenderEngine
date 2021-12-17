#ifndef RENDERER_H
#define RENDERER_H

#include <glfw/glfw3.h>
#include "renderer_exception.h"

enum class InputType
{
    CAMERA_ROTATE,
    CAMERA_PAN,
    CAMERA_ZOOM,
    OBJECT_FRAME_SCREEN,
    RENDER_IMAGE
};

class Renderer
{
protected:
    int winWidth = 1280;
    int winHeight = 720;
    const char *winName = "Window";

public:
    Renderer() = default;
    virtual void start(GLFWwindow* window) = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void destroy() = 0;

    virtual void onWindowResized() = 0;

    virtual void onInput(InputType input, float value_x, float value_y) = 0;
};

#endif