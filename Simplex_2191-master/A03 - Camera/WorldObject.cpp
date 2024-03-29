#include "pch.h"

using namespace Simplex;

#pragma region Memory Management

void WorldObject::Init(void)
{
	position = vector3();
	orientation = IDENTITY_QUAT;
	scale = vector3(1);
	center = vector3(0);
	UpdateTransform();
	model = new Model();
	layer = CollisionLayers::Terrain;
}

void WorldObject::Release(void)
{
	if (model != nullptr)
	{
		model = nullptr;
	}
}

WorldObject::WorldObject()
{
	Init();
}

WorldObject::~WorldObject()
{
	Release();
}

WorldObject::WorldObject(WorldObject& other)
{
	position = other.GetPosition();
	orientation = other.GetRotation();
	scale = other.GetScale();
	UpdateTransform();
	SetModel(other.GetModel());
	layer = other.GetLayer();
}

WorldObject& WorldObject::operator=(WorldObject& other) {
	return WorldObject(other);
}

#pragma endregion

#pragma region Accessors

vector3 WorldObject::GetPosition()
{
	return position;
}

void WorldObject::SetPosition(vector3 value)
{
	position = value;

	UpdateTransform();
}

quaternion WorldObject::GetRotation()
{
	return orientation;
}

void WorldObject::SetRotation(quaternion value)
{
	orientation = value;

	UpdateTransform();
}

vector3 WorldObject::GetScale()
{
	return scale;
}

void WorldObject::SetScale(vector3 value)
{
	scale = value;

	//Set the radius
	UpdateRadius();

	UpdateTransform();
}

matrix4 WorldObject::GetTransform()
{
	return transform;
}

Model* WorldObject::GetModel()
{
	return model;
}

void WorldObject::LoadModel(string modelFilePath, string identifier)
{
	FileReference modelReference = FileReference(modelFilePath, identifier);
	GetModel()->Load(modelReference.GetFilePath());
	SetModel(GetModel());
}

void WorldObject::SetModel(Model* newModel)
{
	model = newModel;

	std::vector<vector3> vertices = model->GetVertexList();
	int count = vertices.size();

	for (int i = 0; i < count; i++) {
		vertices[i] += center;
	}

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
	localHalfWidth = (localMax - localMin) * 0.5f;
	center = (localMin + localMax) * 0.5f;

	//Set the radius
	UpdateRadius();

	//Calculate the global min and max
	CalculateGlobalMinMax();

	/*std::cout << std::endl << "Position: (" << position.x << ", " << position.y << ", " << position.z << "), Scale: (" << scale.x << ", " << scale.y << ", " << scale.z << ")" << std::endl;
	std::cout << "Local Min: (" << localMin.x << ", " << localMin.y << ", " << localMin.z << "), Local Max: (" << localMax.x << ", " << localMax.y << ", " << localMax.z << ")" << std::endl;
	std::cout << "Global Min: (" << globalMin.x << ", " << globalMin.y << ", " << globalMin.z << "), Global Max: (" << globalMax.x << ", " << globalMax.y << ", " << globalMax.z << ")" << std::endl;
	std::cout << "Local Half Width: (" << localHalfWidth.x << ", " << localHalfWidth.y << ", " << localHalfWidth.z << ")" << "Global Half Width : (" << globalHalfWidth.x << ", " << globalHalfWidth.y << ", " << globalHalfWidth.z << ")" <<std::endl;

	vector3 testPosition(1,1,1);
	std::cout << std::endl << "Test Position" << std::endl << "  Before: (" << testPosition.x << ", " << testPosition.y << ", " << testPosition.z << ")" << std::endl;
	testPosition = ToWorld(testPosition);
	std::cout << "  After: (" << testPosition.x << ", " << testPosition.y << ", " << testPosition.z << ")" << std::endl;

	std::cout << std::endl << "Transform Matrix:" << std::endl;
	std::cout << "  " << transform[0][0] << ", " << transform[1][0] << ", " << transform[2][0] << ", " << transform[3][0] << std::endl;
	std::cout << "  " << transform[0][1] << ", " << transform[1][1] << ", " << transform[2][1] << ", " << transform[3][1] << std::endl;
	std::cout << "  " << transform[0][2] << ", " << transform[1][2] << ", " << transform[2][2] << ", " << transform[3][2] << std::endl;
	std::cout << "  " << transform[0][3] << ", " << transform[1][3] << ", " << transform[2][3] << ", " << transform[3][3] << std::endl;*/
}

int WorldObject::GetLayer()
{
	return layer;
}

void WorldObject::SetLayer(int value)
{
	layer = value;
}

int WorldObject::GetIndex()
{
	return index;
}

void WorldObject::SetIndex(int value)
{
	index = value;
}

