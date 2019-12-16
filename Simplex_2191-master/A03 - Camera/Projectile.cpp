#include "pch.h"
#include "Projectile.h"



Simplex::Projectile::Projectile(int dam, float sped)
{
	damage = dam;
	speed = sped;
	direction = vector3(0.0f, 0.0f, 0.0f);
	position = vector3(0.0f, 0.0f, 0.0f);
	usesGravity = false;
}

Simplex::Projectile::Projectile(Projectile& other)
{
	damage = other.damage;
	speed = other.speed;
	direction = other.direction;
	position = other.position;
	usesGravity = false;
}

Simplex::Projectile& Simplex::Projectile::operator=(Projectile& other)
{
	damage = other.damage;
	speed = other.speed;
	direction = other.direction;
	position = other.position;
	usesGravity = false;
	return *this;
}

void Simplex::Projectile::Update(float deltaTime, vector3 direction)
{
	vector3 deltaPosition = deltaTime * 2.0f * direction;
	position += deltaPosition;
	SetPosition(position);

	std::cout << "Direction Projectile: (" << direction.x << ", " << direction.y << ", " << direction.z << ")" << std::endl;
	std::cout << "Position Projectile: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
	//std::cout << "Position" << position.x << ", " << position.y << ", " << position.z << ::endl;
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

	damage = 1.0f;
	speed = 3.0f;
	direction = vector3(0.0f, 0.0f, 0.0f);
	position = vector3(0.0f, 0.0f, 0.0f);
	usesGravity = false;
}

Simplex::Projectile::~Projectile()
{
}
