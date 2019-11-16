#include "pch.h"
#include "PhysicsManager.h"

using namespace Simplex;

void PhysicsObject::Init(void)
{
	WorldObject::Init();

	velocity = vector3(0);
	acceleration = vector3(0);
	mass = 1.0f;
}

void PhysicsObject::Release(void)
{
	WorldObject::Release();
}

PhysicsObject::PhysicsObject()
{
	Init();
}

PhysicsObject::~PhysicsObject()
{
	Release();
}

void PhysicsObject::Update(float deltaTime)
{
	ApplyForce(AXIS_Y * -9.8f);

	std::cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;

	if (position.y < 0) {
		ApplyForce(AXIS_Y * 100);
		SetPosition(vector3(position.x, 0, position.z));
	}

	velocity += acceleration * deltaTime;
	Translate(velocity * deltaTime);
	acceleration = vector3(0);
}

void PhysicsObject::OnCollision(WorldObject other)
{
	return;
}

void PhysicsObject::ApplyForce(vector3 force)
{
	acceleration += force / mass;
}
