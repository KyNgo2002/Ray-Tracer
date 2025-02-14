#include <iostream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "windows.h"
#include "../include/Shader.h"

const unsigned WINDOW_SIZE = 900;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void calculateFPS(unsigned& runningFrameCount, long long& totalFrames);

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_SIZE, WINDOW_SIZE, "Ray Tracer", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glViewport(0, 0, WINDOW_SIZE, WINDOW_SIZE);

    Shader shader("../Shaders/vert.glsl", "../Shaders/frag.glsl");

    float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    auto prevTime = GetTickCount64();
    auto currTime = GetTickCount64();

    unsigned runningFrameCount = 0;
    long long totalFrames = 0;

    while (!glfwWindowShouldClose(window)) {
        // Inputs
        processInput(window);

        //Rendering
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        calculateFPS(runningFrameCount, totalFrames);

        // Check Events and swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void calculateFPS(unsigned& runningFrameCount, long long& totalFrames) {
    static int frames = 0;
    static float lastTime = 0.0f;
    auto currentTime = GetTickCount64() * 0.001f;

    frames++;
    if (currentTime - lastTime >= 1.0f) {
        totalFrames += frames;
        runningFrameCount++;
        std::cout << "FPS: " << frames << std::endl;
        std::cout << "Average FPS: " << totalFrames / runningFrameCount - 1 << std::endl;
        lastTime = currentTime;
        frames = 0;
    }
}