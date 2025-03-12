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

bool raytracingOn = true;

int main() {
    OpenGL openGL(900.0f, 900.0f);
    
    Shader rayShader("Shaders\\Ray.vert", "Shaders\\Ray.frag");

    Camera* camera = openGL.getCamera();

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
    rayShader.setVec2("Resolution", resolution);
    rayShader.setInt("Bounces", 1);
    
    std::vector<glm::vec3> spherePositions;
    std::vector<glm::vec3> sphereColors;
    std::vector<float> sphereRadii;

    spherePositions.push_back(glm::vec3(0.0f, 0.0f, -5.0f));
    spherePositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    sphereColors.push_back(glm::vec3(0.0f, 0.0f, 1.0f));
    sphereColors.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
    sphereRadii.push_back(2.5f);
    sphereRadii.push_back(1.0f);

    rayShader.setVec3v("SpherePositions", spherePositions);
    rayShader.setVec3v("SphereColors", sphereColors);
    rayShader.setFloatv("SphereRadii", sphereRadii);

    // Frame buffer
    GLuint FBO, tex;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

        // Clean Frame
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Process user input
        processInput(openGL.getWindow(), camera, deltaTime);

        glBindVertexArray(rectVAO);
        
        rayShader.use();
        rayShader.setVec3("CamPosition", camera->camPosition);
        rayShader.setVec3("CamDirection", camera->camFront);
        rayShader.setVec3("CamRight", camera->camRight);
        rayShader.setVec3("CamUp", camera->camUp);

        glDrawArrays(GL_TRIANGLES, 0, 6);

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

    glfwTerminate();
	return 0;
}
