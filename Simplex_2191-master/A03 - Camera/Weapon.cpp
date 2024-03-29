#include "pch.h"
#include "Weapon.h"
#include "PhysicsManager.h"

Simplex::Weapon::Weapon(int am, float firer, float reload, int damage, float projSpeed, Projectile* proj)
{
	this->ammo = am;
	this->currentammo = ammo;
	this->firerate = firer;
	this->reloadTime = reload;
	this->damage = damage;
	this->projectileSpeed = projSpeed;
	this->projectile = proj;
	this->shotTimer = 0.0f;
	this->reloadTimer = 0.0f;
}

Simplex::Weapon::~Weapon()
{
	delete projectile;
}

Simplex::Weapon::Weapon(Weapon& other)
{
	ammo = other.ammo;
	currentammo = other.currentammo;
	firerate = other.firerate;
	reloadTime = other.reloadTime;
	damage = other.damage;
	projectileSpeed = other.projectileSpeed;
	projectile = other.projectile;
	shotTimer = other.shotTimer;
	reloadTimer = other.reloadTimer;
}

Simplex::Weapon& Simplex::Weapon::operator=(Weapon& other)
{
	ammo = other.ammo;
	currentammo = other.currentammo;
	firerate = other.firerate;
	reloadTime = other.reloadTime;
	damage = other.damage;
	projectileSpeed = other.projectileSpeed;
	projectile = other.projectile;
	shotTimer = other.shotTimer;
	reloadTimer = other.reloadTimer;

	return *this;
}

void Simplex::Weapon::Shoot(vector3 direction)
{
	if (shotTimer >= firerate)
	{
		Player* player = Simplex::PhysicsManager::GetInstance()->GetPlayer();
		vector3 projposition = player->GetCamera()->GetTarget();
		
		Projectile* projectile = PhysicsManager::GetInstance()->CreateProjectile(damage, projectileSpeed, projposition);
		projectile->SetDirection(glm::normalize(player->GetCamera()->GetTarget() - player->GetCamera()->GetPosition()));

		shotTimer = 0;
	}
}

void Simplex::Weapon::Reload()
{
	if (reloadTimer >= reloadTime)
	{
		currentammo = ammo;
		reloadTimer = 0;
	}

}

void Simplex::Weapon::Update(float dt)
{
	reloadTimer += dt;
	shotTimer += dt;
	Player* player = Simplex::PhysicsManager::GetInstance()->GetPlayer();
	position = player->GetPosition();
}
