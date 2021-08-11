//
// Created by federico on 03/08/2021.
//
#pragma once
#include <glm/glm.hpp>
#include "Illumination.h"
#include <RenderableObject.h>
#include <simdjson.h>
class RenderableObject;

class Scene {
public:
    std::vector<RenderableObject> objects;
    std::unordered_map<std::string,RenderableObject*> objectDOM;
    struct {
        DirectionalLight directionalLight;
        std::vector<PointLight> pointLights;
        FlashLight flashLight;
    } illumination;

    void syncIllumination();
    static void loadSceneFromJSONFile(std::string jsonPath,Scene& scene);

};



