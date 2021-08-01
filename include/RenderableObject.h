//
// Created by federico on 28/07/2021.
//
#pragma once
#include <VertexHandler.h>
#include <ShaderHandler.h>
#include <functional>
#include "Camera.h"

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
};

struct Light {
    glm::vec3 position;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};


class RenderableObject {
public:
    VertexHandler* vertexHandler;
    ShaderHandler* shaderHandler;
    std::function<glm::mat4(float)> objModelFun;
    std::function<void(float,Camera& camera)> postModelFun;

    glm::mat4 objectModel(float t) const {return objModelFun(t);};
    void postModel(float t,Camera& cam) const {postModelFun(t,cam);};

    void setMaterial(Material& mat) {
        if(shaderHandler == nullptr) return;
        shaderHandler->useShader();
        shaderHandler->setVec3Uniform("material.ambient", mat.ambient);
        shaderHandler->setVec3Uniform("material.diffuse", mat.diffuse);
        shaderHandler->setVec3Uniform("material.specular", mat.specular);
        shaderHandler->setScalarUniform("material.shininess", mat.shininess);
    }

    void setIllumination(Light& light) {
        if(shaderHandler == nullptr) return;
        shaderHandler->useShader();
        shaderHandler->setVec3Uniform("light.ambient", light.ambient);
        shaderHandler->setVec3Uniform("light.diffuse", light.diffuse);
        shaderHandler->setVec3Uniform("light.specular", light.specular);
    }

    void setTextureMaterial(Material& mat) {
        if(shaderHandler == nullptr) return;
        shaderHandler->useShader();
        shaderHandler->setScalarUniform("material.diffuse", 0); // GLTEXTURE0 for Diffuse Map
        shaderHandler->setScalarUniform("material.specular", 1); // GLTEXTURE1 for Specular Map
        shaderHandler->setScalarUniform("material.shininess", mat.shininess);
        shaderHandler->setScalarUniform("material.canEmit",false);

    }

    void enableEmissionMap() {
        if(shaderHandler == nullptr) return;
        shaderHandler->useShader();
        shaderHandler->setScalarUniform("material.emission", 2); // GLTEXTURE2 for emission Map
        shaderHandler->setScalarUniform("material.canEmit",true);
    }


};