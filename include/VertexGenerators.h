//
// Created by federico on 30/07/2020.5
//
#include <glm/glm.hpp>
#include <numeric>

namespace glTests {
    using VertexInfo = std::pair<std::vector<Vertex>,std::vector<unsigned int>>;
    VertexInfo createCube(
            const glm::vec3 upperCorner,
            const glm::float32_t sideLength,
            unsigned int& indexOffset,
            const std::array<glm::vec3,8> vertexColours // topRight,topLeft,bottomRight,bottomLeft (RGBA colours)
    ) {
        std::vector<Vertex> vertices(8); // We need 8 unique vertices to build the cube => upper and lower faces


        vertices[0] = Vertex{upperCorner,vertexColours[0],{0.5,0.5}};
        vertices[1] = Vertex{{upperCorner.x+sideLength,upperCorner.y,upperCorner.z},vertexColours[1],{0.5,0.}};
        vertices[2] = Vertex{{upperCorner.x+sideLength,upperCorner.y+sideLength,upperCorner.z},vertexColours[2],{0.,0.}};
        vertices[3] = Vertex{{upperCorner.x,upperCorner.y+sideLength,upperCorner.z},vertexColours[3],{0.,0.5}};

        auto lowerCorner = upperCorner - glm::vec3(0.f,0.f,sideLength);

        vertices[4] = Vertex{lowerCorner,vertexColours[4],{0.5,0.5}};
        vertices[5] = Vertex{{lowerCorner.x+sideLength,lowerCorner.y,lowerCorner.z},vertexColours[5],{0.5,0.5}};
        vertices[6] = Vertex{{lowerCorner.x+sideLength,lowerCorner.y+sideLength,lowerCorner.z},vertexColours[6],{0.5,0.5}};
        vertices[7] = Vertex{{lowerCorner.x,lowerCorner.y+sideLength,lowerCorner.z},vertexColours[7],{0.5,0.5}};

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

    VertexInfo createCube(
            const glm::vec3 upperCorner,
            const glm::float32_t sideLength,
            const std::array<glm::vec3,8> vertexColours // topRight,topLeft,bottomRight,bottomLeft (RGBA colours)
    ) {
        std::vector<Vertex> vertices(8); // We need 8 unique vertices to build the cube => upper and lower faces


        vertices[0] = Vertex{upperCorner,vertexColours[0],{0.5,0.5}};
        vertices[1] = Vertex{{upperCorner.x+sideLength,upperCorner.y,upperCorner.z},vertexColours[1],{0.5,0.}};
        vertices[2] = Vertex{{upperCorner.x+sideLength,upperCorner.y+sideLength,upperCorner.z},vertexColours[2],{0.,0.}};
        vertices[3] = Vertex{{upperCorner.x,upperCorner.y+sideLength,upperCorner.z},vertexColours[3],{0.,0.5}};

        auto lowerCorner = upperCorner - glm::vec3(0.f,0.f,sideLength);

        vertices[4] = Vertex{lowerCorner,vertexColours[4],{0.5,0.5}};
        vertices[5] = Vertex{{lowerCorner.x+sideLength,lowerCorner.y,lowerCorner.z},vertexColours[5],{0.5,0.5}};
        vertices[6] = Vertex{{lowerCorner.x+sideLength,lowerCorner.y+sideLength,lowerCorner.z},vertexColours[6],{0.5,0.5}};
        vertices[7] = Vertex{{lowerCorner.x,lowerCorner.y+sideLength,lowerCorner.z},vertexColours[7],{0.5,0.5}};

        std::vector<unsigned int> indices(6*6); // Six indices to connect the vertices of a face * six faces
        indices = {
                0, 1, 2, 2, 3, 0,
                4, 5, 6, 6, 7, 4,
                3, 2, 6, 6, 7, 3,
                0, 3, 7, 7, 4, 0,
                2, 1, 5, 5, 6, 2,
                1, 0, 4, 4, 5, 1,
        };

        return std::make_pair(vertices,indices);
    }


    VertexInfo createCubeWithNormal(
            const glm::vec3 origin,
            const glm::float32_t sideLength,
            const std::array<glm::vec3,8> vertexColours // topRight,topLeft,bottomRight,bottomLeft (RGBA colours)
    ) {

        std::vector<glm::vec3> corners{
            { -0.5f, -0.5f, -0.5f},
            { 0.5f,  0.5f, -0.5f,},
            { 0.5f, -0.5f, -0.5f,},
            { 0.5f,  0.5f, -0.5f,},
            {-0.5f, -0.5f, -0.5f,},
            {-0.5f,  0.5f, -0.5f,},
            {-0.5f, -0.5f,  0.5f,},
            { 0.5f, -0.5f,  0.5f,},
            { 0.5f,  0.5f,  0.5f,},
            { 0.5f,  0.5f,  0.5f,},
            {-0.5f,  0.5f,  0.5f,},
            {-0.5f, -0.5f,  0.5f,},
            {-0.5f,  0.5f,  0.5f,},
            {-0.5f,  0.5f, -0.5f,},
            {-0.5f, -0.5f, -0.5f,},
            {-0.5f, -0.5f, -0.5f,},
            {-0.5f, -0.5f,  0.5f,},
            {-0.5f,  0.5f,  0.5f,},
            { 0.5f,  0.5f,  0.5f,},
            { 0.5f, -0.5f, -0.5f,},
            { 0.5f,  0.5f, -0.5f,},
            { 0.5f, -0.5f, -0.5f,},
            { 0.5f,  0.5f,  0.5f,},
            { 0.5f, -0.5f,  0.5f,},
            {-0.5f, -0.5f, -0.5f,},
            { 0.5f, -0.5f, -0.5f,},
            { 0.5f, -0.5f,  0.5f,},
            { 0.5f, -0.5f,  0.5f,},
            {-0.5f, -0.5f,  0.5f,},
            {-0.5f, -0.5f, -0.5f,},

            {-0.5f,  0.5f, -0.5f,},
            { 0.5f,  0.5f , 0.5f,},
            { 0.5f,  0.5f, -0.5f,},
            { 0.5f,  0.5f,  0.5f,},
            {-0.5f,  0.5f, -0.5f,},
            {-0.5f,  0.5f,  0.5f,},
        };

        std::vector<glm::vec3> normals {
            { 0.0f,  0.0f, -1.0f},
            { 0.0f,  0.0f, -1.0f},
            { 0.0f,  0.0f, -1.0f},
            { 0.0f,  0.0f, -1.0f},
            { 0.0f,  0.0f, -1.0f},
            { 0.0f,  0.0f, -1.0f},
            { 0.0f,  0.0f,  1.0f},
            { 0.0f,  0.0f,  1.0f},
            { 0.0f,  0.0f,  1.0f},
            { 0.0f,  0.0f,  1.0f},
            { 0.0f,  0.0f,  1.0f},
            { 0.0f,  0.0f,  1.0f},
            {-1.0f,  0.0f,  0.0f},
            {-1.0f,  0.0f,  0.0f},
            {-1.0f,  0.0f,  0.0f},
            {-1.0f,  0.0f,  0.0f},
            {-1.0f,  0.0f,  0.0f},
            {-1.0f,  0.0f,  0.0f},
            { 1.0f,  0.0f,  0.0f},
            { 1.0f,  0.0f,  0.0f},
            { 1.0f,  0.0f,  0.0f},
            { 1.0f,  0.0f,  0.0f},
            { 1.0f,  0.0f,  0.0f},
            { 1.0f,  0.0f,  0.0f},
            { 0.0f, -1.0f,  0.0f},
            { 0.0f, -1.0f,  0.0f},
            { 0.0f, -1.0f,  0.0f},
            { 0.0f, -1.0f,  0.0f},
            { 0.0f, -1.0f,  0.0f},
            { 0.0f, -1.0f,  0.0f},

            { 0.0f,  1.0f,  0.0f},
            { 0.0f,  1.0f,  0.0f},
            { 0.0f,  1.0f,  0.0f},
            { 0.0f,  1.0f,  0.0f},
            { 0.0f,  1.0f,  0.0f},
            { 0.0f,  1.0f,  0.0f},
        };

        std::vector<glm::vec2> texCoords {
            {0.0f, 0.0f},
            {1.0f, 1.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 0.0f},
            {0.0f, 1.0f},
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f},
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f},
            {0.0f, 1.0f},
            {0.0f, 0.0f},
            {1.0f, 0.0f},
            {1.0f, 0.0f},
            {0.0f, 1.0f},
            {1.0f, 1.0f},
            {0.0f, 1.0f},
            {1.0f, 0.0f},
            {0.0f, 0.0f},
            {0.0f, 1.0f},
            {1.0f, 1.0f},
            {1.0f, 0.0f},
            {1.0f, 0.0f},
            {0.0f, 0.0f},
            {0.0f, 1.0f},

            {0.0f, 1.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {1.0f, 0.0f},
            {0.0f, 1.0f},
            {0.0f, 0.0f}
        };


        std::vector<Vertex> vertices; // We need 8 unique vertices to build the cube => upper and lower faces
        std::for_each(corners.begin(),corners.end(),[&,n=0](glm::vec3 corner) mutable {
          auto transCorner = glm::scale(glm::mat4(1.f),glm::vec3(sideLength));
          auto newCorner = transCorner*glm::vec4(corner,1.f);
          auto projCorner = glm::vec3(newCorner.x,newCorner.y,newCorner.z) + origin;
          vertices.push_back(Vertex{
              projCorner,
              vertexColours[n%8],
              texCoords[n],
              normals[n]
          });
          n++;
        });
        std::vector<unsigned int> indices(6*6); // Six indices to connect the vertices of a face * six faces
        std::iota(indices.begin(),indices.end(),0);

        return std::make_pair(vertices,indices);
    }

