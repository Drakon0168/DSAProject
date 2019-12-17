#pragma once
#include "pch.h"
#include "Projectile.h"
namespace Simplex {
	class Weapon
	{
	private:
		int ammo;
		int currentammo;
		float firerate;
		float reloadTime;
		int damage;
		float shotTimer;
		float reloadTimer;
		float projectileSpeed;
		Projectile* projectile;
		Projectile* activeProjectile;
		vector3 position;

	public:
		Weapon(int am, float firer, float reload, int damage, float projSpeed, Projectile* proj);
		~Weapon();
		Weapon(Weapon& other);
		Weapon& operator=(Weapon& other);

		void Shoot(vector3 direction);
		void Reload();
		void Update(float dt);
	};
}

