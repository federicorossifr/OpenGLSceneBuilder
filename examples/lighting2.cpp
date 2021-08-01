//
// Created by federico on 24/07/2021.
//



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ShaderHandler.h>
#include <VertexHandler.h>
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
    glm::vec3 lightPos(25.f, 10.0f, 0.f);
    auto lightTransform = [](float time) {
        return glm::rotate(glm::mat4(1.f),time*glm::radians(0.f),glm::vec3({0.,1.,0.}));
    };
    auto lightColor = W;
    std::array<glm::vec3,8> lightColorV{lightColor,lightColor,lightColor,lightColor,lightColor,lightColor,lightColor,lightColor};

    ApplicationParams params{800,600};
    GLApplication app(params);

    ShaderHandler cube2Shader("shaders/TextureLightingMap.vert.spv","shaders/LightingMap.frag.spv");
    auto cube2 = glTests::createCubeWithNormal({0.f,4.f,0.f},5.f,{R,R,R,R,R,R,R,R});
    VertexHandler cube2Vertex(std::move(cube2.first),std::move(cube2.second));
    cube2Vertex.bindTexture("../textures/metal.jpg",false);
    cube2Vertex.bindTexture("../textures/metal_specular.jpg",false);

    cube2Vertex.bindNormal();
    RenderableObject obj2{};
    obj2.vertexHandler = &cube2Vertex;
    obj2.shaderHandler = &cube2Shader;
    Material material2{};
    material2.specular = {0.5f, 0.5f, 0.5f};
    material2.shininess = 64.0f;
    Light ill{};
    ill.position = lightPos;
    ill.ambient = {0.2f, 0.2f, 0.2f};
    ill.diffuse = { 0.5f, 0.5f, 0.5f};
    ill.specular =  {1.0f, 1.0f, 1.0f};

    obj2.setTextureMaterial(material2);
    obj2.setIllumination(ill);
    obj2.objModelFun = [](float time) {
        return glm::rotate(glm::mat4(1.f),time*glm::radians(90.f),glm::vec3(0.,1.,0.));
    };
    obj2.postModelFun = [&](float time,Camera& camera) {
        obj2.shaderHandler->setVec3Uniform("light.position", lightPos);
        obj2.shaderHandler->setVec3Uniform("viewPos",camera.Position);
    };
    app.addRenderableObject(obj2);



    ShaderHandler planeShader("shaders/TextureLightingMap.vert.spv","shaders/LightingMap.frag.spv");
    auto plane = glTests::createPlaneWithNormal({0.f,-.5f,0.f},50.f,{G,G,G,G,G,G,G});
    VertexHandler planeVertex(std::move(plane.first),std::move(plane.second));
    planeVertex.bindTexture("../textures/metal2.jpg",false);
    planeVertex.bindTexture("../textures/metal2_specular.jpg",false);
    planeVertex.bindNormal();
    RenderableObject planeObj{};
    planeObj.vertexHandler = &planeVertex;
    planeObj.shaderHandler = &planeShader;
    planeObj.setMaterial(material2);
    planeObj.setIllumination(ill);

    planeObj.objModelFun = [](float time) {
        return glm::mat4(1.f);
    };
    planeObj.postModelFun = [&](float time,Camera& camera) {
        planeObj.shaderHandler->setVec3Uniform("light.position", lightPos);
        planeObj.shaderHandler->setVec3Uniform("viewPos",camera.Position);
    };
    app.addRenderableObject(planeObj);




    auto lightCube = glTests::createCubeWithNormal({lightPos.x,lightPos.y,lightPos.z},2.f, lightColorV);
    ShaderHandler lightShader("shaders/TrianglePos.vert.spv","shaders/Triangle.frag.spv");
    VertexHandler lightCubeVertex(std::move(lightCube.first),std::move(lightCube.second));
    RenderableObject light{};
    light.vertexHandler = &lightCubeVertex;
    light.shaderHandler = &lightShader;
    light.objModelFun = [&](float time) {
        return lightTransform(time);
    };
    app.addRenderableObject(light);
    glClearColor(122.0f/255, 204.0f/255, 255.0f/255, 1.0f);
    app.run();
    return 0;
}


