#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "Logger.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class Shader
{
private:
	// Program id
	unsigned int programID;

public:
	Shader(const char* vertexPath, const char* fragmentPath);

	// Use/activate the shader
	void Use() const;

	const unsigned int GetProgramID() const;

	// Utility uniform func
	void SetUniformB(const std::string& name, bool value) const;
	void SetUniformI(const std::string& name, int value) const;
	void SetUniformF(const std::string& name, float value) const;
	void SetUniform4f(const std::string& name, float x, float y, float z, float w) const;
	void SetUniform4f(const std::string& name, const glm::vec4& value) const;
	void SetUniformVec2(const std::string& name, float x, float y) const;
	void SetUniformVec2(const std::string& name, const glm::vec2& value) const;
	void SetUniformVec3(const std::string& name, float x, float y, float z) const;
	void SetUniformVec3(const std::string& name, const glm::vec3& value) const;
	void SetUniformVec4(const std::string& name, float x, float y, float z, float w) const;
	void SetUniformVec4(const std::string& name, const glm::vec4& value) const;
	void SetUniformMat2(const std::string& name, const glm::mat2& mat) const;
	void SetUniformMat3(const std::string& name, const glm::mat3& mat) const;
	void SetUniformMat4(const std::string& name, const glm::mat4& mat) const;
};

#endif // SHADER_H