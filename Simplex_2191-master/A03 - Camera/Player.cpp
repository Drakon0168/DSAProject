#include "pch.h"
#include "Player.h"

using namespace Simplex;

void Player::Init(void)
{
	LivingEntity::Init();

	moveSpeed = 10.0f;
	moveForce = 250.0f;
	jumpForce = 1000.0f;
	health = 100;
	maxHealth = 100;

	maxAmmo = PISTOL_AMMO;
	currentAmmo = maxAmmo;
	currentFireRate = PISTOL_FIRE_RATE;
	currentReloadTime = PISTOL_RELOAD_TIME;
}

void Player::Release(void)
{
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
