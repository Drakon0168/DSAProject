#include "pch.h"
#include "Weapon.h"

Simplex::Weapon::Weapon(int am, float firer, float reload, float projSpeed, Projectile* proj)
{
	ammo = am;
	currentammo = ammo;
	firerate = firer;
	reloadTime = reload;
	projectileSpeed = projSpeed;
	projectile = proj;
	shotTimer = 0.0f;
	reloadTimer = 0.0f;
}

Simplex::Weapon::~Weapon()
{

}

Simplex::Weapon::Weapon(Weapon& other)
{
	ammo = other.ammo;
	currentammo = other.currentammo;
	firerate = other.firerate;
	reloadTime = other.reloadTime;
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
	projectileSpeed = other.projectileSpeed;
	projectile = other.projectile;
	shotTimer = other.shotTimer;
	reloadTimer = other.reloadTimer;

	return *this;
}

void Simplex::Weapon::Shoot(vector3 direction)
{
	if (shotTimer >= firerate && currentammo > 0)
	{
		Projectile newProj = Projectile(*projectile);
		newProj.SetDirection(direction);
		shotTimer = 0;
		currentammo -= 1;
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
}
