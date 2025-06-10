#include "Logger.h"

void Logger::LogShaderCompilation(unsigned int shader, ShaderType type)
{
	int success;
	char infoLog[512];

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);

		const char* typeStr = "UNKNOWN";
		switch (type)
		{
		case ShaderType::Vertex:
			typeStr = "VERTEX";
			break;
		case ShaderType::Fragment:
			typeStr = "FRAGMENT";
			break;
		}

		std::cout << "ERROR::SHADER::" << typeStr << "::COMPILATION_FAILED\n" << infoLog << "\n";
	}
}

void Logger::LogShaderProgramLinking(unsigned int shaderProgram)
{
	int success;
	char infoLog[512];

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
	}
}
