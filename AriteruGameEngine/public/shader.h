#pragma once
#include <string>
#include <glad/glad.h>
using namespace std;

class Shader
{
public:
	Shader(const string vertexPath, const string fragmentPath);

	const unsigned int GetShaderProgramID() const { return shaderID; }
private:

	unsigned int shaderID;

	void CheckCompileErrors(GLuint shader, std::string type);
};