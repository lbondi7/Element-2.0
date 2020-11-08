#include <element/Engine.h>

#include "VknConstants.h"
#include <element/Debugger.h>
#include <element/Inputs.h>
#include "ImGUI_.h"

void Element::Engine::run()
{
    Time epoch;
    start();
    init();
    
    Inputs::Get().init(m_renderer->getWindow()->GetGLFWWindow());

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
            ImGUI::Get().end();
            renderFrame();
            endFrame();
        }
        Inputs::Get().clear();
    }
}

