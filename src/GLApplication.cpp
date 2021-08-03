//
// Created by federico on 28/07/2021.
//

#include <functional>
#include "GLApplication.h"



GLApplication::GLApplication(ApplicationParams& params,glm::vec3 startCamera): applicationParams(params), camera(startCamera) {
    status.frameDeltaTime = 0.f;
    status.lastFrameTime = 0.f;
    status.lastMousePosition = glm::vec2(params.screenWidth/2.f,params.screenHeight/2.f);
    status.isFirstMouseMovement = true;

    std::cout << "Setting up window" << std::endl;
    setupWindow(params);
    std::cout << "Setting up GLAD" << std::endl;
    setupGLAD();
    std::cout << "Setting up additional FBOs" << std::endl;
    setupFBOs();

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

void GLApplication::setupRenderPasses() {
    auto* shadowShader = new ShaderHandler("shaders/ShadowMapping.vert.spv","shaders/ShadowMapping.frag.spv");
    putContext("shadowShader",shadowShader);
    renderPasses = {
            [&]() {
                ShaderHandler* ss;
                getContext("shadowShader",ss);
                ss->useShader();
                glViewport(0, 0, 2048,2048);
                glBindFramebuffer(GL_FRAMEBUFFER, depthMapFrameBuffer);
                glClear(GL_DEPTH_BUFFER_BIT);
                renderScene(status.lastFrameTime,ss,RenderPass::SHADOW);
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                },
                [&]() {
                glViewport(0, 0, applicationParams.screenWidth,applicationParams.screenHeight);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glActiveTexture(GL_TEXTURE31);
                glBindTexture(GL_TEXTURE_2D, depthMapTextureId);
                renderScene(status.lastFrameTime, nullptr,RenderPass::FINAL);
            }
    };
}

void GLApplication::renderLoop() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    setupRenderPasses();

    while (!glfwWindowShouldClose(window)) {

        glfwPollEvents();
        float time = glfwGetTime();
        status.frameDeltaTime = time - status.lastFrameTime;
        status.lastFrameTime = time;
        processKeyboardInput();



        for(const auto& renderPass: renderPasses) renderPass();


        glfwSwapBuffers(window);
    }
    glfwTerminate();
}

void GLApplication::run() {
    renderLoop();
}

void GLApplication::addRenderableObject(RenderableObject obj) {
    renderableScene.objects.push_back(obj);
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

GLFWwindow *GLApplication::getWindowPtr() {
    return window;
}

void GLApplication::setupFBOs() {
    glGenFramebuffers(1,&depthMapFrameBuffer);
    glGenTextures(1,&depthMapTextureId);

    glBindTexture(GL_TEXTURE_2D, depthMapTextureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,2048,2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTextureId, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLApplication::renderScene(float time,ShaderHandler* shader,RenderPass state) {
    auto persp = glm::perspective(glm::radians(camera.Zoom), (float)applicationParams.screenWidth / (float)applicationParams.screenHeight, 0.1f, 100.0f);
    auto lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
    auto lightView = glm::lookAt(-renderableScene.illumination.directionalLight.direction, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    auto lightSpaceMatrix = lightProjection*lightView;
    for(auto& obj: renderableScene.objects) {
        if( shader != nullptr) obj.swapShaderHandler(shader);
        obj.shaderHandler->useShader();
        obj.shaderHandler->applyMat("model",obj.objectModel(time));
        obj.shaderHandler->applyMat("projection",persp);
        obj.shaderHandler->applyMat("view",camera.GetViewMatrix());
        if (state == RenderPass::SHADOW && !obj.canCastShadow) {
            obj.shaderHandler->applyMat("lightSpaceMatrix",glm::mat4(0.f));
        } else {
            obj.shaderHandler->applyMat("lightSpaceMatrix",lightSpaceMatrix);
        }

        if(obj.postModelFun) obj.postModel(time,this);

        obj.vertexHandler->draw();
        if( shader != nullptr) obj.swapShaderHandler();
        glUseProgram(0);
    }
}








