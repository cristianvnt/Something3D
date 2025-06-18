#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <iostream>
#include "Logger.h"
#include "Shader.h"
#include "Camera.h"

using namespace GL::ERR;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void ProcessInput(GLFWwindow* window);
void LoadTextureJPG(Shader& shader, const char* name, unsigned int& texture, const std::string texName);
void LoadTexturePng(Shader& shader, const char* name, unsigned int& texture, const std::string texName);
void FPS(GLFWwindow* window);

// Settings
constexpr int SCREEN_WIDTH = 1000;
constexpr int SCREEN_HEIGHT = 800;

// Timing
float DeltaTime = 0.f;
float LastFrame = 0.f;

// Camera
Camera camera(glm::vec3(0.f, 0.f, 3.f));
constexpr float MAX_VIEW_DIST = 200.f;
constexpr float FAR_PLANE = -20.f;

float MaxVis = 0.1f;
float LastX = SCREEN_HEIGHT / 2.f;
float LastY = SCREEN_WIDTH / 2.f;
bool FirstMouse = true;


int main()
{
	// GLFW init and config
	if (!glfwInit())
	{
		std::cout << "Failed to init\n";
		return -1;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SMTH3D", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, MouseCallback);

	glfwSetScrollCallback(window, ScrollCallback);

	// VSync disabled
	glfwSwapInterval(0);

	// GLAD init
	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		std::cout << "Failed to init GLAD\n";
		return -1;
	}

	GL_CHECK(glEnable(GL_DEPTH_TEST));

	Shader shaderRect("resources/shaders/transform.vert", "resources/shaders/shaderRect.frag");

	float verticesCube[] = {
		// position			 // texture
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.f, 0.f, 0.f),
		glm::vec3(2.0f, 5.0f, -10.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -9.5f),
		glm::vec3(3.5f, 4.0f, -2.5f),
		glm::vec3(1.5f, 2.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	// Vertex buffer obj and vertex array obj
	// Store vertex data in memory on GPU
	unsigned int VBO, VAO;

	GL_CHECK(glGenVertexArrays(1, &VAO));
	GL_CHECK(glGenBuffers(1, &VBO));

	unsigned int texture1, texture2;
	// Rectangle
	GL_CHECK(glBindVertexArray(VAO));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCube), verticesCube, GL_STATIC_DRAW));

	GL_CHECK(glEnableVertexAttribArray(0));
	GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0));
	GL_CHECK(glEnableVertexAttribArray(1));
	GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))));

	LoadTextureJPG(shaderRect, "resources/textures/container.jpg", texture1, "texture1");
	LoadTexturePng(shaderRect, "resources/textures/awesomeface.png", texture2, "texture2");

	// View via wireframe mode
	GL_CHECK(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		DeltaTime = currentFrame - LastFrame;
		LastFrame = currentFrame;

		// FPS
		FPS(window);

		// Input
		ProcessInput(window);

		// Rendering
		glClearColor(0.2f, 0.1f, 0.5f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw
		// bind textures on corresponding texture units
		GL_CHECK(glActiveTexture(GL_TEXTURE0));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture1));
		GL_CHECK(glActiveTexture(GL_TEXTURE1));
		GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture2));

		// Render cubes
		shaderRect.Use();
		shaderRect.SetUniformF("visible", MaxVis);

		// Camera
		// Projection matrix
		glm::mat4 projection = glm::mat4(1.f);
		projection = glm::perspective(glm::radians(CameraDefaults::FOV), static_cast<float>(SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, MAX_VIEW_DIST);
		shaderRect.SetUniformMat4fv("projection", projection);

		glm::mat4 view = camera.GetViewMatrix();
		shaderRect.SetUniformMat4fv("view", view);

		GL_CHECK(glBindVertexArray(VAO));
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 25.f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.f, 1.f, 0.5f));
			shaderRect.SetUniformMat4fv("model", model);

			GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 36));
		}
		GL_CHECK(glBindVertexArray(0));

		// Check events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate all resources once its over
	GL_CHECK(glDeleteVertexArrays(1, &VAO));
	GL_CHECK(glDeleteBuffers(1, &VBO));
	GL_CHECK(glBindVertexArray(0));

	glfwTerminate();
	return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	GL_CHECK(glViewport(0, 0, width, height));
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		MaxVis += 0.0005f;
		if (MaxVis >= 1.f)
			MaxVis = 1.f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		MaxVis -= 0.0005f;
		if (MaxVis <= 0.f)
			MaxVis = 0.f;
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, DeltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, DeltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, DeltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, DeltaTime);
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ScrollCallback(static_cast<float>(yOffset));
}

void MouseCallback(GLFWwindow* window, double xPosition, double yPosition)
{
	float xPos = static_cast<float>(xPosition);
	float yPos = static_cast<float>(yPosition);

	if (FirstMouse)
	{
		LastX = xPos;
		LastY = yPos;
		FirstMouse = false;
		return;
	}

	float xOffset = xPos - LastX;
	float yOffset = LastY - yPos; // reversed since y-coords range from bottom to top
	LastX = xPos;
	LastY = yPos;

	camera.MouseCallback(xOffset, yOffset);
}

void LoadTextureJPG(Shader& shader, const char* fName, unsigned int& texture, const std::string texName)
{
	GL_CHECK(glGenTextures(1, &texture));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture));

	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	int width, height, noChannels;
	unsigned char* data = stbi_load(fName, &width, &height, &noChannels, 0);
	if (!data)
	{
		std::cout << "Texture load failed\n";
		return;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	shader.Use();
	shader.SetUniformI(texName, 0);
}

void LoadTexturePng(Shader& shader, const char* fName, unsigned int& texture, const std::string texName)
{
	GL_CHECK(glGenTextures(1, &texture));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture));

	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	int width, height, noChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(fName, &width, &height, &noChannels, 0);
	if (!data)
	{
		std::cout << "Texture load failed\n";
		return;
	}

	GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data));
	GL_CHECK(glGenerateMipmap(GL_TEXTURE_2D));

	stbi_image_free(data);

	shader.Use();
	shader.SetUniformI(texName, 1);
}

void FPS(GLFWwindow* window)
{
	static float timerSec = 0.f;
	static int fpsCount = 0;

	timerSec += DeltaTime;
	fpsCount++;

	if (timerSec >= 0.1f)
	{
		int avgFPS = (int)(fpsCount / timerSec);
		std::string title = "SMTH3D - FPS: " + std::to_string(avgFPS);
		glfwSetWindowTitle(window, title.c_str());

		timerSec = 0.f;
		fpsCount = 0;
	}
}