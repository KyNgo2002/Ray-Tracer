#pragma once
#include <iostream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "windows.h"
#include <../include/Camera.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Camera& camera, float deltaTime);
void calculateFPS(unsigned& runningFrameCount, long long& totalFrames);


// Class that encapsulates the OpenGL state machine
class OpenGL {
private:
	GLFWwindow* window;
	Camera camera;

	const unsigned SCR_WIDTH = 1000;
	const unsigned SCR_HEIGHT = 800;

public:
	float lastX;
	float lastY;
	bool firstMouse;

	// Constructors/Destructors
	OpenGL();
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

	
};

