//
// Created by federico on 28/07/2021.
//

#include <functional>
#include "GLApplication.h"



GLApplication::GLApplication(ApplicationParams& params): applicationParams(params), camera((glm::vec3(0.0f, 0.f, 3.0f))) {
    status.frameDeltaTime = 0.f;
    status.lastFrameTime = 0.f;
    status.lastMousePosition = glm::vec2(params.screenWidth/2.f,params.screenHeight/2.f);
    status.isFirstMouseMovement = true;

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    setProcessMouseCallback();
}

void GLApplication::renderLoop() {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glfwPollEvents();
        float time = glfwGetTime();
        status.frameDeltaTime = time - status.lastFrameTime;
        status.lastFrameTime = time;
        processKeyboardInput();

        auto persp = glm::perspective(glm::radians(camera.Zoom), (float)applicationParams.screenWidth / (float)applicationParams.screenHeight, 0.1f, 100.0f);
        for(const auto& obj: objects) {
            obj.shaderHandler->useShader();
            obj.shaderHandler->applyMat("model",obj.objectModel(time));
            obj.shaderHandler->applyMat("projection",persp);
            obj.shaderHandler->applyMat("view",camera.GetViewMatrix());
            if(obj.postModelFun) obj.postModel(time,camera);
            obj.vertexHandler->draw();
            glUseProgram(0);
        }
        glfwSwapBuffers(window);
    }
    glfwTerminate();
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
        camera.ProcessKeyboard(FORWARD, status.frameDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, status.frameDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, status.frameDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, status.frameDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, status.frameDeltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, status.frameDeltaTime);
}

void GLApplication::setProcessMouseCallback() {
    glfwSetCursorPosCallback(window,[](GLFWwindow* window, double xpos, double ypos) {
        auto app = reinterpret_cast<GLApplication*>(glfwGetWindowUserPointer(window));
        if (app->status.isFirstMouseMovement)
        {
            app->status.lastMousePosition.x = xpos;
            app->status.lastMousePosition.y = ypos;
            app->status.isFirstMouseMovement = false;
        }

        float xoffset = xpos - app->status.lastMousePosition.x;
        float yoffset = app->status.lastMousePosition.y - ypos; // reversed since y-coordinates go from bottom to top

        app->status.lastMousePosition.x = xpos;
        app->status.lastMousePosition.y = ypos;

        app->camera.ProcessMouseMovement(xoffset, yoffset);
    });

    glfwSetScrollCallback(window,[](GLFWwindow* window, double xoffset, double yoffset) {
        auto app = reinterpret_cast<GLApplication*>(glfwGetWindowUserPointer(window));
        std::cout << " Scroll " << yoffset;
        app->camera.ProcessMouseScroll(yoffset);
    });

}






