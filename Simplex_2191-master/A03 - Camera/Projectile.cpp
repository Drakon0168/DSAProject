#include "pch.h"
#include "Projectile.h"
#include "PhysicsManager.h"
#include "Enemy.h"


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
	velocity = direction * speed;

	PhysicsObject::Update(deltaTime);
	//vector3 deltaPosition = deltaTime * 2.0f * direction;
	//position += deltaPosition;
	//SetPosition(position);

	//std::cout << "Direction Projectile: (" << direction.x << ", " << direction.y << ", " << direction.z << ")" << std::endl;
	//std::cout << "Position Projectile: (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
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
	PhysicsObject::Release();
	PhysicsManager::GetInstance()->DestroyObject(this);
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
	Release();
}

void Simplex::Projectile::OnCollision(WorldObject* other)
{
	switch (other->GetLayer()) 
	{
	case CollisionLayers::Terrain:
		Release();
		break;
	case CollisionLayers::Player:
		//cout << "Attacked!" << endl;
		Release();
		break;
	case CollisionLayers::Enemy:
		// Try to damage the enemy
		Release();
		break;
	default:
		Release();
		break;
	}
	//cout << "Enemy's on collision called!" << endl;
	WorldObject::OnCollision(other);
}

void Simplex::Projectile::DamageEnemy(WorldObject* potentialEnemy)
{
	Enemy* enemy = dynamic_cast<Enemy*>(potentialEnemy);
	enemy->TakeDamage(damage);
	Release();
}
