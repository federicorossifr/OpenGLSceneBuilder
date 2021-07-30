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


int main()
{

    ApplicationParams params{800,600};
    GLApplication app(params);
    ShaderHandler cubeShader("shaders/TrianglePos.vert.spv","shaders/Triangle.frag.spv");
    glm::vec3 minCorn{-5.f,-5.f,-5.f};
    glm::vec3 maxCorn{5.f,5.f,5.f};
    std::vector<glm::vec3> randomCorners(50);
    std::generate(randomCorners.begin(),randomCorners.end(),[&]() {
        return glm::linearRand(minCorn,maxCorn);
    });
    for(unsigned  int i = 0; i < 50; ++i) {
        auto random = randomCorners.at(i);
        auto cube = glTests::createCube(random,0.2f,{R,G,B,R,G,B,R,G});
        VertexHandler* cubeVertex = new VertexHandler(std::move(cube.first),std::move(cube.second));
        RenderableObject obj{};
        obj.vertexHandler = cubeVertex;
        obj.shaderHandler = &cubeShader;
        obj.objModelFun = [&,k=i](float time) {
            return glm::rotate(glm::mat4(1.f),time*glm::radians(90.f), randomCorners.at(k));
        };
        app.addRenderableObject(obj);
    }

    glClearColor(0.,0.,0.,1.);
    app.run();
    return 0;
}


