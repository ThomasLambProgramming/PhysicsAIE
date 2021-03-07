#include "Spring.h"

Spring::Spring(Rigidbody* a_actor1, Rigidbody* a_actor2, float a_damping, float a_springCoefficient, float a_restLength /*= 0.0f*/, 
	glm::vec2 a_contact1 /*= glm::vec2(0)*/, glm::vec2 a_contact2 /*= glm::vec2(0)*/) :
	//this is just for the init holy crap
	PhysicsObject(JOINT), m_actor1(a_actor1), m_actor2(a_actor2), m_damping(a_damping), m_springCoefficient(a_springCoefficient), m_restLength(a_restLength), m_contact1(a_contact1), m_contact2(a_contact2)
{
	if (a_restLength == 0)
		m_restLength = glm::distance(GetContact1(), GetContact2());
}

void Spring::FixedUpdate(glm::vec2 a_gravity, float a_timeStep)
{
	//get the world coordinates of the start and end points of the spring
	glm::vec2 p1 = GetContact1();
	glm::vec2 p2 = GetContact2();

	float length = glm::distance(p1, p2);
	glm::vec2 direction = glm::normalize(p2 - p1);

	//applying damping
	glm::vec2 relativeVelocity = m_actor2->GetVelocity() - m_actor1->GetVelocity();

	//f = -k*x - b*v
	glm::vec2 force = direction * m_springCoefficient * (m_restLength - length) - m_damping * relativeVelocity;

	//we cap the spring force to a max size to prevent numerical instability
	//we dont want it to be too large or it will be unstable (think capes going apeshit in games because there is no limit)
	const float threshold = 1000.0f;
	float forceMag = glm::length(force);
	if (forceMag > threshold)
	{
		force *= threshold / forceMag;
	}

	m_actor1->ApplyForce(-force * a_timeStep, p1 - m_actor1->GetPosition());
	m_actor2->ApplyForce(force * a_timeStep, p2 - m_actor2->GetPosition());
}

void Spring::MakeGizmo()
{
	aie::Gizmos::add2DLine(GetContact1(), GetContact2(), glm::vec4(1,0,1,1));
}
