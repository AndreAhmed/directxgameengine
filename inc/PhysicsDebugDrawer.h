#pragma once

#include "btBulletCollisionCommon.h"
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include "Globals.h"
#include "Graphics.h"
#include "Effects.h"
#include <wrl.h>
//
class BulletDebugDrawer : public btIDebugDraw
{
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> line;
	cGraphics *m_pGraphics;
	std::unique_ptr<DirectX::BasicEffect> basicEffect;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;
	DebugDrawModes               m_DebugModes;
public:
	BulletDebugDrawer(cGraphics *graphics);
	virtual ~BulletDebugDrawer(void);

public:
	// btIDebugDraw interface
	virtual void	drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color) override;
	virtual void	reportErrorWarning(const char* warningString) override;
	virtual void	draw3dText(const btVector3& location,const char* textString) override;
 	virtual void	drawLine(const btVector3& from,const btVector3& to,const btVector3& color) override;
	virtual void	setDebugMode(int debugMode) override;
	virtual int		getDebugMode() const override;
	btIDebugDraw * CreatePhysicsSystem(cGraphics *graphics)
	{
		return new BulletDebugDrawer(graphics);
	}
};