    VertexInfo createPlaneWithNormal(
            const glm::vec3 origin,
            const glm::float32_t sideLength,
            const std::array<glm::vec3,8> vertexColours // topRight,topLeft,bottomRight,bottomLeft (RGBA colours)
            ) {

        std::vector<glm::vec3> corners{
            {-0.5f,  0.f, -0.5f,},
            { 0.5f,  0.f, 0.5f,},
            { 0.5f,  0.f, -0.5f,},
            { 0.5f,  0.f,  0.5f,},
            {-0.5f,  0.f, -0.5f,},
            {-0.5f,  0.f,  0.5f,},
        };

        std::vector<glm::vec3> normals {
            {0.f,1.f,0.f},
            {0.f,1.f,0.f},
            {0.f,1.f,0.f},
            {0.f,1.f,0.f},
            {0.f,1.f,0.f},
            {0.f,1.f,0.f},
        };

        std::vector<glm::vec2> texCoords {
            {0.0f, 1.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {1.0f, 0.0f},
            {0.0f, 1.0f},
            {0.0f, 0.0f}
        };


        std::vector<Vertex> vertices; // We need 8 unique vertices to build the cube => upper and lower faces
        std::for_each(corners.begin(),corners.end(),[&,n=0](glm::vec3 corner) mutable {
            auto transCorner = glm::scale(glm::mat4(1.f),glm::vec3(sideLength));
            auto newCorner = transCorner*glm::vec4(corner,1.f);
            auto projCorner = glm::vec3(newCorner.x,newCorner.y,newCorner.z) + origin;
            vertices.push_back(Vertex{
                projCorner,
                vertexColours[n%8],
                texCoords[n],
                normals[n]
            });
            n++;
        });
        std::vector<unsigned int> indices(6*6); // Six indices to connect the vertices of a face * six faces
        std::iota(indices.begin(),indices.end(),0);

        return std::make_pair(vertices,indices);
    }

}