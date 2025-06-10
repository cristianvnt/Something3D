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
"	FragColor = vec4(1.f, 0.3f, 0.2f, 1.f);\n"
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

	// Init Triangle
	float vertices[] = {
	-0.5f, -0.5f, 0.f,
	0.5f, -0.5f, 0.f,
	0.f, 0.5f, 0.f
	};

	/// Vertex and fragment shader source code
	/// This shader processes vertex data for rendering

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Attach shader source code to the actual shader object and compile
	glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr);
	glCompileShader(vertexShader);

	// Check if shader compilation was successful
	Logger::LogShaderCompilation(vertexShader, ShaderType::Vertex);

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
	glCompileShader(fragmentShader);
	Logger::LogShaderCompilation(fragmentShader, ShaderType::Fragment);

	// Link shaders
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	Logger::LogShaderProgramLinking(shaderProgram);

	// Activate the program object
	glUseProgram(shaderProgram);

	// After linking the shaders we no longer need them
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Vertex buffer object and vertex array object
	// Store vertex data in memory on GPU
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Interpret vertex data (per attribute)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Unbind VAO since it was already bound
	glBindVertexArray(0);


	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		// Input
		ProcessInput(window);

		// Rendering
		glClearColor(0.2f, 0.1f, 0.6f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Check events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// De-allocate all resources once its over
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

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