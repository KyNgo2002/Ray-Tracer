#include "../include/Camera.h"

Camera::Camera(float scrWidth, float scrHeight)
    : camPosition(glm::vec3(0.0f, 0.0f, 2.0f)),
      camFront(glm::vec3(0.0f, 0.0f, -1.0f)),
      camUp(glm::vec3(0.0f, 1.0f, 0.0f)),
      worldUp(camUp),
      camRight(glm::vec3(0.0f, 0.0f, 0.0f)),
	  lookAt(glm::lookAt(camPosition, camPosition + camFront, camUp)),
      inverseLookAt(glm::inverse(lookAt)),
      perspective(glm::perspective(glm::radians(45.0f), scrWidth / scrHeight, 0.1f, 100.0f)),
      inversePerspective(glm::inverse(perspective)) {

    yaw = -90.0f;
    pitch = 0.0f;
    frames = 1;
    moved = false;
    editing = false;

    updateCameraVectors();
}

void Camera::calculateLookAt() {
	lookAt = glm::lookAt(camPosition, camPosition + camFront, camUp);
    inverseLookAt = glm::inverse(lookAt);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    if (direction == FORWARD) 
        camPosition += cameraSpeed * deltaTime * glm::normalize(glm::vec3(camFront[0], 0.0f, camFront[2]));
    else if (direction == BACKWARD)
        camPosition -= cameraSpeed * deltaTime * glm::normalize(glm::vec3(camFront[0], 0.0f, camFront[2]));
    else if (direction == LEFT)
        camPosition -= cameraSpeed * deltaTime * camRight;
    else if (direction == RIGHT)
        camPosition += cameraSpeed * deltaTime * camRight;
    else if (direction == UP)
        camPosition += glm::vec3(0.0f, 1.0f, 0.0f) * cameraSpeed * deltaTime;
    else if (direction == DOWN)
        camPosition += glm::vec3(0.0f, -1.0f, 0.0f) * cameraSpeed * deltaTime;
    calculateLookAt();
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch) {
    xoffset *= MOUSE_SENSITIVITY;
    yoffset *= MOUSE_SENSITIVITY;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    camFront = glm::normalize(front);
    // also re-calculate the Right and Up vector
    camRight = glm::normalize(glm::cross(camFront, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    camUp = glm::normalize(glm::cross(camRight, camFront));
}

void Camera::setMoved() {
    moved = true;
}

void Camera::changeEditingMode() {
    editing = !editing;
}
