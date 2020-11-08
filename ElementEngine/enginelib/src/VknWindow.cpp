#include "VknWindow.h"
#include <element/GameSettings.h>
#include <iostream>

Element::VknWindow::VknWindow()
{
    const auto& settings = GameSettings::Instance();
    this->width = windowedWidth = settings.windowWidth;
    this->height = windowedHeight = settings.windowHeight;
    this->refreshRate = settings.fpsLimit;
    this->title = settings.windowTitle;
    this->mode = settings.windowMode;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    glfwWindow = glfwCreateWindow(width, height,
        title.c_str(), nullptr, nullptr);

    glfwGetWindowPos(glfwWindow, &xpos, &ypos);

    glfwMonitors = glfwGetMonitors(&monitorCount);

    for (int i = 0; i < monitorCount; i++)
    {
        monitors.emplace_back();
        monitors[i].glfwMonitor = glfwMonitors[i];
        const GLFWvidmode* mode = glfwGetVideoMode(glfwMonitors[i]);
        monitors[i].height = mode->height;
        monitors[i].width = mode->width;
        monitors[i].refreshRate = mode->refreshRate;
        monitors[i].id = i;
        monitors[i].name = glfwGetMonitorName(glfwMonitors[i]);
    }
    currentMonitor = 1;

    setMode(mode);
}

Element::VknWindow::~VknWindow()
{
    for (auto& monitor : monitors)
    {
        monitor.glfwMonitor = nullptr;
    }
    glfwMonitors = nullptr;
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
}

void Element::VknWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
    auto app = reinterpret_cast<Element::VknWindow*>(glfwGetWindowUserPointer(window));
    app->framebufferResized = true;
}

void Element::VknWindow::setResizeCallback()
{
    glfwSetFramebufferSizeCallback(glfwWindow, framebufferResizeCallback);
}

void Element::VknWindow::setUserPointer(void* pointer)
{
    glfwSetWindowUserPointer(glfwWindow, pointer);
}

GLFWwindow* Element::VknWindow::GetGLFWWindow()
{
    return glfwWindow;
}

glm::vec2 Element::VknWindow::getSize()
{
    glfwGetWindowSize(glfwWindow, &width, &height);
    return glm::vec2(width, height);
}

glm::vec2 Element::VknWindow::getPosition()
{
    glfwGetWindowPos(glfwWindow, &xpos, &ypos);
    return glm::vec2(xpos, ypos);
}

void Element::VknWindow::setTitle(const std::string& _title)
{
    title = _title;
    glfwSetWindowTitle(glfwWindow, title.c_str());
}

void Element::VknWindow::setSize(int _width, int _height)
{
    if (mode == WindowMode::FULL_SCREEN || mode == WindowMode::BORDERLESS_FULLSCREEN)
        return;

    width = windowedWidth =_width;
    height = windowedHeight = _height;

    glfwSetWindowSize(glfwWindow, width, height);
    framebufferResized = true;
}

void Element::VknWindow::setMode(WindowMode _mode)
{
    if (_mode == Element::WindowMode::BORDERLESS_WINDOWED || _mode == mode)
        return;

    int refresh = refreshRate <= monitors[currentMonitor].refreshRate ? refreshRate : monitors[currentMonitor].refreshRate;

    switch (_mode)
    {
    case Element::WindowMode::WINDOWED:
        glfwSetWindowMonitor(glfwWindow, nullptr, 50, 50, windowedWidth, windowedHeight, refresh);
        break;
    case Element::WindowMode::FULL_SCREEN:
        glfwGetWindowSize(glfwWindow, &windowedWidth, &windowedHeight);
        glfwSetWindowMonitor(glfwWindow, monitors[currentMonitor].glfwMonitor, 0, 0,
            monitors[currentMonitor].width, monitors[currentMonitor].height, refresh);
        break;
    case Element::WindowMode::BORDERLESS_FULLSCREEN:
        glfwGetWindowSize(glfwWindow, &windowedWidth, &windowedHeight);
        glfwSetWindowMonitor(glfwWindow, monitors[currentMonitor].glfwMonitor, 0, 0,
            monitors[currentMonitor].width, monitors[currentMonitor].height, monitors[currentMonitor].refreshRate);
        break;
//#if NDEBUG
//    case Element::WindowMode::FULL_SCREEN:
//        glfwSetWindowMonitor(glfwWindow, monitors[currentMonitor].glfwMonitor, 0, 0,
//            monitors[currentMonitor].width, monitors[currentMonitor].height, refresh);
//        break;
//    case Element::WindowMode::BORDERLESS_FULLSCREEN:
//        glfwSetWindowMonitor(glfwWindow, monitors[currentMonitor].glfwMonitor, 0, 0,
//            monitors[currentMonitor].width, monitors[currentMonitor].height, monitors[currentMonitor].refreshRate);
//        break;
//#else
//    default:
//        return;
//
    }

    mode = _mode;
    framebufferResized = true;
}

void Element::VknWindow::setCurrentMonitor(int _monitor)
{
    if (_monitor >= monitors.size())
        _monitor = monitors.size() - 1;

    currentMonitor = _monitor;

    setMode(mode);
}

void Element::VknWindow::close() {
    glfwSetWindowShouldClose(glfwWindow, GLFW_TRUE);
}

bool Element::VknWindow::tooSmall()
{
    glfwGetWindowSize(glfwWindow, &width, &height);
    return width == 0 || height == 0;
}

int Element::VknWindow::shouldClose()
{
    return glfwWindowShouldClose(glfwWindow);
}

void Element::VknWindow::pollEvents()
{
    glfwPollEvents();
}
