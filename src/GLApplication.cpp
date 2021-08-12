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
    auto* pointShadowShader = new ShaderHandler("shaders/PointShadowMapping.vert.spv","shaders/PointShadowMapping.frag.spv","shaders/PointShadowMapping.geom.spv");

    putContext("shadowShader",shadowShader);
    putContext("pointShadowShader",pointShadowShader);
    renderPasses = {
            [&]() {
                ShaderHandler* ss;
                getContext("shadowShader",ss);
                ss->useShader();
                glViewport(0, 0, 2048,2048);
                glBindFramebuffer(GL_FRAMEBUFFER, depthMapFrameBuffer);
                glClear(GL_DEPTH_BUFFER_BIT);
                renderScene(status.lastFrameTime,ss,RenderPass::Shadow);
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
            },
            [&]() {
                ShaderHandler* ss;
                getContext("pointShadowShader",ss);
                ss->useShader();
                glViewport(0, 0, 2048,2048);
                glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFrameBuffer);
                glClear(GL_DEPTH_BUFFER_BIT);
                renderScene(status.lastFrameTime,ss,RenderPass::PointShadow);
                glBindFramebuffer(GL_FRAMEBUFFER, 0);

            },
            [&]() {
                glViewport(0, 0, applicationParams.screenWidth,applicationParams.screenHeight);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                glActiveTexture(GL_TEXTURE31);
                glBindTexture(GL_TEXTURE_2D, depthMapTextureId);
                glActiveTexture(GL_TEXTURE30);
                glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMapTextureId);
                renderScene(status.lastFrameTime, nullptr,RenderPass::Final);
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
    renderableScene->objects.push_back(obj);
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

    // Create Framebuffer for depth maps

    glGenFramebuffers(1,&depthMapFrameBuffer);
    glGenFramebuffers(1,&depthCubemapFrameBuffer);
    // Attach depth map texture for directional shadows
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


    // Create cubemap for the point shadows

    glGenTextures(1, &depthCubeMapTextureId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMapTextureId);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                     2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    glBindFramebuffer(GL_FRAMEBUFFER, depthCubemapFrameBuffer);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMapTextureId, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLApplication::renderScene(float time,ShaderHandler* shader,const RenderPass state) {
    float far_plane = 100.f;
    auto persp = glm::perspective(glm::radians(camera.Zoom), (float)applicationParams.screenWidth / (float)applicationParams.screenHeight, 0.1f, far_plane);
    auto lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 100.0f);
    auto lightView = glm::lookAt(-renderableScene->illumination.directionalLight.direction, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    auto lightSpaceMatrix = lightProjection*lightView;

    auto shadowProjection = glm::perspective(glm::radians(90.0f), 1.f, 0.1f, 100.0f);

    for(auto& obj: renderableScene->objects) {
        if(state == RenderPass::Final) {
            renderableScene->syncIllumination();
            glEnable(GL_FRAMEBUFFER_SRGB);
        }
        if( shader != nullptr) obj.swapShaderHandler(shader);
        obj.shaderHandler->useShader();
        obj.shaderHandler->applyMat("model",obj.objectModel(time));
        obj.shaderHandler->applyMat("projection",persp);
        obj.shaderHandler->applyMat("view",camera.GetViewMatrix());
        obj.shaderHandler->setScalarUniform("farPlane",far_plane);
        if (state == RenderPass::Shadow && !obj.canCastShadow) {
            obj.shaderHandler->applyMat("lightSpaceMatrix",glm::mat4(0.f));
        } else {
            obj.shaderHandler->applyMat("lightSpaceMatrix",lightSpaceMatrix);
        }

        if (state == RenderPass::PointShadow && !renderableScene->illumination.pointLights.empty()) {
            // Set matrices for pointshadow shader
            auto lightPos = renderableScene->illumination.pointLights[0].position;
            std::vector<glm::mat4> shadowTransforms;
            shadowTransforms.push_back(shadowProjection *
            glm::lookAt(lightPos, lightPos + glm::vec3( 1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
            shadowTransforms.push_back(shadowProjection *
            glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0,-1.0, 0.0)));
            shadowTransforms.push_back(shadowProjection *
            glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
            shadowTransforms.push_back(shadowProjection *
            glm::lookAt(lightPos, lightPos + glm::vec3( 0.0,-1.0, 0.0), glm::vec3(0.0, 0.0,-1.0)));
            shadowTransforms.push_back(shadowProjection *
            glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0, 1.0), glm::vec3(0.0,-1.0, 0.0)));
            shadowTransforms.push_back(shadowProjection *
            glm::lookAt(lightPos, lightPos + glm::vec3( 0.0, 0.0,-1.0), glm::vec3(0.0,-1.0,0.0)));

            std::for_each(shadowTransforms.begin(),shadowTransforms.end(),[&,n=0](auto& mat) mutable {
                obj.shaderHandler->applyMat("shadowMatrices[" + std::to_string(n++) + "]", mat);
            });
            obj.shaderHandler->setScalarUniform("far_plane",far_plane);
            obj.shaderHandler->setVec3Uniform("lightPos",lightPos);
        }

        if(obj.postModelFun) obj.postModel(time,this);
        obj.vertexHandler->draw();

        if( shader != nullptr) obj.swapShaderHandler();
        glUseProgram(0);
    }
}








