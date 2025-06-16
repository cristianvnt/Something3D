#include "Logger.h"

void GL::LOG::LogShaderCompilation(unsigned int shader, ShaderType type)
{
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success)
		return;

	char infoLog[512];
	GL_CHECK(glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog));

	const char* typeStr = [&]() 
		{
			switch (type)
			{
			case ShaderType::Vertex:
				return "VERTEX";
				break;
			case ShaderType::Fragment:
				return "FRAGMENT";
				break;
			default:
				return "UNKNOWN";
			}
		}();

	std::cout << "ERROR::SHADER::" << typeStr << "::COMPILATION_FAILED\n" << infoLog << "\n";
}

void GL::LOG::LogShaderProgramLinking(unsigned int shaderProgram)
{
	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success)
		return;

	char infoLog[512];
	GL_CHECK(glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog));

	std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
}

void GL::LOG::LogShader(unsigned int shader, ShaderType type)
{
	if (type == ShaderType::Program)
		LogShaderProgramLinking(shader);
	else
		LogShaderCompilation(shader, type);
}

void GL::ERR::glClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GL::ERR::glCheckError_(const char* func, const char* file, int line)
{
	while (GLenum err = glGetError())
	{
		std::cout << "[OpenGL ] " << GetGLErrorString(err) << " | " << func << "\n | " << file << " (" << line << ")\n";
		return false;
	}
	return true;
}

constexpr const char* GL::ERR::GetGLErrorString(GLenum err)
{
	switch (err)
	{
	case GL_INVALID_ENUM:
		return "INVALID_ENUM";
	case GL_INVALID_VALUE:
		return "INVALID_VALUE";
	case GL_INVALID_OPERATION:
		return "INVALID_OPERATION";
	case GL_STACK_UNDERFLOW:
		return "STACK_UNDERFLOW";
	case GL_STACK_OVERFLOW:
		return "STACK_OVERFLOW";
	case GL_OUT_OF_MEMORY:
		return "OUT_OF_MEMORY";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "INVALID_FRAMEBUFFER_OPERATION";
	default:
		return "UNKNOWN_GL_ERROR";
	}
}
