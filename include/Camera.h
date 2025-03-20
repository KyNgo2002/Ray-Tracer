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
public:
	// Camera
	glm::vec3 camPosition;
	glm::vec3 camFront;
	glm::vec3 camUp;
	glm::vec3 camRight;
	glm::vec3 worldUp;

	// Matrices
	glm::mat4 lookAt;
	glm::mat4 inverseLookAt;
	glm::mat4 perspective;
	glm::mat4 inversePerspective;

	const float cameraSpeed = 0.009f;
	const float MOUSE_SENSITIVITY = 0.04f;
	float yaw;
	float pitch;
	int frames;
	bool moved;
	bool editing;

public:
	// Constructor
	Camera(float scrWidth, float scrHeight);
	void calculateLookAt();
	
	// Callbacks
	void processKeyboard(CameraMovement direction, float deltaTime);
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);
	void updateCameraVectors();

	// Modifiers
	void setMoved();
	void changeEditingMode();

};

