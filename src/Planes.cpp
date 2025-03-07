#include "Planes.h"

Planes::Planes(Camera* camera, float scrWidth, float scrHeight) : 
    camera{ camera },
    scrWidth{ scrWidth },
    scrHeight{ scrHeight },
    projection{ glm::perspective(glm::radians(45.0f), scrWidth / scrHeight, 0.1f, 100.0f) } {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeData), planeData, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

Planes::~Planes() {
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VAO);
}

void Planes::draw(Shader* shader) {
	shader->use();
	shader->setMat4("view", camera->lookAt);
	shader->setMat4("projection", projection);
	glBindVertexArray(VAO);
	for (size_t i = 0; i < planeModelMatrices.size(); ++i) {
		shader->setMat4("model", planeModelMatrices[i]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
}

void Planes::addPlane() {
	planeModelMatrices.push_back(glm::mat4(1.0f));
}

void Planes::translatePlane(unsigned index, const glm::vec3& translationUnit) {
	if (index >= planeModelMatrices.size()) {
		std::cerr << "Error::function::translateCube index out of bounds!" << std::endl;
		return;
	}
	planeModelMatrices[index] = glm::translate(planeModelMatrices[index], translationUnit);
}

void Planes::scalePlane(unsigned index, const glm::vec3& scaleUnit) {
	if (index >= planeModelMatrices.size()) {
		std::cerr << "Error::function::scaleCube index out of bounds!" << std::endl;
		return;
	}
	planeModelMatrices[index] = glm::scale(planeModelMatrices[index], scaleUnit);
}

void Planes::rotatePlane(unsigned index, float angle, const glm::vec3& axis) {
	if (index >= planeModelMatrices.size()) {
		std::cerr << "Error::function::scaleCube index out of bounds!" << std::endl;
		return;
	}
	planeModelMatrices[index] = glm::rotate(planeModelMatrices[index], glm::radians(angle), axis);
}

