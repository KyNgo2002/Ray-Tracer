#include <iostream>
#include "windows.h"
#include "../include/Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <../include/Camera.h>
#include "../include/OpenGL.h"
#include "../include/stb_image.h"

unsigned int loadTexture(char const* path);

int main() {
    OpenGL openGL("Shaders/Vert.vert", "Shaders/Frag.frag", "Shaders/LightVert.vert", "Shaders/LightFrag.frag");
    Shader* cubeShader = openGL.getShader();
    Shader* lightShader = openGL.getLightShader();
    Camera* camera = openGL.getCamera();

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f,  0.2f,  2.0f),
        glm::vec3(2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3(0.0f,  0.0f, -3.0f)
    };

    // Vertex Attributes
    unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);

    // Vertex Buffer
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Normal Atribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //Texture Coordinate Attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Light VAO
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // MVP matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = 
        glm::perspective(glm::radians(45.0f), (float)openGL.getScreenWidth() / (float)openGL.getScreenHeight(), 0.1f, 100.0f);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

    unsigned int diffuseMap = loadTexture("./Textures/container.png");
    unsigned int specularMap = loadTexture("./Textures/container1.png");

    cubeShader->use();
    cubeShader->setInt("material.diffuse", 0);
    cubeShader->setInt("material.specular", 1);
    cubeShader->setFloat("material.shininess", 64.0f);

    cubeShader->setMat4("model", model);
    cubeShader->setMat4("view", camera->lookAt);
    cubeShader->setMat4("projection", projection);
    
    // point light 1
    cubeShader->setVec3("pointLights[0].position", pointLightPositions[0]);
    cubeShader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
    cubeShader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
    cubeShader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
    cubeShader->setFloat("pointLights[0].constant", 1.0f);
    cubeShader->setFloat("pointLights[0].linear", 0.09f);
    cubeShader->setFloat("pointLights[0].quadratic", 0.032f);
    // point light 2
    cubeShader->setVec3("pointLights[1].position", pointLightPositions[1]);
    cubeShader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
    cubeShader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
    cubeShader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
    cubeShader->setFloat("pointLights[1].constant", 1.0f);
    cubeShader->setFloat("pointLights[1].linear", 0.09f);
    cubeShader->setFloat("pointLights[1].quadratic", 0.032f);
    // point light 3
    cubeShader->setVec3("pointLights[2].position", pointLightPositions[2]);
    cubeShader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
    cubeShader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
    cubeShader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
    cubeShader->setFloat("pointLights[2].constant", 1.0f);
    cubeShader->setFloat("pointLights[2].linear", 0.09f);
    cubeShader->setFloat("pointLights[2].quadratic", 0.032f);
    // point light 4
    cubeShader->setVec3("pointLights[3].position", pointLightPositions[3]);
    cubeShader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
    cubeShader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
    cubeShader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
    cubeShader->setFloat("pointLights[3].constant", 1.0f);
    cubeShader->setFloat("pointLights[3].linear", 0.09f);
    cubeShader->setFloat("pointLights[3].quadratic", 0.032f);


    cubeShader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    cubeShader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    cubeShader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    cubeShader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);

    lightShader->use();
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f));
    lightShader->setMat4("model", model);
    lightShader->setMat4("view", view);
    lightShader->setMat4("projection", projection);

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
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Process user input
        processInput(openGL.getWindow(), camera, deltaTime);

        // Regular Cube Shader setup
        cubeShader->use();
        cubeShader->setVec3("ViewPos", camera->camPosition);
        cubeShader->setMat4("view", camera->lookAt);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // Regular Cube Render
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Light Cube shader setup
        lightShader->use();
        lightShader->setMat4("view", camera->lookAt);
        
        glBindVertexArray(lightCubeVAO);
        for (int i = 0; i < 4; ++i) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader->setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        calculateFPS(runningFrameCount, totalFrames);

        // Check Events and swap buffers
        glfwSwapBuffers(openGL.getWindow());
        glfwPollEvents();
    }

    // Clean vertex arrays
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    glfwTerminate();
	return 0;
}

unsigned int loadTexture(char const* path) {
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        std::cout << "Successfully loaded texture file" << std::endl;

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}