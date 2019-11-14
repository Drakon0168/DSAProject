#include "pch.h"
#include "PhysicsManager.h"
using namespace Simplex;
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Andrew Brook, Justin Dennis, Kent Reese, and Nicholas Zehl";

	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUpward(AXIS_Z * 10.0f, ZERO_V3, AXIS_Y);

	//init the camera
	m_pCamera = new MyCamera();
	m_pCamera->SetPositionTargetAndUpward(
			vector3(0.0f, 3.0f, 20.0f), //Where my eyes are
			vector3(0.0f, 3.0f, 19.0f), //where what I'm looking at is
			AXIS_Y);					//what is up

	PhysicsManager::GetInstance()->SetCamera(m_pCamera);

	//Get the singleton
	m_pMyMeshMngr = MyMeshManager::GetInstance();
	m_pMyMeshMngr->SetCamera(m_pCamera);

	ShowCursor(false);
	SetCursorPos((m_pSystem->GetWindowX() + m_pSystem->GetWindowWidth() / 2), (m_pSystem->GetWindowY() + m_pSystem->GetWindowHeight() / 2));
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();
	//TODO: Get delta time correctly using current and last time variables to avoid spike at launch
	float deltaTime = 1.0f / m_pSystem->GetFPS();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Physics
	PhysicsManager::GetInstance()->Update(deltaTime);

	//Add objects to the Manager
	//m_pMyMeshMngr->AddMeshToRenderList(m_pMyMeshMngr->GenerateCube(vector3(100, 0.2f, 100), vector3(0, 0, 0)),(glm::translate(vector3(0, 0.0f, 0))));
	//m_pMyMeshMngr->AddMeshToRenderList(m_pMyMeshMngr->GenerateCube(vector3(5, 5, 5), vector3(0, 1, 0)), (glm::translate(vector3(0, 0.0f, 0))));

}
void Application::Display(void)
{
	//Clear the screen
	ClearScreen();

	m_pMeshMngr->Render();

	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//Render the list of MyMeshManager
	m_pMyMeshMngr->Render();
	
	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the MyMeshManager list
	m_pMyMeshMngr->ClearRenderList();
	
	//draw gui
	DrawGUI();
	
	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//release the singleton
	MyMeshManager::ReleaseInstance();
	PhysicsManager::ReleaseInstance();

	//release the camera
	SafeDelete(m_pCamera);

	//release GUI
	ShutdownGUI();
}
