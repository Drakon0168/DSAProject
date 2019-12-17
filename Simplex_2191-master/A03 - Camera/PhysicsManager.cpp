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
	terrain = CreateWorldObject(CollisionLayers::Terrain, vector3(0, -1, 0), vector3(250, 1, 250), glm::angleAxis((float)PI * 0.1f, AXIS_Y));
	terrain->LoadModel("Minecraft\\Cube.fbx", "Cube");
	terrain->SetPosition(vector3(0, -1 * terrain->GetGlobalHalfWidth().y, 0));
	terrainMin = terrain->GetGlobalMin();
	terrainMax = terrain->GetGlobalMax();

	Player* player = CreatePlayer(vector3(0, 5, 0), vector3(1.8, 1.8, 1.8));
	player->LoadModel("Minecraft\\Steve.fbx", "Steve");

	Enemy* teddy = CreateEnemy(1.1f, 50, vector3(10, 0, 10), vector3(1.5, 1.5, 1.5));
	teddy->LoadModel("Sunshine\\TeddyBear.fbx", "TeddyBear");
	teddy->SetUsesGravity(false);
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
	if(instance == nullptr)
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
			for (int i = 0; i < collidables[CollisionLayers::Enemy].size(); i++) {
				bool grounded = false;

				Enemy* enemy = dynamic_cast<Enemy*>(collidables[CollisionLayers::Enemy][i]);

				// Check enemies against player
				for (int k = 0; k < collidables[CollisionLayers::Player].size(); k++)
				{
					if (enemy->GetAlive()) {
						CheckCollision(enemy, collidables[CollisionLayers::Player][k]);
					}
				}

				//Check enemies against player projectiles
				if (enemy->GetAlive()) {
					for (int j = 0; j < collidables[CollisionLayers::PlayerProjectile].size(); j++) {
						CheckCollision(enemy, collidables[CollisionLayers::PlayerProjectile][j]);
					}
				}

				// Check enemies against terrain
				if (enemy->GetAlive()) {
					for (int j = 0; j < collidables[CollisionLayers::Terrain].size(); j++) {
						if (CheckCollision(enemy, collidables[CollisionLayers::Terrain][j])) {
							grounded = true;
						}
					}

					enemy->SetGrounded(grounded);
				}

				//Delete the enemy if it died
				if (!enemy->GetAlive()) {
					DestroyObject(enemy);
				}
			}
			break;
		case CollisionLayers::EnemyProjectile:
			//Enemy projectiles are checked against the player in the player case
			break;
		case CollisionLayers::Player:
			//Check against enemy bullets and terrain
			for (int i = 0; i < collidables[CollisionLayers::Player].size(); i++) {
				bool grounded = false;

				for (int j = 0; j < collidables[CollisionLayers::Terrain].size(); j++) {
					if (CheckCollision(collidables[CollisionLayers::Player][i], collidables[CollisionLayers::Terrain][j])) {
						grounded = true;
					}
				}//Check player against enemy projectiles
				for (int j = 0; j < collidables[CollisionLayers::EnemyProjectile].size(); j++) {
					CheckCollision(collidables[CollisionLayers::Player][i], collidables[CollisionLayers::EnemyProjectile][j]);
				}

				dynamic_cast<PhysicsObject*>(collidables[CollisionLayers::Player][i])->SetGrounded(grounded);
			}
			break;
		case CollisionLayers::PlayerProjectile:
			//Player projectiles are checked against enemies in the enemy case
			break;
		case CollisionLayers::Terrain:
			//Terrain is checked against enemies and the player in the player and enemy cases
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

	// Update enemy spawn timer
	if (GetEnemyCount() < maxEnemies)
	{
		enemySpawnCounter += deltaTime;
		if (enemySpawnCounter > enemySpawnDelay)
		{
			Enemy* teddy = CreateEnemy(1.1f, 10, GetRandomBoundsPosition(), vector3(1.5, 1.5, 1.5));
			teddy->LoadModel("Sunshine\\TeddyBear.fbx", "TeddyBear");
			teddy->SetUsesGravity(false);

			enemySpawnCounter = 0.0f;
		}
	}
	else
	{
		if (enemySpawnCounter != 0.0f) enemySpawnCounter = 0.0f;
	}
}

