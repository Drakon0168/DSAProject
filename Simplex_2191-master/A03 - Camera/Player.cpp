#include "pch.h"
#include "Player.h"
#include "PhysicsManager.h"
using namespace Simplex;

void Player::Init(void)
{
	LivingEntity::Init();

	//Setup player stats
	moveSpeed = 10.0f;
	sprintSpeed = 20.0f;
	moveForce = 250.0f;
	jumpForce = 500.0f;
	health = 100;
	maxHealth = 100;

	//Setup weapon
	maxAmmo = PISTOL_AMMO;
	currentAmmo = maxAmmo;
	currentFireRate = PISTOL_FIRE_RATE;
	currentReloadTime = PISTOL_RELOAD_TIME;
	
	//Setup arms
	playerArms = new WorldObject();
	playerArms->SetScale(vector3(0.006f));
	playerArms->LoadModel("Sunshine\\FPS_Arms\\source\\arms@throwing.fbx", "PlayerArms");
	playerArms->SetRenderCollider(false);
}

void Player::Release(void)
{
	delete playerArms;
	LivingEntity::Release();
}

Player::Player()
{
	Init();
}

Player::~Player()
{
	Release();
}

Player::Player(Player& other)
{
	maxAmmo = other.maxAmmo;
	currentAmmo = other.currentAmmo;
	currentFireRate = other.currentFireRate;
	currentReloadTime = other.currentReloadTime;
}

void Player::Update(float deltaTime)
{
	if (pistol)
	{
		pistol->Update(deltaTime);
	}
	else
	{
		pistolBullet = new Projectile(5, 15);
		pistolBullet->LoadModel("\\Minecraft\\Cube.fbx", "Cube");
		pistol = new Weapon(maxAmmo, currentFireRate, currentReloadTime, 5, 30.0f, pistolBullet);
	}

	PhysicsObject::Update(deltaTime);

	camera->SetPosition(position + cameraOffset);

	//Face camera target
	//	Get Direction
	vector3 targetDirection = camera->GetTarget() - position;
	targetDirection.y = 0;
	//	Normalize
	float length = (targetDirection.x * targetDirection.x) + (targetDirection.z * targetDirection.z);
	targetDirection /= length;
	//	Set rotation
	float angle = (-1 * glm::atan(targetDirection.z / targetDirection.x)) + (PI / 2);

	if (targetDirection.x < 0) {
		angle += PI;
	}

	SetRotation(glm::angleAxis(angle, AXIS_Y));

	//Match arms to player
	playerArms->SetPosition(position + playerArmsOffset);
	playerArms->SetRotation(orientation);

	if (position.y < -5) {
		Die();
	}
}

//Sets the players arms
void Player::SetPlayerArms(WorldObject* armsPointer)
{
	playerArms = armsPointer;
}

WorldObject* Player::GetPlayerArms()
{
	return playerArms;
}

void Simplex::Player::SetCamera(MyCamera* value)
{
	camera = value;
}

MyCamera* Player::GetCamera()
{
	return camera;
}

void Player::Render(matrix4 projection, matrix4 view)
{
	playerArms->Render(projection, view);

	if (renderCollider) {
		vector3 globalCenter = ToWorld(center);

		if (showSphere) {
			matrix4 transformation = IDENTITY_M4;
			transformation *= glm::translate(globalCenter);
			transformation *= glm::scale(vector3(radius));

			MeshManager::GetInstance()->AddWireSphereToRenderList(transformation, C_BLUE);
		}
		if (showAABB) {
			matrix4 transformation = IDENTITY_M4;
			transformation *= glm::translate(globalCenter);
			transformation *= glm::scale(globalMax - globalMin);

			MeshManager::GetInstance()->AddWireCubeToRenderList(transformation, C_YELLOW);
		}
		if (showARBB) {
			matrix4 transformation = IDENTITY_M4;
			transformation *= glm::translate(globalCenter);
			transformation *= glm::scale((localMax - localMin) * scale);
			transformation *= glm::toMat4(orientation);

			MeshManager::GetInstance()->AddWireCubeToRenderList(transformation, C_MAGENTA);
		}
	}
}

vector3 Player::GetPlayerArmsOffset()
{
	return playerArmsOffset;
}

void Player::Die()
{
	//TODO: Reset the game / take the player back to the main menu
	std::cout << "Player Died" << std::endl;
	SetPosition(vector3(0, 10, 0));
	velocity = vector3(0);
	acceleration = vector3(0);
	health = maxHealth;

	PhysicsManager::GetInstance()->ResetLevel();
}

void Player::Attack()
{
	//TODO: Shoot the currently equipped gun
	vector3 targetDirection = camera->GetTarget() - camera->GetPosition();

	pistol->Shoot(targetDirection);
}

void Player::Jump()
{
	ApplyForce(AXIS_Y * jumpForce);
}
