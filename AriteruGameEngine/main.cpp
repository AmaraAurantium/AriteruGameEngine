#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "public/shader.h"
#include "public/camera.h"
#include "public/model.h"
#include "public/light.h"
#include "public/utils.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>



using namespace std;

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 900;

const float NEAR_PLANE = 1.0f;
const float FAR_PLANE = 500.0f;

int SHADOW_WIDTH = 4096;
int SHADOW_HEIGHT = 4096;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

bool saveScreen = false;
void save_screen(int width, int height);

Camera camera(glm::vec3(0.0f, 40.0f, 80.0f));

glm::vec3 lightColor(1.0f);
glm::vec3 lightPosition(20.0f, 20.0f, -55.0f);
Light sun(lightPosition, glm::vec3(0), 10.0f, 500.0f, lightColor, ELightType::LT_DIRECTION, SHADOW_WIDTH, SHADOW_HEIGHT);

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

bool bShowCameraInfo = false;
bool DebugPanelEnabled = false;
bool ShiftPressed = false;

#define SHADOW_MAPPING_ENABLE
#define SKYBOX_ENABLE

void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Game Engine", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Shader shader("D:/Workspace/AriteruGameEngine/shaders/vertex.vs", "D:/Workspace/AriteruGameEngine/shaders/frag.fs");

	Model customModel("D:/Workspace/AriteruGameEngine/models/ara/ara.fbx");

	Shader skyBoxShader("D:/Workspace/AriteruGameEngine/shaders/skybox.vs", "D:/Workspace/AriteruGameEngine/shaders/skybox.fs");
	Shader planeShader("D:/Workspace/AriteruGameEngine/shaders/plane.vs", "D:/Workspace/AriteruGameEngine/shaders/plane.fs");
	Shader shader("D:/Workspace/AriteruGameEngine/shaders/shadow_mapping.vs","D:/Workspace/AriteruGameEngine/shaders/shadow_mapping.fs");

	glEnable(GL_DEPTH_TEST);
	// Set matrices
	shader.use();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);
	glm::mat4 view = camera.GetViewMatrix();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);

	const float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	vector<std::string> faces
	{
		"D:/Workspace/AriteruGameEngine/textures/right.PNG",
		"D:/Workspace/AriteruGameEngine/textures/left.PNG",
		"D:/Workspace/AriteruGameEngine/textures/top.PNG",
		"D:/Workspace/AriteruGameEngine/textures/bottom.PNG",
		"D:/Workspace/AriteruGameEngine/textures/front.PNG",
		"D:/Workspace/AriteruGameEngine/textures/back.PNG"
	};

	unsigned int skyBoxCubemapTexture = LoadCubemap(faces);

	skyBoxShader.use();
	skyBoxShader.setInt("skybox", 0);

	Shader shadowMapShader("D:/Workspace/AriteruGameEngine/shaders/shadow_mapping_depth.vs","D:/Workspace/AriteruGameEngine/shaders/shadow_mapping_depth.fs");

	unsigned int shadowMapFBO, shadowTexture;
	sun.CreateShadowBuffer(shadowMapFBO, shadowTexture);

	shadowMapShader.use();

	shader.use();
	shader.setInt("shadowMap", 3);

	//plane VAO & VBO declaration and instantiation
	unsigned int planeVAO = 0;
	unsigned int planeVBO;

	{
		float planeVertices[] = {
			// positions            // normal         // texCoords
			-40.0f, 25.0f, 50.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  //
			-40.0f, 25.0f, -40.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //
			40.0f, 25.0f, 50.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,   //
			40.0f, 25.0f, -40.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  //
			//40.0f, -25.0f, 40.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  //
			//-40.0f, -25.0f, 40.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 
			//-40.0f, -25.0f, -40.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, //

			//40.0f, -25.0f, 40.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   //
			//-40.0f, -25.0f, -40.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   //
			//40.0f, -25.0f, -40.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  //
		};

		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &planeVBO);
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindVertexArray(0);
	}

	glEnable(GL_DEPTH_TEST);

	// rendering loop
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		if (bShowCameraInfo)
		{
			camera.ShowInfo();
		}

		//render loop subpart 1

		// Render model
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 25.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(100.0f));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, shadowMapFBO);
		//glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glClear(GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, shadowTexture);

		shadowMapShader.use();
		shadowMapShader.setMat4("lightSpaceMatrix", sun.GetLightSpaceMatrix()); // light space view projection matrix

		shadowMapShader.setMat4("model", modelMatrix);
		customModel.Draw(shadowMapShader);

		shadowMapShader.setMat4("model", glm::mat4(1.0));
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);



		//render loop subpart 2
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glUseProgram(shader.GetShaderProgramID());

		glm::mat4 view = camera.GetViewMatrix();
		unsigned int viewLoc = glGetUniformLocation(shader.GetShaderProgramID(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();  // Activate shader
		shader.setMat4("model", modelMatrix);
		shader.setMat4("view", camera.GetViewMatrix());
		shader.setVec3("viewPos", camera.GetPosition());
		shader.setVec3("lightPos", sun.GetLightPos());
		shader.setVec3("lightColor", sun.GetLightColor());
		shader.setMat4("lightSpaceMatrix", sun.GetLightSpaceMatrix());
		shader.setInt("shadowMap", 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, shadowTexture);




		customModel.Draw(shader);

		glDepthFunc(GL_LEQUAL);
		skyBoxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skyBoxShader.setMat4("view", view);
		skyBoxShader.setMat4("projection", projection);
		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyBoxCubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // set depth function back to default

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime);

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		bShowCameraInfo = true;
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	static bool firstMouse = true;
	static float lastX, lastY;
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	// reversed since y-coordinates go from bottom to top
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
