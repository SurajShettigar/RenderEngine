#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <stdexcept>
#include <glfw/glfw3.h>

#include "renderer.h"

class Window
{
private:
    GLFWwindow *m_window;
    Renderer *m_renderer;

    int m_width;
    int m_height;
    const char *m_name;

    double m_prevMouseXPos;
    double m_prevMouseYPos;

    void (*m_render_callback)();

protected:
    void handleInput();

    void start();
    void update();
    void render();
    void destroy();

public:
    Window(Renderer *renderer, int width, int height, const char *name)
        : m_renderer{renderer}, m_width{width}, m_height{height}, m_name{name} {}
        
    int show();
    void setRenderListener(void (*callback)())
    {
        m_render_callback = callback;
    }
    void onWindowResized();
};
#endif