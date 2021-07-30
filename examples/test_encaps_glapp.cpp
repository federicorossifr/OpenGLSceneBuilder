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

std::vector<Vertex> vv{
        // positions                                        // colors                   // texture coords
        {{5.f,  5.f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},   // top right
        {{5.f, -5.f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},   // bottom right
        {{-5.f, -5.f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},   // bottom left
        {{ -5.f,  5.f, 0.0f}, {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}    // top left
};

std::vector<unsigned int> idx{  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
};

int main()
{
    auto cube = glTests::createCube({-0.1f,-0.1f,0.f},0.2f,{R,G,B,R,G,B,R,G});

    ApplicationParams params{800,600};
    GLApplication app(params);
    ShaderHandler cubeShader("shaders/TrianglePos.vert.spv","shaders/Triangle.frag.spv");
    ShaderHandler cubeTextureShader("shaders/Texture.vert.spv","shaders/Texture.frag.spv");

    VertexHandler cube1Vertex(std::move(cube.first),std::move(cube.second));
    RenderableObject obj2{};
    obj2.vertexHandler = &cube1Vertex;
    obj2.shaderHandler = &cubeShader;
    obj2.objModelFun = [](float time) {
        return glm::rotate(glm::mat4(1.f),time*glm::radians(180.f),glm::vec3(0.f,0.f,1.0f));
    };

    VertexHandler cube2Vertex(std::move(vv),std::move(idx));
    cube2Vertex.bindTexture("../textures/bricks.jpg");
    RenderableObject obj{};
    obj.vertexHandler = &cube2Vertex;
    obj.shaderHandler = &cubeTextureShader;
    obj.objModelFun = [](float time) {
        return glm::mat4(1.f);
    };

    app.addRenderableObject(obj);
    app.addRenderableObject(obj2);

    glClearColor(0.,0.,0.,1.);
    app.run();
    return 0;
}


