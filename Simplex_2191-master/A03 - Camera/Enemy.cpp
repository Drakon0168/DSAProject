#include "pch.h"
#include "Enemy.h"
#include "PhysicsManager.h"
#include <iostream>
using namespace Simplex;
using namespace std;
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

int Enemy::GetDamage()
{
	return damage;
}

void Enemy::SetDamage(int value)
{
	damage = value;
}


void Enemy::Update(float deltaTime)
{
	// Cache the player's current position
	vector3 playerPosition = PhysicsManager::GetInstance()->GetPlayer()->GetPosition();
	// Calculate the seeking direction
	vector3 seekDirection = Seek(playerPosition);
	// Move in the seek direction
	Move(seekDirection);
	// Handle rotation
	/*
	glm::mat4 RotationMatrix = glm::transpose(glm::lookAt(position, playerPosition, glm::vec3(0.0f, 1.0f, 0.0f)));
	*/
	//	Set rotation
	float angle = (-1 * glm::atan(seekDirection.z / seekDirection.x)) + (PI / 2);

	if (seekDirection.x < 0) {
		angle += PI;
	}

	// Call the base implementation
	Simplex::PhysicsObject::Update(deltaTime);
}

void Enemy::OnCollision(WorldObject* other)
{
	switch (other->GetLayer()) {
	case CollisionLayers::Terrain:
		//Move on top of the terrain;
		SetPosition(vector3(position.x, other->GetGlobalMax().y + (position.y - globalMin.y), position.z));
		break;
	case CollisionLayers::Player:
		// Attack!
		//cout << "Attacked!" << endl;
		Attack();
		break;
	default:
		//Move half of the distance away from the collision
		break;
	}
	//cout << "Enemy's on collision called!" << endl;
	WorldObject::OnCollision(other);
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
	isAlive = false;
}

void Enemy::Attack()
{
	// Damage the player
	PhysicsManager::GetInstance()->GetPlayer()->TakeDamage(damage);
	// Explosion effect?
	// Kill the enemy
	Die();
}


