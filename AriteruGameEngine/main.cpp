#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
 
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
Light sun(lightPosition, glm::vec3(0), 10.0f, 100.0f, lightColor, ELightType::LT_DIRECTION, SHADOW_WIDTH, SHADOW_HEIGHT);

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

	Shader shader("D:/Workspace/AriteruGameEngine/shaders/vertex.vs", "D:/Workspace/AriteruGameEngine/shaders/frag.fs");

	Model model("D:/Workspace/AriteruGameEngine/models/ara/ara.fbx");

#ifdef SHADOW_MAPPING_ENABLE
	Shader shader("D:/Workspace/AriteruGameEngine/shaders/shadow_mapping.fs");
#else
	Shader shader("D:/Workspace/AriteruGameEngine/shaders/model_loading.fs");
#endif

	Shader planeShader("D:/Workspace/AriteruGameEngine/shaders/plane.vs");
	Shader skyBoxShader("D:/Workspace/AriteruGameEngine/shaders/skybox.fs");

 

	glEnable(GL_DEPTH_TEST);
	// Set matrices
	shader.use();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 500.0f);
	glm::mat4 view = camera.GetViewMatrix();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);

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
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

		glUseProgram(shader.GetShaderProgramID());

		glm::mat4 view = camera.GetViewMatrix();
		unsigned int viewLoc = glGetUniformLocation(shader.GetShaderProgramID(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();  // Activate shader


		// Render model
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 25.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(100.0f));


		shader.setMat4("model", modelMatrix);

		model.Draw(shader);

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
