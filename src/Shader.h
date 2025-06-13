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
	void SetBool(const std::string name, bool value) const;
	void SetInt(const std::string name, int value) const;
	void SetFloat(const std::string name, float value) const;
	void Set4f(const std::string name, float x, float y, float z, float w) const;
};

#endif // SHADER_H