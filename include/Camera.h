#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
	Camera();
	glm::vec3 camPosition;
	glm::vec3 camDirection;
	glm::vec3 camRight;
	glm::vec3 camUp;

};

