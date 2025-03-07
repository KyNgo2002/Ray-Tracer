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

int main() {
    OpenGL openGL("Shaders/Vert.vert", "Shaders/Frag.frag", "Shaders/LightVert.vert", "Shaders/LightFrag.frag");
    
    Shader modelShader("Shaders\\ModelLoading.vert", "Shaders\\ModelLoading.frag");
    Shader* lightShader = openGL.getLightShader();
   
    Camera* camera = openGL.getCamera();

    Cubes cubes(camera, openGL.getScreenWidth(), openGL.getScreenHeight());
    cubes.addCube();
    cubes.addCube();
    cubes.translateCube(1, glm::vec3(1.0f, 1.0f, 1.0f));
    cubes.scaleCube(0, glm::vec3(0.5f, 0.1f, 2.2f));

    Planes planes(camera, openGL.getScreenWidth(), openGL.getScreenHeight());
    planes.addPlane();
    planes.scalePlane(0, glm::vec3(200.0f, 200.0f, 200.0f));
    planes.rotatePlane(0, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    // MVP matrices
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    modelShader.setMat4("model", model);
    glm::mat4 projection = 
        glm::perspective(glm::radians(45.0f), (float)openGL.getScreenWidth() / (float)openGL.getScreenHeight(), 0.1f, 100.0f);
    
    modelShader.setMat4("view", camera->lookAt);
    modelShader.setMat4("projection", projection); 

    char path[] = "C:\\Users\\kyngo\\Downloads\\backpack\\backpack.obj";

    // Model loading
    Model backpack(path);

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

        modelShader.use();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        modelShader.setMat4("model", model);
        modelShader.setMat4("view", camera->lookAt);
        modelShader.setMat4("projection", projection);

        //backpack.Draw(&modelShader);

        cubes.draw(lightShader);
        planes.draw(lightShader);

        calculateFPS(runningFrameCount, totalFrames);

        // Check Events and swap buffers
        glfwSwapBuffers(openGL.getWindow());
        glfwPollEvents();
    }

    glfwTerminate();
	return 0;
}
