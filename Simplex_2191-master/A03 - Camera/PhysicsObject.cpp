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

bool PhysicsObject::GetGrounded()
{
	return grounded;
}

void PhysicsObject::SetGrounded(bool value)
{
	grounded = value;
}

void PhysicsObject::SetUsesGravity(bool value)
{
	usesGravity = value;
}

void PhysicsObject::Update(float deltaTime)
{
	//Only apply gravity when above the ground
	if (usesGravity && !grounded) {
		ApplyForce(gravity);
	}

	if (grounded) {
		//Stop downwards velocity and acceleration
		if (velocity.y < 0) {
			velocity.y = 0;
		}

		if (acceleration.y < 0) {
			acceleration.y = 0;
		}
	}

	velocity += acceleration * deltaTime;
	Translate(velocity * deltaTime);
	acceleration = vector3(0);
}

void PhysicsObject::OnCollision(WorldObject* other)
{
	WorldObject::OnCollision(other);

	switch (other->GetLayer()) {
		case CollisionLayers::Terrain:
			//Move on top of the terrain;
			SetPosition(vector3(position.x, other->GetGlobalMax().y + (position.y - globalMin.y), position.z));
			break;
		default:
			//Move half of the distance away from the collision
			break;
	}
}

void PhysicsObject::ApplyForce(vector3 force)
{
	acceleration += force / mass;
}
