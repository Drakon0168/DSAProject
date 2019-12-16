#pragma once

#include "pch.h"
#include "Weapon.h"
#include "LivingEntity.h"

namespace Simplex {
	class Player : public LivingEntity
	{
	protected:
		const int PISTOL_AMMO = 60;
		const float PISTOL_FIRE_RATE = 0.2f;
		const float PISTOL_RELOAD_TIME = 0.5f;
		const vector3 playerArmsOffset = vector3(0, 0.9f, 0.2f);
		const vector3 cameraOffset = vector3(0, 1.05f, 0);

		//TODO: Store the players current weapon
		int currentAmmo; //The amount of ammo left in the player's clip
		float currentFireRate;
		float currentReloadTime;
		int maxAmmo; //The total amount of ammo in a clip

		float jumpForce;

		WorldObject* playerArms;

		MyCamera* camera;

		


	public:
		Projectile* pistolBullet;
		Weapon* pistol;
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
#pragma region Accessors
		void SetPlayerArms(WorldObject* armsPointer);
		vector3 GetPlayerArmsOffset();
		WorldObject* GetPlayerArms();
		void SetCamera(MyCamera* value);
#pragma endregion
#pragma region Render
		void Render(matrix4 projection, matrix4 view) override;
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
#pragma region Entity
		/*
		USAGE: Kills the player and resets the game
		ARGUMENTS: --
		OUTPUT: ---
		*/
		virtual void Die() override;
		/*
		USAGE : Shoots the players weapon in the direction they are facing
		ARGUMENTS : --
		OUTPUT : -- -
		*/
		virtual void Attack() override;
#pragma endregion
#pragma region Player
		void Jump();
#pragma endregion
	};
}