#include "Object.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <stdexcept>
#include <chrono>
#include <random>

Object::Object()
{
    std::random_device rd;
    std::uniform_int_distribution<uint32_t> idDist(1, 10000000);
    id = idDist(rd);

    model = nullptr;
}

Object::~Object()
{

}

void Object::init()
{

    //model = "cube";
    //texture = "default";
}
