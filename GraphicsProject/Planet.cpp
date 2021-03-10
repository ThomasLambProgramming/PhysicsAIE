#include "Planet.h"
#include "Gizmos.h"
void Planet::Draw()
{
	aie::Gizmos::addSphere(worldPosition, Size, 20, 20, Color);
}
void Planet::Update(float deltaTime)
{
	glm::mat2 rotationMatrix = glm::mat2(glm::cos(rotation),-glm::sin(rotation), glm::sin(rotation),glm::cos(rotation));
	glm::vec2 normal = glm::normalize(glm::vec2(worldPosition.x, worldPosition.z));
	normal = normal * rotationMatrix;
	glm::vec2 pos = normal * distanceFromSun;
	worldPosition = glm::vec3(pos.x, 0, pos.y);



	
	rotation = spinSpeed * deltaTime;
	if (rotation >= 360.0f || rotation <= -360.0f)
	{
		//rotation = 0.0f;
	}
	
}

