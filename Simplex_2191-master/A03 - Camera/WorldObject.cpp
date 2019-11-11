#include "pch.h"

using namespace Simplex;

void WorldObject::Init(void)
{
	position = vector3();
	orientation = IDENTITY_QUAT;
	scale = vector3(1, 1, 1);
	model = nullptr;
	layer = CollisionLayers::Terrain;
}

void WorldObject::Release(void)
{
	model->Release();
}

WorldObject::WorldObject()
{
	Init();
}

WorldObject::~WorldObject()
{
	Release();
}

#pragma region Accessors

vector3 WorldObject::GetPosition()
{
	return position;
}

quaternion WorldObject::GetRotation()
{
	return orientation;
}

vector3 WorldObject::GetScale()
{
	return scale;
}

void WorldObject::SetScale(vector3 value)
{
	scale = value;
}

MyMesh* WorldObject::GetModel()
{
	return model;
}

void WorldObject::SetModel(MyMesh* mesh)
{
	model = mesh;

	std::vector<vector3> vertices = model->GetVertices();
	int count = vertices.size();

	localMin = vertices[0];
	localMax = vertices[0];

	//Skip the first vertex because it has already been set
	for (int i = 1; i < count; i++) {
		if (localMin.x < vertices[i].x) {
			localMin.x = vertices[i].x;
		}
		if (localMax.x > vertices[i].x) {
			localMax.x = vertices[i].x;
		}
		if (localMin.y < vertices[i].y) {
			localMin.y = vertices[i].y;
		}
		if (localMax.y > vertices[i].y) {
			localMax.y = vertices[i].y;
		}
		if (localMin.z < vertices[i].z) {
			localMin.z = vertices[i].z;
		}
		if (localMax.z > vertices[i].z) {
			localMax.z = vertices[i].z;
		}
	}

	//Set the halfWidth
	halfWidth = (localMax - localMin) * 0.5f;

	//Find the corners of the bounding box
	vector3 corners[8];

	corners[0] = ToWorld(vector3(-halfWidth.x, -halfWidth.y, -halfWidth.z));
	corners[1] = ToWorld(vector3(-halfWidth.x, -halfWidth.y, halfWidth.z));
	corners[2] = ToWorld(vector3(-halfWidth.x, halfWidth.y, -halfWidth.z));
	corners[3] = ToWorld(vector3(-halfWidth.x, halfWidth.y, halfWidth.z));
	corners[4] = ToWorld(vector3(halfWidth.x, -halfWidth.y, -halfWidth.z));
	corners[5] = ToWorld(vector3(halfWidth.x, -halfWidth.y, halfWidth.z));
	corners[6] = ToWorld(vector3(halfWidth.x, halfWidth.y, -halfWidth.z));
	corners[7] = ToWorld(vector3(halfWidth.x, halfWidth.y, halfWidth.z));

	//Find the global min and max
	globalMin = corners[0];
	globalMax = corners[0];

	//Skip the first vertex because it has already been set
	for (int i = 1; i < 8; i++) {
		if (globalMin.x < corners[i].x) {
			globalMin.x = corners[i].x;
		}
		if (globalMax.x > corners[i].x) {
			globalMax.x = corners[i].x;
		}
		if (globalMin.y < corners[i].y) {
			globalMin.y = corners[i].y;
		}
		if (globalMax.y > corners[i].y) {
			globalMax.y = corners[i].y;
		}
		if (globalMin.z < corners[i].z) {
			globalMin.z = corners[i].z;
		}
		if (globalMax.z > corners[i].z) {
			globalMax.z = corners[i].z;
		}
	}

	vector3 globalHalfWidth = (globalMax - globalMin) * 0.5f;
	radius = glm::sqrt((globalHalfWidth.x * globalHalfWidth.x) + (globalHalfWidth.y * globalHalfWidth.y) + (globalHalfWidth.z * globalHalfWidth.z));
}

int WorldObject::GetLayer()
{
	return layer;
}

vector3 WorldObject::GetLocalMin()
{
	return localMin;
}

vector3 WorldObject::GetLocalMax()
{
	return localMax;
}

vector3 WorldObject::GetGlobalMin()
{
	return globalMin;
}

vector3 WorldObject::GetGlobalMax()
{
	return globalMax;
}

#pragma endregion

void WorldObject::Render(matrix4 a_mProjection, matrix4 a_mView)
{
	matrix4 toWorld = glm::toMat4(orientation);
	toWorld *= glm::scale(scale);
	toWorld *= glm::translate(position);
}

void WorldObject::Render(MyCamera* a_pCamera)
{
	Render(a_pCamera->GetProjectionMatrix(), a_pCamera->GetViewMatrix());
}

void WorldObject::RenderCollider()
{
	if (showSphere) {
		matrix4 transformation = matrix4(
			radius, 0, 0, 0,
			0, radius, 0, 0,
			0, 0, radius, 0,
			position.x, position.y, position.x, 1
		);

		MeshManager::GetInstance()->AddWireSphereToRenderList(transformation, C_WHITE, RENDER_WIRE);
	}
	if (showAABB) {
		matrix4 transformation = matrix4(
			localMax.x, 0, 0, 0,
			0, localMax.y, 0, 0,
			0, 0, localMax.z, 0,
			position.x, position.y, position.x, 1
		);
		MeshManager::GetInstance()->AddWireCubeToRenderList(transformation, C_YELLOW, RENDER_WIRE);
	}
	if (showARBB) {
		matrix4 transformation = IDENTITY_M4 * glm::toMat4(orientation);
		transformation *= glm::scale(scale);
		transformation *= glm::translate(position);
		MeshManager::GetInstance()->AddWireCubeToRenderList(transformation, C_GREEN, RENDER_WIRE);
	}
}

vector3 WorldObject::ToWorld(vector3 position)
{
	position = glm::toMat4(orientation) * vector4(position, 0);
	position *= scale;
	position += position;
	return position;
}
