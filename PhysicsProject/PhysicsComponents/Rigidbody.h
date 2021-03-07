#pragma once
#include "PhysicsObject.h"
#include <functional>
#include <list>
#include <iostream>
#include "Texture.h"

class Rigidbody : public PhysicsObject
{
public:
	Rigidbody(ShapeType a_shapeID, glm::vec2 a_position, glm::vec2 a_velocity, float a_mass, float a_rotation);
	~Rigidbody() {};

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	virtual void Debug() {};
	void ApplyForce(glm::vec2 a_force, glm::vec2 a_pos);

	void ResolveCollision(Rigidbody* a_otherActor, glm::vec2 a_contact, glm::vec2* a_collisionNormal = nullptr, float a_pen = 0);

	glm::vec2 GetPosition() const { return m_position; }
	glm::vec2 GetVelocity() { return m_velocity; }
	void SetVelocity(glm::vec2 a_vel) { m_velocity = a_vel; }
	float GetMass() { return m_isKinematic ? INT_MAX : m_mass; }
	float GetRotation() { return m_rotation; }

	float GetMoment() { return m_isKinematic ? INT_MAX : m_moment; }
	float GetAngularVelocity() { return m_angularVelocity; }
	void SetAngularVel(float a_Vel) { m_angularVelocity = a_Vel; }
	float SetRotation(float a_rotate) { return m_rotation = a_rotate; }
	glm::vec2 SetPosition(glm::vec2 a_position) { return m_position = a_position; }
	float GetLinearDrag() { return m_linearDrag; }
	float GetAngularDrag() { return m_angularDrag; }

	glm::vec2 ToWorld(glm::vec2 a_localPos);

	void TriggerEnter(PhysicsObject* a_otherActor);

	std::function<void(PhysicsObject*)> m_collisionCallback;
	std::function<void(PhysicsObject*)> triggerEnter;
	std::function<void(PhysicsObject*)> triggerExit;

	bool IsTrigger() { return m_isTrigger; }
	bool SetTrigger(bool a_state) { return m_isTrigger = a_state; }


protected:
	aie::Texture* m_texture;

	std::list<PhysicsObject*> m_objectInside;
	std::list<PhysicsObject*> m_objectInsideThisFrame;

	bool m_isTrigger;

	glm::vec2 m_position;
	glm::vec2 m_velocity;
	
	float m_mass;
	//this is in radians!
	float m_rotation;
	float m_angularVelocity;
	float m_moment;

	float m_linearDrag = 0.005f;
	float m_angularDrag;

	// These will store the local x and y axes of the box based on it's angle of rotation
	glm::vec2 m_localX;
	glm::vec2 m_localY;
};
