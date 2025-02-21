#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
private:
	
public:
	Camera();
	glm::vec3 camPosition;
	glm::vec3 camFront;
	glm::vec3 camUp;

	glm::mat4 lookAt;

	const float cameraSpeed = 0.045f;

	void calculateLookAt();


};

