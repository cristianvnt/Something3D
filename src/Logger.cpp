#include "Logger.h"

void GL::LOG::LogShaderCompilation(unsigned int shader, ShaderType type)
{
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success)
		return;

	char infoLog[512];
	glGetShaderInfoLog(shader, sizeof(infoLog), nullptr, infoLog);

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

	std::cerr << "ERROR::SHADER::" << typeStr << "::COMPILATION_FAILED\n" << infoLog << "\n";
}

void GL::LOG::LogShaderProgramLinking(unsigned int shaderProgram)
{
	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (success)
		return;

	char infoLog[512];
	glGetProgramInfoLog(shaderProgram, sizeof(infoLog), nullptr, infoLog); 

	std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
}

void GL::LOG::LogShader(unsigned int shader, ShaderType type)
{
	if (type == ShaderType::Program)
		LogShaderProgramLinking(shader);
	else
		LogShaderCompilation(shader, type);
}

GLenum GL::ERR::glCheckError_(const char* file, int line)
{
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
		std::cerr << "[GL] " << GetGLErrorString(err) << "\n | " << file << " (" << line << ")\n";
	return err;
}

constexpr const char* GL::ERR::GetGLErrorString(GLenum err)
{
	switch (err)
	{
	case GL_INVALID_ENUM:
		return "INVALID_ENUM >> Enum parameter is not legal.";
	case GL_INVALID_VALUE:
		return "INVALID_VALUE >> Value is out of range.";
	case GL_INVALID_OPERATION:
		return "INVALID_OPERATION >> Operation is not allowed in this state.";
	case GL_STACK_UNDERFLOW:
		return "STACK_UNDERFLOW >> Attempt to pop from an empty stack.";
	case GL_STACK_OVERFLOW:
		return "STACK_OVERFLOW >> Attempt to push onto a full stack.";
	case GL_OUT_OF_MEMORY:
		return "OUT_OF_MEMORY >> Not enough memory to complete the operation.";
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		return "INVALID_FRAMEBUFFER_OPERATION >> Incomplete framebuffer.";
	default:
		return "UNKNOWN_GL_ERROR >> Unrecognized OpenGL error.";
	}
}
