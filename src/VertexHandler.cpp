//
// Created by federico on 24/07/2021.
//

#include <VertexHandler.h>

VertexHandler::VertexHandler(std::vector<float>&& vertices,std::vector<unsigned int>&& indices):
    numIndices(indices.size()),numVertices(sizeof(vertices)) {
    glGenVertexArrays(1, &vertexAttributeObject);
    glGenBuffers(1, &vertexBufferObject);
    glGenBuffers(1, &eBufferObject);

    glBindVertexArray(vertexAttributeObject);
    std::cout << sizeof(vertices) << std::endl;

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void VertexHandler::draw() {
    glBindVertexArray(vertexAttributeObject);
    if(numIndices > 0) glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0); // MULTIPLE VERTEX TRIANGLES
    else glDrawArrays(GL_TRIANGLES,0,3); // SINGLE VERTEX TRIANGLE
}


