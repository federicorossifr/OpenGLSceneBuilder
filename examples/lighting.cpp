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

#define R glm::vec3(1.,0.,0.)
#define G glm::vec3(0.,1.,0.)
#define B glm::vec3(0.,0.,1.)
#define W glm::vec3(1.,1.,1.)
#define WHITE_LIGHT {W,W,W,W,W,W,W,W}
#define BLUE_LIGHT {B,B,B,B,B,B,B,B}
#define YELLOW_LIGHT {G,G,G,G,G,G,G,G}




int main()
{
    glm::vec4 lightPos{3.f, 2.0f, 0.f,0.f};
    auto lightTransform = [](float time) {
        return glm::rotate(glm::mat4(1.f),time*glm::radians(0.f),glm::vec3(0.0f,0.f,1.f));
    };
    auto lightColor = W;
    std::array<glm::vec3,8> lightColorV{lightColor,lightColor,lightColor,lightColor,lightColor,lightColor,lightColor,lightColor};

    ApplicationParams params{800,600};
    GLApplication app(params);
    ShaderHandler cubeShader("shaders/TextureLighting.vert.spv","shaders/Lighting.frag.spv");
    auto cube = glTests::createCubeWithNormal({1.f,0.f,0.f},0.5f,{R,R,R,R,R,R,R,R});
    VertexHandler cubeVertex(std::move(cube.first),std::move(cube.second));
    cubeVertex.bindNormal();
    RenderableObject obj{};
    obj.vertexHandler = &cubeVertex;
    obj.shaderHandler = &cubeShader;
    obj.objModelFun = [](float time) {
        return glm::rotate(glm::mat4(1.f),time*glm::radians(0.f),{0.0f,1.0f,0.0f});
    };
    obj.postModelFun = [&](float time) {
        obj.shaderHandler->setVec3Uniform("lightColor", W);
        auto lpos = lightTransform(time)*lightPos;
        obj.shaderHandler->setVec3Uniform<float>("lightPos", {lpos.x,lpos.y,lpos.z});
        obj.shaderHandler->setScalarUniform<float>("ambientStrength",0.2f);
    };
    app.addRenderableObject(obj);


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
    glClearColor(0.,0.,0.,1.);
    app.run();
    return 0;
}


