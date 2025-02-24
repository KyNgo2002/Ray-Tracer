#include "Camera.h"

Camera::Camera(float scrWidth, float scrHeight)
	: camPosition(glm::vec3(0.0f, 0.0f, 3.0f)),
	  camFront(glm::vec3(0.0f, 0.0f, -1.0f)),
	  camUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	  lookAt(glm::lookAt(camPosition, camPosition + camFront, camUp)) {

    yaw = 0.0f;
    pitch = 0.0f;
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
    else if (direction == UP)
        camPosition += glm::vec3(0.0f, 1.0f, 0.0f) * cameraSpeed * deltaTime;
    else if (direction == DOWN)
        camPosition += glm::vec3(0.0f, -1.0f, 0.0f) * cameraSpeed * deltaTime;
    calculateLookAt();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}
