#include "Game.hpp"
#include <element/GameSettings.h>
#include <element/IdHandler.h>
#include <element/Debugger.h>
#include <element/Inputs.h>
#include <element/DebugWindow.h>

Game::Game()
{
	using namespace Element;
    GameSettings::get().windowWidth = 900;
    GameSettings::get().windowHeight = 600;
    GameSettings::get().windowTitle = "Epic Element Engine";
    GameSettings::get().gameDimension = GameSettings::Dimension::_3D;
    GameSettings::get().windowMode = WindowMode::WINDOWED;
    GameSettings::get().fpsLimit = 60;
    GameSettings::get().msaaLevel = GameSettings::Multisample::MSAA_8;
    GameSettings::get().depthEnabled = true;
} 

Game::~Game()
{

}

void Game::init()
{
	Vec3 pos(0, 0, 0);
	objects.resize(1);
	for (auto& object : objects)
	{
		object.model = m_renderer->createModel();
		object.model->SetMesh(m_renderer->getMesh("dragon"));
		object.model->GetTransform().setPosition(pos);
		pos += Vec3(1.2, 1, -2);
	} 
	 
	camera = m_renderer->createUniqueCamera(Element::CameraType::PERSPECTIVE);
    //camerab = m_renderer->createUniqueCamera(Element::CameraType::PERSPECTIVE);
    //camerab->setEnabled(false);
	//camera = m_renderer->createOrthographicCamera();
	//glm::vec4 viewport = camera->GetViewport();
	//viewport.x = 0.5f; 
	//viewport.y = 0.5f;
	//viewport.z = 0.5f;
	//viewport.w  = 0.5f;
	//camera->setViewport(viewport);

	//camera->SetPosX(m_renderer->getWindow()->getSize().x );
	//camera->SetPosY(m_renderer->getWindow()->getSize().y );
    //m_renderer->addCamera(camera, 0);

	//sprite = m_renderer->createNewSprite();
	//sprite->GetTransform().setPosition(0.0f, 0.0f);
	//sprite->GetTransform().setSize(100, 100);
	frameTime.resize(10);
}

void Game::update(Element::Time& epoch)
{
	auto dt_sec = epoch.getDelta_f();

	//sprite->GetTransform().setPositionY(sprite->GetTransform().getPositionY() + dt_sec);

	float zoomSpeed = 100.0f;

    float moveSpeed = 10.0f;

//	if (Element::Inputs::get().keyHeld(Element::KEYS::KEY_KP_1))
//		camera->SetZoom(camera->GetZoom() + zoomSpeed * dt_sec);
//	if (Element::Inputs::get().keyHeld(Element::KEYS::KEY_KP_3))
//		camera->SetZoom(camera->GetZoom() - zoomSpeed * dt_sec);

    if (Element::Inputs::get().keyDown(Element::KEYS::KEY_P))
        objects[0].model->SetTexture(m_renderer->getTexture("texture"));

    if (Element::Inputs::get().keyHeld(Element::KEYS::KEY_KP_9))
        camera->setRotY( camera->getRotY() - zoomSpeed * dt_sec);
    if (Element::Inputs::get().keyHeld(Element::KEYS::KEY_KP_7))
        camera->setRotY(camera->getRotY() + zoomSpeed * dt_sec);

    if (Element::Inputs::get().keyHeld(Element::KEYS::KEY_KP_8))
        camera->setPos( camera->getPos() + (camera->GetForward() * moveSpeed * dt_sec));
    if (Element::Inputs::get().keyHeld(Element::KEYS::KEY_KP_5))
        camera->setPos( camera->getPos() - (camera->GetForward() * moveSpeed * dt_sec));

    if (Element::Inputs::get().keyHeld(Element::KEYS::KEY_KP_4))
        camera->setPos( camera->getPos() - (camera->GetRight() * moveSpeed * dt_sec));
    if (Element::Inputs::get().keyHeld(Element::KEYS::KEY_KP_6))
        camera->setPos( camera->getPos() + (camera->GetRight() * moveSpeed * dt_sec));

    if (Element::Inputs::get().keyHeld(Element::KEYS::KEY_KP_1))
        camera->setPos( camera->getPos() - (camera->GetUp() * moveSpeed * dt_sec));
    if (Element::Inputs::get().keyHeld(Element::KEYS::KEY_KP_3))
        camera->setPos( camera->getPos() + (camera->GetUp() * moveSpeed * dt_sec));


	if (Element::Inputs::get().keyDown(Element::KEYS::KEY_ESCAPE))
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
		//m_renderer->renderSprite(sprite);



	Element::DebugWindow window;
	window.graph("My Int", &frameTime[0], 900.0f, 1300.0f, 10);

}


