//
// Created by federico on 28/07/2021.
//
#pragma once
#include <ApplicationParams.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ShaderHandler.h>
#include <VertexHandler.h>
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtc/matrix_transform.hpp>
#include <RenderableObject.h>
#include <functional>
#include "Camera.h"
#include "Scene.h"
#include <any>
class RenderableObject;
class GLApplication {
protected:
    GLFWwindow* window;
    ApplicationParams applicationParams;
    std::unordered_map<std::string,std::any> context;
    std::vector<std::function<void(void)>> renderPasses;
    unsigned int depthMapFrameBuffer, depthMapTextureId;
    unsigned int depthCubemapFrameBuffer,depthCubeMapTextureId;

    enum RenderPass {
        PointShadow,Shadow,Final
    };

    struct {
        float frameDeltaTime;
        float lastFrameTime;
        glm::vec2 lastMousePosition;
        bool isFirstMouseMovement;
    } status;
    void setupGLAD();
    void setupWindow(ApplicationParams& params);
    void setupFBOs();
    void renderLoop();
    void processKeyboardInput();
    void setProcessMouseCallback();
    void renderScene(float time,ShaderHandler* shader,const RenderPass state);

public:
    Scene renderableScene;

    explicit GLApplication(ApplicationParams& params,glm::vec3 startCamera = glm::vec3(0.,0.,0.));
    void addRenderableObject(RenderableObject obj);



    void putContext(const std::string& key,std::any value) {
        context[key] = value;
    }

    template <class T>
    void getContext(const std::string& key,T& value) {
        value = std::any_cast<T>(context[key]);
    }

    GLFWwindow* getWindowPtr();
    void run();

    Camera camera;

    void setupRenderPasses();
};


