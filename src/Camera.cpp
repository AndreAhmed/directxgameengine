#include "Camera.h"
 
RTTI_DEFINITIONS(cCamera)
using namespace DirectX;

cCamera::cCamera()
{


}
cCamera::~cCamera()
{


}
void cCamera::Initialize()
{
	Reset();
}


void cCamera::Reset()
{
	// config for left hand camera system
	m_Position = XMFLOAT3(0.0f, 3.0f, -150.0f);
	m_Forward = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_Up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_Right = XMFLOAT3(1.0f, 0.0f, 0.0f);

	UpdateViewMatrix();
}

const XMFLOAT3& cCamera::Position() const
{
	return m_Position;
}

const XMFLOAT3& cCamera::Forward() const
{
	return m_Forward;
}

const XMFLOAT3& cCamera::Up() const
{
	return m_Up;
}

const XMFLOAT3& cCamera::Right() const
{
	return m_Right;
}

float cCamera::AspectRatio() const
{
	return m_AspectRatio;
}

float cCamera::FieldOfView() const
{
	return m_FieldOfView;
}

float cCamera::NearPlaneDistance() const
{
	return m_NearPlaneDistance;
}

float cCamera::FarPlaneDistance() const
{
	return m_FarPlaneDistance;
}

XMMATRIX cCamera::ViewMatrix()
{
	return XMLoadFloat4x4(&m_ViewMatrix);
}

XMMATRIX cCamera::ProjectionMatrix()
{
	return XMLoadFloat4x4(&m_ProjectionMatrix);
}

XMMATRIX cCamera::ViewProjectionMatrix()
{
	XMMATRIX viewMatrix = XMLoadFloat4x4(&m_ViewMatrix);
	XMMATRIX projectionMatrix = XMLoadFloat4x4(&m_ProjectionMatrix);

	return XMMatrixMultiply(viewMatrix, projectionMatrix);
}

void cCamera::SetPosition(FLOAT x, FLOAT y, FLOAT z)
{
	XMVECTOR position = XMVectorSet(x, y, z, 0);
	XMStoreFloat3(&m_Position, position);
}

void cCamera::SetPerspective(float fov, float aspect, float nearPlane, float farPlane)
{
	m_FieldOfView = fov;
	m_AspectRatio = aspect;
	m_NearPlaneDistance = nearPlane;
	m_FarPlaneDistance = farPlane;

	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(m_FieldOfView, m_AspectRatio, m_NearPlaneDistance, m_FarPlaneDistance);
	XMStoreFloat4x4(&m_ProjectionMatrix, projectionMatrix);
}
void cCamera::SetOrthographic(float viewWidth, float viewHeight, float zn, float zf)
{
	XMMATRIX projectionMatrix = XMMatrixOrthographicLH(viewWidth, viewHeight, zn, zf);
	XMStoreFloat4x4(&m_ProjectionMatrix, projectionMatrix);
}
void cCamera::LookAt(const XMVECTOR &target)
{
	XMVECTOR p = XMLoadFloat3(&m_Position);
	XMVECTOR look = target - p;
	XMStoreFloat3(&m_Forward, look);
}

void cCamera::UpdateViewMatrix()
{
	XMVECTOR R = XMLoadFloat3(&m_Right);
	XMVECTOR U = XMLoadFloat3(&m_Up);
	XMVECTOR L = XMLoadFloat3(&m_Forward);
	XMVECTOR P = XMLoadFloat3(&m_Position);

	// Keep camera's axes orthogonal to each other and of unit length.
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = XMVector3Cross(U, L);

	// Fill in the view matrix entries.
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&m_Right, R);
	XMStoreFloat3(&m_Up, U);
	XMStoreFloat3(&m_Forward, L);

	m_ViewMatrix(0, 0) = m_Right.x;
	m_ViewMatrix(1, 0) = m_Right.y;
	m_ViewMatrix(2, 0) = m_Right.z;
	m_ViewMatrix(3, 0) = x;

	m_ViewMatrix(0, 1) = m_Up.x;
	m_ViewMatrix(1, 1) = m_Up.y;
	m_ViewMatrix(2, 1) = m_Up.z;
	m_ViewMatrix(3, 1) = y;

	m_ViewMatrix(0, 2) = m_Forward.x;
	m_ViewMatrix(1, 2) = m_Forward.y;
	m_ViewMatrix(2, 2) = m_Forward.z;
	m_ViewMatrix(3, 2) = z;

	m_ViewMatrix(0, 3) = 0.0f;
	m_ViewMatrix(1, 3) = 0.0f;
	m_ViewMatrix(2, 3) = 0.0f;
	m_ViewMatrix(3, 3) = 1.0f;
}


void cCamera::Update(float dt)
{
	if (GetAsyncKeyState('W') & 0x8000)
		Walk(35 * dt);

	if (GetAsyncKeyState('S') & 0x8000)
		Walk(-35 * dt);

	if (GetAsyncKeyState('A') & 0x8000)
		Strafe(-35 * dt);

	if (GetAsyncKeyState('D') & 0x8000)
		Strafe(35 * dt);

	if (GetAsyncKeyState('Z') & 0x8000)
		MoveUp(-35 * dt);

	if (GetAsyncKeyState('X') & 0x8000)
		MoveUp(35 * dt);

	UpdateViewMatrix();
}

void cCamera::MoveUp(float d)
{
	// mPosition += d*Up
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR u = XMLoadFloat3(&m_Up);
	XMVECTOR p = XMLoadFloat3(&m_Position);
	XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(s, u, p));
}

void cCamera::Strafe(float d)
{
	// mPosition += d*Right
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR r = XMLoadFloat3(&m_Right);
	XMVECTOR p = XMLoadFloat3(&m_Position);
	XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(s, r, p));
}

void cCamera::Walk(float d)
{
	// mPosition += d*Forward
	XMVECTOR s = XMVectorReplicate(d);
	XMVECTOR l = XMLoadFloat3(&m_Forward);
	XMVECTOR p = XMLoadFloat3(&m_Position);
	XMStoreFloat3(&m_Position, XMVectorMultiplyAdd(s, l, p));
}

void cCamera::Pitch(float angle)
{
	// Rotate up and forward vector about the right vector.

	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&m_Right), angle);

	XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
	XMStoreFloat3(&m_Forward, XMVector3TransformNormal(XMLoadFloat3(&m_Forward), R));
}

void cCamera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	XMMATRIX R = XMMatrixRotationY(angle);

	XMStoreFloat3(&m_Right, XMVector3TransformNormal(XMLoadFloat3(&m_Right), R));
	XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
	XMStoreFloat3(&m_Forward, XMVector3TransformNormal(XMLoadFloat3(&m_Forward), R));
}
