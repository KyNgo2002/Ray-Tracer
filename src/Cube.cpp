#include "Cube.h"



Cube::Cube() : model{ glm::mat4(1.0f) } {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeData), cubeData, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

void Cube::Draw(Shader* shader) {
	
}

void Cube::addCube() {
	cubeModelMatrices.push_back(glm::mat4(1.0f));
}

void Cube::translateCube(unsigned index, glm::vec3& translationUnit) {
	if (index >= cubeModelMatrices.size()) {
		std::cerr << "Error: index out of bounds!" << std::endl;
		return;
	}
	cubeModelMatrices[index] = glm::translate(cubeModelMatrices[index], translationUnit);
}




