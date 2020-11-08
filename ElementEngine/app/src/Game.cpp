#include "Game.hpp"
#include <element/GameSettings.h>
#include <element/IdHandler.h>
#include <element/Debugger.h>
#include <element/Inputs.h>
#include <element/DebugWindow.h>

Game::Game()
{
	using namespace Element;
	GameSettings::Instance().windowWidth = 900;
	GameSettings::Instance().windowHeight = 600;
	GameSettings::Instance().windowTitle = "Epic Element Engine";
	GameSettings::Instance().gameDimension = GameSettings::Dimension::_3D;
	GameSettings::Instance().windowMode = WindowMode::WINDOWED;
	GameSettings::Instance().fpsLimit = 60;
	GameSettings::Instance().msaaLevel = GameSettings::Multisample::MSAA_1;
	GameSettings::Instance().depthEnabled = true;
} 

Game::~Game()
{
    m_renderer->deInit();
}

void Game::init()
{
	glm::vec3 pos(0, 0, 0);
	objects.resize(1);
	for (auto& object : objects)
	{
		object.model = m_renderer->createModel();
		object.model->SetMesh(m_renderer->getMesh("cube"));
		object.model->GetTransform().setPosition(pos); 
		//object.model->GetTransform().setScale();
		pos += glm::vec3(2, 2, 2); 
	} 
	 
	camera = m_renderer->createCamera(Element::CameraType::ORTHOGRAPHIC);
	//camera = m_renderer->createOrthographicCamera();
	//glm::vec4 viewport = camera->GetViewport();
	//viewport.x = 0.5f; 
	//viewport.y = 0.5f;
	//viewport.z = 0.5f;
	//viewport.w  = 0.5f;
	//camera->setViewport(viewport);

	//camera->SetPosX(m_renderer->getWindow()->getSize().x );
	//camera->SetPosY(m_renderer->getWindow()->getSize().y );
	m_renderer->setCamera(camera);

	sprite = m_renderer->createNewSprite();
	sprite->GetTransform().setPosition(0.0f, 0.0f);
	sprite->GetTransform().setSize(100, 100);
	frameTime.resize(10);
}

void Game::update(Element::Time& epoch)
{
	auto dt_sec = epoch.getDelta_f();

	//sprite->GetTransform().setPositionY(sprite->GetTransform().getPositionY() + dt_sec);

	float zoomSpeed = 100.0f;

	if (!Element::Inputs::Get().keyDown(Element::KEYS::KEY_KP_1))
		camera->SetZoom(camera->GetZoom() + zoomSpeed * dt_sec);
	if (!Element::Inputs::Get().keyDown(Element::KEYS::KEY_KP_3))
		camera->SetZoom(camera->GetZoom() - zoomSpeed * dt_sec);


	if (Element::Inputs::Get().keyDown(Element::KEYS::KEY_ESCAPE))
	{
		m_renderer->signalExit();
	}

	epoch.getFPS(fps);
	if (fps != static_cast<int>(frameTime[counter]))
	{
		frameTime[counter] = fps;
		if (counter + 1 >= frameTime.size())
		{
			frameTime[0] = fps;
			counter = 0;
		}
		else {
			frameTime[counter + 1] = fps;
		}
		counter++;
	}
}

void Game::render()
{
	for (const auto& object : objects)
	{
		m_renderer->renderModel(object.model);
	}
	//if(Help)
	//	m_renderer->renderSprite(sprite);



	Element::DebugWindow window;
	window.graph("My Int", &frameTime[0], 900.0f, 1300.0f, 10);

}


