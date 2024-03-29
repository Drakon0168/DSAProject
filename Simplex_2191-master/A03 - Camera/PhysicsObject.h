#pragma once

#include "pch.h"

namespace Simplex {
	class PhysicsObject : public WorldObject
	{
	protected:
		const vector3 gravity = AXIS_Y * -9.8f; //The force to apply fue to gravity

		vector3 velocity; //The current velocity of the object
		vector3 acceleration; //The current acceleration of the object
		float mass; //The mass of the object used when calculating forces
		bool grounded = false; //Whether or not the object is on the ground
		bool usesGravity = true;
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

#pragma region Accessors
		/*
		USAGE: Returns the mass of the object
		ARGUMENTS: ---
		OUTPUT: The object's mass
		*/
		float GetMass();
		/*
		USAGE: Sets the mass of the object to the specified value
		ARGUMENTS: 
		-	float value -> The value to set the mass equal to
		OUTPUT: ---
		*/
		void SetMass(float value);
		/*
		USAGE: Returns whether or not the object is on the ground
		ARGUMENTS:
		OUTPUT: Whether or not the object is on the ground
		*/
		bool GetGrounded();
		/*
		USAGE: Sets whether or not the object is on the ground
		ARGUMENTS:
		-	bool value -> Whether or not the object is grounded
		OUTPUT: ---
		*/
		void SetGrounded(bool value);
		/*
		USAGE: Sets whether or not the object is affected by gravity
		ARGUMENTS:
		-	bool value -> Whether or not the object is affected by gravity
		OUTPUT: ---
		*/
		void SetUsesGravity(bool value);
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
		virtual void OnCollision(WorldObject* other) override;
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