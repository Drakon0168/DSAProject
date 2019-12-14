#pragma once

#include "pch.h"
#include "PhysicsObject.h"

namespace Simplex {
	class Projectile : public PhysicsObject
	{
	protected:
		int damage; //The amount of damage the projectile will deal when it collides with something
		float speed;
		vector3 direction;
		vector3 position;
	public:
#pragma region Memory Management
		/*
		USAGE: Initialize the object's fields
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		virtual void Init(void);
		/*
		USAGE: Releases the object from memory
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		virtual void Release(void);
		/*
		USAGE: Constructor
		ARGUMENTS: ---
		OUTPUT: class object
		*/
		Projectile();
		/*
		USAGE: Destructor
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		~Projectile();
#pragma endregion
#pragma region Rule Of Three
		/*
		USAGE: Copy Constructor
		ARGUMENTS: class object (to copy)
		OUTPUT: class object
		*/
		Projectile(int dam, float sped);

		Projectile(Projectile& other);
		/*
		USAGE: Copy Assignment Operator
		ARGUMENTS: class object (to copy)
		OUTPUT: class object
		*/
		Projectile& operator=(Projectile& other);
#pragma endregion
#pragma region Update
		/*
		USAGE: Updates the object.
		ARGUMENTS:
		-	float deltaTime -> The amount of time that has passed since the last update
		OUTPUT: ---
		*/
		void Update(float deltaTime, float speed, vector3 direction);

		void SetDirection(vector3 dir);
#pragma endregion
#pragma region Physics
		/*
		USAGE: Destroys itself, and deals damage to the world object
			it collided with if that object is an entity
		ARGUMENTS: 
		-	WorldObject other -> The object that has been collided with
		OUTPUT: ---
		*/
		void OnCollision(WorldObject other) override;
#pragma endregion
	};
}

