#include "Shader.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::fstream vShaderFile;
	std::fstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		
		// Read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure& err)
	{
		std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << err.what() << "\n";
	}

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// Vertex and fragment shader source code
	// This shader processes vertex data for rendering
	unsigned int vertex;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	// Attach shader source code to the actual shader object and compile
	glShaderSource(vertex, 1, &vShaderCode, nullptr);
	glCompileShader(vertex);
	// Check if shader compilation was successful
	GL::LOG::LogShaderCompilation(vertex, ShaderType::Vertex);

	unsigned int fragment;
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	GL::LOG::LogShaderCompilation(fragment, ShaderType::Fragment);

	// Link shaders
	_ID = glCreateProgram();
	glAttachShader(_ID, vertex);
	glAttachShader(_ID, fragment);
	glLinkProgram(_ID);
	GL::LOG::LogShaderProgramLinking(_ID);

	// After linking the shaders we no longer need them
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	glUseProgram(0);
	glDeleteProgram(_ID);
}

void Shader::Use() const
{
	glUseProgram(_ID);
}

const unsigned int Shader::GetProgramID() const
{
	return _ID;
}

void Shader::SetUniformB(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(_ID, name.c_str()), (int)value);
}

void Shader::SetUniformI(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(_ID, name.c_str()), value);
}

void Shader::SetUniformF(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(_ID, name.c_str()), value);
}

void Shader::SetUniform4f(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(_ID, name.c_str()), x, y, z, w);
}

void Shader::SetUniform4f(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(_ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetUniformVec2(const std::string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(_ID, name.c_str()), x, y);
}

void Shader::SetUniformVec2(const std::string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(_ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetUniformVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(_ID, name.c_str()), x, y, z);
}

void Shader::SetUniformVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(_ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetUniformVec4(const std::string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(_ID, name.c_str()), x, y, z, w);
}

void Shader::SetUniformVec4(const std::string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(_ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetUniformMat2fv(const std::string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetUniformMat3fv(const std::string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::SetUniformMat4fv(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

