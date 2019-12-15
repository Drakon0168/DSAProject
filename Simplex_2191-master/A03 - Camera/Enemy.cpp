#include "pch.h"
#include "Enemy.h"
#include "PhysicsManager.h"
using namespace Simplex;

void Simplex::Enemy::Update(float deltaTime)
{
	// Calculate the seeking direction
	vector3 seekDirection = Seek(PhysicsManager::GetInstance()->GetPlayer()->GetPosition());
	// Move in the seek direction
	Move(seekDirection);
	// Call the base implementation
	Simplex::PhysicsObject::Update(deltaTime);
}

vector3 Simplex::Enemy::Seek(vector3 targetPosition)
{
	// Calculate the desired velocity
	vector3 desiredVelocity = targetPosition - position;
	// Normalize the desired velocity
	glm::normalize(desiredVelocity);
	// Return the desired velocity
	return desiredVelocity;

}


