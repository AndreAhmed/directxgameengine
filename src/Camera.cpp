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
	m_Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
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


void cCamera::SetPosition(const XMFLOAT3& position)
{
	m_Position = position;
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
	XMVECTOR eyePosition = XMLoadFloat3(&m_Position);
	XMVECTOR direction = XMLoadFloat3(&m_Forward);
	XMVECTOR upDirection = XMLoadFloat3(&m_Up);

	XMMATRIX viewMatrix = XMMatrixLookToLH(eyePosition, direction, upDirection);
	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

}


void cCamera::Update(float dt)
{
	if (GetAsyncKeyState('W') & 0x8000)
		Walk(45 * dt);

	if (GetAsyncKeyState('S') & 0x8000)
		Walk(-45 * dt);

	if (GetAsyncKeyState('A') & 0x8000)
		Strafe(-45 * dt);

	if (GetAsyncKeyState('D') & 0x8000)
		Strafe(45 * dt);

	UpdateViewMatrix();
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
