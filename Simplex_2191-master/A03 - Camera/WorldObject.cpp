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
	if(model != nullptr)
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
		if (localMin.x > vertices[i].x) {
			localMin.x = vertices[i].x;
		}
		if (localMax.x < vertices[i].x) {
			localMax.x = vertices[i].x;
		}
		if (localMin.y > vertices[i].y) {
			localMin.y = vertices[i].y;
		}
		if (localMax.y < vertices[i].y) {
			localMax.y = vertices[i].y;
		}
		if (localMin.z > vertices[i].z) {
			localMin.z = vertices[i].z;
		}
		if (localMax.z < vertices[i].z) {
			localMax.z = vertices[i].z;
		}
	}

	//Set the halfWidth
	halfWidth = (localMax - localMin) * 0.5f;

	//Calculate the global min and max
	CalculateGlobalMinMax();

	/*
	std::cout << std::endl << "Position: (" << position.x << ", " << position.y << ", " << position.z << "), Scale: (" << scale.x << ", " << scale.y << ", " << scale.z << ")" << std::endl;
	std::cout << "Local Min: (" << localMin.x << ", " << localMin.y << ", " << localMin.z << "), Local Max: (" << localMax.x << ", " << localMax.y << ", " << localMax.z << ")" << std::endl;
	std::cout << "Global Min: (" << globalMin.x << ", " << globalMin.y << ", " << globalMin.z << "), Global Max: (" << globalMax.x << ", " << globalMax.y << ", " << globalMax.z << ")" << std::endl;
	std::cout << "Half Width: (" << halfWidth.x << ", " << halfWidth.y << ", " << halfWidth.z << ")" << std::endl;

	vector3 testPosition(1,1,1);
	std::cout << std::endl << "Test Position" << std::endl << "  Before: (" << testPosition.x << ", " << testPosition.y << ", " << testPosition.z << ")" << std::endl;
	testPosition = ToWorld(testPosition);
	std::cout << "  After: (" << testPosition.x << ", " << testPosition.y << ", " << testPosition.z << ")" << std::endl;

	matrix4 transformation = IDENTITY_M4;
	transformation *= glm::toMat4(orientation);
	transformation *= glm::scale(scale);
	transformation *= glm::translate(position);

	std::cout << std::endl << "To World Matrix:" << std::endl;
	std::cout << "  " << transformation[0][0] << ", " << transformation[1][0] << ", " << transformation[2][0] << ", " << transformation[3][0] << std::endl;
	std::cout << "  " << transformation[0][1] << ", " << transformation[1][1] << ", " << transformation[2][1] << ", " << transformation[3][1] << std::endl;
	std::cout << "  " << transformation[0][2] << ", " << transformation[1][2] << ", " << transformation[2][2] << ", " << transformation[3][2] << std::endl;
	std::cout << "  " << transformation[0][3] << ", " << transformation[1][3] << ", " << transformation[2][3] << ", " << transformation[3][3] << std::endl;
	*/
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

void WorldObject::Render(matrix4 projection, matrix4 view)
{
	matrix4 modelMatrix = IDENTITY_M4;
	modelMatrix *= glm::toMat4(orientation);
	modelMatrix *= glm::scale(scale);
	modelMatrix *= glm::translate(position);
	model->Render(projection, view, modelMatrix);
	//MyMeshManager::GetInstance()->AddMeshToRenderList(model, modelMatrix);

	//std::cout << "Rendering Mesh" << std::endl;
}

void WorldObject::Render(MyCamera* camera)
{
	Render(camera->GetProjectionMatrix(), camera->GetViewMatrix());
}

#pragma endregion

void WorldObject::RenderCollider()
{
	if (showSphere) {
		matrix4 transformation = IDENTITY_M4;
		transformation *= glm::scale(scale);
		transformation *= glm::translate(position);

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

	//std::cout << "Rendering Collider" << std::endl;
}

void WorldObject::Translate(vector3 displacement)
{
	position += displacement;
}

vector3 WorldObject::ToWorld(vector3 point)
{
	matrix4 transformation = IDENTITY_M4;
	transformation *= glm::toMat4(orientation);
	transformation *= glm::scale(scale);
	transformation *= glm::translate(position);

	point = transformation * vector4(point, 1);
	return point;
}

void WorldObject::CalculateGlobalMinMax()
{
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
		if (globalMin.x > corners[i].x) {
			globalMin.x = corners[i].x;
		}
		if (globalMax.x < corners[i].x) {
			globalMax.x = corners[i].x;
		}
		if (globalMin.y > corners[i].y) {
			globalMin.y = corners[i].y;
		}
		if (globalMax.y < corners[i].y) {
			globalMax.y = corners[i].y;
		}
		if (globalMin.z > corners[i].z) {
			globalMin.z = corners[i].z;
		}
		if (globalMax.z < corners[i].z) {
			globalMax.z = corners[i].z;
		}
	}

	//Recalculate global half width and sphere radius
	vector3 globalHalfWidth = (globalMax - globalMin) * 0.5f;
	radius = glm::sqrt((globalHalfWidth.x * globalHalfWidth.x) + (globalHalfWidth.y * globalHalfWidth.y) + (globalHalfWidth.z * globalHalfWidth.z));
}