//
// Created by federico on 11/08/2021.
//


#include <Scene.h>

int main() {
    ApplicationParams params{800,600};
    GLApplication app(params);

    Scene scene{};
    app.renderableScene = &scene;
    Scene::loadSceneFromJSONFile("../scenes/testScene.json",*app.renderableScene);
    app.renderableScene->syncIllumination();
    app.renderableScene->objectDOM["central_cube"]->objModelFun = [](float time) {
        return glm::rotate(glm::mat4(1.f),time*glm::radians(90.f),glm::vec3(0.,1.,0.));
    };
    app.renderableScene->objectDOM["light_cube"]->canCastShadow = false;
    std::cout << "Running application..." << std::endl;
    app.run();
}