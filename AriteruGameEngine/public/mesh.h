#ifndef MESH_H
#define MESH_H

#include "shader.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;

	// bone indexes which will influence this vertex
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	// weights from each bone
	float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture
{
	unsigned int id;
	string       type;
	string       path;
};

class Mesh {
public:
	vector<Vertex>       vertices;
	vector<unsigned int> indices;
	vector<Texture>      textures;
	unsigned int         VAO;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);

	void UpdateInstanceInfo();

	void Draw(Shader& shader, const int num = 1);

private:
	unsigned int VBO, EBO;

	void SetUpMesh();
};

#endif