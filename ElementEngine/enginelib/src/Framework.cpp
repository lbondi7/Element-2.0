#include <element/Framework.h>
#include "VknRenderer.h"

Element::Framework::~Framework()
{
//    m_renderer->deInit();
}

void Element::Framework::renderFrame()
{
   m_renderer->renderFrame();
 }

void Element::Framework::beginFrame()
{
    m_renderer->beginFrame();
}

void Element::Framework::endFrame()
{
    m_renderer->endFrame();
}

void Element::Framework::start()
{
    m_renderer = std::make_unique<VknRenderer>();
    m_renderer->init();
}
