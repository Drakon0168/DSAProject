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


	m_pCamera->drawDebugUI = false;

	PhysicsManager::GetInstance()->SetCamera(m_pCamera);

	//Get the singleton
	m_pMyMeshMngr = MyMeshManager::GetInstance();
	m_pMyMeshMngr->SetCamera(m_pCamera);

	ShowCursor(false);
	SetCursorPos((m_pSystem->GetWindowX() + m_pSystem->GetWindowWidth() / 2), (m_pSystem->GetWindowY() + m_pSystem->GetWindowHeight() / 2));
	
	clock = m_pSystem->GenClock();
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();
	float deltaTime = m_pSystem->GetDeltaTime(clock);

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	if (!m_paused)
	{
		CameraRotation();
	}

	//Update Physics
	PhysicsManager::GetInstance()->Update(deltaTime);
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
