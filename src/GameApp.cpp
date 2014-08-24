#include "GameApp.h"


using namespace DirectX;

GameApp::GameApp()
{
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

float GameApp::Clamp(float x, float low, float high)
{
	return x < low ? low : (x > high ? high : x);
}
void GameApp::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi += dy;

		// Restrict the angle mPhi.
		mPhi = Clamp(mPhi, 0.1f, XM_PI - 0.1f);
	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.2 unit in the scene.
		float dx = 0.2f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.2f*static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = Clamp(mRadius, 50.0f, 500.0f);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}


void GameApp::Game_Render()
{

	m_Graphics.Clear();

	
	m_Grid.DrawGrid();
	// 2D Rendering; Disable Z-Buffer, Stencil,..etc
	mRenderStateHelper->SaveAll();
	m_Sprite->Begin();
	XMFLOAT2 pos(240, 400);
 	m_Sprite->Draw(m_SpriteTexture, pos);
	m_Sprite->End();

	m_Sprite->Begin();
	m_SpriteFont->DrawString(m_Sprite.get(), L"Mixing 2D/3D Graphics Demo", XMFLOAT2(10, 10));
	m_Sprite->End();
	 
	mRenderStateHelper->RestoreAll();

	m_Graphics.Render();
}

void GameApp::Game_Update()
{

	static float t = 0.0f;
	static ULONGLONG timeStart = 0;
	ULONGLONG timeCur = GetTickCount64();
	if (timeStart == 0)
		timeStart = timeCur;
	t = (timeCur - timeStart) / 1000.0f;
 
	// Convert Spherical to Cartesian coordinates.
	float x = mRadius * sinf(mPhi)*cosf(mTheta);
	float z = mRadius * sinf(mPhi)*sinf(mTheta);
	float y = mRadius * cosf(mPhi);
	m_Graphics.LookAt(x, y, z);
}

void GameApp::Game_CleanUp()
{
	m_Graphics.Release();
	m_Grid.Release(); 
}

void GameApp::Game_Init(HWND handle)
{

	HRESULT result = m_Graphics.Initialize(handle, true);
	m_Graphics.LookAt(0, 0, -5.0f);
	m_Graphics.SetPerspective(0.25f*DirectX::XM_PI, 0.01f, 100.0f);


	m_Grid.InitGrid(&m_Graphics);
	m_Grid.CreateGrid(160.0f, 160.0f, 20, 20);
	m_Grid.CompileFX();

    //HRESULT hr = m_Graphics.SetWireFrameMode(TRUE);

	result = CreateWICTextureFromFile(m_Graphics.getDevice(), m_Graphics.getContext(), L"Bird.png", NULL, &m_SpriteTexture, NULL);
	m_Sprite = std::shared_ptr<SpriteBatch>(new SpriteBatch(m_Graphics.getContext()));
	m_SpriteFont = std::shared_ptr<SpriteFont>(new SpriteFont(m_Graphics.getDevice(), L"Arial_14_Regular.spritefont"));

	mRenderStateHelper = new RenderStateHelper(&m_Graphics);
}
