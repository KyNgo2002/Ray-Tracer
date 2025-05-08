#include <iostream>
#include <chrono> 
#include <time.h>
#include <cstdlib>

#include "../include/Shader.h"
#include <../include/Camera.h>
#include "../include/OpenGL.h"
#include "../include/Scene.h"
#include "../include/TexturedBuffer.h"
#include "../include/stb_image.h"

unsigned int loadCubemap(std::vector<std::string> faces);

int main() {
    const float SCREEN_SIZE = 900.0f;

    // Set up program context
    OpenGL openGL(SCREEN_SIZE, SCREEN_SIZE);
    Camera* camera = openGL.getCamera();

    // Shader initialization
    //Shader rayShader("Shaders\\QuadShader.vert", "Shaders\\Ray.frag");
    Shader rayShader("Shaders\\QuadShader.vert", "Shaders\\RayDemo.frag");
    Shader brightnessShader("Shaders\\QuadShader.vert", "Shaders\\BrightnessShader.frag");
    Shader blurShader("Shaders\\QuadShader.vert", "Shaders\\BlurShader.frag");
    Shader bloomShader("Shaders\\QuadShader.vert", "Shaders\\BloomShader.frag");
    Shader screenShader("Shaders\\QuadShader.vert", "Shaders\\ScreenShader.frag");
    

    // Set up scene
    Scene scene(camera);
    //scene.loadModel("Assets\\Pawn.obj");
    scene.createImGuiEditor(openGL.getWindow());

    std::chrono::high_resolution_clock clock;

    // Ray Tracing Shader setup
    float vertices[] = {
        // Vertex Coords        Texture Coords
         -1.0f, -1.0f, 0.0f,    0.0f, 0.0f,
         -1.0f, 1.0f, 0.0f,     0.0f, 1.0f,
         1.0f, 1.0f, 0.0f,      1.0f, 1.0f,

         1.0f, 1.0f, 0.0f,      1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
         -1.0f, -1.0f, 0.0f,    0.0f, 0.0f
    };

    // Skybox asset locations
    std::vector<std::string> faces{
        "Assets\\skybox\\right.jpg",
        "Assets\\skybox\\left.jpg",
        "Assets\\skybox\\top.jpg",
        "Assets\\skybox\\bottom.jpg",
        "Assets\\skybox\\front.jpg",
        "Assets\\skybox\\back.jpg"
    };

    // Gaussian weights for blurring shader pass
    std::vector<float> gaussianWeights = { 0.227027f, 0.1945946f, 0.1216216f, 0.054054f, 0.016216f };

    // Sky box texture initialization
    unsigned int cubeMapTextureID = loadCubemap(faces);
    std::cout << "Skybox cube map textureID: " << cubeMapTextureID << std::endl;

    // Quad rendering setup
    unsigned rectVAO, rectVBO;
    glGenVertexArrays(1, &rectVAO);
    glGenBuffers(1, &rectVBO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindVertexArray(rectVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Shader uniform initialization
    rayShader.use();
    rayShader.setInt("NumSpheres", scene.numSpheres);
    rayShader.setInt("NumTriangles", scene.numTriangles);
    rayShader.setVec2("Resolution", openGL.getScreenWidth(), openGL.getScreenHeight());
    rayShader.setInt("Bounces", 1);
    rayShader.setInt("Time", rand());
    rayShader.setInt("Skybox", 0);
    rayShader.setInt("AccumulationTexture", 1);

    brightnessShader.use();
    brightnessShader.setUInt("Frames", camera->frames);
    brightnessShader.setInt("AccumulationTexture", 1);

    blurShader.use();
    blurShader.setFloatv("Weights", gaussianWeights);
    blurShader.setBool("Horizontal", true);
    blurShader.setFloat("TextureOffset", 1.0f / SCREEN_SIZE);

    bloomShader.use();
    bloomShader.setUInt("Frames", camera->frames);
    bloomShader.setInt("AccumulationTexture", 1);
    bloomShader.setInt("BlurTexture", 4);

    screenShader.use();
    screenShader.setInt("ScreenTexture", 5);

    // Accumulation framebuffer and texture
    GLuint accumulationFBO, accumulationTex;
    glGenFramebuffers(1, &accumulationFBO);
    glGenTextures(1, &accumulationTex);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, accumulationTex);

    // Texture configuration
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCREEN_SIZE, SCREEN_SIZE, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, accumulationFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumulationTex, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;

    // Brightness framebuffer and texture
    GLuint brightnessFBO, brightnessTex;
    glGenFramebuffers(1, &brightnessFBO);
    glGenTextures(1, &brightnessTex);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, brightnessTex);

    // Texture configuration
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCREEN_SIZE, SCREEN_SIZE, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, brightnessFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brightnessTex, 0);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;

    // Blur framebuffers and textures
    GLuint blurFBO[2];
    GLuint blurTex[2];
    for (int i = 0; i < 2; ++i) {
        // Create and bind framebuffer and texture
        glGenFramebuffers(1, &blurFBO[i]);
        glGenTextures(1, &blurTex[i]);
        glActiveTexture(GL_TEXTURE3 + i);
        glBindTexture(GL_TEXTURE_2D, blurTex[i]);

        // Texture configuration
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCREEN_SIZE, SCREEN_SIZE, 0, GL_RGBA, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Framebuffer setup
        glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[i]);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurTex[i], 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cerr << "Framebuffer not complete!" << std::endl;
    }

    // Blur framebuffer and texture
    GLuint bloomFBO, bloomTex;
    glGenFramebuffers(1, &bloomFBO);
    glGenTextures(1, &bloomTex);
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, bloomTex);

    // Texture configuration
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCREEN_SIZE, SCREEN_SIZE, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bloomTex, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;

    // Timing variables
    auto prevTime = clock.now();
    auto currTime = clock.now();
    unsigned runningFrameCount = 0;
    long long totalFrames = 0;

    GLenum err;
    while (!glfwWindowShouldClose(openGL.getWindow())) {
        // Per frame time logic
        currTime = clock.now();
        auto deltaTime = duration_cast<std::chrono::milliseconds>(currTime - prevTime);
        prevTime = currTime;

        // User input
        openGL.processInput(static_cast<float>(deltaTime.count()));
        
        // First pass accumulation buffer
        glBindFramebuffer(GL_FRAMEBUFFER, accumulationFBO);
        if (camera->moved || scene.checkEdits()) {
            glClear(GL_COLOR_BUFFER_BIT);
            scene.handleEdits();
            camera->moved = false;
            camera->frames = 1;
        }

        // Set viewing angle uniforms
        rayShader.use();
        rayShader.setVec3("CamPosition", camera->camPosition);
        rayShader.setVec3("CamDirection", camera->camFront);
        rayShader.setVec3("CamRight", camera->camRight);
        rayShader.setVec3("CamUp", camera->camUp);
        rayShader.setInt("NumSpheres", scene.numSpheres);
        rayShader.setInt("Time", rand());
        glDrawArrays(GL_TRIANGLES, 0, 6);

        if (scene.doBloom) {
            // Second pass with brightness framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, brightnessFBO);
            glClear(GL_COLOR_BUFFER_BIT);
            brightnessShader.use();
            brightnessShader.setUInt("Frames", camera->frames);
            glDrawArrays(GL_TRIANGLES, 0, 6);

            // Third pass blur framebuffer ping ponging
            bool firstIteration = true;
            bool horizontal = true;
            for (int i = 0; i < scene.blurPasses * 2; ++i) {
                glBindFramebuffer(GL_FRAMEBUFFER, blurFBO[!horizontal]);
                // Set appropriate blur shader texture for ping ponging
                blurShader.use();
                blurShader.setBool("Horizontal", horizontal);
                glActiveTexture(firstIteration ? GL_TEXTURE2 : GL_TEXTURE3 + horizontal);
                glBindTexture(GL_TEXTURE_2D, firstIteration ? brightnessTex : blurTex[horizontal]);
                blurShader.setInt("Texture", firstIteration ? 2 : 3 + horizontal);
                glDrawArrays(GL_TRIANGLES, 0, 6);

                if (firstIteration)
                    firstIteration = false;
                horizontal = !horizontal;
            }
            // Fourth pass apply bloom shader
            glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO);
            glClear(GL_COLOR_BUFFER_BIT);
            bloomShader.use();
            bloomShader.setUInt("Frames", camera->frames);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
        
        // Final pass with default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        screenShader.use();
        screenShader.setFloat("Frames", camera->frames);
        screenShader.setBool("DoBloom", scene.doBloom);
        if (scene.doBloom)
            screenShader.setInt("ScreenTexture", 5);
        else
            screenShader.setInt("ScreenTexture", 1);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // If in editing mode, render ImGUI editing components
        if (openGL.editingMode) 
            scene.displayEditor();
        
        // Check Events and swap buffers
        glfwSwapBuffers(openGL.getWindow());
        glfwPollEvents();
        camera->frames++;

        // Check for OpenGL errors
        err = glGetError();
        if (err != GL_NO_ERROR) 
            std::cerr << "OpenGL error: " << std::hex << err << std::endl;
    }
    
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Cleanup OpenGL resources
    glDeleteFramebuffers(1, &accumulationFBO);
    glDeleteFramebuffers(1, &brightnessFBO);
    glDeleteFramebuffers(2, blurFBO);
    glDeleteTextures(1, &accumulationTex);
    glDeleteTextures(1, &brightnessTex);
    glDeleteTextures(2, blurTex);
    
    // Cleanup glfw resources
    glfwTerminate();

    // Terminate
	return 0;
}

// Helper function for loading skybox
unsigned int loadCubemap(std::vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    stbi_set_flip_vertically_on_load(false);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            std::cout << "Sucessfully loaded texture: " << faces[i].c_str() << std::endl;
            stbi_image_free(data);
        }
        else {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    stbi_set_flip_vertically_on_load(true);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}