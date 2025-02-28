#pragma once
#include <glad/glad.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

	// Uniform Functionality (Constants)
	// ---------------------------------
	void setFloat(const char* name, float f);

	// Uniform Functionality (Vectors)
	// -------------------------------
	void setVec2(const char* name, float f0, float f1);
	void setVec3(const char* name, float f0, float f1, float f2);
	void setVec4(const char* name, float f0, float f1, float f2, float f3);

	// Uniform Functionality (Matrices)
	// --------------------------------
	void setMat2(const char* name, glm::mat2& matrix);
	void setMat3(const char* name, glm::mat2& matrix);
	void setMat4(const char* name, glm::mat2& matrix);

	

	

};