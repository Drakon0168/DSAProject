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

float PhysicsObject::GetMass()
{
	return mass;
}

void PhysicsObject::SetMass(float value)
{
	mass = value;
}

void PhysicsObject::Update(float deltaTime)
{
	//TODO: Change this to use collisions not just position
	//Only apply gravity when above the ground
	if (globalMin.y + (velocity.y * deltaTime) > 0) {
		ApplyForce(AXIS_Y * -9.8f);
	}
	else {
		if (velocity.y < 0) {
			velocity.y = 0;
		}
		
		if (acceleration.y < 0) {
			acceleration.y = 0;
		}

		//ApplyForce(AXIS_Y * 500);
		SetPosition(vector3(position.x, globalHalfWidth.y, position.z));
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
