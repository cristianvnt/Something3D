#include "Camera.h"

Camera::Camera(glm::vec3 position /*= glm::vec3(0.0f, 0.0f, 0.0f)*/, glm::vec3 up /*= glm::vec3(0.0f, 1.0f, 0.0f)*/,
	float yaw /*= CameraDefaults::YAW*/, float pitch /*= CameraDefaults::PITCH*/)
	: Front(glm::vec3(0.f, 0.f, -1.f)), MovementSpeed(CameraDefaults::SPEED), 
	MouseSensitivity(CameraDefaults::SENSITIVITY), Zoom(CameraDefaults::FOV)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: Front(glm::vec3(0.f, 0.f, -1.f)), MovementSpeed(CameraDefaults::SPEED),
	MouseSensitivity(CameraDefaults::SENSITIVITY), Zoom(CameraDefaults::FOV)
{
	Position = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front =
	{
		glm::cos(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch)),
		glm::sin(glm::radians(Pitch)),
		glm::sin(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch))
	};

	Front = glm::normalize(front);
	// Then also recalculate Right and Up
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}

glm::mat4 Camera::GetViewMatrix() const
{
	//return LookAt(Position, Position + Front, Up);
	return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	const float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += Front * velocity;
	if (direction == BACKWARD)
		Position -= Front * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
}

void Camera::MouseCallback(float xOffset, float yOffset, GLboolean constrainPitch /*= true*/)
{
	xOffset *= MouseSensitivity;
	yOffset *= MouseSensitivity;

	Yaw += xOffset;
	Pitch += yOffset;

	if (constrainPitch)
	{
		if (Pitch > 89.f)
			Pitch = 89.f;
		if (Pitch < -89.f)
			Pitch = -89.f;
	}

	UpdateCameraVectors();
}

void Camera::ScrollCallback(float yOffset)
{
	Zoom -= yOffset;
	if (Zoom < 1.f)
		Zoom = 1.f;
	if (Zoom > 60.f)
		Zoom = 60.f;
}

glm::mat4 Camera::LookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up)
{
	glm::vec3 zAxis = glm::normalize(pos - target);
	glm::vec3 xAxis = glm::normalize(glm::cross(glm::normalize(up), zAxis));
	glm::vec3 yAxis = glm::cross(zAxis, xAxis);

	glm::mat4 rotation = glm::mat4(1.f);
	rotation[0][0] = xAxis.x;
	rotation[1][0] = xAxis.y;
	rotation[2][0] = xAxis.z;
	rotation[0][1] = yAxis.x;
	rotation[1][1] = yAxis.y;
	rotation[2][1] = yAxis.z;
	rotation[0][2] = zAxis.x;
	rotation[1][2] = zAxis.y;
	rotation[2][2] = zAxis.z;

	glm::mat4 translation = glm::mat4(1.f);
	translation[3][0] = -pos.x;
	translation[3][1] = -pos.y;
	translation[3][2] = -pos.z;

	return rotation * translation;
}
