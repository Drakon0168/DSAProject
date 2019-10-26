#pragma once

#include "pch.h"
#include "LivingEntity.h"

namespace Simplex {
	class Enemy : public LivingEntity
	{
	protected:
	private:
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
		Enemy();
		/*
		USAGE: Destructor
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		~Enemy();
#pragma endregion
#pragma region Rule Of Three
		/*
		USAGE: Copy Constructor
		ARGUMENTS: class object (to copy)
		OUTPUT: class object
		*/
		Enemy(Enemy& other);
		/*
		USAGE: Copy Assignment Operator
		ARGUMENTS: class object (to copy)
		OUTPUT: class object
		*/
		Enemy& operator=(Enemy& other);
#pragma endregion
#pragma region Update
		/*
		USAGE: Updates the object.
		ARGUMENTS:
		-	float deltaTime -> The amount of time that has passed since the last update
		OUTPUT: ---
		*/
		virtual void Update(float deltaTime) override;
#pragma endregion
#pragma region Enemy
		/*
		USAGE: Launches an Attack, should be overriden by child classes
		ARGUMENTS: --
		OUTPUT: ---
		*/
		virtual void Attack() override;
#pragma endregion
	};
}