float WorldObject::GetRadius()
{
	return radius;
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

vector3 WorldObject::GetLocalHalfWidth()
{
	return localHalfWidth;
}

vector3 WorldObject::GetGlobalHalfWidth()
{
	return globalHalfWidth;
}

bool WorldObject::GetRenderCollider()
{
	return renderCollider;
}

void WorldObject::SetRenderCollider(bool value)
{
	renderCollider = value;
}

#pragma endregion

#pragma region Rendering

void WorldObject::Render(matrix4 projection, matrix4 view)
{
	if (model != nullptr) {
		model->SetModelMatrix(transform);
		model->AddToRenderList();
	}
	
	if (renderCollider) {
		vector3 globalCenter = (globalMin + globalMax) * 0.5f;

		if (showSphere) {
			matrix4 transformation = IDENTITY_M4;
			transformation *= glm::translate(globalCenter);
			transformation *= glm::scale(vector3(radius));

			MeshManager::GetInstance()->AddWireSphereToRenderList(transformation, C_BLUE);
		}
		if (showAABB) {
			matrix4 transformation = IDENTITY_M4;
			transformation *= glm::translate(globalCenter);
			transformation *= glm::scale(globalMax - globalMin);
			
			MeshManager::GetInstance()->AddWireCubeToRenderList(transformation, C_YELLOW);
		}
		if (showARBB) {
			matrix4 transformation = IDENTITY_M4;
			vector3 ARBBScale = (localMax - localMin) * scale;

			if (layer == CollisionLayers::Player) {
				std::cout << "ARBB Scale: (" << ARBBScale.x << ", " << ARBBScale.y << ", " << ARBBScale.z << ")" << std::endl;
			}
			
			transformation *= glm::translate(globalCenter);
			transformation *= glm::toMat4(orientation);
			transformation *= glm::scale(ARBBScale);

			MeshManager::GetInstance()->AddWireCubeToRenderList(transformation, C_MAGENTA);
		}
	}
}

void WorldObject::Render(MyCamera* camera)
{
	Render(camera->GetProjectionMatrix(), camera->GetViewMatrix());
}

#pragma endregion

#pragma region Transformations

void WorldObject::Translate(vector3 displacement)
{
	position += displacement;

	UpdateTransform();
}

void WorldObject::Rotate(vector3 rotation)
{
	//Find the local axis
	vector3 localXAxis = (vector3)(glm::toMat4(orientation) * vector4(AXIS_X, 1));
	vector3 localYAxis = (vector3)(glm::toMat4(orientation) * vector4(AXIS_Y, 1));
	vector3 localZAxis = (vector3)(glm::toMat4(orientation) * vector4(AXIS_Z, 1));

	rotation *= (float)PI / 180;

	//Create rotation quaternion
	quaternion rotationQuat = glm::angleAxis(rotation.x, localXAxis) * glm::angleAxis(rotation.y, localYAxis) * glm::angleAxis(rotation.z, localZAxis);

	//Rotation orientation
	Rotate(rotationQuat);
}

void WorldObject::Rotate(quaternion rotation)
{
	orientation *= rotation;

	UpdateTransform();
}

void WorldObject::Scale(vector3 scaleAmount)
{
	scale *= scaleAmount;

	//Update the radius
	UpdateRadius();

	UpdateTransform();
}

void WorldObject::Scale(float scaleAmount)
{
	scale *= scaleAmount;

	//Update the radius
	UpdateRadius();

	UpdateTransform();
}

void WorldObject::UpdateRadius()
{
	radius = glm::sqrt(((localHalfWidth.x * scale.x) * (localHalfWidth.x * scale.x)) + ((localHalfWidth.y * scale.y) * (localHalfWidth.y * scale.y)) + ((localHalfWidth.z * scale.z) * (localHalfWidth.z * scale.z)));
}

void WorldObject::UpdateTransform()
{
	transform = IDENTITY_M4;
	transform *= glm::translate(position - (center * scale));
	transform *= glm::toMat4(orientation);
	transform *= glm::scale(scale);
	
	CalculateGlobalMinMax();
}

#pragma endregion

#pragma region Helper Methods

void WorldObject::OnCollision(WorldObject* other)
{
}

vector3 WorldObject::ToWorld(vector3 point)
{
	return transform * vector4(point, 1);
}

void WorldObject::CalculateGlobalMinMax()
{
	//Find the corners of the bounding box
	vector3 corners[8];

	corners[0] = ToWorld(center + vector3(-localHalfWidth.x, -localHalfWidth.y, -localHalfWidth.z));
	corners[1] = ToWorld(center + vector3(-localHalfWidth.x, -localHalfWidth.y, localHalfWidth.z));
	corners[2] = ToWorld(center + vector3(-localHalfWidth.x, localHalfWidth.y, -localHalfWidth.z));
	corners[3] = ToWorld(center + vector3(-localHalfWidth.x, localHalfWidth.y, localHalfWidth.z));
	corners[4] = ToWorld(center + vector3(localHalfWidth.x, -localHalfWidth.y, -localHalfWidth.z));
	corners[5] = ToWorld(center + vector3(localHalfWidth.x, -localHalfWidth.y, localHalfWidth.z));
	corners[6] = ToWorld(center + vector3(localHalfWidth.x, localHalfWidth.y, -localHalfWidth.z));
	corners[7] = ToWorld(center + vector3(localHalfWidth.x, localHalfWidth.y, localHalfWidth.z));

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

	//Recalculate global half width
	globalHalfWidth = (globalMax - globalMin) * 0.5f;
}

#pragma endregion