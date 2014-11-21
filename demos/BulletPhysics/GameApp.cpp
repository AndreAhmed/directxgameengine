#include "GameApp.h"

using namespace std;
using namespace DirectX;

GameApp::GameApp()
{
	m_CubeAngle = 0;
}


GameApp::~GameApp()
{
}


void GameApp::OnMouseDown(WPARAM btnState, int x, int y, HWND handle)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	SetCapture(handle);
}

void GameApp::OnMouseUp(WPARAM btnState, int x, int y)
{
	ReleaseCapture();
}
 
void GameApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		m_Camera.Pitch(dy);
		m_Camera.RotateY(dx);

	}
 

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

// Render Update
void GameApp::Game_Render()
{
	m_Graphics.Clear();
	 
	if (m_pBoxMotionState) 
	{
		btTransform transform;
		// get the world transform from our motion state
		m_pBoxMotionState->getWorldTransform(transform);
	 	btQuaternion quat;
		//quat.setEuler(0, m_CubeAngle, 0); //or quat.setEulerZYX depending on the ordering you want
//	 	transform.setRotation(quat);
 		
		XMMATRIX world = btTransform_to_XMMATRIX(transform);
		m_Cube->Draw(world, m_Graphics.getViewMatrix(), m_Graphics.getProjectionMatrix());
	}

	if (m_pBoxSecondMotionState)
	{
		btTransform transform;
		// get the world transform from our motion state
		m_pBoxSecondMotionState->getWorldTransform(transform);
		btQuaternion quat;
		//quat.setEuler(0, m_CubeAngle, 0); //or quat.setEulerZYX depending on the ordering you want
		//	 	transform.setRotation(quat);

		XMMATRIX world = btTransform_to_XMMATRIX(transform);
		m_CubeSecond->Draw(world, m_Graphics.getViewMatrix(), m_Graphics.getProjectionMatrix());
	}
	if (m_pGroundMotionState)
	{
	    btTransform groundTransform; 
		m_pGroundMotionState->getWorldTransform(groundTransform);
		XMMATRIX transformatigon = btTransform_to_XMMATRIX(groundTransform);  
		m_Grid.DrawGrid();
	}
	
	Graphics_2D();

	m_Graphics.Render();
}

XMMATRIX GameApp::btTransform_to_XMMATRIX(btTransform const & trans)
{
	XMMATRIX transform = XMMatrixIdentity();
	XMFLOAT4X4 data;

	trans.getOpenGLMatrix((float*)&data);
	transform = (XMLoadFloat4x4(&data));
	return transform;
}


// all your 2D drawings call are called here
void GameApp::Graphics_2D()
{
	// 2D Rendering; Disable Z-Buffer, Stencil,..etc
	m_RenderStateHelper->SaveAll();

	m_Graphics.getSpriteBatch()->Begin();
	
	//m_Animated->Draw();
	Draw_Info();
	
	TwDraw();

	m_Graphics.getSpriteBatch()->End();

	m_RenderStateHelper->RestoreAll();
}

// Game Loop Update
void GameApp::Game_Update()
{

	float dt = m_Timer.GetTime();
	static float angle = 10.0f;
	angle += 5.f*dt;
	m_Camera.Update(dt);
	
	if (m_pWorld)
	{
		m_pWorld->stepSimulation(dt);
	}
	//angle += 0.05f*dt;
	//m_Animated->SetAngle(angle);
	//m_Animated->Update(dt);
}

void GameApp::Game_CleanUp()
{
	m_Graphics.Release();
	m_Grid.Release(); 
	m_Animated->Release();
	TwTerminate();

}
 
