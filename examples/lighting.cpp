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
    glm::vec4 lightPos{10.f, 30.f, 0.f,0.f};
    auto lightTransform = [](float time) {
        return glm::rotate(glm::mat4(1.f),time*glm::radians(0.f),glm::vec3({0.,1.,0.}));
    };
    auto lightColor = W;
    std::array<glm::vec3,8> lightColorV{lightColor,lightColor,lightColor,lightColor,lightColor,lightColor,lightColor,lightColor};

    ApplicationParams params{800,600};
    GLApplication app(params);


    ShaderHandler cubeShader("shaders/TextureLighting.vert.spv","shaders/Lighting.frag.spv");
    auto cube = glTests::createCubeWithNormal({0.f,0.f,0.f},5.f,{R,R,R,R,R,R,R,R});
    VertexHandler cubeVertex(std::move(cube.first),std::move(cube.second));
    cubeVertex.bindNormal();
    RenderableObject obj{};
    obj.vertexHandler = &cubeVertex;
    obj.shaderHandler = &cubeShader;
    Material material {
        {0.0, 0.0, 0.0},
        {0.5, 0.0, 0.0},
        {0.7,  0.6,  0.6},
        1.
    };
    obj.setMaterial(material);

    obj.objModelFun = [](float time) {
        return glm::scale(glm::mat4(1.f),glm::vec3(5.f,5.f,5.f));
    };
    obj.postModelFun = [&](float time,Camera& camera) {
        obj.shaderHandler->setVec3Uniform("lightColor", W);
        auto lpos = lightTransform(time)*lightPos;
        obj.shaderHandler->setVec3Uniform<float>("lightPos", {lpos.x,lpos.y,lpos.z});
        obj.shaderHandler->setVec3Uniform("viewPos",camera.Position);
    };
    app.addRenderableObject(obj);



    ShaderHandler cube2Shader("shaders/TextureLighting.vert.spv","shaders/Lighting.frag.spv");
    auto cube2 = glTests::createCubeWithNormal({0.f,13.f,0.f},5.f,{W,W,W,W,W,W,W,W});
    VertexHandler cube2Vertex(std::move(cube2.first),std::move(cube2.second));
    cubeVertex.bindNormal();
    RenderableObject obj2{};
    obj2.vertexHandler = &cube2Vertex;
    obj2.shaderHandler = &cube2Shader;
    Material material2 {
        {.2f, .2f, .2f},
        {0.5, 0.0, 0.0},
        {0.7,  0.6,  0.6},
        1.
    };
    obj2.setMaterial(material2);

    obj2.objModelFun = [](float time) {
        return glm::rotate(glm::mat4(1.f),time*glm::radians(30.f),glm::vec3(0.f,1.f,0.f));
    };
    obj2.postModelFun = [&](float time,Camera& camera) {
        obj2.shaderHandler->setVec3Uniform("lightColor", W);
        auto lpos = lightTransform(time)*lightPos;
        obj2.shaderHandler->setVec3Uniform<float>("lightPos", {lpos.x,lpos.y,lpos.z});
        obj2.shaderHandler->setVec3Uniform("viewPos",camera.Position);
    };
    app.addRenderableObject(obj2);




    auto lightCube = glTests::createCubeWithNormal({lightPos.x,lightPos.y,lightPos.z},.1f, lightColorV);
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


