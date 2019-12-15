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
	WorldObject* terrain = CreateWorldObject(CollisionLayers::Terrain, vector3(0, -1, 0), vector3(100, 0.2, 100));
	terrain->LoadModel("Minecraft\\Cube.fbx", "Cube");
	terrain->SetPosition(vector3(0, -1 * terrain->GetGlobalHalfWidth().y, 0));

	Player* player = CreatePlayer(vector3(0, 5, 0), vector3(1.8, 1.8, 1.8));// , vector3(1), glm::angleAxis((float)PI * 0.5f, AXIS_Y));
	player->LoadModel("Minecraft\\Steve.fbx", "Steve");

	Enemy* teddy = CreateEnemy(1.1f, vector3(10, 5, 10), vector3(1.5, 1.5, 1.5));
	teddy->LoadModel("Sunshine\\TeddyBear.fbx", "TeddyBear");


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
	GetPlayer()->SetCamera(value);
}

Player* Simplex::PhysicsManager::GetPlayer()
{
	return dynamic_cast<Player*>(collidables[CollisionLayers::Player][0]);
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
				PhysicsObject* obj = dynamic_cast<PhysicsObject*>(collidables[i][j]);

				if (obj) {
					obj->Update(deltaTime);
				}
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
			//Check enemies against other enemies and terrain
		case CollisionLayers::EnemyProjectile:
			//Check enemy projectiles against the player
		case CollisionLayers::Player:
			//Check the player against enemies, and terrain
		case CollisionLayers::PlayerProjectile:
			//Check player projectiles against the enemies
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
	
	//Draw All Objects
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
				if (camera != nullptr) 
				{
					collidables[i][j]->Render(camera);
				}
			}
			break;
		}
	}

	/*matrix4 transform = IDENTITY_M4;
	transform *= glm::translate(vector3(0));
	transform *= glm::scale(vector3(0.1f));

	MeshManager::GetInstance()->AddSphereToRenderList(transform, C_BLACK);*/
}

bool PhysicsManager::CheckCollision(WorldObject* a, WorldObject* b)
{
	//TODO: Fully test collision detection once we can move objects

	//Check for Sphere collision
	if (!CheckSphereCollision(a, b)) {
		return false;
	}

	//Check for AABB collision if there is a Sphere collision
	if (!CheckAABBCollision(a, b)) {
		return false;
	}

	//Check for ARBB collision if there is an AABB collision and a Sphere collision
	if (!CheckARBBCollision(a, b)) {
		return false;
	}

	//Return true only if all other checks are true
	return true;
}

WorldObject* PhysicsManager::CreateWorldObject(int layer, vector3 position, vector3 scale, quaternion orientation)
{
	WorldObject* newObject = new WorldObject();

	newObject->SetPosition(position);
	newObject->SetScale(scale);
	newObject->SetRotation(orientation);

	collidables[layer].push_back(newObject);
	return newObject;
}

PhysicsObject* PhysicsManager::CreatePhysicsObject(int layer, vector3 position, vector3 scale, quaternion orientation)
{
	PhysicsObject* newObject = new PhysicsObject();

	newObject->SetPosition(position);
	newObject->SetScale(scale);
	newObject->SetRotation(orientation);

	collidables[layer].push_back(newObject);
	return newObject;
}

Player* PhysicsManager::CreatePlayer(vector3 position, vector3 scale, quaternion orientation)
{
	Player* newPlayer = new Player();

	newPlayer->SetPosition(position);
	newPlayer->SetScale(scale);
	newPlayer->SetRotation(orientation);

	collidables[CollisionLayers::Player].push_back(newPlayer);
	return newPlayer;
}

Simplex::Enemy* PhysicsManager::CreateEnemy(float moveSpeed, vector3 position, vector3 scale, quaternion orientation)
{
	Enemy* enemy = new Enemy();

	enemy->SetPosition(position);
	enemy->SetScale(scale);
	enemy->SetRotation(orientation);
	enemy->SetMoveSpeed(moveSpeed);

	collidables[CollisionLayers::Enemy].push_back(enemy);

	return enemy;
}

