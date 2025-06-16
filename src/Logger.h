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
		void glClearError();
		bool glCheckError_(const char* func, const char* file, int line);
		constexpr const char* GetGLErrorString(GLenum err);
	}
};

#define ASSERT(x) \
	do { \
		if (!(x)) \
		__debugbreak(); \
	} while (false)

#define GL_CHECK(x) \
	do { \
		GL::ERR::glClearError(); \
		x; \
		ASSERT(GL::ERR::glCheckError_(#x, __FILE__, __LINE__)); \
	} while (false)

#endif
