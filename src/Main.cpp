#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Logger.h"
#include "Shader.h"
#include <stb_image.h>

constexpr int SCREEN_WIDTH = 1000;
constexpr int SCREEN_HEIGHT = 800;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
void ChangeColor(Shader& shader);
void LoadTextureJPG(Shader& shader, const char* name, unsigned int& texture, const std::string texName);
void LoadTexturePng(Shader& shader, const char* name, unsigned int& texture, const std::string texName);

float maxVis = 0.1f;

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

	Shader shader("resources/shaders/shader.vert", "resources/shaders/shader.frag");
	Shader shaderTwo("resources/shaders/shader.vert", "resources/shaders/shaderTwo.frag");
	Shader shaderRect("resources/shaders/shader.vert", "resources/shaders/shaderRect.frag");

	float vertices[] = 
	{
		// position		   color
		0.75f, 0.75f, 0.f, 1.f, 0.f, 0.f,
		0.75f, 0.25f, 0.f, 0.f, 1.f, 0.f,
		0.25f, 0.25f, 0.f, 0.f, 0.f, 1.f
	};

	float vertices2[] =
	{
		-0.25f, -0.25f, 0.f,
		-0.5, -0.75f, 0.f,
		-0.75f, -0.15f, 0.f
	};

	float verticesRect[] =
	{
		// position		   color		  texure
		0.75f, -0.25f, 0.f, 1.f, 0.f, 0.f, 1.f, 1.f,
		0.75f, -0.65f, 0.f, 0.f, 1.f, 0.f, 1.f, 0.f,
		0.25f, -0.65f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f,
		0.25f, -0.25f, 0.f, 0.f, 1.f, 0.f, 0.f, 1.f
	};

	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	// Vertex buffer obj and vertex array obj
	// Store vertex data in memory on GPU
	unsigned int VBOs[3], VAOs[3], EBO;

	glGenVertexArrays(3, VAOs);
	glGenBuffers(3, VBOs);
	glGenBuffers(1, &EBO);
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
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(6 * sizeof(float)));
	GL_CHECK_ERROR();

	// Second triangle
	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	GL_CHECK_ERROR();

	// Interpret vertex data (per attribute)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	GL_CHECK_ERROR();

	unsigned int texture1, texture2;
	// Rectangle
	glBindVertexArray(VAOs[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRect), verticesRect, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	LoadTextureJPG(shaderRect, "resources/textures/container.jpg", texture1, "texture1");
	LoadTexturePng(shaderRect, "resources/textures/awesomeface.png", texture2, "texture2");

	// View via wireframe mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Input
		ProcessInput(window);

		// Rendering
		glClearColor(0.2f, 0.1f, 0.5f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw
		// Triangle 1
		shader.Use();
		float someTime = glfwGetTime();
		float posChange = (sin(someTime) / 2.f) - 0.4;
		shader.SetUniformF("offset", posChange);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Triangle 2
		ChangeColor(shaderTwo);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Rectangle
		// bind textures on corresponding texture units
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Render container
		shaderRect.Use();
		shaderRect.SetUniformF("visible", maxVis);
		glBindVertexArray(VAOs[2]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Check events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate all resources once its over
	glDeleteBuffers(3, VBOs);
	glDeleteVertexArrays(3, VAOs);
	glDeleteBuffers(1, &EBO);

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
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		maxVis += 0.01f;
		if (maxVis >= 1.f)
			maxVis = 1.f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		maxVis -= 0.01f;
		if (maxVis <= 0.f)
			maxVis = 0.f;
	}
}

void ChangeColor(Shader& shader)
{
	float timeValue = glfwGetTime();
	float colorValue = (sin(timeValue) / 2.f) + 0.6f;
	shader.Use();
	shader.SetUniform4f("ourColor", 0.f, colorValue, 0.f, 1.f);
}

void LoadTextureJPG(Shader& shader, const char* fName, unsigned int& texture, const std::string texName)
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int width, height, noChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(fName, &width, &height, &noChannels, 0);
	if (!data)
	{
		std::cout << "Texture load failed\n";
		return;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	shader.Use();
	shader.SetUniformI(texName, 1);
}