bool PhysicsManager::CheckCollision(WorldObject* a, WorldObject* b)
{
	//Spatial Optimization Grid
	vector2 gridSegmentAMin = vector2(glm::floor(a->GetGlobalMin().x / GRID_SIZE), glm::floor(a->GetGlobalMin().z / GRID_SIZE));
	vector2 gridSegmentAMax = vector2(glm::floor(a->GetGlobalMax().x / GRID_SIZE), glm::floor(a->GetGlobalMax().z / GRID_SIZE));
	vector2 gridSegmentBMin = vector2(glm::floor(b->GetGlobalMin().x / GRID_SIZE), glm::floor(b->GetGlobalMin().z / GRID_SIZE));
	vector2 gridSegmentBMax = vector2(glm::floor(b->GetGlobalMax().x / GRID_SIZE), glm::floor(b->GetGlobalMax().z / GRID_SIZE));

	//Check for overlap in grid segments that contain the objects
	if (gridSegmentAMin.x > gridSegmentBMax.x || gridSegmentAMin.y > gridSegmentBMax.y || gridSegmentBMin.x > gridSegmentAMax.x || gridSegmentBMin.y > gridSegmentAMax.y) {
		return false;
	}

	//Check for Sphere collision
	if (!CheckSphereCollision(a, b)) {
		return false;
	}

	////Check for AABB collision if there is a Sphere collision
	if (!CheckAABBCollision(a, b)) {
		return false;
	}

	//Check for ARBB collision if there is an AABB collision and a Sphere collision
	if (!CheckARBBCollision(a, b)) {
		return false;
	}

	//Resolve the collisions
	a->OnCollision(b);
	b->OnCollision(a);

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
	newPlayer->SetLayer(CollisionLayers::Player);
	collidables[CollisionLayers::Player].push_back(newPlayer);
	newPlayer->SetIndex(collidables[CollisionLayers::Player].size() - 1);
	return newPlayer;
}

Simplex::Enemy* PhysicsManager::CreateEnemy(float moveSpeed, int damage, vector3 position, vector3 scale, quaternion orientation)
{
	Enemy* enemy = new Enemy();

	enemy->SetPosition(position);
	enemy->SetScale(scale);
	enemy->SetRotation(orientation);
	enemy->SetMoveSpeed(moveSpeed);
	enemy->SetDamage(damage);
	enemy->SetLayer(CollisionLayers::Enemy);

	collidables[CollisionLayers::Enemy].push_back(enemy);
	enemy->SetIndex(collidables[CollisionLayers::Enemy].size()-1);

	return enemy;
}

Simplex::Projectile* PhysicsManager::CreateProjectile(int damage, float flightSpeed, vector3 position, vector3 scale, quaternion orientation)
{
	Projectile* proj = new Projectile(damage, flightSpeed);
	proj->LoadModel("\\Minecraft\\Cube.fbx", "Cube");

	proj->SetPosition(position);
	proj->SetScale(scale);
	proj->SetRotation(orientation);
	proj->SetLayer(CollisionLayers::PlayerProjectile);

	collidables[CollisionLayers::PlayerProjectile].push_back(proj);
	proj->SetIndex(collidables[CollisionLayers::PlayerProjectile].size() - 1);

	return proj;
}


