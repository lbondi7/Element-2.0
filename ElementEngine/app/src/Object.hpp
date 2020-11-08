#ifndef VULKANTEST_OBJECT_H
#define VULKANTEST_OBJECT_H

#include <element/Model.h>

class Object
{
public:
	Object();
	~Object();

    uint32_t id = -1;
    Element::Model* model;
    void init();

private:

};

#endif