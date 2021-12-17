#include "window.h"

static void onFrameBufferResized(GLFWwindow *window, int width, int height)
{
    Window *windowObj = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
    windowObj->onWindowResized();
}

void Window::handleInput()
{
    /// Handle Camera orbit, pan, zoom
    if (glfwGetKey(m_window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS)
    {
        if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS)
        {
            double xPos, yPos;
            glfwGetCursorPos(m_window, &xPos, &yPos);
            double deltaX = xPos;
            double deltaY = yPos;

            if(xPos < prevMouseXPos)
                deltaX = -1.0;
            else if(xPos > prevMouseXPos)
                deltaX = 1.0;
            else
                deltaX = 0.0;
                
            if(yPos < prevMouseYPos)
                deltaY = -1.0;
            else if(yPos > prevMouseYPos)
                deltaY = 1.0;
            else
                deltaY = 0.0;

            m_renderer->onInput(InputType::CAMERA_ROTATE, deltaX, deltaY);
        }
        else if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
        {
            double xPos, yPos;
            glfwGetCursorPos(m_window, &xPos, &yPos);
            double deltaX = xPos - prevMouseXPos;
            double deltaY = xPos - prevMouseXPos;
            m_renderer->onInput(InputType::CAMERA_PAN, deltaX, deltaY);
        }
        else if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS)
        {
            double xPos, yPos;
            glfwGetCursorPos(m_window, &xPos, &yPos);
            double deltaX = xPos - prevMouseXPos;
            double deltaY = xPos - prevMouseXPos;
            m_renderer->onInput(InputType::CAMERA_ZOOM, deltaX, deltaY);
        }
    }
    glfwGetCursorPos(m_window, &prevMouseXPos, &prevMouseXPos);
}

void Window::start()
{
    if (glfwInit() == GLFW_FALSE)
    {
        throw std::runtime_error("Failed to initialize Window");
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_window = glfwCreateWindow(m_width, m_height, m_name, nullptr, nullptr);
    glfwSetWindowUserPointer(m_window, this);
    glfwSetFramebufferSizeCallback(m_window, onFrameBufferResized);

    m_renderer->start(m_window);
}

void Window::update()
{
    m_renderer->update();
}

void Window::render()
{
    m_renderer->render();
}

void Window::destroy()
{
    m_renderer->destroy();
}

int Window::show()
{
    try
    {
        start();
        while (!glfwWindowShouldClose(m_window))
        {
            handleInput();
            glfwPollEvents();
            update();
            render();
        }
        destroy();
        return 0;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
}

void Window::onWindowResized()
{
    m_renderer->onWindowResized();
}