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

struct LightProperties {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct DirectionalLight {
    glm::vec3 direction;
    LightProperties properties;
};

struct PointLight {
    glm::vec3 position;

    LightProperties properties;

    float constant;
    float linear;
    float quadratic;
};


class RenderableObject {
private:
    const GLuint maxSpotLights{4};
    GLuint64 spotLightIdx{0};
public:
    VertexHandler* vertexHandler;
    ShaderHandler* shaderHandler;
    std::function<glm::mat4(float)> objModelFun;
    std::function<void(float,Camera& camera)> postModelFun;

    RenderableObject(std::string&& vertexFile, std::string&& fragmentFile, std::vector<Vertex>&& vertices,std::vector<unsigned int>&& indices) {
        vertexHandler = new VertexHandler(std::move(vertices),std::move(indices));
        shaderHandler = new ShaderHandler(std::move(vertexFile),std::move(fragmentFile));
    }

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

    void setIllumination(LightProperties& light,const std::string& lightPropertyName = "light") {
        if(shaderHandler == nullptr) return;
        shaderHandler->useShader();
        shaderHandler->setVec3Uniform(lightPropertyName+".ambient", light.ambient);
        shaderHandler->setVec3Uniform(lightPropertyName+".diffuse", light.diffuse);
        shaderHandler->setVec3Uniform(lightPropertyName+".specular", light.specular);
    }

    void setDirectionalLight(DirectionalLight& dir) {
        if(shaderHandler == nullptr) return;
        shaderHandler->useShader();
        setIllumination(dir.properties,"dirLight");
        shaderHandler->setVec3Uniform("dirLight.direction",dir.direction);
    }

    void addPointLight(PointLight& pl) {
        if(spotLightIdx == maxSpotLights-1) return;
        if(shaderHandler == nullptr) return;
        shaderHandler->useShader();
        std::string baseName = "pointLights[" + std::to_string(spotLightIdx++) + "]";
        setIllumination(pl.properties,baseName);
        shaderHandler->setScalarUniform("numPointLights",(int)spotLightIdx); // Already incremented before ;)
        shaderHandler->setVec3Uniform(baseName+".position",     pl.position);
        shaderHandler->setScalarUniform(baseName+".constant",   pl.constant);
        shaderHandler->setScalarUniform(baseName+".linear",     pl.linear);
        shaderHandler->setScalarUniform(baseName+".quadratic",  pl.quadratic);
    }

    void setTextureMaterial(Material& mat) {
        if(shaderHandler == nullptr) return;
        shaderHandler->useShader();
        shaderHandler->setScalarUniform("material.diffuse", 0); // GLTEXTURE0 for Diffuse Map
        shaderHandler->setScalarUniform("material.specular", 1); // GLTEXTURE1 for Specular Map
        shaderHandler->setScalarUniform("material.shininess", mat.shininess);
        shaderHandler->setScalarUniform("material.canEmit",false);

    }

    void enableNormalAttribute() {
        vertexHandler->bindNormal();
    }

    void enableEmissionMap() {
        if(shaderHandler == nullptr) return;
        shaderHandler->useShader();
        shaderHandler->setScalarUniform("material.emission", 2); // GLTEXTURE2 for emission Map
        shaderHandler->setScalarUniform("material.canEmit",true);
    }

    void setTexture(std::string&& filename, bool hasAlpha) {
        vertexHandler->bindTexture(std::move(filename),hasAlpha);
    }




};