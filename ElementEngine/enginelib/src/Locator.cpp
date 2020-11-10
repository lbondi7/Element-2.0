//
// Created by lbondi7 on 10/11/2020.
//

#include "Locator.h"
#include "Resources.h"

std::unique_ptr<Element::Resources> Element::Locator::resources;

Element::Resources* Element::Locator::getResource() {
    return resources.get();
}

void Element::Locator::initResources(std::unique_ptr<Resources> _resources) {
    resources = std::move(_resources);
    resources->init();
}

void Element::Locator::deInit() {
    resources->deInit();
}