bool PhysicsManager::CheckSphereCollision(WorldObject* a, WorldObject* b)
{
	vector3 distanceVector = a->GetPosition() - b->GetPosition();
	float sqrDistance = (distanceVector.x * distanceVector.x) + (distanceVector.y * distanceVector.y) + (distanceVector.z * distanceVector.z);
	float sqrRadius = a->GetRadius() + b->GetRadius();
	sqrRadius *= sqrRadius;

	if (sqrDistance < sqrRadius) {
		return true;
	}

	return false;
}

bool PhysicsManager::CheckAABBCollision(WorldObject* a, WorldObject* b)
{
	vector3 aMin = a->GetGlobalMin();
	vector3 aMax = a->GetGlobalMax();
	vector3 bMin = b->GetGlobalMin();
	vector3 bMax = b->GetGlobalMax();

	if (aMin.x > bMax.x || bMin.x > aMax.x) {
		return false;
	}
	if (aMin.y > bMax.y || bMin.y > aMax.y) {
		return false;
	}
	if (aMin.z > bMax.z || bMin.z > aMax.z) {
		return false;
	}

	return true;
}

bool PhysicsManager::CheckARBBCollision(WorldObject* a, WorldObject* b)
{
	vector3 axis[15];
	axis[0] = AXIS_X * a->GetRotation();
	axis[1] = AXIS_Y * a->GetRotation();
	axis[2] = AXIS_Z * a->GetRotation();
	axis[3] = AXIS_X * b->GetRotation();
	axis[4] = AXIS_Y * b->GetRotation();
	axis[5] = AXIS_Z * b->GetRotation();

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			axis[6 + (i * 3) + j] = glm::cross(axis[i], axis[j + 3]);
		}
	}

	for (int i = 0; i < 15; i++) {
		//MeshManager::GetInstance()->AddLineToRenderList(IDENTITY_M4, vector3(0), axis[i] * 100, C_BLACK, C_BLACK);

		vector2 aMinMax = ProjectSATAxis(axis[i], a);
		vector2 bMinMax = ProjectSATAxis(axis[i], b);

		if (aMinMax.x > bMinMax.y || bMinMax.x > aMinMax.y) {
			return false;
		}
	}

	return true;
}

vector2 PhysicsManager::ProjectSATAxis(vector3 axis, WorldObject* a)
{
	vector3 corners[8];
	vector3 halfWidth = a->GetLocalHalfWidth();

	//Find the corners of the box
	corners[0] = a->ToWorld(vector3(halfWidth.x, halfWidth.y, halfWidth.z));
	corners[1] = a->ToWorld(vector3(halfWidth.x, halfWidth.y, -halfWidth.z));
	corners[2] = a->ToWorld(vector3(halfWidth.x, -halfWidth.y, halfWidth.z));
	corners[3] = a->ToWorld(vector3(halfWidth.x, -halfWidth.y, -halfWidth.z));
	corners[4] = a->ToWorld(vector3(-halfWidth.x, halfWidth.y, halfWidth.z));
	corners[5] = a->ToWorld(vector3(-halfWidth.x, halfWidth.y, -halfWidth.z));
	corners[6] = a->ToWorld(vector3(-halfWidth.x, -halfWidth.y, halfWidth.z));
	corners[7] = a->ToWorld(vector3(-halfWidth.x, -halfWidth.y, -halfWidth.z));

	//Find min and max values along the axis
	vector2 minMax = vector2(0, 0);
	minMax.x = glm::dot(axis, corners[0]);
	minMax.y = minMax.x;

	//TODO: Get rid of min and max vectors they are only for debug purposes
	//vector3 min;
	//vector3 max;

	for (int i = 1; i < 8; i++) {
		float projectionDistance = glm::dot(axis, corners[i]);

		if (projectionDistance < minMax.x) {
			minMax.x = projectionDistance;
			//min = corners[i];
		}

		if (projectionDistance > minMax.y) {
			minMax.y = projectionDistance;
			//max = corners[i];
		}
	}

	//MeshManager::GetInstance()->AddLineToRenderList(IDENTITY_M4, min, axis * minMax.x, C_WHITE, C_WHITE);
	//MeshManager::GetInstance()->AddLineToRenderList(IDENTITY_M4, max, axis * minMax.y, C_WHITE, C_WHITE);

	return minMax;
}
