#pragma once
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

	glm::mat4 lookAt;

	const float cameraSpeed = 0.01f;

	float yaw;
	float pitch;

	Camera(float scrWidth, float scrHeight);
	void calculateLookAt();
	void processKeyboard(CameraMovement direction, float deltaTime);

};

