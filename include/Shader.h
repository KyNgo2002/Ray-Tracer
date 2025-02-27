#pragma once

#include <glad/glad.h>
#include <fstream>

class Shader {
public:
	// Shader Program ID
	unsigned int shaderProgramID;

	// Constructors/Destructor
	Shader();
	Shader(const char* vertexSource, const char* fragSource);
	~Shader();

	void clean();
	void use();
};