//
// Created by federico on 11/08/2021.
//


#include <Scene.h>

int main() {
    ApplicationParams params{800,600};
    GLApplication app(params);

    Scene scene{};
    app.renderableScene = &scene;
    Scene::loadSceneFromJSONFile("../scenes/testScene.json",scene);
    app.renderableScene->syncIllumination();
    app.run();
}