void PhysicsManager::PushBackToCollidables(int layer, WorldObject* wo)
{
	collidables[layer].push_back(wo);
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
	axis[0] = (vector3)(glm::toMat4(a->GetRotation()) * vector4(AXIS_X, 1));
	axis[1] = (vector3)(glm::toMat4(a->GetRotation()) * vector4(AXIS_Y, 1));
	axis[2] = (vector3)(glm::toMat4(a->GetRotation()) * vector4(AXIS_Z, 1));
	axis[3] = (vector3)(glm::toMat4(b->GetRotation()) * vector4(AXIS_X, 1));
	axis[4] = (vector3)(glm::toMat4(b->GetRotation()) * vector4(AXIS_Y, 1));
	axis[5] = (vector3)(glm::toMat4(b->GetRotation()) * vector4(AXIS_Z, 1));

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			int index = 6 + (i * 3) + j;
			//Find the cross product axis
			axis[index] = glm::normalize(glm::cross(axis[i], axis[j + 3]));
		}
	}

	for (int i = 0; i < 15; i++) {
		//MeshManager::GetInstance()->AddLineToRenderList(IDENTITY_M4, vector3(0, 1, 0), axis[i] * 100, C_MAGENTA, C_MAGENTA);

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
	vector3 min = a->GetLocalMin();
	vector3 max = a->GetLocalMax();

	//Find the corners of the box
	corners[0] = a->ToWorld(vector3(min.x, min.y, min.z));
	corners[1] = a->ToWorld(vector3(min.x, min.y, max.z));
	corners[2] = a->ToWorld(vector3(min.x, max.y, min.z));
	corners[3] = a->ToWorld(vector3(min.x, max.y, max.z));
	corners[4] = a->ToWorld(vector3(max.x, min.y, min.z));
	corners[5] = a->ToWorld(vector3(max.x, min.y, max.z));
	corners[6] = a->ToWorld(vector3(max.x, max.y, min.z));
	corners[7] = a->ToWorld(vector3(max.x, max.y, max.z));

	//Find min and max values along the axis
	vector2 minMax = vector2(0, 0); //Vector representing the minimum and maximum values of the projection with x as a min and y as a max
	minMax.x = glm::dot(axis, corners[0]);
	minMax.y = minMax.x;

	for (int i = 1; i < 8; i++) {
		float projectionDistance = glm::dot(axis, corners[i]);

		if (projectionDistance < minMax.x) {
			minMax.x = projectionDistance;
		}

		if (projectionDistance > minMax.y) {
			minMax.y = projectionDistance;
		}
	}

	return minMax;
}

void PhysicsManager::ResetLevel()
{
	//Clear all enemies
	for (int i = 0; i < collidables[CollisionLayers::Enemy].size(); i++) {
		dynamic_cast<Enemy*>(collidables[CollisionLayers::Enemy][i])->Die();
	}

	//Clear all projectiles on death
	for (int i = 0; i < collidables[CollisionLayers::PlayerProjectile].size(); i++) {
		delete collidables[CollisionLayers::PlayerProjectile][i];
	}

	collidables[CollisionLayers::PlayerProjectile].clear();
}

void PhysicsManager::DestroyObject(WorldObject* object)
{
	for (int i = object->GetIndex() + 1; i < collidables[object->GetLayer()].size(); i++)
	{
		collidables[object->GetLayer()][i]->SetIndex(i - 1);
	}

	collidables[object->GetLayer()].erase(collidables[object->GetLayer()].begin() + object->GetIndex());
	delete object;
}

int PhysicsManager::GetEnemyCount()
{
	return collidables[CollisionLayers::Enemy].size();
}

vector3 PhysicsManager::GetRandomBoundsPosition()
{
	// Get the min and max values of the terrain for easy access

	// Generate a random integer to choose which side of the terrain bounds to get a position from
	int randomSide = RandomInt(0, 3);
	// Create an integer for the random value on the selected side
	int randAxisPoint;
	// Now, go through and check each side case
	switch (randomSide)
	{
		// Left
		case 0:
			// Get a random Z value for the spawn position
			randAxisPoint = RandomInt(terrainMin.z, terrainMax.z);
			return vector3(terrainMin.x, 0, randAxisPoint);
		// Right
		case 1:
			// Get a random Z value for the spawn position
			randAxisPoint = RandomInt(terrainMin.z, terrainMax.z);
			return vector3(terrainMax.x, 0, randAxisPoint);
			break;
		// Top
		case 2:
			// Get a random X value for the spawn position
			randAxisPoint = RandomInt(terrainMin.x, terrainMax.x);
			return vector3(randAxisPoint, 0, terrainMax.z);
			break;
		// Bottom
		case 3: 
			// Get a random X value for the spawn position
			randAxisPoint = RandomInt(terrainMin.x, terrainMax.x);
			return vector3(randAxisPoint, 0, terrainMin.z);
			break;
	}
	return vector3(0, 0, 0);
}

int PhysicsManager::RandomInt(int min, int max)
{
	return rand() % (max - min + 1) + min;
}