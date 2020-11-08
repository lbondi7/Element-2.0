#ifndef VULKANTEST_GAME_H
#define VULKANTEST_GAME_H

#include <element/Framework.h>

#include <element/Sprite.h>
#include "Object.hpp"
#include <vector>

class Game : public Element::Framework
{
public:
	Game();
	~Game();

	void init() override;
	void update(Element::Time& epoch) override ;
	void render() override;

private:

	std::vector<Object> objects;
	Element::Camera* camera;
	Element::Sprite* sprite;
	float timeCount = 0.0f;

	bool renderStuff = true;
	float number = 2.0f;
	int fps = 0;
	std::vector<float> frameTime{ 0.0f };
	int counter = 0;
	bool Help = true;
};

#endif