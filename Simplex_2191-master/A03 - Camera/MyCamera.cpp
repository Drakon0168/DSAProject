#include "pch.h"
using namespace Simplex;

//Accessors
void Simplex::MyCamera::SetPosition(vector3 a_v3Position) { m_v3Position = a_v3Position; CameraManager::GetInstance()->SetPosition(a_v3Position); }
vector3 Simplex::MyCamera::GetPosition(void) { return m_v3Position; }
void Simplex::MyCamera::SetTarget(vector3 a_v3Target) { m_v3Target = a_v3Target; CameraManager::GetInstance()->SetPosition(a_v3Target); }
vector3 Simplex::MyCamera::GetTarget(void) { return m_v3Target; }
void Simplex::MyCamera::SetAbove(vector3 a_v3Above) { m_v3Above = a_v3Above; CameraManager::GetInstance()->SetPosition(a_v3Above); }
vector3 Simplex::MyCamera::GetAbove(void) { return m_v3Above; }
void Simplex::MyCamera::SetPerspective(bool a_bPerspective) { m_bPerspective = a_bPerspective; }
void Simplex::MyCamera::SetFOV(float a_fFOV) { m_fFOV = a_fFOV; }
void Simplex::MyCamera::SetResolution(vector2 a_v2Resolution) { m_v2Resolution = a_v2Resolution; }
void Simplex::MyCamera::SetNearFar(vector2 a_v2NearFar) { m_v2NearFar = a_v2NearFar; }
void Simplex::MyCamera::SetHorizontalPlanes(vector2 a_v2Horizontal) { m_v2Horizontal = a_v2Horizontal; }
void Simplex::MyCamera::SetVerticalPlanes(vector2 a_v2Vertical) { m_v2Vertical = a_v2Vertical; }
matrix4 Simplex::MyCamera::GetProjectionMatrix(void) { return m_m4Projection; }
matrix4 Simplex::MyCamera::GetViewMatrix(void) { CalculateViewMatrix(); return m_m4View; }
vector3 transformForward;
vector3 transformRight;
vector3 transformUp;

Simplex::MyCamera::MyCamera()
{
	Init(); //Init the object with default values
}

Simplex::MyCamera::MyCamera(vector3 a_v3Position, vector3 a_v3Target, vector3 a_v3Upward)
{
	Init(); //Initialize the object
	SetPositionTargetAndUpward(a_v3Position, a_v3Target, a_v3Upward); //set the position, target and upward
}

Simplex::MyCamera::MyCamera(MyCamera const& other)
{
	m_v3Position = other.m_v3Position;
	m_v3Target = other.m_v3Target;
	m_v3Above = other.m_v3Above;

	m_bPerspective = other.m_bPerspective;

	m_fFOV = other.m_fFOV;

	m_v2Resolution = other.m_v2Resolution;
	m_v2NearFar = other.m_v2NearFar;

	m_v2Horizontal = other.m_v2Horizontal;
	m_v2Vertical = other.m_v2Vertical;

	m_m4View = other.m_m4View;
	m_m4Projection = other.m_m4Projection;
}

MyCamera& Simplex::MyCamera::operator=(MyCamera const& other)
{
	if (this != &other)
	{
		Release();
		SetPositionTargetAndUpward(other.m_v3Position, other.m_v3Target, other.m_v3Above);
		MyCamera temp(other);
		Swap(temp);
	}
	return *this;
}

void Simplex::MyCamera::Init(void)
{
	ResetCamera();
	CalculateProjectionMatrix();
	CalculateViewMatrix();
	transformForward = vector3(0, 0, 1);
	//No pointers to initialize here
}

void Simplex::MyCamera::Release(void)
{
	//No pointers to deallocate
}

void Simplex::MyCamera::Swap(MyCamera & other)
{
	std::swap(m_v3Position, other.m_v3Position);
	std::swap(m_v3Target, other.m_v3Target);
	std::swap(m_v3Above, other.m_v3Above);
	
	std::swap(m_bPerspective, other.m_bPerspective);

	std::swap(m_fFOV, other.m_fFOV);

	std::swap(m_v2Resolution, other.m_v2Resolution);
	std::swap(m_v2NearFar, other.m_v2NearFar);

	std::swap(m_v2Horizontal, other.m_v2Horizontal);
	std::swap(m_v2Vertical, other.m_v2Vertical);

	std::swap(m_m4View, other.m_m4View);
	std::swap(m_m4Projection, other.m_m4Projection);
}

Simplex::MyCamera::~MyCamera(void)
{
	Release();
}

void Simplex::MyCamera::ResetCamera(void)
{
	m_v3Position = vector3(0.0f, 0.0f, 10.0f); //Where my camera is located
	m_v3Target = vector3(0.0f, 0.0f, 0.0f); //What I'm looking at
	m_v3Above = vector3(0.0f, 1.0f, 0.0f); //What is above the camera

	m_bPerspective = true; //perspective view? False is Orthographic

	m_fFOV = 45.0f; //Field of View

	m_v2Resolution = vector2(1280.0f, 720.0f); //Resolution of the window
	m_v2NearFar = vector2(0.001f, 1000.0f); //Near and Far planes

	m_v2Horizontal = vector2(-5.0f, 5.0f); //Orthographic horizontal projection
	m_v2Vertical = vector2(-5.0f, 5.0f); //Orthographic vertical projection

	CalculateProjectionMatrix();
	CalculateViewMatrix();

	CameraManager::GetInstance()->ResetCamera();
}

