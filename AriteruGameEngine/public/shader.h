#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
using namespace std;

class Shader
{
public:
	unsigned int ID; //program ID
	Shader(const string vertexPath, const string fragmentPath);

	const unsigned int GetShaderProgramID() const { return shaderID; }

	void use() { glUseProgram(ID); }
	void setMat4(const std::string& name, const glm::mat4& mat) const {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
private:

	unsigned int shaderID;

	void CheckCompileErrors(GLuint shader, std::string type);
};