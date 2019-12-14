#include "pch.h"
#include "Projectile.h"



Simplex::Projectile::Projectile(int dam, float sped)
{
	damage = dam;
	speed = sped;
}

Simplex::Projectile::Projectile(Projectile& other)
{
	damage = other.damage;
	speed = other.speed;
}

Simplex::Projectile& Simplex::Projectile::operator=(Projectile& other)
{
	return *this;
}

void Simplex::Projectile::Update(float deltaTime, float speed, vector3 direction)
{
}

void Simplex::Projectile::SetDirection(vector3 dir)
{
}

void Simplex::Projectile::OnCollision(WorldObject other)
{
}

void Simplex::Projectile::Init(void)
{
}

void Simplex::Projectile::Release(void)
{
}

Simplex::Projectile::Projectile()
{
}

Simplex::Projectile::~Projectile()
{
}
