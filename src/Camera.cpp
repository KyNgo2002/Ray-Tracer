#include "Camera.h"

Camera::Camera()
	: camPosition(glm::vec3(0.0f, 0.0f, 3.0f)),
	  camDirection(glm::normalize(camPosition - glm::vec3(0.0f, 0.0f, 0.0f))),
	  camRight(glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f), camDirection))),
	  camUp(glm::cross(camDirection, camRight)) {

}
