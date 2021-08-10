//
// Created by federico on 10/08/2021.
//

#include <Scene.h>

void Scene::syncIllumination() {
    for(auto obj:objects) {
        obj.setDirectionalLight(this->illumination.directionalLight);
        obj.setPointLights(this->illumination.pointLights);
        obj.setFlashLight(this->illumination.flashLight);
    }
}
