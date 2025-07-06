#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum ELightType
{
	LT_POINT,
	LT_DIRECTION
};

class Light
{
private:
	glm::vec3 Position;
	glm::vec3 WorldUp;
	glm::vec3 Direction;
	glm::vec3 Color;

	float zNear;
	float zFar;

	ELightType Type;

	int SHADOW_WIDTH;
	int SHADOW_HEIGHT;

public:
	Light(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 direction = glm::vec3(-1.2f, -1.0f, -2.0f),
		float near = 1.0f,
		float far = 3000.0f,
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f),
		ELightType type = ELightType::LT_POINT,
		int width = 2048,
		int height = 2048)
		: Position(position), Color(color), Direction(direction), zNear(near),
		zFar(far), Type(type), WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
		SHADOW_WIDTH(width), SHADOW_HEIGHT(height) {
	}

	const glm::mat4 GetLightSpaceMatrix() const
	{
		glm::mat4 projection = glm::mat4(1.0f);
		if (Type == ELightType::LT_DIRECTION)
			projection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, zNear, zFar);
		else if (Type == ELightType::LT_POINT) // TODO
			projection = glm::perspective(glm::radians(45.0f), 1.0f, zNear, zFar);

		glm::mat4 view = glm::lookAt(Position, glm::vec3(0.0f), WorldUp);
		return projection * view;
	}

	const glm::vec3 GetLightPos() const
	{
		return Position;
	}

	const glm::vec3 GetLightDirection() const
	{
		return Direction;
	}

	const float GetFarPlane() const
	{
		return zFar;
	}

	const float GetNearPlane() const
	{
		return zNear;
	}

	bool CreateShadowBuffer(unsigned int& frameBuffer, unsigned int& renderTexture);

	glm::vec3 GetLightColor() const { return Color; }

	void SetLightColor(glm::vec3& color) { Color = color; }
	void SetLightPosition(glm::vec3& position) { Position = position; }
};

#endif