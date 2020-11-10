//
// Created by lbondi7 on 10/11/2020.
//

#pragma once

#include <memory>

namespace Element {
    class Resources;
    class VknResources;
}

namespace Element {

    struct Locator {
    public:

        static Resources* getResource();

        static void initResources(std::unique_ptr<Resources> _resources);

        static VknResources* getVknResource();

        static void initVknResources(std::unique_ptr<VknResources> _resources);

        static void deInit();

    private:
        static std::unique_ptr<Resources> resources;
        static std::unique_ptr<VknResources> vknResources;
    };

}
//ELEMENTENGINE_LOCATOR_H
