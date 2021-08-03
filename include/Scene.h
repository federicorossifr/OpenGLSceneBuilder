//
// Created by federico on 03/08/2021.
//
#include "Illumination.h"
#include <RenderableObject.h>
class RenderableObject;
class Scene {
public:
    std::vector<RenderableObject> objects;
    struct {
        DirectionalLight directionalLight;
        std::vector<PointLight> pointLights;
        FlashLight flashLight;
    } illumination;
};