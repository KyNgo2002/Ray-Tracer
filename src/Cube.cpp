#include "Cube.h"
#include "Camera.h"

Cube::Cube(Camera* camera, float scrWidth, float scrHeight) : 
	camera{ camera },
	scrWidth{scrWidth}, scrHeight{scrHeight}, 
	projection{glm::perspective(glm::radians(45.0f), scrWidth / scrHeight, 0.1f, 100.0f) }  {

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

	glDeleteBuffers(1, &VBO);
	glBindVertexArray(0);
}

Cube::~Cube() {
	glDeleteBuffers(1, &VAO);
}

void Cube::draw(Shader* shader) {
	shader->use();
	shader->setMat4("projection", projection);
	for (size_t i = 0; i < cubeModelMatrices.size(); ++i) {
		shader->setMat4("model", cubeModelMatrices[i]);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void Cube::addCube() {
	cubeModelMatrices.push_back(glm::mat4(1.0f));
}

void Cube::translateCube(unsigned index, glm::vec3& translationUnit) {
	if (index >= cubeModelMatrices.size()) {
		std::cerr << "Error::function::translateCube index out of bounds!" << std::endl;
		return;
	}
	cubeModelMatrices[index] = glm::translate(cubeModelMatrices[index], translationUnit);
}

void Cube::scaleCube(unsigned index, glm::vec3& scaleUnit) {
	if (index >= cubeModelMatrices.size()) {
		std::cerr << "Error::function::scaleCube index out of bounds!" << std::endl;
		return;
	}
	cubeModelMatrices[index] = glm::scale(cubeModelMatrices[index], scaleUnit);
}






