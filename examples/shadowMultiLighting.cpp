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
    glm::vec4 lightPos(2.,4.,2.,1.);
    auto lightTransform = [](float time) {
        return glm::rotate(glm::mat4(1.f),time*glm::radians(0.f),glm::vec3({0.,1.,0.}));
    };
    glm::vec3 lightPos2(-0.,1.,9.);
    glm::vec3 directional(20, -20.f, 20.f);

    Material material2{};
    material2.specular = {0.5f, 0.5f, 0.5f};
    material2.shininess = 32.0f;


    LightProperties ill{};
    ill.ambient = {0.5f, 0.5f, 0.5f};
    ill.diffuse = { 0.7f, 0.7f, 0.7f};
    ill.specular =  {1.f, 1.f, 1.f};


    DirectionalLight dirLight{};
    dirLight.properties = ill;
    dirLight.direction = directional;

    PointLight pointLight{};
    pointLight.position = lightPos;
    pointLight.properties = ill;
    pointLight.constant = 1.f;
    pointLight.linear = 0.09f;
    pointLight.quadratic = 0.032f;

    ApplicationParams params{800,600};
    GLApplication app(params);
    //app.renderableScene.illumination.directionalLight = dirLight;
    app.renderableScene.illumination.pointLights.push_back(pointLight);
    auto cube2 = glTests::createPlaneWithNormal({0.f,0.f,0.f},30.f,{W,W,W,W,W,W,W,W});
    RenderableObject obj2("shaders/ShadowTextureLightingMap.vert.spv","shaders/ShadowMultiLightingMap.frag.spv",std::move(cube2.first),std::move(cube2.second));
    obj2.setTexture("../textures/wood.jpg",false);
    obj2.setTexture("../textures/wood_specular.jpg",false);
    obj2.enableNormalAttribute();
    obj2.setTextureMaterial(material2);
    //obj2.setDirectionalLight(dirLight);
    obj2.addPointLight(pointLight);
    obj2.shaderHandler->setScalarUniform("shadowMap",31);
    obj2.shaderHandler->setScalarUniform("depthMap",30);

    obj2.objModelFun = [](float time) {
        return glm::rotate(glm::mat4(1.f),time*glm::radians(0.f),glm::vec3(0.,1.,0.));
    };
    obj2.postModelFun = [&](float time,GLApplication* app) {
        obj2.shaderHandler->setVec3Uniform("viewPos",app->camera.Position);
    };
    app.addRenderableObject(obj2);

    glm::vec3 cubePos{-2.5f,4.f,-2.5f};
    float side = 2.f;
    auto cube = glTests::createCubeWithNormal(cubePos,side,{R,R,R,R,R,R,R,R});
    RenderableObject cubeObj("shaders/ShadowTextureLightingMap.vert.spv","shaders/ShadowMultiLightingMap.frag.spv",std::move(cube.first),std::move(cube.second));
    cubeObj.setTexture("../textures/container2.png",true);
    cubeObj.setTexture("../textures/container2_specular.png",true);
    cubeObj.enableNormalAttribute();
    cubeObj.setTextureMaterial(material2);
    //cubeObj.setDirectionalLight(dirLight);
    cubeObj.addPointLight(pointLight);

    cubeObj.shaderHandler->setScalarUniform("shadowMap",31);
    cubeObj.shaderHandler->setScalarUniform("depthMap",30);

    cubeObj.objModelFun = [&](float time) {
        glm::vec3 newH(0.,0.5*side/glm::cos(glm::radians(45.f))-0.5*side*glm::cos(glm::radians(45.f)),0.) ;
        auto mod = glm::translate(glm::mat4(1.f),cubePos+newH);
        mod = glm::rotate(mod,time*glm::radians(45.f),glm::vec3(0.,1.,0.));
        mod = glm::rotate(mod,glm::radians(45.f),glm::vec3(1.,0.,0.));
        mod = glm::rotate(mod,glm::radians(45.f),glm::vec3(0.,0.,1.));
        mod = glm::translate(mod,-cubePos);
        return mod;
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
    //cube3Obj.setDirectionalLight(dirLight);
    cube3Obj.addPointLight(pointLight);
    cube3Obj.shaderHandler->setScalarUniform("shadowMap",31);
    cube3Obj.shaderHandler->setScalarUniform("depthMap",30);

    cube3Obj.objModelFun = [](float time) {
        return glm::rotate(glm::mat4(1.f),time*glm::radians(0.f),glm::vec3(0.,1.,0.));
    };
    cube3Obj.postModelFun = [&](float time,GLApplication* app) {
        cube3Obj.shaderHandler->setVec3Uniform("viewPos",app->camera.Position);
    };
    app.addRenderableObject(cube3Obj);



    auto cube4 = glTests::createCubeWithNormal({8.f,1.5f,8.f},3.f,{R,R,R,R,R,R,R,R});
    RenderableObject cube4Obj("shaders/ShadowTextureLightingMap.vert.spv","shaders/ShadowMultiLightingMap.frag.spv",std::move(cube4.first),std::move(cube4.second));
    cube4Obj.setTexture("../textures/container2.png",true);
    cube4Obj.setTexture("../textures/container2_specular.png",true);
    cube4Obj.enableNormalAttribute();
    cube4Obj.setTextureMaterial(material2);
    //cube3Obj.setDirectionalLight(dirLight);
    cube4Obj.addPointLight(pointLight);
    cube4Obj.shaderHandler->setScalarUniform("shadowMap",31);
    cube4Obj.shaderHandler->setScalarUniform("depthMap",30);

    cube4Obj.objModelFun = [](float time) {
        return glm::rotate(glm::mat4(1.f),time*glm::radians(0.f),glm::vec3(0.,1.,0.));
    };
    cube4Obj.postModelFun = [&](float time,GLApplication* app) {
        cube4Obj.shaderHandler->setVec3Uniform("viewPos",app->camera.Position);
    };
    app.addRenderableObject(cube4Obj);










    auto lightCube2 = glTests::createCubeWithNormal(-directional,1.f, {W,W,W,W,W,W,W,W});
    RenderableObject light2("shaders/TrianglePos.vert.spv","shaders/Triangle.frag.spv",std::move(lightCube2.first),std::move(lightCube2.second));
    light2.objModelFun = [&](float time) {

        return glm::mat4(1.f);
    };
    light2.canCastShadow = false;
    //app.addRenderableObject(light2);

    auto lightCube3 = glTests::createCubeWithNormal(lightPos,1.f, {W,W,W,W,W,W,W,W});
    RenderableObject light33("shaders/TrianglePos.vert.spv","shaders/Triangle.frag.spv",std::move(lightCube3.first),std::move(lightCube3.second));
    light33.objModelFun = [&](float time) {
        auto lpos = lightTransform(time)*lightPos;
        app.renderableScene.illumination.pointLights[0].position = lpos;
        return lightTransform(time);
    };
    light33.canCastShadow = false;
    app.addRenderableObject(light33);

    app.run();
    return 0;
}


