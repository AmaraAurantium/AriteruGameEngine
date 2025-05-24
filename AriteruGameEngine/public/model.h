#ifndef MODEL_H
#define MODEL_H

#include "mesh.h"
#include "shader.h"

#include <assimp/scene.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <map>

#include <string>
#include <vector>

#include <stb/stb_image.h>

using namespace std;

unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false, unsigned int wrap_s = GL_REPEAT, unsigned int wrap_t = GL_REPEAT);

class Model
{
public:
	vector<Texture> textures_loaded;
	vector<Mesh> meshes;
	string directory;
	bool gammaCorrection;

	Model(const string& path, bool gamma = false);

	void Draw(Shader& shader, const int num = 1);

	void UpdateInstanceInfo(const glm::mat4* modelMatrices, const int num);

private:
	void LoadModel(const string& path);

	void ProcessNode(aiNode* node, const aiScene* scene);

	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};
#endif