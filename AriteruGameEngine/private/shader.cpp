#include "../public/Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const string vertexPath, const string fragmentPath)
{
	ifstream vShaderFile;
	ifstream fShaderFile;

	vShaderFile.open(vertexPath);
	fShaderFile.open(fragmentPath);

	std::stringstream vShadeStream, fShaderSteam;

	vShadeStream << vShaderFile.rdbuf();
	fShaderSteam << fShaderFile.rdbuf();

	// close opened files
	vShaderFile.close();
	fShaderFile.close();

	std::string vertexCode = vShadeStream.str();
	std::string fragmentCode = fShaderSteam.str();

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	CheckCompileErrors(vertex, "VERTEX");

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	CheckCompileErrors(fragment, "FRAGMENT");

	shaderID = glCreateProgram();
	glAttachShader(shaderID, vertex);
	glAttachShader(shaderID, fragment);

	glLinkProgram(shaderID);
	CheckCompileErrors(shaderID, "PROGRAM");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::CheckCompileErrors(GLuint shader, std::string type)
{
	GLint  success;
	GLchar infoLog[1024];
	if (type != "PROGRAM") {
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "ERROE::SHADER::COMPILE_ERROR of type " << type << std::endl
				<< infoLog << std::endl;
		}
	}
	else {
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cerr << "ERROE::SHADER::PROGRAM_LINK_ERROR of type " << type << std::endl
				<< infoLog << std::endl;
		}
	}
}

