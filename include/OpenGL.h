#pragma once
#include <iostream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include "windows.h"
#include "Camera.h"
#include "Shader.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Class that encapsulates the OpenGL state machine
class OpenGL {
private:
	// OpenGl window and Camera
	GLFWwindow* window;
	Camera* camera;

	const float SCR_WIDTH;
	const float SCR_HEIGHT;

public:
	// Mouse variables
	float lastX;
	float lastY;
	bool firstMouse;

	// Editing move flag
	bool editingMode;

public:
	// Constructors/Destructors
	OpenGL(float SCR_WIDTH, float SCR_HEIGHT);

	// Getters/setters
	Camera* getCamera();
	GLFWwindow* getWindow();
	float getScreenWidth();
	float getScreenHeight();
	void changeEditingMode();

	// Modifiers
	void processInput(float deltaTime);

};

