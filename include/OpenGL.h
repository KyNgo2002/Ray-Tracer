#pragma once
#include <iostream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "windows.h"
#include "Camera.h"
#include "Shader.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Camera* camera, float deltaTime);
void calculateFPS(unsigned& runningFrameCount, long long& totalFrames);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Class that encapsulates the OpenGL state machine
class OpenGL {
private:
	GLFWwindow* window;
	Camera* camera;

	const float SCR_WIDTH;
	const float SCR_HEIGHT;

public:
	float lastX;
	float lastY;
	bool firstMouse;

public:
	// Constructors/Destructors
	OpenGL(float SCR_WIDTH, float SCR_HEIGHT);

	// Getters/setters
	Camera* getCamera();
	GLFWwindow* getWindow();

	float getScreenWidth();
	float getScreenHeight();
	
};

