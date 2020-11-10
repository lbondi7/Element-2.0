//
// Created by lbondi7 on 10/11/2020.
//

#pragma once

#include <memory>

namespace Element {
    class Resources;
}

namespace Element {

    struct Locator {
    public:

        static Resources* getResource();

        static void initResources(std::unique_ptr<Resources> _resources);

        static void deInit();

    private:
        static std::unique_ptr<Resources> resources;

    };

}
//ELEMENTENGINE_LOCATOR_H
