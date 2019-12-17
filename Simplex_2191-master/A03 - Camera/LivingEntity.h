#pragma once

#include "pch.h"
#include "PhysicsObject.h"

namespace Simplex {
	class LivingEntity : public PhysicsObject
	{
	protected:
		float moveSpeed; //The maximum move speed of the entity used when calculating movement forces
		float sprintSpeed; //The maximum move speed of the entity when sprinting
		float moveForce; //The amount of force to apply for movement
		float health; //The current health of the entity
		float maxHealth; //The maximum health of the entity
		bool isAlive = true;
		bool sprinting = false;
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
#pragma region Accessors
		/*
		USAGE: Returns the move speed of the object
		ARGUMENTS: ---
		OUTPUT: The object's move speed
		*/
		virtual float GetMoveSpeed();
		/*
		USAGE: Returns the move speed of the object
		ARGUMENTS: ---
		OUTPUT: The object's move speed
		*/
		float GetMoveForce();
		/*
		USAGE: Returns the current health of the object
		ARGUMENTS: ---
		OUTPUT: The object's current health
		*/
		float GetHealth();
		/*
		USAGE: Returns the maximum health of the object
		ARGUMENTS: ---
		OUTPUT: The object's maximum health
		*/
		float GetMaxHealth();

		/*
		USAGE: Sets the move speed of the object to the specified value
		ARGUMENTS:
		-	float value -> The value to set the move speed equal to
		OUTPUT: ---
		*/
		void SetMoveSpeed(float value);
		/*
		USAGE: Sets the move force of the object to the specified value
		ARGUMENTS:
		-	float value -> The value to set the move force equal to
		OUTPUT: ---
		*/
		void SetMoveForce(float value);
		/*
		USAGE: Sets the sprint bool of the object to the specified value
		ARGUMENTS:
		-	bool value -> The value to set the sprint bool equal to
		OUTPUT: ---
		*/
		void SetSprinting(bool value);
		/*
		USAGE: Sets the current health of the object to the specified value
		ARGUMENTS:
		-	float value -> The value to set the current health equal to
		OUTPUT: ---
		*/
		void SetHealth(float value);
		/*
		USAGE: Sets the maximum health of the object to the specified value
		ARGUMENTS:
		-	float value -> The value to set the maximum health equal to
		OUTPUT: ---
		*/
		void SetMaxHealth(float value);
		/*
		USAGE: Returns whether or not the enemy is alive
		ARGUMENTS: ---
		OUTPUT: Whether or not the enemy is alive
		*/
		bool GetAlive();
#pragma endregion
#pragma region Entity
		/*
		USAGE: Decrements the Entity's health based on the damage dealt
			also checks for death
		ARGUMENTS: 
		-	int damage -> The amount of damage to be taken
		OUTPUT: ---
		*/
		void TakeDamage(int damage);

		/*
		USAGE: Applies a movement force to the entity towards the target position
		ARGUMENTS:
		-	vector3 moveDirection -> The direction to move in
		OUTPUT: ---
		*/
		virtual void Move(vector3 moveDirection);

		/*
		USAGE: Kills the entity applying any on death effects
		ARGUMENTS: --
		OUTPUT: ---
		*/
		virtual void Die() = 0;

		/*
		USAGE: Launches an Attack
		ARGUMENTS: --
		OUTPUT: ---
		*/
		virtual void Attack() = 0;
#pragma endregion
	};
}