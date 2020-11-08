#include <element/Renderer.h>

void Element::Renderer::setCamera(Camera* _camera)
{
    camera = _camera;
    camera->setReady(true);

}
