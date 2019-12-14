#pragma once
#include "pch.h"
#include "Projectile.h"
namespace Simplex {
	class Weapon
	{
	private:
		int ammo;
		float firerate;
		float reloadTime;
		float shotTimer;
		float reloadTimer;
		float projectileSpeed;
		Projectile* projectile;

	public:
		Weapon(int am, float firer, float reload, float projSpeed, Projectile* proj);
		~Weapon();
		Weapon(Weapon& other);
		Weapon& operator=(Weapon& other);

		void Shoot(vector3 direction);
		void Reload();
		void Update(float dt);
	};
}

