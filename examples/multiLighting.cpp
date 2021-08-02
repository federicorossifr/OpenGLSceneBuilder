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
    glm::vec3 lightPos(9.,5.,0.);
    glm::vec3 lightPos2(-0.,5.,9.);
    glm::vec3 directional(9.f, -50.0f, -0.f);

    Material material2{};
    material2.specular = {0.5f, 0.5f, 0.5f};
    material2.shininess = 64.0f;


    LightProperties ill{};
    ill.ambient = {0.1f, 0.1f, 0.1f};
    ill.diffuse = { 0.5f, 0.5f, 0.5f};
    ill.specular =  {1.0f, 1.0f, 1.0f};


    DirectionalLight dirLight{};
    dirLight.properties = ill;
    dirLight.direction = directional;

    PointLight pLight{};
    pLight.properties = ill;
    pLight.position = lightPos;
    pLight.constant = 1.f;
    pLight.linear = 0.09f;
    pLight.quadratic = 0.032f;

    PointLight pLight2{pLight};
    pLight2.position = lightPos2;


    ApplicationParams params{800,600};
    GLApplication app(params);

    auto cube2 = glTests::createCubeWithNormal({0.f,4.f,0.f},5.f,{R,R,R,R,R,R,R,R});
    RenderableObject obj2("shaders/TextureLightingMap.vert.spv","shaders/MultiLightingMap.frag.spv",std::move(cube2.first),std::move(cube2.second));
    obj2.setTexture("../textures/metal.jpg",false);
    obj2.setTexture("../textures/metal_specular.jpg",false);
    obj2.enableNormalAttribute();
    obj2.setTextureMaterial(material2);
    obj2.addPointLight(pLight);
    obj2.addPointLight(pLight2);
    obj2.objModelFun = [](float time) {
        return glm::rotate(glm::mat4(1.f),time*glm::radians(90.f),glm::vec3(0.,1.,0.));
    };
    obj2.postModelFun = [&](float time,Camera& camera) {
       obj2.shaderHandler->setVec3Uniform("viewPos",camera.Position);
    };
    app.addRenderableObject(obj2);



   /* auto plane = glTests::createPlaneWithNormal({0.f,-.5f,0.f},50.f,{G,G,G,G,G,G,G});

    RenderableObject planeObj{"shaders/TextureLightingMap.vert.spv","shaders/LightingMap.frag.spv",std::move(plane.first),std::move(plane.second)};
    planeObj.setTexture("../textures/metal2.jpg",false);
    planeObj.setTexture("../textures/metal2_specular.jpg",false);
    planeObj.enableNormalAttribute();
    planeObj.setMaterial(material2);
    planeObj.setIllumination(ill);

    planeObj.objModelFun = [](float time) {
        return glm::mat4(1.f);
    };
    planeObj.postModelFun = [&](float time,Camera& camera) {
        planeObj.shaderHandler->setVec3Uniform("light.position", lightPos);
        planeObj.shaderHandler->setVec3Uniform("viewPos",camera.Position);
    };*/
    //app.addRenderableObject(planeObj);




    auto lightCube = glTests::createCubeWithNormal({lightPos.x,lightPos.y,lightPos.z},1.f, {W,W,W,W,W,W,W,W});
    RenderableObject light("shaders/TrianglePos.vert.spv","shaders/Triangle.frag.spv",std::move(lightCube.first),std::move(lightCube.second));
    light.objModelFun = [&](float time) {
        return glm::mat4(1.f);
    };
    app.addRenderableObject(light);

    auto lightCube2 = glTests::createCubeWithNormal(lightPos2,1.f, {W,W,W,W,W,W,W,W});
    RenderableObject light2("shaders/TrianglePos.vert.spv","shaders/Triangle.frag.spv",std::move(lightCube2.first),std::move(lightCube2.second));
    light2.objModelFun = [&](float time) {
        return glm::mat4(1.f);
    };
    app.addRenderableObject(light2);

    auto lightCube3 = glTests::createCubeWithNormal(-directional,1.f, {G,G,G,G,G,G,G,G});
    RenderableObject light3("shaders/TrianglePos.vert.spv","shaders/Triangle.frag.spv",std::move(lightCube3.first),std::move(lightCube3.second));
    light3.objModelFun = [&](float time) {
        return glm::mat4(1.f);
    };
    app.addRenderableObject(light3);



    glClearColor(0.,0.,0., 1.0f);
    app.run();
    return 0;
}


