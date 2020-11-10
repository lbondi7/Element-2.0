//
// Created by lbondi7 on 10/11/2020.
//

#include "Locator.h"
#include "Resources.h"
#include "VknResources.h"

std::unique_ptr<Element::Resources> Element::Locator::resources;
std::unique_ptr<Element::VknResources> Element::Locator::vknResources;

Element::Resources* Element::Locator::getResource() {
    return resources.get();
}

void Element::Locator::initResources(std::unique_ptr<Resources> _resources) {
    resources = std::move(_resources);
    resources->init();
}

void Element::Locator::deInit() {
    resources->destroy();
    vknResources->destroy();
}

Element::VknResources *Element::Locator::getVknResource() {
    return vknResources.get();
}

void Element::Locator::initVknResources(std::unique_ptr<VknResources> _resources) {
    vknResources = std::move(_resources);
}
