#ifndef LOGGER_H
#define LOGGER_H

#include <glad/glad.h>

#include <iostream>

enum class ShaderType
{
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER
};

namespace Logger
{
	void LogShaderCompilation(unsigned int shader, ShaderType type);
	void LogShaderProgramLinking(unsigned int shaderProgram);
};

#endif
