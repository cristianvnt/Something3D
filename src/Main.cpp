#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Logger.h"
#include "Shader.h"

constexpr int SCREEN_WIDTH = 1000;
constexpr int SCREEN_HEIGHT = 800;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void ChangeColor(Shader shader);

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
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LALALALALA", nullptr, nullptr);
	if (!window)
	{
		std::cout << "Failed to create GLFW window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);

	// GLAD init
	if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
	{
		std::cout << "Failed to init GLAD\n";
		return -1;
	}

	Shader shader("shaders/shader.vert", "shaders/shader.frag");
	Shader shaderTwo("shaders/shader.vert", "shaders/shaderTwo.frag");

	float vertices[] = {
		// position		   color
		0.75f, 0.75f, 0.f, 1.f, 0.f, 0.f,
		0.75f, 0.25f, 0.f, 0.f, 0.f, 0.f,
		-0.25f, -0.25f, 0.f, 0.f, 0.f, 1.f
	};

	float vertices2[] = {
		-0.25f, -0.25f, 0.f,
		-0.5, -0.75f, 0.f,
		-0.75f, -0.15f, 0.f
	};

	// Vertex buffer obj and vertex array obj and element buffer obj
	// Store vertex data in memory on GPU
	unsigned int VBOs[2], VAOs[2];

	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	GL_CHECK_ERROR();
	
	// First triangle
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	GL_CHECK_ERROR();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	GL_CHECK_ERROR();

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	GL_CHECK_ERROR();

	// Interpret vertex data (per attribute)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	GL_CHECK_ERROR();

	// View via wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Input
		ProcessInput(window);

		// Rendering
		glClearColor(0.2f, 0.1f, 0.6f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw
		shader.Use();
		shader.SetFloat("offset", 0.2f);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		ChangeColor(shaderTwo);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Check events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate all resources once its over
	glDeleteBuffers(2, VBOs);
	glDeleteVertexArrays(2, VAOs);

	glfwTerminate();
	return 0;
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void ChangeColor(Shader shader)
{
	float timeValue = glfwGetTime();
	float colorValue = (sin(timeValue) / 2.f) + 0.6f;
	shader.Use();
	shader.Set4f("ourColor", 0.f, colorValue, 0.f, 1.f);
}
