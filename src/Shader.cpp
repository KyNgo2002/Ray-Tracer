#include "../include/Shader.h"
#include <iostream>
#include <sstream>

Shader::Shader() {
	shaderProgramID = -1;
}

Shader::Shader(const char* vertexSource, const char* fragSource) {
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::string vertString;
	std::string fragString;

	try {
		vShaderFile.open(vertexSource);
		fShaderFile.open(fragSource);

		if (!vShaderFile.is_open())
			std::cerr << "ERROR: Could not open vertex shader file" << std::endl;
		else
			std::cout << "SUCCESS: Loaded vertex shader -> " << vertexSource << std::endl;
		if (!fShaderFile.is_open())
			std::cerr << "ERROR: could not open fragment shader file" << std::endl;
		else
			std::cout << "SUCCESS: Loaded fragment Shader -> " << fragSource << std::endl;


		// Initialize string streams to read from shader files
		std::stringstream vStream, fStream, cStream;
		vStream << vShaderFile.rdbuf();
		fStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertString = vStream.str();
		fragString = fStream.str();
	}
	catch (std::ifstream::failure err) {
		std::cerr << "ERROR: Shader file unsuccessfully read" << std::endl;
	}

	const char* vertCode = vertString.c_str();
	const char* fragCode = fragString.c_str();

	// Vertex Shader initialization
	int vertexShader = glCreateShader(GL_VERTEX_SHADER); 
	glShaderSource(vertexShader, 1, &vertCode, NULL);
	glCompileShader(vertexShader);

	// Vertex shader compilation error checking
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment shader initialization
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragCode, NULL);
	glCompileShader(fragmentShader);

	// Fragment Shader compilation error checking
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Linked both/all compiled shaders objects into final shader program object;
	shaderProgramID = glCreateProgram();
	std::cout << "ShaderProgramID: -> " << shaderProgramID << std::endl;
	glAttachShader(shaderProgramID, vertexShader);
	glAttachShader(shaderProgramID, fragmentShader);
	glLinkProgram(shaderProgramID);

	// Error checking final shader program linking
	glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramID, 512, NULL, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Clean up shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::~Shader() {
	shaderProgramID = -1;
}

void Shader::clean() {
	glDeleteProgram(shaderProgramID);
}

void Shader::use() {
	glUseProgram(shaderProgramID);
}

void Shader::setFloat(const char* name, float f) {
	int floatUniformLocation = glGetUniformLocation(shaderProgramID, name);
	glUniform1f(floatUniformLocation, f);
}

void Shader::setVec2(const char* name, float f0, float f1) {
	int vectorUniformLocation = glGetUniformLocation(shaderProgramID, name);
	glUniform2f(vectorUniformLocation, f0, f1);
}

void Shader::setVec3(const char* name, float f0, float f1, float f2) {
	int vectorUniformLocation = glGetUniformLocation(shaderProgramID, name);
	glUniform3f(vectorUniformLocation, f0, f1, f2);
}

void Shader::setVec4(const char* name, float f0, float f1, float f2, float f3) {
	int vectorUniformLocation = glGetUniformLocation(shaderProgramID, name);
	glUniform4f(vectorUniformLocation, f0, f1, f2, f3);
}

void Shader::setMat2(const char* name, glm::mat2& matrix) {
	int matrixUniformLocation = glGetUniformLocation(shaderProgramID, name);
	glUniformMatrix2fv(matrixUniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setMat3(const char* name, glm::mat2& matrix) {
	int matrixUniformLocation = glGetUniformLocation(shaderProgramID, name);
	glUniformMatrix3fv(matrixUniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setMat4(const char* name, glm::mat2& matrix) {
	int matrixUniformLocation = glGetUniformLocation(shaderProgramID, name);
	glUniformMatrix4fv(matrixUniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}


