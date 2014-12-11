#pragma once

#include "Globals.h"
#include "GameObject.h"

class cCamera : public cGameObject
{
	RTTI_DECLARATIONS(cCamera, cGameObject);
public:
	cCamera();
	virtual ~cCamera();
	  void Initialize()  ;
	  void Update(float dt)  ;

	void SetPerspective(float fov, float aspect, float nearPlane, float farPlane);
	void SetOrthographic(float viewWidth, FLOAT viewHeight, float zn, float zf);
	void LookAt(const DirectX::XMVECTOR& target);
	
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_Forward;
	DirectX::XMFLOAT3 m_Up;
	DirectX::XMFLOAT3 m_Right;

	float m_FieldOfView;
	float m_AspectRatio;
	float m_NearPlaneDistance;
	float m_FarPlaneDistance;

	DirectX::XMFLOAT4X4 m_ViewMatrix;
	DirectX::XMFLOAT4X4 m_ProjectionMatrix;

public:
	const DirectX::XMFLOAT3& Position() const;
	const DirectX::XMFLOAT3& Forward() const;
	const DirectX::XMFLOAT3& Up() const;
	const DirectX::XMFLOAT3& Right() const;
 
	float AspectRatio() const;
	float FieldOfView() const;
	float NearPlaneDistance() const;
	float FarPlaneDistance() const;

	DirectX::XMMATRIX ViewMatrix();
	DirectX::XMMATRIX ProjectionMatrix();
	DirectX::XMMATRIX ViewProjectionMatrix();
 
	void SetPosition(FLOAT x, FLOAT y, FLOAT z);
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	void ApplyRotation(const DirectX::XMFLOAT4X4& transform);
	void Reset();
	void Pitch(float angle);
	void RotateY(float angle);
	void Walk(float d);
	void Strafe(float d);
    void MoveUp(float d);

};