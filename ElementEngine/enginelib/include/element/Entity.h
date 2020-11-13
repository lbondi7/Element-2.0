//
// Created by lbondi7 on 13/11/2020.
//

#pragma once

namespace Element {

    class Entity{

    public:

        //bool isDestroyed() const {return destroyed;}

        virtual void destroy() = 0;

    protected:
        bool destroyed = false;

    };

}

//ELEMENTENGINE_ENTITY_H
