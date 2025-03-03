#pragma once
#include <iostream>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Camera {
private:
	
public:
	glm::vec3 camPosition;
	glm::vec3 camFront;
	glm::vec3 camUp;
	glm::vec3 camRight;
	glm::vec3 worldUp;

	glm::mat4 lookAt;

	const float cameraSpeed = 0.01f;
	const float MOUSE_SENSITIVITY = 0.05f;
	float yaw;
	float pitch;

	// Constructor
	Camera(float scrWidth, float scrHeight);


	void calculateLookAt();
	
	// Callbacks
	void processKeyboard(CameraMovement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void updateCameraVectors();

	// Getters
	
};

