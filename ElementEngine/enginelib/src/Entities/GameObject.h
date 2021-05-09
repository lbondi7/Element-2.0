//
// Created by Lewis on 11/03/2021.
//

#pragma once

#include <Components/Component.h>
#include <Physics/Collider/Collider.h>

#include <vector>
#include <map>

namespace Element{
    class GameObject {
    public:
        GameObject() =default;

        ~GameObject() = default;


        template<class T>
        T GetComponent(int val);

    private:

        std::map<int, Component*> comps;

    };


    template<class T>
    T Element::GameObject::GetComponent(int val) {

        switch (val) {
            case 0:
                return dynamic_cast<Collider*>(comps[0]);
            case 1:
                return dynamic_cast<Collider*>(comps[0]);
        }
        return nullptr;
    }
};

//ELEMENTENGINE_GAMEOBJECT_H
