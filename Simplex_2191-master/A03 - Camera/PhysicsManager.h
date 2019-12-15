#pragma once

#include "pch.h"

namespace Simplex {
	class PhysicsManager
	{
	private:
		const static int LAYER_COUNT = 6;
		static PhysicsManager* instance;
		MyCamera* camera;
		
		std::vector<WorldObject*> collidables[LAYER_COUNT]; //All objects that can be collided with separated into layers
	public:
#pragma region Singleton
		/*
		USAGE : Returns the instance of the physics manager
		ARGUMENTS : --
		OUTPUT : PhysicsManager* -> Instance
		*/
		static PhysicsManager* GetInstance();
		/*
		USAGE: Destroys the singleton
		ARGUMENTS: ---
		OUTPUT: ---
		*/
		static void ReleaseInstance();
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
#pragma region Accessors
		void SetCamera(MyCamera* value);
		/*
		USAGE: Returns a pointer to the player
		ARGUMENTS: ---
		OUTPUT: Player* player -> The current player
		*/
		Player* GetPlayer();
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
		void DestroyObject(WorldObject* obj);
		/*
		USAGE : Creates a new WorldObject
		ARGUMENTS :
		-	CollisionLayer layer -> The layer of the object
		-	Vector3 position -> The position of the object
		-	Vector3 scale -> The scale of the object
		-	Quaternion orientation -> The rotation of the object
		OUTPUT : --
		*/
		WorldObject* CreateWorldObject(int layer, vector3 position = vector3(0), vector3 scale = vector3(1), quaternion orientation = quaternion());
		/*
		USAGE : Creates a new PhysicsObject
		ARGUMENTS :
		-	CollisionLayer layer -> The layer of the object
		-	Vector3 position -> The position of the object
		-	Vector3 scale -> The scale of the object
		-	Quaternion orientation -> The rotation of the object
		OUTPUT : --
		*/
		PhysicsObject* CreatePhysicsObject(int layer, vector3 position = vector3(0), vector3 scale = vector3(1), quaternion orientation = quaternion());
		/*
		USAGE : Creates a new Player
		ARGUMENTS :
		-	CollisionLayer layer -> The layer of the player
		-	Vector3 position -> The position of the player
		-	Vector3 scale -> The scale of the player
		-	Quaternion orientation -> The rotation of the player
		OUTPUT : --
		*/
		Player* CreatePlayer(vector3 position = vector3(0), vector3 scale = vector3(1), quaternion orientation = quaternion());
		/*
		USAGE : Creates a new Enemy
		ARGUMENTS :
		-	CollisionLayer layer -> The layer of the enemy
		-	Vector3 position -> The position of the enemy
		-	Vector3 scale -> The scale of the enemy
		-	Quaternion orientation -> The rotation of the enemy
		OUTPUT : --
		*/
		Enemy* CreateEnemy(float maxSpeed, int damage, vector3 position = vector3(0), vector3 scale = vector3(1), quaternion orientation = quaternion());
		/*
		USAGE : Creates a new Projectile
		ARGUMENTS :
		-	CollisionLayer layer -> The layer of the projectile
		-	Vector3 position -> The position of the projectile
		-	Vector3 scale -> The scale of the projectile
		-	Quaternion orientation -> The rotation of the projectile
		OUTPUT : --
		*/
		Projectile* CreateProjectile(float maxSpeed, vector3 position = vector3(0), vector3 scale = vector3(1), quaternion orientation = quaternion());
		/*
		USAGE : Pushes a world object to the colliables list
		ARGUMENTS :
		-	CollisionLayer layer -> The layer of the object
		-	WorldObject wo the defined object
		OUTPUT : --
		*/
		void PushBackToCollidables(int layer, WorldObject* wo);
		/*
		USAGE : Checks for a collision based on the sphere colliders of the objects
		ARGUMENTS : --
		OUTPUT : Whether or not the objects sphere colliders intersect
		*/
		bool CheckSphereCollision(WorldObject* a, WorldObject* b);

		/*
		USAGE : Checks for a collision based on the AABB colliders of the objects
		ARGUMENTS : --
		OUTPUT : Whether or not the objects AABB colliders intersect
		*/
		bool CheckAABBCollision(WorldObject* a, WorldObject* b);

		/*
		USAGE : Checks for a collision based on the ARBB colliders of the objects
		ARGUMENTS : --
		OUTPUT : Whether or not the objects ARBB colliders intersect
		*/
		bool CheckARBBCollision(WorldObject* a, WorldObject* b);

		/*
		USAGE : Gets the min and max of an object along an axis
		ARGUMENTS :
		-	vector3 axis -> The axis to project along (Should already be normalized)
		-	WorldObject* a -> the object to project onto the axis
		OUTPUT : Vector2 projection -> x = the minimum of the object along the axis, y = the maximum of the object along the axis
		*/
		vector2 ProjectSATAxis(vector3 axis, WorldObject* a);
#pragma endregion
	};

}