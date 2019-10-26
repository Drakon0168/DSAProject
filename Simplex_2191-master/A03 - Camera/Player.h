#pragma once

#include "pch.h"
#include "LivingEntity.h"

namespace Simplex {
	class Player : public LivingEntity
	{
	protected:
		//TODO: Store the players current weapon
		int currentAmmo; //The amount of ammo left in the player's clip
		int maxAmmo; //The total amount of ammo in a clip
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
		Player();
		/*
		USAGE: Destructor
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		~Player();
#pragma endregion
#pragma region Rule Of Three
		/*
		USAGE: Copy Constructor
		ARGUMENTS: class object (to copy)
		OUTPUT: class object
		*/
		Player(Player& other);
		/*
		USAGE: Copy Assignment Operator
		ARGUMENTS: class object (to copy)
		OUTPUT: class object
		*/
		Player& operator=(Player& other);
#pragma endregion
#pragma region Update
		/*
		USAGE: Updates the object.
		ARGUMENTS:
		-	float deltaTime -> The amount of time that has passed since the last update
		OUTPUT: ---
		*/
		virtual void Update(float deltaTime) override;

		/*
		USAGE: Takes in player input and performs the corresponding actions
		ARGUMENTS: --
		OUTPUT: ---
		*/
		void ProcessInput();
#pragma endregion
#pragma region Enemy
		/*
		USAGE : Shoots the players weapon in the direction they are facing
		ARGUMENTS : --
		OUTPUT : -- -
		*/
		virtual void Attack() override;
#pragma endregion
	};
}