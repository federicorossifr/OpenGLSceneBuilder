//
// Created by federico on 24/07/2021.
//



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ShaderHandler.h>
#include <VertexHandler.h>
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
std::vector<float> state;
ShaderHandler* tri_shader;

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    });

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    tri_shader = new ShaderHandler("Triangle.vertex.cc","Triangle.fragment.cc");
    std::vector<float> v = {
            0.5f,  -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            0.0f,0.5f,0.0f
    };
    std::vector<unsigned int> indices{  // note that we start from 0!
            0, 1, 2  // second Triangle
    };

    VertexHandler triangle(std::move(v),std::move(indices));

    glfwSetMouseButtonCallback(window,[](GLFWwindow* window, int button, int action, int mods) -> void{
        double x,y;
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            glfwGetCursorPos(window, &x, &y);
            x/=SCR_WIDTH;
            y/=SCR_HEIGHT;
            std::cout << "Click at " << x << " " << y << "\n";
        }
    });

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);
        float offset = glfwGetTime();
        float offsetVal = (std::sin(offset) / 2.0f);
        tri_shader->setScalarUniform<float>("offset",offsetVal);
        tri_shader->useShader();
        triangle.draw();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}


