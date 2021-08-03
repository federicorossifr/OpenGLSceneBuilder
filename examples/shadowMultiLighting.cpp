//
// Created by federico on 24/07/2021.
//



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ShaderHandler.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <RenderableObject.h>
#include <GLApplication.h>
#include <VertexGenerators.h>
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

#define R glm::vec3(1.0f, 0.5f, 0.31f)
#define G glm::vec3(0.,1.,0.)
#define B glm::vec3(0.,0.,1.)
#define W glm::vec3(1.,1.,1.)
#define WHITE_LIGHT {W,W,W,W,W,W,W,W}
#define BLUE_LIGHT {B,B,B,B,B,B,B,B}
#define YELLOW_LIGHT {G,G,G,G,G,G,G,G}
#define NOTEXT {-1.,-1.}



int main()
{
    glm::vec3 lightPos(9.,1.,0.);
    glm::vec3 lightPos2(-0.,1.,9.);
    glm::vec3 directional(20, -20.f, 20.f);

    Material material2{};
    material2.specular = {0.5f, 0.5f, 0.5f};
    material2.shininess = 32.0f;


    LightProperties ill{};
    ill.ambient = {0.2f, 0.2f, 0.2f};
    ill.diffuse = { 0.4f, 0.4f, 0.4f};
    ill.specular =  {0.2f, 0.2f, 0.2f};


    DirectionalLight dirLight{};
    dirLight.properties = ill;
    dirLight.direction = directional;


    ApplicationParams params{800,600};
    GLApplication app(params);
    app.renderableScene.illumination.directionalLight = dirLight;
    auto cube2 = glTests::createPlaneWithNormal({0.f,0.f,0.f},10.f,{R,R,R,R,R,R,R,R});
    RenderableObject obj2("shaders/ShadowTextureLightingMap.vert.spv","shaders/ShadowMultiLightingMap.frag.spv",std::move(cube2.first),std::move(cube2.second));
    obj2.setTexture("../textures/metal.jpg",false);
    obj2.setTexture("../textures/metal_specular.jpg",false);
    obj2.enableNormalAttribute();
    obj2.setTextureMaterial(material2);
    obj2.setDirectionalLight(dirLight);
    obj2.shaderHandler->setScalarUniform("shadowMap",31);
    obj2.objModelFun = [](float time) {
        return glm::rotate(glm::mat4(1.f),time*glm::radians(0.f),glm::vec3(0.,1.,0.));
    };
    obj2.postModelFun = [&](float time,GLApplication* app) {
        obj2.shaderHandler->setVec3Uniform("viewPos",app->camera.Position);
    };
    app.addRenderableObject(obj2);

    auto cube = glTests::createCubeWithNormal({-2.5f,4.f,-2.5f},2.f,{R,R,R,R,R,R,R,R});
    RenderableObject cubeObj("shaders/ShadowTextureLightingMap.vert.spv","shaders/ShadowMultiLightingMap.frag.spv",std::move(cube.first),std::move(cube.second));
    cubeObj.setTexture("../textures/container2.png",true);
    cubeObj.setTexture("../textures/container2_specular.png",true);
    cubeObj.enableNormalAttribute();
    cubeObj.setTextureMaterial(material2);
    cubeObj.setDirectionalLight(dirLight);
    cubeObj.shaderHandler->setScalarUniform("shadowMap",31);

    cubeObj.objModelFun = [](float time) {
        return glm::rotate(glm::mat4(1.f),time*glm::radians(0.f),glm::vec3(0.,1.,0.));
    };
    cubeObj.postModelFun = [&](float time,GLApplication* app) {
        cubeObj.shaderHandler->setVec3Uniform("viewPos",app->camera.Position);
    };
    app.addRenderableObject(cubeObj);

    auto cube3 = glTests::createCubeWithNormal({-2.f,1.5f,-2.f},3.f,{R,R,R,R,R,R,R,R});
    RenderableObject cube3Obj("shaders/ShadowTextureLightingMap.vert.spv","shaders/ShadowMultiLightingMap.frag.spv",std::move(cube3.first),std::move(cube3.second));
    cube3Obj.setTexture("../textures/container2.png",true);
    cube3Obj.setTexture("../textures/container2_specular.png",true);
    cube3Obj.enableNormalAttribute();
    cube3Obj.setTextureMaterial(material2);
    cube3Obj.setDirectionalLight(dirLight);
    cube3Obj.shaderHandler->setScalarUniform("shadowMap",31);

    cube3Obj.objModelFun = [](float time) {
        return glm::rotate(glm::mat4(1.f),time*glm::radians(0.f),glm::vec3(0.,1.,0.));
    };
    cube3Obj.postModelFun = [&](float time,GLApplication* app) {
        cube3Obj.shaderHandler->setVec3Uniform("viewPos",app->camera.Position);
    };
    app.addRenderableObject(cube3Obj);

    auto lightCube2 = glTests::createCubeWithNormal(-directional,1.f, {W,W,W,W,W,W,W,W});
    RenderableObject light2("shaders/TrianglePos.vert.spv","shaders/Triangle.frag.spv",std::move(lightCube2.first),std::move(lightCube2.second));
    light2.objModelFun = [&](float time) {
        return glm::mat4(1.f);
    };
    app.addRenderableObject(light2);

    app.run();
    return 0;
}


