#include "Camera.h"

Camera::Camera()
	: camPosition(glm::vec3(0.0f, 0.0f, 3.0f)),
	  camFront(glm::vec3(0.0f, 0.0f, -1.0f)),
	  camUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	  lookAt(glm::lookAt(camPosition, camPosition + camFront, camUp)) {
	
}

void Camera::calculateLookAt() {
	lookAt = glm::lookAt(camPosition, camPosition + camFront, camUp);
}
