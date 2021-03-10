#pragma once
#include "glm/glm.hpp"

class Planet
{
public:
	float rotation = 0.0f;
	float spinSpeed = 1.0f;
	float distanceFromSun = 5.0f;

	glm::vec2 m_localX;
	glm::vec2 m_localY;
	
	float Size = 2.0f;
	glm::vec3 worldPosition;
	glm::vec4 Color;
	
	Planet(float a_distance, float a_spinSpeed, float a_size, glm::vec4 a_color, glm::vec3 Pos)
	{
		distanceFromSun = a_distance;
		spinSpeed = a_spinSpeed;
		Size = a_size;
		Color = a_color;
		worldPosition = Pos;
	}

	
	void Draw();
	void Update(float deltaTime);
};

