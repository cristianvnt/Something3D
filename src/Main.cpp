#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Logger.h"

constexpr int SCREEN_WIDTH = 1500;
constexpr int SCREEN_HEIGHT = 900;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);

// Vertex shader source
const char* vertexShaderSrc = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// Fragment shader source
const char* fragmentShaderSrc =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.5f, 0.1f, 0.7f, 1.f);\n"
"}\0";

// Fragment shader source
const char* fragmentShaderSrc2 =
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"	FragColor = vec4(0.5f, 0.6f, 0.1f, 1.f);\n"
"}\0";


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

	/// Vertex and fragment shader source code
	/// This shader processes vertex data for rendering

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Attach shader source code to the actual shader object and compile
	glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
	glCompileShader(vertexShader);

	glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
	glCompileShader(vertexShader);

	// Check if shader compilation was successful
	GL::LOG::LogShaderCompilation(vertexShader, ShaderType::Vertex);
	GL::LOG::LogShaderCompilation(vertexShader, ShaderType::Vertex);

	unsigned int fragmentShader[2];
	fragmentShader[0] = glCreateShader(GL_FRAGMENT_SHADER);
	fragmentShader[1] = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader[0], 1, &fragmentShaderSrc, nullptr);
	glCompileShader(fragmentShader[0]);

	glShaderSource(fragmentShader[1], 1, &fragmentShaderSrc2, nullptr);
	glCompileShader(fragmentShader[1]);
	GL::LOG::LogShaderCompilation(fragmentShader[1], ShaderType::Fragment);
	GL::LOG::LogShaderCompilation(fragmentShader[1], ShaderType::Fragment);

	// Link shaders
	unsigned int shaderProgram[2];
	shaderProgram[0] = glCreateProgram();
	shaderProgram[1] = glCreateProgram();

	glAttachShader(shaderProgram[0], vertexShader);
	glAttachShader(shaderProgram[0], fragmentShader[0]);
	glLinkProgram(shaderProgram[0]);

	glAttachShader(shaderProgram[1], vertexShader);
	glAttachShader(shaderProgram[1], fragmentShader[1]);
	glLinkProgram(shaderProgram[1]);
	GL::LOG::LogShaderProgramLinking(shaderProgram[0]);
	GL::LOG::LogShaderProgramLinking(shaderProgram[1]);

	// After linking the shaders we no longer need them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader[0]);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader[1]);

	float vertices[] = {
		0.75f, 0.75f, 0.f,
		0.75f, 0.5f, 0.f,
		0.5f, 0.5f, 0.f,
		0.5f, 0.5f, 0.f
	};

	float vertices2[] = {
		-0.5f, -0.5f, 0.f,
		-0.5, -0.75f, 0.f,
		-0.75f, -0.25f, 0.f,
		-0.75f, -0.25f, 0.f
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle(top right)
		1, 2, 3  // second triangle(bottom left)
	};

	// Vertex buffer obj and vertex array obj and element buffer obj
	// Store vertex data in memory on GPU
	unsigned int VBOs[2], VAOs[2], EBO;

	glGenVertexArrays(2, VAOs);
	glGenBuffers(2, VBOs);
	glGenBuffers(1, &EBO);
	GL_CHECK_ERROR();
	
	// First triangle
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	GL_CHECK_ERROR();

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	GL_CHECK_ERROR();

	glBindVertexArray(VAOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	GL_CHECK_ERROR();

	// Interpret vertex data (per attribute)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	GL_CHECK_ERROR();

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	GL_CHECK_ERROR();*/

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
		glUseProgram(shaderProgram[0]);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		glUseProgram(shaderProgram[1]);
		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Check events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate all resources once its over
	glDeleteBuffers(2, VBOs);
	glDeleteVertexArrays(2, VAOs);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram[0]);
	glDeleteProgram(shaderProgram[1]);

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