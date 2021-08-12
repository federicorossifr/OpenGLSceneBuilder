//
// Created by federico on 11/08/2021.
//


#include <Scene.h>

auto lightTransform = [](float time) -> glm::mat4 {
    return glm::rotate(glm::mat4(1.f),time*glm::radians(90.f),glm::vec3(0.,1.,0.));
};

int main() {
    ApplicationParams params{800,600};
    GLApplication app(params);

    Scene scene{};
    app.renderableScene = &scene;
    Scene::loadSceneFromJSONFile("../scenes/testScene.json",*app.renderableScene);
    app.renderableScene->syncIllumination();

    const auto initialLightPos = glm::vec4(app.renderableScene->illumination.pointLights[0].position,1.);

    app.renderableScene->objectDOM["light_cube"]->canCastShadow = false;
    app.renderableScene->objectDOM["light_cube"]->objModelFun = [&](float time) {
        auto lpos = lightTransform(time)*initialLightPos;
        app.renderableScene->illumination.pointLights[0].position = lpos;
        return lightTransform(time);
    };

    std::cout << "Running application..." << std::endl;
    app.run();
}