//
// Created by federico on 28/07/2021.
//
#pragma once
#include <VertexHandler.h>
#include <ShaderHandler.h>
#include <functional>
#include "Camera.h"
#include "GLApplication.h"
#include "Illumination.h"


class GLApplication;
class RenderableObject {
private:
    const GLuint maxSpotLights{4};
    GLuint64 spotLightIdx{0};
    ShaderHandler* swappedHandler;
public:
    VertexHandler* vertexHandler;
    ShaderHandler* shaderHandler;
    std::function<glm::mat4(float)> objModelFun;
    std::function<void(float,GLApplication* app)> postModelFun;
    bool canCastShadow{true};

    RenderableObject() = default;

    RenderableObject(std::string&& vertexFile, std::string&& fragmentFile, std::vector<Vertex>&& vertices,std::vector<unsigned int>&& indices);

    [[nodiscard]] glm::mat4 objectModel(float t) const {return objModelFun(t);};

    void postModel(float t,GLApplication* app) const {postModelFun(t,app);};

    void setMaterial(Material& mat);

    void setIllumination(const LightProperties& light,const std::string& lightPropertyName = "light");

    void setDirectionalLight(DirectionalLight& dir);

    void setFlashLight(FlashLight& fl);

    void addPointLight(const PointLight& pl);

    void setPointLights(std::vector<PointLight>& pls);

    void setTextureMaterial(Material& mat);

    void enableNormalAttribute();

    void enableEmissionMap();

    void setTexture(std::string&& filename, bool hasAlpha);

    void swapShaderHandler(ShaderHandler* handler) {
        swappedHandler = handler;
        std::swap(shaderHandler,swappedHandler);
    }

    void swapShaderHandler() {
        std::swap(shaderHandler,swappedHandler);
    }
};