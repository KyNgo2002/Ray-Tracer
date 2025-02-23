#pragma once
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

	float Yaw;
	float Pitch;

	Camera();
	void calculateLookAt();
	void processKeyboard(CameraMovement direction, float deltaTime);


};

