#pragma once
#include <iostream>

#include <glad/glad.h> 
#include <GLFW/glfw3.h>

class TexturedBuffer {
private:
public:
	float SCR_WIDTH;
	float SCR_HEIGHT;

	GLuint bufferID;
	GLuint textureID;

	// Constructor/Destructor
	TexturedBuffer(float SCR_WIDTH, float SCR_HEIGHT);
	~TexturedBuffer();
	
	void bind();

};

