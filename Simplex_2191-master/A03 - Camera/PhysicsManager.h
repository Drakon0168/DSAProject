#pragma once

#include "pch.h"

namespace Simplex {
	class PhysicsManager
	{
	private:
		const float gravity = -9.8f; //Acceleration constant due to gravity

		//TODO: Change this to an array of vectors if possible
		std::vector<std::vector<WorldObject*>> collidables; //All objects that can be collided with separated into layers
	public:
#pragma region Singleton
		static PhysicsManager* instance;
		/*
		USAGE : Returns the instance of the physics manager
		ARGUMENTS : --
		OUTPUT : PhysicsManager* -> Instance
		*/
		PhysicsManager* GetInstance();
#pragma endregion
#pragma region Memory Management
		/*
		USAGE: Initialize the object's fields
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Init(void);
		/*
		USAGE: Releases the object from memory
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		void Release(void);
		/*
		USAGE: Constructor
		ARGUMENTS: ---
		OUTPUT: class object
		*/
		PhysicsManager();
		/*
		USAGE: Destructor
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		~PhysicsManager();
#pragma endregion
#pragma region Rule Of Three
		/*
		USAGE: Copy Constructor
		ARGUMENTS: class object (to copy)
		OUTPUT: class object
		*/
		PhysicsManager(PhysicsManager& other);
		/*
		USAGE: Copy Assignment Operator
		ARGUMENTS: class object (to copy)
		OUTPUT: class object
		*/
		PhysicsManager& operator=(PhysicsManager& other);
#pragma endregion
#pragma region Update
		/*
		USAGE: Updates all movable objects.
		ARGUMENTS:
		-	float deltaTime -> The amount of time that has passed since the last update
		OUTPUT: ---
		*/
		void Update(float deltaTime);
#pragma endregion
#pragma region Physics
		/*
		USAGE : Checks whether two objects are colliding or not
		ARGUMENTS : --
		OUTPUT : bool -> Whether or not the two objects are colliding
		*/
		bool CheckCollision(WorldObject* a, WorldObject* b);
		/*
		USAGE : Moves two objects in response to a collision and calls
			the objects' OnCollision methods
		ARGUMENTS : --
		OUTPUT : --
		*/
		void ResolveCollision(WorldObject* a, WorldObject* b);
		/*
		USAGE : Removes the object from the simulation
		ARGUMENTS : --
		OUTPUT : --
		*/
		void DestroyObject(WorldObject* a);
#pragma endregion
	};
}