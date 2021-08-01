//
// Created by federico on 24/07/2021.
//

#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <string>
#include <TextureImage.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <Vertex.h>
#include <string>
#include <optional>

class VertexHandler {
public:
    unsigned int vertexBufferObject;
    unsigned int vertexAttributeObject;
    unsigned int eBufferObject;
    const size_t numIndices;
    std::vector<unsigned int> textureIds;

    VertexHandler(std::vector<Vertex>&& vertices,std::vector<unsigned int>&& indices);

    void bindTexture(std::string&& filename,bool hasAlpha = false);
    void bindNormal();
    ~VertexHandler() {
        glDeleteVertexArrays(1, &vertexAttributeObject);
        glDeleteBuffers(1, &vertexBufferObject);
        glDeleteBuffers(1, &eBufferObject);
    }


    void draw() const;


};
