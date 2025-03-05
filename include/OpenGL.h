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
	Shader* shader;
	Shader* lightShader;

	const unsigned SCR_WIDTH = 1000;
	const unsigned SCR_HEIGHT = 800;

public:
	float lastX;
	float lastY;
	bool firstMouse;

	// Constructors/Destructors
	OpenGL(const char* vertexSource, const char* fragSource, const char* lightVertSource, const char* lightFragSource);

	// Getters/setters
	Camera* getCamera();
	GLFWwindow* getWindow();
	Shader* getShader();
	Shader* getLightShader();

	unsigned getScreenWidth();
	unsigned getScreenHeight();
	
	
};

