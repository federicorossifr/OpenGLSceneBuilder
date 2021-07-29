//
// Created by federico on 24/07/2021.
//

#include <VertexHandler.h>

VertexHandler::VertexHandler(std::vector<Vertex>&& vertices,std::vector<unsigned int>&& indices):
    numIndices(indices.size()) {
    glGenVertexArrays(1, &vertexAttributeObject);
    glGenBuffers(1, &vertexBufferObject);
    glGenBuffers(1, &eBufferObject);

    glBindVertexArray(vertexAttributeObject);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    // color

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex,colour)));
    glEnableVertexAttribArray(1);

}

void VertexHandler::draw() const {
    if(textureId.has_value()) {
        glBindTexture(GL_TEXTURE_2D, textureId.value());
    }

    glBindVertexArray(vertexAttributeObject);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
}

void VertexHandler::bindTexture(std::string&& filename) {
    TextureImage txt(filename);
    textureId = txt.textureId;
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex,textureCoordinates)));
    glEnableVertexAttribArray(2);
}





