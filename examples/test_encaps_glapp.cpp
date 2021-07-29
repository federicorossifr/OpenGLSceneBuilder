//
// Created by federico on 24/07/2021.
//



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ShaderHandler.h>
#include <VertexHandler.h>
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtc/matrix_transform.hpp>
#include <RenderableObject.h>
#include <GLApplication.h>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;



using CubeInfo = std::pair<std::vector<Vertex>,std::vector<unsigned int>>;

CubeInfo createCube(
        const glm::vec3 upperCorner,
        const glm::float32_t sideLength,
        unsigned int& indexOffset,
        const std::array<glm::vec3,8> vertexColours // topRight,topLeft,bottomRight,bottomLeft (RGBA colours)
) {
    std::vector<Vertex> vertices(8); // We need 8 unique vertices to build the cube => upper and lower faces


    vertices[0] = Vertex{upperCorner,vertexColours[0]};
    vertices[1] = Vertex{{upperCorner.x+sideLength,upperCorner.y,upperCorner.z},vertexColours[1]};
    vertices[2] = Vertex{{upperCorner.x+sideLength,upperCorner.y+sideLength,upperCorner.z},vertexColours[2]};
    vertices[3] = Vertex{{upperCorner.x,upperCorner.y+sideLength,upperCorner.z},vertexColours[3]};

    auto lowerCorner = upperCorner - glm::vec3(0.f,0.f,sideLength);

    vertices[4] = Vertex{lowerCorner,vertexColours[4]};
    vertices[5] = Vertex{{lowerCorner.x+sideLength,lowerCorner.y,lowerCorner.z},vertexColours[5]};
    vertices[6] = Vertex{{lowerCorner.x+sideLength,lowerCorner.y+sideLength,lowerCorner.z},vertexColours[6]};
    vertices[7] = Vertex{{lowerCorner.x,lowerCorner.y+sideLength,lowerCorner.z},vertexColours[7]};

    std::vector<unsigned int> indices(6*6); // Six indices to connect the vertices of a face * six faces
    indices = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            3, 2, 6, 6, 7, 3,
            0, 3, 7, 7, 4, 0,
            2, 1, 5, 5, 6, 2,
            1, 0, 4, 4, 5, 1,
    };

    // Apply the offset to all indices
    std::transform(indices.begin(),indices.end(),indices.begin(),[&indexOffset](unsigned int& el) {
        return el+indexOffset;
    });
    indexOffset+=8;
    return std::make_pair(vertices,indices);
}

#define R glm::vec3(1.,0.,0.)
#define G glm::vec3(0.,1.,0.)
#define B glm::vec3(0.,0.,1.)

int main()
{


    unsigned int maxIndex{0};
    auto cube = createCube({-0.1f,-0.1f,0.},0.2f,maxIndex,{R,G,B,R,G,B,R,G});
    ApplicationParams params{800,600};
    GLApplication app(params);

    ShaderHandler triangleShader("shaders/TrianglePos.vert.spv","shaders/Triangle.frag.spv");
    VertexHandler triangleVertex(std::move(cube.first),std::move(cube.second));
    RenderableObject obj{};
    obj.vertexHandler = &triangleVertex;
    obj.shaderHandler = &triangleShader;

    app.addRenderableObject(obj);
    glClearColor(1.,1.,0.,1.);
    app.run();
    return 0;
}


