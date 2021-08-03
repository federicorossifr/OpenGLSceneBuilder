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

const std::array<GLuint ,31> glTextureMap
{
    GL_TEXTURE0,
    GL_TEXTURE1,
    GL_TEXTURE2,
    GL_TEXTURE3,
    GL_TEXTURE4,
    GL_TEXTURE5,
    GL_TEXTURE6,
    GL_TEXTURE7,
    GL_TEXTURE8,
    GL_TEXTURE9,
    GL_TEXTURE10,
    GL_TEXTURE11,
    GL_TEXTURE12,
    GL_TEXTURE13,
    GL_TEXTURE14,
    GL_TEXTURE15,
    GL_TEXTURE16,
    GL_TEXTURE17,
    GL_TEXTURE18,
    GL_TEXTURE19,
    GL_TEXTURE20,
    GL_TEXTURE21,
    GL_TEXTURE22,
    GL_TEXTURE23,
    GL_TEXTURE24,
    GL_TEXTURE25,
    GL_TEXTURE26,
    GL_TEXTURE27,
    GL_TEXTURE28,
    GL_TEXTURE29,
    GL_TEXTURE30
};


void VertexHandler::draw() const {
    std::for_each(textureIds.begin(),textureIds.end(),[idx=0] (auto& textureId) mutable {
        glActiveTexture(glTextureMap.at(idx));
        glBindTexture(GL_TEXTURE_2D, textureId);
        idx++;
    });

    glBindVertexArray(vertexAttributeObject);
    if(numIndices > 0)
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, 3);
}

void VertexHandler::bindTexture(std::string&& filename, bool hasAlpha) {
    TextureImage txt(filename,hasAlpha);
    textureIds.push_back(txt.textureId);

    if(textureIds.size() == 1) { // only first time binding a texture
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex,textureCoordinates)));
        glEnableVertexAttribArray(2);
    }
}

void VertexHandler::bindNormal() {
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex,normal)));
    glEnableVertexAttribArray(3);
}





