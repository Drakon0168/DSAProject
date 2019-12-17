#include "pch.h"
#include "LivingEntity.h"

using namespace Simplex;

void LivingEntity::Init(void)
{
	PhysicsObject::Init();

	moveSpeed = 5;
	moveForce = 250; //Prevent NaN issues when trying to move with no move force
	health = 1;
	maxHealth = 1;
}

void LivingEntity::Release(void)
{
	PhysicsObject::Release();
}

LivingEntity::LivingEntity()
{
	Init();
}

Simplex::LivingEntity::~LivingEntity()
{
	Release();
}

float LivingEntity::GetMoveSpeed()
{
	return moveSpeed;
}

float LivingEntity::GetMoveForce()
{
	return moveForce;
}

float LivingEntity::GetHealth()
{
	return health;
}

float LivingEntity::GetMaxHealth()
{
	return maxHealth;
}

void LivingEntity::SetMoveSpeed(float value)
{
	moveSpeed = value;
}

void LivingEntity::SetMoveForce(float value)
{
	moveForce = value;
}

void LivingEntity::SetSprinting(bool value)
{
	sprinting = value;
}

void LivingEntity::SetHealth(float value)
{
	health = value;

	if (health < 0) {
		health = 0;
	}
	else if (health > maxHealth) {
		health = maxHealth;
	}
}

void LivingEntity::SetMaxHealth(float value)
{
	maxHealth = value;

	if (health > maxHealth) {
		health = maxHealth;
	}
}

bool LivingEntity::GetAlive()
{
	return isAlive;
}

void LivingEntity::TakeDamage(int damage)
{
	health -= damage;

	if (health < 0) {
		health = 0;
		Die();
	}
}

void LivingEntity::Move(vector3 moveDirection)
{
	//Find the target velocity to achieve
	vector3 targetVelocity = moveDirection;

	if (sprinting) {
		targetVelocity *= sprintSpeed;
	}
	else {
		targetVelocity *= moveSpeed;
	}

	targetVelocity.y = velocity.y;

	//Find the force direction and magnitude to apply to reach the target velocity
	vector3 forceDirection = targetVelocity - velocity;
	float forceLength = glm::sqrt((forceDirection.x * forceDirection.x) + (forceDirection.y * forceDirection.y) + (forceDirection.z * forceDirection.z));

	//Protect from division by 0
	if (forceLength == 0) {
		forceDirection = vector3(0);
	}
	else {
		forceDirection = forceDirection / forceLength;
	}
	
	//Scale the force to the movement force
	vector3 force = forceDirection * (forceLength / (moveSpeed * 2)) * moveForce;

	//Apply the movement force
	ApplyForce(force);
}