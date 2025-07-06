#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h>

#include <vector>
#include <string>

using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false, unsigned int wrap_s = GL_REPEAT, unsigned int wrap_t = GL_REPEAT);

unsigned int LoadCubemap(vector<std::string> faces);

#endif