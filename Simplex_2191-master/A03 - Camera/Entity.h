#pragma once

#include "pch.h"
#include "PhysicsObject.h"

namespace Simplex {
	class LivingEntity : public PhysicsObject
	{
	protected:
		float moveSpeed; //The maximum move speed of the entity used when calculating movement forces
		int health; //The current health of the entity
		int maxHealth; //The maximum health of the entity
	public:
#pragma region Memory Management
		/*
		USAGE: Initialize the object's fields
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		virtual void Init(void) override;
		/*
		USAGE: Releases the object from memory
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		virtual void Release(void) override;
		/*
		USAGE: Constructor
		ARGUMENTS: ---
		OUTPUT: class object
		*/
		LivingEntity();
		/*
		USAGE: Destructor
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		~LivingEntity();
#pragma endregion
#pragma region Rule Of Three
		/*
		USAGE: Copy Constructor
		ARGUMENTS: class object (to copy)
		OUTPUT: class object
		*/
		LivingEntity(LivingEntity& other);
		/*
		USAGE: Copy Assignment Operator
		ARGUMENTS: class object (to copy)
		OUTPUT: class object
		*/
		LivingEntity& operator=(LivingEntity& other);
#pragma endregion
#pragma region Update
		/*
		USAGE: Updates the object.
		ARGUMENTS:
		-	float deltaTime -> The amount of time that has passed since the last update
		OUTPUT: ---
		*/
		virtual void Update(float deltaTime);
#pragma endregion
#pragma region Entity
		/*
		USAGE: Decrements the Entity's health based on the damage dealt
			also checks for death
		ARGUMENTS: 
		-	int damage -> The amount of damage to be taken
		OUTPUT: ---
		*/
		virtual void TakeDamage(int damage);

		/*
		USAGE: Applies a movement force to the entity towards the target position
		ARGUMENTS:
		-	vector3 target -> The position to move towards
		OUTPUT: ---
		*/
		virtual void Move(vector3 target);

		/*
		USAGE: Kills the entity applying any on death effects
		ARGUMENTS: --
		OUTPUT: ---
		*/
		virtual void Die();

		/*
		USAGE: Launches an attack should be overridden by child classes
		ARGUMENTS: --
		OUTPUT: ---
		*/
		virtual void Attack();
#pragma endregion
	};
}