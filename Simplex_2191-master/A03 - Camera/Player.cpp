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

	pistolBulletObj = new WorldObject();
	pistolBulletObj->SetScale(vector3(1.0f));
	pistolBulletObj->LoadModel("Minecraft\\Cube.fbx", "Cube");
	pistolBullet = new Projectile(1.0f, 1.0f, pistolBulletObj);

	pistol = new Weapon(maxAmmo, currentFireRate, currentReloadTime, 1.0f, pistolBullet);


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
	PhysicsObject::Update(deltaTime);
	pistol->Update(deltaTime);
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

	//std::cout << "Direction: (" << targetDirection.x << ", " << targetDirection.z << "), Angle: " << angle << std::endl;
	SetRotation(glm::angleAxis(angle, AXIS_Y));

	//Match arms to player
	playerArms->SetPosition(position + playerArmsOffset);
	playerArms->SetRotation(orientation);
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
}

void Player::Attack()
{
	//TODO: Shoot the currently equipped gun
}

void Player::Jump()
{
	ApplyForce(AXIS_Y * jumpForce);
}
