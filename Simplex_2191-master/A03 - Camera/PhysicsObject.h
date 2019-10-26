#pragma once

#include "pch.h"

namespace Simplex {
	class PhysicsObject : public WorldObject
	{
	protected:
		vector3 velocity; //The current velocity of the object
		vector3 acceleration; //The current acceleration of the object
		float mass; //The mass of the object used when calculating forces
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
		PhysicsObject();
		/*
		USAGE: Destructor
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		~PhysicsObject();
#pragma endregion
#pragma region Rule Of Three
		/*
		USAGE: Copy Constructor
		ARGUMENTS: class object (to copy)
		OUTPUT: class object
		*/
		PhysicsObject(PhysicsObject& other);
		/*
		USAGE: Copy Assignment Operator
		ARGUMENTS: class object (to copy)
		OUTPUT: class object
		*/
		PhysicsObject& operator=(PhysicsObject& other);
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
#pragma region Physics
		/*
		USAGE: Applies logic based changes when this object collides with another object actual
			forces and collision resolution are handled by the physics manager
		ARGUMENTS: 
		-	WorldObject other -> The object that has been collided with
		OUTPUT: ---
		*/
		virtual void OnCollision(WorldObject other);

		/*
		USAGE: Applies a force to the object updating it's acceleration
		ARGUMENTS:
		-	vector3 force -> The force to apply
		OUTPUT: ---
		*/
		void ApplyForce(vector3 force);
#pragma endregion
	};
}