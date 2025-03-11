#pragma once
#include <glad/glad.h>
#include <fstream>
#include <vector>
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

	// Uniform Location 
	int getUniformLocation(const char* name);

	// Uniform Functionality (Constants)
	void setInt(const char* name, int i);
	void setFloat(const char* name, float f);

	// Uniform Functionality (Vectors)
	void setFloatv(const char* name, std::vector<float>& floats);

	void setVec2(const char* name, glm::vec2& vector);
	void setVec2(const char* name, float f0, float f1);
	void setVec3(const char* name, glm::vec3& vector);
	void setVec3(const char* name, float f0, float f1, float f2);
	void setVec4(const char* name, glm::vec4& vector);
	void setVec4(const char* name, float f0, float f1, float f2, float f3);

	// Uniform Functionality (Matrices)
	void setMat2(const char* name, glm::mat2& matrix);
	void setMat3(const char* name, glm::mat3& matrix);
	void setMat4(const char* name, glm::mat4& matrix);

	// Uniform Functionality (Vectors)
	void setVec2v(const char* name, std::vector<glm::vec2>& vectors);
	void setVec3v(const char* name, std::vector<glm::vec3>& vectors);
	void setVec4v(const char* name, std::vector<glm::vec4>& vectors);
};