void GameApp::Game_Init(HWND handle)
{

	m_Graphics.Initialize(handle, true);
	m_Camera.Initialize();
	m_Camera.SetPerspective(0.25f*DirectX::XM_PI, m_Graphics.AspectRatio(), 0.01f, 1000.0f);
	m_Camera.SetPosition(XMFLOAT3(-140.0f, 18.0f, -8.0f));
	m_Camera.RotateY(-30.0f);
	m_Graphics.SetCamera(&m_Camera);
	//HRESULT hr = m_Graphics.SetWireFrameMode(TRUE);

	m_Graphics.CreateSpriteBatch();

	m_Grid.InitGrid(&m_Graphics);
	m_Grid.CompileFX();
	m_Grid.CreateGrid(360.0f, 360.0f, 50, 50);
	m_Grid.SetPosition(1, -90, 1);
	
 
	m_SpriteFont = std::shared_ptr<SpriteFont>(new SpriteFont(m_Graphics.getDevice(), L"Arial_14_Regular.spritefont"));

	m_RenderStateHelper = new RenderStateHelper(&m_Graphics);

	m_Animated = std::shared_ptr<cAnimatedSprite>(new cAnimatedSprite(&m_Graphics, "bird.png", 2, 1, 0.3f, true));
	m_Animated->SetPos(XMFLOAT2(240, 250));

	m_Cube = GeometricPrimitive::CreateCube(m_Graphics.getContext(), 5.0f, false);
	m_CubeSecond = GeometricPrimitive::CreateCube(m_Graphics.getContext(), 5.0f, false);

	m_pCollisionConfiguration = new btDefaultCollisionConfiguration();
	// create the dispatcher
	m_pDispatcher = new btCollisionDispatcher(m_pCollisionConfiguration);
	// create the broadphase
	m_pBroadphase = new btDbvtBroadphase();
	// create the constraint solver
	m_pSolver = new btSequentialImpulseConstraintSolver();
	// create the world
	m_pWorld = new btDiscreteDynamicsWorld(m_pDispatcher, m_pBroadphase, m_pSolver, m_pCollisionConfiguration);
	m_pWorld->setGravity(btVector3(0, -10, 0));
  
	btBoxShape* pBoxShape = new btBoxShape(btVector3(2, 2, 2));
	// give our box an initial position of (0,0,0)
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(0.0f, 10, 0.0f));
	btVector3 inertia(0, 0, 0);
	pBoxShape->calculateLocalInertia(5.0f, inertia);
	// create a motion state
	m_pBoxMotionState = new btDefaultMotionState(transform);
	// create the rigid body construction info object, giving it a 
	btRigidBody::btRigidBodyConstructionInfo rbInfo(5.0f, m_pBoxMotionState, pBoxShape, inertia);
	btRigidBody* pRigidBody = new btRigidBody(rbInfo);
  	m_pWorld->addRigidBody(pRigidBody);


	btBoxShape* pBoxSecondShape = new btBoxShape(btVector3(2, 2, 2));
	// give our box an initial position of (0,0,0)
	btTransform transformSecond;
	transformSecond.setIdentity();
	transformSecond.setOrigin(btVector3(0.0f, 0, 0.0f));
	btVector3 inertiaSecond(0, 0, 0);
	pBoxSecondShape->calculateLocalInertia(5.0f, inertiaSecond);
	// create a motion state
	m_pBoxSecondMotionState = new btDefaultMotionState(transformSecond);
	// create the rigid body construction info object, giving it a 
	btRigidBody::btRigidBodyConstructionInfo rbInfoSecond(5.0f, m_pBoxSecondMotionState, pBoxSecondShape, inertiaSecond);
	btRigidBody* pRigidBodySecond = new btRigidBody(rbInfoSecond);
	m_pWorld->addRigidBody(pRigidBodySecond);

	//// ground 
	btCollisionShape* pGroundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 0);
	btTransform groundShapeTransform;
	groundShapeTransform.setIdentity();
	groundShapeTransform.setOrigin(btVector3(1, -90, 1));
	 
	// create a motion state
	m_pGroundMotionState = new btDefaultMotionState(groundShapeTransform);
	btRigidBody::btRigidBodyConstructionInfo rbGroundInfo(0, m_pGroundMotionState, pGroundShape);
 
	btRigidBody* pRigidBodyGround = new btRigidBody(rbGroundInfo);

	// inform our world that we just created a new rigid body for 
	// it to manage
	m_pWorld->addRigidBody(pRigidBodyGround);
	
	TwInit(TW_DIRECT3D11, m_Graphics.getDevice());
	TwWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	TwBar *bar = TwNewBar("Mixing 2D and 3D");
	TwDefine("Mixing 2D/3D Graphics Demo, Keys WASD, and Mouse LB (FPS Camera) ");  
	// Add rotation and scale vars to bar
	TwAddVarRW(bar, "Rotation", TW_TYPE_FLOAT, &m_CubeAngle, "Rotates the cube ");
	m_Timer.Initialize();
}

void GameApp::Draw_Info()
{
	int fps = m_Timer.GetFps();
	ostringstream ss;
	ss << fps;
	string fpsString = "FPS " + ss.str();
	wstring widestrFPS = wstring(fpsString.begin(), fpsString.end());
 	m_SpriteFont->DrawString(m_Graphics.getSpriteBatch().get(), widestrFPS.c_str(), XMFLOAT2(WINDOW_WIDTH-100, 30), Colors::Red);
}

void GameApp::UpdateTimers()
{
	m_Timer.UpdateFPS();
	m_Timer.UpdateTimer();
}
