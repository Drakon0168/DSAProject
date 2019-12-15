#include "pch.h"
#include "Enemy.h"
#include "PhysicsManager.h"

using namespace Simplex;

Enemy::Enemy()
{
	Init();
}

Enemy::~Enemy()
{
	Release();
}

void Enemy::Init()
{
	// Call base functionality
	LivingEntity::Init();
	// Set base values
}

void Enemy::Release()
{
	LivingEntity::Release();
}

void Enemy::Update(float deltaTime)
{
	// Calculate the seeking direction
	vector3 seekDirection = Seek(PhysicsManager::GetInstance()->GetPlayer()->GetPosition());
	// Move in the seek direction
	Move(seekDirection);
	// Call the base implementation
	PhysicsObject::Update(deltaTime);
}

vector3 Enemy::Seek(vector3 targetPosition)
{
	// Calculate the desired velocity
	vector3 desiredVelocity = targetPosition - position;
	// Normalize the desired velocity
	glm::normalize(desiredVelocity);
	// Return the desired velocity
	return desiredVelocity;
}

void Enemy::Die()
{
	//TODO: Reset the game / take the player back to the main menu
}

void Enemy::Attack()
{
	//TODO: Shoot the currently equipped gun
}


