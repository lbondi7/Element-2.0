#include <element/Engine.h>

#include "VknRenderer.h"
#include "VknConstants.h"
#include "ValidationLayers.h"
#include "ImGUI_.h"

#include <element/Debugger.h>
#include <element/Inputs.h>

void Element::Engine::run()
{
    Time epoch;
    start();
    init();

    epoch.updateDeltaTime();

    while (!m_renderer->getWindow()->shouldClose())
    {
        epoch.updateDeltaTime();
        epoch.updateFPS();

        m_renderer->getWindow()->pollEvents();
        update(epoch);
        if (m_renderer->getWindow()->tooSmall())
        {
            //Debugger::Get().log("Too small", DebuggerColours::BRIGHT_RED);
            continue;
        }


        if (epoch.getFrame(epoch.getRenderFrameStep()))
        {
           // Debugger::Get().log("COOL", DebuggerColours::BRIGHT_BLUE);
            beginFrame();
            ImGUI::Get().start();
            render();
            //ImGUI::Get().end();
            renderFrame();
            endFrame();
        }
        Inputs::get().clear();
    }
    deInit();
}

void Element::Engine::renderFrame()
{
    m_renderer->renderFrame();
}

void Element::Engine::beginFrame()
{
    m_renderer->beginFrame();
}

void Element::Engine::endFrame()
{
    m_renderer->endFrame();
}

void Element::Engine::start()
{
    m_renderer = std::make_unique<VknRenderer>();
    m_renderer->init();
}

void Element::Engine::deInit() {
    m_renderer->deInit();
    ValidationLayers::get().Destroy();
}


