#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include "Logger.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class Shader
{
public:
	// Program id
	unsigned int programID;

	Shader(const char* vertexPath, const char* fragmentPath);

	// Use/activate the shader
	void Use() const;

	// Utility uniform func
	void SetUniformB(const std::string name, bool value) const;
	void SetUniformI(const std::string name, int value) const;
	void SetUniformF(const std::string name, float value) const;
	void SetUniform4f(const std::string name, float x, float y, float z, float w) const;
};

#endif // SHADER_H