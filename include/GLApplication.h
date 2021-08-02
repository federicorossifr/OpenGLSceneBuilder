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
#include <any>
class RenderableObject;
class GLApplication {
protected:
    GLFWwindow* window;
    std::vector<RenderableObject> objects;
    ApplicationParams applicationParams;
    struct {
        float frameDeltaTime;
        float lastFrameTime;
        glm::vec2 lastMousePosition;
        bool isFirstMouseMovement;
    } status;
    void setupGLAD();
    void setupWindow(ApplicationParams& params);
    void renderLoop();
    void processKeyboardInput();
    void setProcessMouseCallback();


public:
    GLApplication(ApplicationParams& params);
    void addRenderableObject(RenderableObject obj);


    std::unordered_map<std::string,std::any> context;

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
};


