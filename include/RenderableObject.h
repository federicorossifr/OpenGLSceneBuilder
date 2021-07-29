//
// Created by federico on 28/07/2021.
//
#pragma once
#include <VertexHandler.h>
#include <ShaderHandler.h>
#include <functional>
struct RenderableObject {
    VertexHandler* vertexHandler;
    ShaderHandler* shaderHandler;
    std::function<glm::mat4(float)> objModelFun;
    glm::mat4 objectModel(float t) const {return objModelFun(t);};
};