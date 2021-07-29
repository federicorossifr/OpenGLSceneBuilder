//
// Created by federico on 28/07/2021.
//

#include <functional>
#include "GLApplication.h"

float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;

GLApplication::GLApplication(ApplicationParams& params): applicationParams(params), camera((glm::vec3(0.0f, 0.0f, 3.0f))) {
    std::cout << "Setting up window" << std::endl;
    setupWindow(params);
    std::cout << "Setting up GLAD" << std::endl;
    setupGLAD();
}

void GLApplication::setupGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(-1);
    }
}

void GLApplication::setupWindow(ApplicationParams& params) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(params.screenWidth, params.screenWidth, "LearnOpenGL", NULL, NULL);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window,this);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

}

void GLApplication::renderLoop() {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // ToDo somemhow handle inputs
        glfwPollEvents();
        float time = glfwGetTime();
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        processKeyboardInput();

        auto rot = glm::rotate(glm::mat4(1.f),glm::radians(90.f),glm::vec3(1.f,0.f,0.));
        auto persp = glm::perspective(glm::radians(45.f), (float)applicationParams.screenWidth / (float)applicationParams.screenHeight, 0.1f, 100.0f);
        for(const auto& obj: objects) {
            obj.shaderHandler->applyMat("model",rot*glm::rotate(glm::mat4(1.f),time*glm::radians(45.f),glm::vec3(0.f,0.f,1.)));
            obj.shaderHandler->applyMat("projection",persp);
            obj.shaderHandler->applyMat("view",camera.GetViewMatrix());

            obj.shaderHandler->useShader();
            obj.vertexHandler->draw();
        }
        glfwSwapBuffers(window);
    }
}

void GLApplication::run() {
    renderLoop();
}

void GLApplication::addRenderableObject(RenderableObject obj) {
    objects.push_back(obj);
}

void GLApplication::processKeyboardInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);

}




