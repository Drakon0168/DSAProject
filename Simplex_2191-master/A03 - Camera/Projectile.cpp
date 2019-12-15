#include "pch.h"
#include "Projectile.h"



Simplex::Projectile::Projectile(int dam, float sped, Simplex::WorldObject* obj)
{
	damage = dam;
	speed = sped;
	direction = vector3(0.0f, 0.0f, 0.0f);
	position = vector3(0.0f, 0.0f, 0.0f);
	projBody = obj;
}

Simplex::Projectile::Projectile(Projectile& other)
{
	damage = other.damage;
	speed = other.speed;
	direction = other.direction;
	position = other.position;
	projBody = other.projBody;
}

Simplex::Projectile& Simplex::Projectile::operator=(Projectile& other)
{
	damage = other.damage;
	speed = other.speed;
	direction = other.direction;
	position = other.position;
	projBody = other.projBody;

	return *this;
}

void Simplex::Projectile::Update(float deltaTime, float speed, vector3 direction)
{
	vector3 deltaPosition = deltaTime * speed * direction;
	position = position + deltaPosition;
	projBody->SetPosition(position);
}

void Simplex::Projectile::SetDirection(vector3 dir)
{
	direction = dir;
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
