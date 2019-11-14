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
	WorldObject* terrain = CreateObject(CollisionLayers::Terrain, vector3(0, -1, 0), vector3(100, 0.2, 100), glm::angleAxis((float)PI * 0.25f, AXIS_Y));
	Mesh* terrainMesh = new Mesh();
	terrainMesh->GenerateCube(1, C_WHITE);
	terrain->SetModel(terrainMesh);

	WorldObject* player = CreateObject(CollisionLayers::Player);
	Mesh* playerMesh = new Mesh();
	playerMesh->GenerateCube(1, C_WHITE);
	player->SetModel(playerMesh);
}

void PhysicsManager::Release(void)
{
	for (int i = 0; i < LAYER_COUNT; i++) {
		int currentLayerSize = collidables[i].size();

		for (int j = 0; j < currentLayerSize; j++) {
			delete collidables[i][j];
		}
	}
}

PhysicsManager::PhysicsManager()
{
	Init();
}

PhysicsManager::~PhysicsManager()
{
	Release();
}

void PhysicsManager::SetCamera(MyCamera* value)
{
	camera = value;
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
				collidables[i][j]->Rotate(vector3((float)PI * 0.1f, 0, (float)PI * 0.1f));
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
				if(camera != nullptr)
					collidables[i][j]->Render(camera);
			}
			break;
		}
	}
}

WorldObject* PhysicsManager::CreateObject(CollisionLayers layer, vector3 position, vector3 scale, quaternion orientation)
{
	WorldObject* newObject = new WorldObject;

	newObject->SetPosition(position);
	newObject->SetScale(scale);
	newObject->SetRotation(orientation);

	collidables[layer].push_back(newObject);
	return newObject;
}
