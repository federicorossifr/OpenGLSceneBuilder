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
#include <glm/gtc/random.hpp>
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtc/matrix_transform.hpp>
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
ShaderHandler* tri_shader;
glm::vec4 trasl4D;
glm::vec4 anchor;
glm::vec4 colorState;
float speed = 1.f;
void updateTraslMatrix(double x,double y, double z) {
    trasl4D.x = x;
    trasl4D.y = y;
    trasl4D.z = z;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        trasl4D.w += .005f*speed;
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        trasl4D.x -= .005f;
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        trasl4D.x += .005f;
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
       speed = 5.f;
    }

    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        speed = 1.f;
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        trasl4D.w -= .005f*speed;
    }
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

    tri_shader = new ShaderHandler("shaders/TrianglePos.vert.spv","shaders/Triangle.frag.spv");

    std::vector<float> v = {
            0.0f,  0.5f, 0.0f,
            -0.5f, 0.f, 0.0f,
            0.0f,0.0f,0.0f,
            0.5f,0.0f,0.0f
    };
    anchor = glm::vec4( 0.0f,  0.0f, 0.0f,1.f);
    std::vector<unsigned int> indices{  // note that we start from 0!
            0, 1, 2,  // second Triangle
            0, 2, 3
    };

    VertexHandler triangle(std::move(v),std::move(indices));

    glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        x/=width;
        y/=height;

        double x_displ = anchor.x - (2*x-1);
        double y_displ = anchor.y + (2*y-1);

        updateTraslMatrix(-x_displ,-y_displ,0.);
    });

    glfwSetMouseButtonCallback(window,[](GLFWwindow* window, int button, int action, int mods) -> void{

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            colorState.x = glm::linearRand(0.,1.);
            colorState.y = glm::linearRand(0.,1.);
            colorState.z = glm::linearRand(0.,1.);
            colorState.w = 1.;

        }

        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
            glClearColor(glm::linearRand(0.,1.), glm::linearRand(0.,1.), glm::linearRand(0.,1.), glm::linearRand(0.,1.));
            glClear(GL_COLOR_BUFFER_BIT);
        }
    });
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        processInput(window);
        glfwPollEvents();

        float offset = glfwGetTime();
        float alpha = glm::radians(100*offset);

        glm::mat4 rot2D = glm::mat4(1.0f);
        rot2D = glm::rotate(rot2D, alpha, glm::vec3(0.0, 0.0, 1.0));

        tri_shader->applyMat("rotation",rot2D);
        tri_shader->applyVec4("translation",trasl4D);
        tri_shader->applyVec4("color",colorState);
        tri_shader->useShader();
        triangle.draw();
        glfwSwapBuffers(window);

    }
    glfwTerminate();
    return 0;
}


