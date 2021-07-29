//
// Created by federico on 24/07/2021.
//

#pragma once
#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <Vertex.h>
class VertexHandler {
public:
    unsigned int vertexBufferObject;
    unsigned int vertexAttributeObject;
    unsigned int eBufferObject;
    const size_t numVertices,numIndices;

    VertexHandler(std::vector<Vertex>&& vertices,std::vector<unsigned int>&& indices);

    ~VertexHandler() {
        glDeleteVertexArrays(1, &vertexAttributeObject);
        glDeleteBuffers(1, &vertexBufferObject);
        glDeleteBuffers(1, &eBufferObject);
    }
    void draw() const;


};
