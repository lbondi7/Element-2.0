#include <element/Window.h>

const std::string& Element::Window::getTitle()
{
    return title;
}


bool Element::Window::getFramebufferResized()
{
    return framebufferResized;
}

void Element::Window::setFramebufferResized(bool _framebufferResized)
{
    framebufferResized = _framebufferResized;
}
