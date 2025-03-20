#include <iostream>
#include <chrono> 
#include <time.h>
#include <cstdlib>
#include "../include/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <../include/Camera.h>
#include "../include/OpenGL.h"
#include "../include/Model.h"
#include "../include/stb_image.h"
#include "Cubes.h"
#include "Planes.h"

#include "ImGui/imgui.h"
#include "ImGui/backend/imgui_impl_glfw.h"
#include "ImGui/backend/imgui_impl_opengl3.h"

//Shader modelShader("Shaders\\ModelLoading.vert", "Shaders\\ModelLoading.frag");
 
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

// Model Code
/*modelShader.use();
glm::mat4 model = glm::mat4(1.0f);
model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
modelShader.setMat4("model", model);
modelShader.setMat4("view", camera->lookAt);
modelShader.setMat4("projection", projection);*/

//backpack.Draw(&modelShader);

unsigned int loadCubemap(std::vector<std::string> faces);

int main() {
    const float SCREEN_SIZE = 900.0f;

    OpenGL openGL(SCREEN_SIZE, SCREEN_SIZE);
    
    Shader rayShader("Shaders\\Ray.vert", "Shaders\\Ray.frag");
    Shader screenShader("Shaders\\ScreenShader.vert", "Shaders\\ScreenShader.frag");

    Camera* camera = openGL.getCamera();

    srand(time(0));

    std::chrono::high_resolution_clock clock;

    float vertices[] = {
        // Vertex Coords        Texture Coords
         -1.0f, -1.0f, 0.0f,    0.0f, 0.0f,
         -1.0f, 1.0f, 0.0f,     0.0f, 1.0f,
         1.0f, 1.0f, 0.0f,      1.0f, 1.0f,

         1.0f, 1.0f, 0.0f,      1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
         -1.0f, -1.0f, 0.0f,    0.0f, 0.0f
    };

    rayShader.use();
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

    screenShader.use();
    glBindVertexArray(rectVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    screenShader.setUInt("Frames", camera->frames);

    rayShader.use();
    // Uniforms
    glm::vec2 resolution(openGL.getScreenWidth(), openGL.getScreenHeight());
    rayShader.setInt("NumSpheres", 4);
    rayShader.setVec2("Resolution", resolution);
    rayShader.setInt("Bounces", 5);
    rayShader.setInt("Time", rand());

    std::vector<glm::vec3> spherePositions;
    std::vector<glm::vec3> sphereColors;
    std::vector<float> sphereRadii;
    std::vector<float> roughness;
    std::vector<float> metallic;
    std::vector<glm::vec3> emissionColor;
    std::vector<float> emissionPower;

    spherePositions.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    spherePositions.push_back(glm::vec3(0.0f, -101.f, 0.0f));
    spherePositions.push_back(glm::vec3(2.0f, 0.0f, 0.0f));
    spherePositions.push_back(glm::vec3(-3.0f, 2.0f, 3.0f));
    sphereColors.push_back(glm::vec3(1.0f, 0.0f, 1.0f));
    sphereColors.push_back(glm::vec3(0.2f, 0.3f, 1.0f));
    sphereColors.push_back(glm::vec3(0.8f, 0.5f, 0.2f));
    sphereColors.push_back(glm::vec3(0.8f, 0.5f, 0.2f));

    sphereRadii.push_back(1.0f);
    sphereRadii.push_back(100.0f);
    sphereRadii.push_back(1.0f);
    sphereRadii.push_back(1.0f);

    roughness.push_back(0.0f);
    roughness.push_back(1.0f);
    roughness.push_back(1.0f);
    roughness.push_back(0.0f);

    metallic.push_back(0.5f);
    metallic.push_back(0.5f);
    metallic.push_back(0.5f);
    metallic.push_back(0.5f);

    emissionColor.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    emissionColor.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
    emissionColor.push_back(glm::vec3(0.8f, 0.5f, 0.2f));
    emissionColor.push_back(glm::vec3(0.8f, 0.5f, 0.2f));

    emissionPower.push_back(0.0f);
    emissionPower.push_back(0.0f);
    emissionPower.push_back(2.0f);
    emissionPower.push_back(2.0f);

    rayShader.setVec3v("SpherePositions", spherePositions);
    rayShader.setVec3v("SphereColors", sphereColors);
    rayShader.setFloatv("SphereRadii", sphereRadii);
    rayShader.setFloatv("Roughness", roughness);
    //rayShader.setFloatv("Metallic", metallic);
    rayShader.setVec3v("EmissionColor", emissionColor);
    rayShader.setFloatv("EmissionPower", emissionPower);

    // Frame buffer objects
    GLuint accumulationFBO, accumulationTex;
    glGenFramebuffers(1, &accumulationFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, accumulationFBO);

    glGenTextures(1, &accumulationTex);
    glBindTexture(GL_TEXTURE_2D, accumulationTex);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, SCREEN_SIZE, SCREEN_SIZE, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Attach texture to framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, accumulationTex, 0);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    std::vector<std::string> faces{
        "Textures\\skybox\\right.jpg",
        "Textures\\skybox\\left.jpg",
        "Textures\\skybox\\top.jpg",
        "Textures\\skybox\\bottom.jpg",
        "Textures\\skybox\\front.jpg",
        "Textures\\skybox\\back.jpg"
    };
    
    unsigned int cubeMapTextureID = loadCubemap(faces);
    std::cout << "Skybox cube map textureID: " << cubeMapTextureID << std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplGlfw_InitForOpenGL(openGL.getWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    auto prevTime = clock.now();
    auto currTime = clock.now();

    unsigned runningFrameCount = 0;
    long long totalFrames = 0;

    while (!glfwWindowShouldClose(openGL.getWindow())) {
        // Per frame time logic
        currTime = clock.now();
        auto deltaTime = duration_cast<std::chrono::milliseconds>(currTime - prevTime);
        prevTime = currTime;

        // User input
        openGL.processInput(static_cast<float>(deltaTime.count()));

        // First pass accumulation buffer
        glBindVertexArray(rectVAO);
        glBindFramebuffer(GL_FRAMEBUFFER, accumulationFBO);
        if (camera->moved) {
            glClear(GL_COLOR_BUFFER_BIT);
            camera->moved = false;
            camera->frames = 1;
            screenShader.setUInt("Frames", camera->frames);
        }
        rayShader.use();
        rayShader.setVec3("CamPosition", camera->camPosition);
        rayShader.setVec3("CamDirection", camera->camFront);
        rayShader.setVec3("CamRight", camera->camRight);
        rayShader.setVec3("CamUp", camera->camUp);
        rayShader.setVec3v("SpherePositions", spherePositions);
        rayShader.setFloatv("SphereRadii", sphereRadii);
        rayShader.setVec3v("SphereColors", sphereColors);
        rayShader.setFloatv("Roughness", roughness);
        rayShader.setVec3v("EmissionColor", emissionColor);
        rayShader.setFloatv("EmissionPower", emissionPower);

        int time = rand();
        srand(time);
        rayShader.setInt("Time", rand());
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
        rayShader.setInt("Skybox", 1);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Second pass with default framebuffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        screenShader.use();
        screenShader.setUInt("Frames", camera->frames);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, accumulationTex);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // If in editing mode, render ImGUI editing components
        if (openGL.editing) {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGui::Begin("Scene properties");

            //ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            //ImGui::DockSpace(dockspace_id);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::Separator();

            for (int i = 0; i < spherePositions.size(); ++i) {
                ImGui::Text("Sphere %d", (i + 1));

                ImGui::PushID(i);
                ImGui::DragFloat3("Position", glm::value_ptr(spherePositions[i]), 0.1f);
                ImGui::ColorEdit3("Color", glm::value_ptr(sphereColors[i]));
                ImGui::DragFloat("Radius", &sphereRadii[i], 0.1f);
                ImGui::DragFloat("Roughness", &roughness[i], 0.01f, 0.0f, 1.0f);
                ImGui::ColorEdit3("Emission Color", glm::value_ptr(emissionColor[i]));
                ImGui::DragFloat("Emission Power", &emissionPower[i], 0.01f, 0.0f, 1.0f);
                ImGui::Separator();
                ImGui::PopID();
            }

            ImGui::End();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }
        }
        

        // FPS
        calculateFPS( runningFrameCount, totalFrames);

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