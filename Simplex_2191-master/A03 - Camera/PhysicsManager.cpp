#include "pch.h"
#include "PhysicsManager.h"

using namespace Simplex;

PhysicsManager* PhysicsManager::instance = nullptr;

PhysicsManager* PhysicsManager::GetInstance()
{
	if (instance == nullptr) {
		instance = new PhysicsManager();
	}

	return instance;
}

void PhysicsManager::ReleaseInstance()
{
	if (instance != nullptr)
	{
		delete instance;
		instance = nullptr;
	}
}

void PhysicsManager::Init(void)
{
	for (int i = 0; i < LAYER_COUNT; i++) {
		collidables[i] = std::vector<WorldObject*>();
	}
	
	//TODO: Setup starting objects in the level
	collidables[CollisionLayers::Player].push_back(new WorldObject());
}

void PhysicsManager::Release(void)
{
	for (int i = 0; i < LAYER_COUNT; i++) {
		int currentLayerSize = collidables[i].size();

		for (int j = 0; j < currentLayerSize; j++) {
			delete collidables[i][j];
		}
	}

	delete[] collidables;
}

PhysicsManager::PhysicsManager()
{
	Init();
}

PhysicsManager::~PhysicsManager()
{
	Release();
}

void PhysicsManager::Update(float deltaTime)
{
	float count = 0;

	//Update Physics
	for (int i = 0; i < LAYER_COUNT; i++) {
		switch (i) {
		case CollisionLayers::Enemy:
		case CollisionLayers::EnemyProjectile:
		case CollisionLayers::Player:
		case CollisionLayers::PlayerProjectile:
		case CollisionLayers::NonCollidable:
			count = collidables[i].size();

			for (int j = 0; j < count; j++) {
				//TODO: Update the objects that can move
			}
			break;
		case CollisionLayers::Terrain:
			//Terrain doesn't move so it shouldn't be updated
			break;
		}
	}

	//Resolve Collisions
	for (int i = 0; i < LAYER_COUNT; i++) {
		switch (i) {
		case CollisionLayers::Enemy:
		case CollisionLayers::EnemyProjectile:
		case CollisionLayers::Player:
		case CollisionLayers::PlayerProjectile:
		case CollisionLayers::Terrain:
			count = collidables[i].size();

			for (int j = 0; j < count; j++) {
				//TODO: Check the objects collisions with only the other layers that it can collide with
			}
			break;
		case CollisionLayers::NonCollidable:
			//Don't need to check non-collidable objects
			break;
		}
	}

	//DrawObjects
	for (int i = 0; i < LAYER_COUNT; i++) {
		switch (i) {
		case CollisionLayers::Enemy:
		case CollisionLayers::EnemyProjectile:
		case CollisionLayers::Player:
		case CollisionLayers::PlayerProjectile:
		case CollisionLayers::Terrain:
		case CollisionLayers::NonCollidable:
			count = collidables[i].size();

			for (int j = 0; j < count; j++) {
				//TODO: Draw the object
				collidables[i][j]->RenderCollider();
			}
			break;
		}
	}
}
