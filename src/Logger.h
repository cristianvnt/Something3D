#ifndef LOGGER_H
#define LOGGER_H

#include <glad/glad.h>
#include <iostream>

enum class ShaderType
{
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER,
	Program
};

namespace GL
{
	namespace LOG
	{
		void LogShaderCompilation(unsigned int shader, ShaderType type);
		void LogShaderProgramLinking(unsigned int shaderProgram);

		void LogShader(unsigned int shader, ShaderType type);
	}
	
	namespace ERR
	{
		GLenum glCheckError_(const char* file, int line);
		
		constexpr const char* GetGLErrorString(GLenum err);
	}
};

#define GL_CHECK_ERROR() GL::ERR::glCheckError_(__FILE__, __LINE__);

#endif
