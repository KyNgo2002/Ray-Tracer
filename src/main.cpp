#include <iostream>
#include <chrono> 
#include <time.h>
#include <cstdlib>
#include "../include/Shader.h"

#include <../include/Camera.h>
#include "../include/OpenGL.h"
#include "../include/Scene.h"

#include "../include/Model.h"
#include "../include/stb_image.h"

unsigned int loadCubemap(std::vector<std::string> faces);

int main() {
    const float SCREEN_SIZE = 900.0f;

    // Set up program context
    OpenGL openGL(SCREEN_SIZE, SCREEN_SIZE);
    
    // Set up required shaders
    Shader rayShader("Shaders\\Ray.vert", "Shaders\\Ray.frag");
    Shader brightnessShader("Shaders\\BrightnessShader.vert", "Shaders\\BrightnessShader.frag");
    Shader blurShader("Shaders\\BlurShader.vert", "Shaders\\BlurShader.frag");
    Shader screenShader("Shaders\\ScreenShader.vert", "Shaders\\ScreenShader.frag");
    
    // Set up 
    Scene scene;
    scene.loadModel("Assets\\Pawn.obj");
    scene.createImGuiEditor(openGL.getWindow());
    Camera* camera = openGL.getCamera();

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

    // Ray tracing stage shader setup
    unsigned rectVAO, rectVBO;
    glGenVertexArrays(1, &rectVAO);
    glGenBuffers(1, &rectVBO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    rayShader.use();
    glBindVertexArray(rectVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    rayShader.setInt("NumSpheres", scene.numSpheres);
    rayShader.setInt("NumTriangles", scene.numTriangles);
    rayShader.setVec2("Resolution", openGL.getScreenWidth(), openGL.getScreenHeight());
    rayShader.setInt("Bounces", 5);
    rayShader.setInt("Time", rand());

    brightnessShader.use();
    glBindVertexArray(rectVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    brightnessShader.setUInt("Frames", camera->frames);

    blurShader.use();
    glBindVertexArray(rectVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::vector<float> gaussianWeights = { 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216 };
    blurShader.setFloatv("Weights", gaussianWeights);
    blurShader.setBool("Horizantal", true);
    blurShader.setFloat("TextureOffset", 1.0f / SCREEN_SIZE);

    // Set up Sky box
    std::vector<std::string> faces{
        "Assets\\skybox\\right.jpg",
        "Assets\\skybox\\left.jpg",
        "Assets\\skybox\\top.jpg",
        "Assets\\skybox\\bottom.jpg",
        "Assets\\skybox\\front.jpg",
        "Assets\\skybox\\back.jpg"
    };

    unsigned int cubeMapTextureID = loadCubemap(faces);
    std::cout << "Skybox cube map textureID: " << cubeMapTextureID << std::endl;

    // Accumulation framebuffer and texture
    GLuint accumulationFBO, accumulationTex;
    glGenFramebuffers(1, &accumulationFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, accumulationFBO);
    glGenTextures(1, &accumulationTex);
    glBindTexture(GL_TEXTURE_2D, accumulationTex);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCREEN_SIZE, SCREEN_SIZE, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumulationTex, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Brightness framebuffer and texture
    GLuint brightnessFBO, brightnessTex;
    glGenFramebuffers(1, &brightnessFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, brightnessFBO);
    glGenTextures(1, &brightnessTex);
    glBindTexture(GL_TEXTURE_2D, brightnessTex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCREEN_SIZE, SCREEN_SIZE, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brightnessTex, 0);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Blur framebuffers and textures
    GLuint blurFBOH, blurTexH;
    glGenFramebuffers(1, &blurFBOH);
    glBindFramebuffer(GL_FRAMEBUFFER, blurFBOH);
    glGenTextures(1, &blurTexH);
    glBindTexture(GL_TEXTURE_2D, blurTexH);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCREEN_SIZE, SCREEN_SIZE, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurTexH, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    GLuint blurFBOV, blurTexV;
    glGenFramebuffers(1, &blurFBOV);
    glBindFramebuffer(GL_FRAMEBUFFER, blurFBOV);
    glGenTextures(1, &blurTexV);
    glBindTexture(GL_TEXTURE_2D, blurTexV);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCREEN_SIZE, SCREEN_SIZE, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, blurTexV, 0);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Timing setup
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
            scene.handleEdits();
            glClear(GL_COLOR_BUFFER_BIT);
            camera->moved = false;
            camera->frames = 1;
            brightnessShader.use();
            brightnessShader.setUInt("Frames", camera->frames);
        }
        // Set viewing angle uniforms
        rayShader.use();
        rayShader.setVec3("CamPosition", camera->camPosition);
        rayShader.setVec3("CamDirection", camera->camFront);
        rayShader.setVec3("CamRight", camera->camRight);
        rayShader.setVec3("CamUp", camera->camUp);
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, accumulationTex);
        rayShader.setInt("Skybox", 0);
        rayShader.setInt("AccumulationTexture", 1);
        rayShader.setInt("Time", rand());
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Second pass with brightness framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, brightnessFBO);
        glClear(GL_COLOR_BUFFER_BIT);
        brightnessShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, accumulationTex);
        brightnessShader.setInt("AccumulationTexture", 1);
        brightnessShader.setUInt("Frames", camera->frames);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Third pass with blur shader
        glBindFramebuffer(GL_FRAMEBUFFER, blurFBOH);
        glClear(GL_COLOR_BUFFER_BIT);
        blurShader.use();
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, brightnessTex);
        blurShader.setInt("BrightnessTexture", 2);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Final pass with default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        screenShader.use();
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, blurTexH);
        screenShader.setInt("ScreenTexture", 3);      
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // If in editing mode, render ImGUI editing components
        if (openGL.editingMode) 
            scene.displayEditor();
        
        // Check Events and swap buffers
        glfwSwapBuffers(openGL.getWindow());
        glfwPollEvents();
        camera->frames++;
        
    }
    // Cleanup ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glDeleteFramebuffers(1, &accumulationFBO);
    glfwTerminate();
	return 0;
}

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