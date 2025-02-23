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

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    if (direction == FORWARD)
        camPosition += cameraSpeed * deltaTime * camFront;
    else if (direction == BACKWARD)
        camPosition -= cameraSpeed * deltaTime * camFront;
    else if (direction == LEFT)
        camPosition -= glm::normalize(glm::cross(camFront, camUp)) * cameraSpeed * deltaTime;
    else if (direction == RIGHT)
        camPosition += glm::normalize(glm::cross(camFront, camUp)) * cameraSpeed * deltaTime;
    if (direction == UP)
        camPosition += glm::vec3(0.0f, 1.0f, 0.0f) * cameraSpeed * deltaTime;
    if (direction == DOWN)
        camPosition += glm::vec3(0.0f, -1.0f, 0.0f) * cameraSpeed * deltaTime;
    calculateLookAt();
}
