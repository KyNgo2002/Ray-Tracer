#include <iostream>
#include "windows.h"
#include "../include/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <../include/Camera.h>
#include "../include/OpenGL.h"
#include "../include/Model.h"
#include "Cubes.h"
#include "Planes.h"
#include <time.h>
#include <cstdlib>

int main() {
    const float SCREEN_SIZE = 900.0f;

    OpenGL openGL(SCREEN_SIZE, SCREEN_SIZE);
    
    Shader rayShader("Shaders\\Ray.vert", "Shaders\\Ray.frag");
    Shader screenShader("Shaders\\ScreenShader.vert", "Shaders\\ScreenShader.frag");

    Camera* camera = openGL.getCamera();

    srand(time(0));
    //Shader modelShader("Shaders\\ModelLoading.vert", "Shaders\\ModelLoading.frag");
   
    /*Cubes cubes(camera, openGL.getScreenWidth(), openGL.getScreenHeight());
    cubes.addCube();
    cubes.addCube();
    cubes.translateCube(1, glm::vec3(1.0f, 1.0f, 1.0f));
    cubes.scaleCube(0, glm::vec3(0.5f, 0.1f, 2.2f));*/

    /*Planes planes(camera, openGL.getScreenWidth(), openGL.getScreenHeight());
    planes.addPlane();
    planes.scalePlane(0, glm::vec3(200.0f, 200.0f, 200.0f));
    planes.rotatePlane(0, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));*/

    // MVP matrices
    /*glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    modelShader.setMat4("model", model);
    glm::mat4 projection = 
        glm::perspective(glm::radians(45.0f), (float)openGL.getScreenWidth() / (float)openGL.getScreenHeight(), 0.1f, 100.0f);
    
    modelShader.setMat4("view", camera->lookAt);
    modelShader.setMat4("projection", projection);*/ 

    //char path[] = "C:\\Users\\kyngo\\Downloads\\backpack\\backpack.obj";

    // Model loading
    //Model backpack(path);
    float vertices[] = {
         -1.0f, -1.0f, 0.0f,
         -1.0f, 1.0f, 0.0f,
         1.0f, 1.0f, 0.0f,

         1.0f, 1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         -1.0f, -1.0f, 0.0f
    };

    rayShader.use();
    unsigned rectVAO, rectVBO;
    glGenVertexArrays(1, &rectVAO);
    glGenBuffers(1, &rectVBO);

    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(rectVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Uniforms
    glm::vec2 resolution(openGL.getScreenWidth(), openGL.getScreenHeight());
    rayShader.setInt("NumSpheres", 2);
    rayShader.setVec2("Resolution", resolution);
    rayShader.setInt("Bounces", 5);
    rayShader.setInt("Time", rand());

    std::vector<glm::vec3> spherePositions;
    std::vector<glm::vec3> sphereColors;
    std::vector<float> sphereRadii;
    std::vector<float> roughness;
    std::vector<float> metallic;

    spherePositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    spherePositions.push_back(glm::vec3(0.0f, -101.f, 0.0f));
    sphereColors.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
    sphereColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

    sphereRadii.push_back(1.0f);
    sphereRadii.push_back(100.0f);
    roughness.push_back(0.0f);
    roughness.push_back(0.5f);
    metallic.push_back(0.5f);
    metallic.push_back(0.5);

    rayShader.setVec3v("SpherePositions", spherePositions);
    rayShader.setVec3v("SphereColors", sphereColors);
    rayShader.setFloatv("SphereRadii", sphereRadii);
    rayShader.setFloatv("Roughness", roughness);
    //rayShader.setFloatv("Metallic", metallic);


    // Frame buffer objects
    GLuint FBO, tex;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    // use GL_RGBA32F if higher floating point precision needed.
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SCREEN_SIZE, SCREEN_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto prevTime = GetTickCount64();
    auto currTime = GetTickCount64();

    unsigned runningFrameCount = 0;
    long long totalFrames = 0;

    while (!glfwWindowShouldClose(openGL.getWindow())) {

        // Per frame time logic
        currTime = GetTickCount64();
        float deltaTime = currTime - prevTime;
        prevTime = currTime;

        // Bind ray tracing framebuffer
        //glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        // Clean Frame
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Process user input
        processInput(openGL.getWindow(), camera, deltaTime);

        glBindVertexArray(rectVAO);
        rayShader.use();
        rayShader.setVec3("CamPosition", camera->camPosition);
        rayShader.setVec3("CamDirection", camera->camFront);
        rayShader.setVec3("CamRight", camera->camRight);
        rayShader.setVec3("CamUp", camera->camUp);
        rayShader.setInt("Time", rand());

        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Second pass with default framebuffer
        /*glBindFramebuffer(GL_FRAMEBUFFER, 0);
        screenShader.use();
        glBindTexture(GL_TEXTURE_2D, tex);
        glDrawArrays(GL_TRIANGLES, 0, 6);*/
        /*modelShader.use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        modelShader.setMat4("model", model);
        modelShader.setMat4("view", camera->lookAt);
        modelShader.setMat4("projection", projection);*/

        //backpack.Draw(&modelShader);

        //cubes.draw(lightShader);
        //planes.draw(lightShader);

        calculateFPS(runningFrameCount, totalFrames);

        // Check Events and swap buffers
        glfwSwapBuffers(openGL.getWindow());
        glfwPollEvents();
    }
    glDeleteFramebuffers(1, &FBO);
    glfwTerminate();
	return 0;
}