void Simplex::MyCamera::SetPositionTargetAndUpward(vector3 a_v3Position, vector3 a_v3Target, vector3 a_v3Upward)
{
	m_v3Position = a_v3Position;
	m_v3Target = a_v3Target;

	m_v3Above = a_v3Position + glm::normalize(a_v3Upward);
	
	CameraManager::GetInstance()->SetPositionTargetAndUpward(a_v3Position, a_v3Target, a_v3Upward);

	//Calculate the Matrix
	CalculateProjectionMatrix();
}

void Simplex::MyCamera::CalculateViewMatrix(void)
{
	//Calculate the look at most of your assignment will be reflected in this method
	m_m4View = glm::lookAt(m_v3Position, m_v3Target, glm::normalize(m_v3Above - m_v3Position)); //position, target, upward
}

void Simplex::MyCamera::CalculateProjectionMatrix(void)
{
	float fRatio = m_v2Resolution.x / m_v2Resolution.y;
	if (m_bPerspective) //perspective
	{
		m_m4Projection = glm::perspective(m_fFOV, fRatio, m_v2NearFar.x, m_v2NearFar.y);
	}
	else //Orthographic
	{
		m_m4Projection = glm::ortho(m_v2Horizontal.x * fRatio, m_v2Horizontal.y * fRatio, //horizontal
			m_v2Vertical.x, m_v2Vertical.y, //vertical
			m_v2NearFar.x, m_v2NearFar.y); //near and far
	}
}

void MyCamera::MoveForward(float a_fDistance)
{
	CameraManager::GetInstance()->MoveForward(a_fDistance);

	//Update Vectors
	UpdateForward();

	//Update Values
	m_v3Position += (transformForward * a_fDistance);
	m_v3Target += (transformForward * a_fDistance);
	m_v3Above += (transformForward * a_fDistance);
}

void MyCamera::MoveLeft(float a_fDistance)
{
	CameraManager::GetInstance()->MoveSideways(-a_fDistance);

	//Update Vectors
	UpdateForward();

	//Update Values
	m_v3Position += (transformRight * a_fDistance);
	m_v3Target += (transformRight * a_fDistance);
	m_v3Above += (transformRight * a_fDistance);
}

void MyCamera::MoveUp(float a_fDistance)
{
	CameraManager::GetInstance()->MoveVertical(a_fDistance);

	//Update Vectors
	UpdateForward();

	m_v3Position += (transformUp * a_fDistance);
	m_v3Target += (transformUp * a_fDistance);
	m_v3Above += (transformUp * a_fDistance);
}

void MyCamera::RotateLeft(float a_fDistance)
{
	CameraManager::GetInstance()->ChangePitch(-a_fDistance);

	//Update Vectors
	UpdateForward();

	//Get Rotated Vector
	glm::quat newRotation = glm::angleAxis(a_fDistance, (transformRight));
	//std::cout << "New Rotation: " << newRotation.x << " | " << newRotation.y << " | " << newRotation.z << std::endl;
	vector3 rotatedVector = ((newRotation * transformForward) * glm::conjugate(newRotation));
	//std::cout << "rotatedVector: " << rotatedVector.x << " | " << rotatedVector.y << " | " << rotatedVector.z << std::endl;

	//Update Target
	m_v3Target = (m_v3Position + rotatedVector);
}

void MyCamera::RotateDown(float a_fDistance)
{
	CameraManager::GetInstance()->ChangeYaw(a_fDistance);

	//Update Vectors
	UpdateForward();

	//Get Rotated Vector
	glm::quat newRotation = glm::angleAxis(a_fDistance, vector3(0, 1, 0));
	//std::cout << "New Rotation: " << newRotation.x << " | " << newRotation.y << " | " << newRotation.z << std::endl;
	vector3 rotatedVector = ((newRotation * transformForward) * glm::conjugate(newRotation));
	//std::cout << "Totated Vector: " << rotatedVector.x << " | " << rotatedVector.y << " | " << rotatedVector.z << std::endl;

	//Update Target
	m_v3Target = (m_v3Position + rotatedVector);
}

//Gets Forward Vector by subtracting the position of the target by the current position and normalizing it for direction
//After, the right vector is found by doing the cross product of the unit y vector with the forward vector
void MyCamera::UpdateForward()
{
	transformForward = glm::normalize(m_v3Target - m_v3Position);
	//std::cout << "transformForward: " << transformForward.x << " | " << transformForward.y << " | " << transformForward.z << std::endl;

	transformRight = glm::normalize(glm::cross(vector3(0, 1, 0), transformForward));
	//std::cout << "Right Cross: " << rightCross.x << " | " << rightCross.y << " | " << rightCross.z << std::endl;

	transformUp = glm::normalize(m_v3Above - m_v3Position);
	//std::cout << "Transform Up: " << transformUp.x << " | " << transformUp.y << " | " << transformUp.z << std::endl;
}