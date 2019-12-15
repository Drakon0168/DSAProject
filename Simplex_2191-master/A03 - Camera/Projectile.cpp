#include "pch.h"
#include "Projectile.h"



Simplex::Projectile::Projectile(int dam, float sped, Simplex::WorldObject* obj)
{
	damage = dam;
	speed = sped;
	direction = vector3(0.0f, 0.0f, 0.0f);
	position = vector3(0.0f, 0.0f, 0.0f);
}

Simplex::Projectile::Projectile(Projectile& other)
{
	damage = other.damage;
	speed = other.speed;
	direction = other.direction;
	position = other.position;
}

Simplex::Projectile& Simplex::Projectile::operator=(Projectile& other)
{
	damage = other.damage;
	speed = other.speed;
	direction = other.direction;
	position = other.position;

	return *this;
}

void Simplex::Projectile::Update(float deltaTime, vector3 direction)
{
	vector3 deltaPosition = deltaTime * 1.0f * direction;
	position = position + deltaPosition;
	SetPosition(position);

	std::cout << "MinMax Projectile: (" << GetGlobalMin().x << ", " << GetGlobalMin().y << ", " << GetGlobalMin().z << GetGlobalMax().x << ", " << GetGlobalMax().y << ", " << GetGlobalMax().z << std::endl;
	std::cout << "Position" << position.x << ", " << position.y << ", " << position.z << ::endl;
}

void Simplex::Projectile::SetDirection(vector3 dir)
{
	direction = dir;
}

Simplex::vector3 Simplex::Projectile::GetDirection()
{
	return direction;
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
