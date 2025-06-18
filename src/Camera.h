#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

namespace CameraDefaults
{
	constexpr float FOV = 60.f;
	constexpr float YAW = -90.f;
	constexpr float PITCH = 0.f;
	constexpr float SPEED = 2.5f;
	constexpr float SENSITIVITY = 0.01f;
}

class Camera
{
private:
	void UpdateCameraVectors();

public:
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = CameraDefaults::YAW, float pitch = CameraDefaults::PITCH);
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

	// Euler angles
	float Yaw;
	float Pitch;

	// Camera attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	glm::mat4 GetViewMatrix() const;
	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	void MouseCallback(float xOffset, float yOffset, GLboolean constrainPitch = true);
	void ScrollCallback(float yOffset);
	glm::mat4 LookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up);

};

#endif
