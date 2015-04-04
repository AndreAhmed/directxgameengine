#include "PhysicsDebugDrawer.h"
#include "CommonStates.h"

using namespace DirectX;
void BulletDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{
	// draw a line to represent the normal.  This only lasts one frame, and is hard to see.
	//   it might help to linger this drawn object onscreen for a while to make it more noticeable
	
	btVector3 const startPoint = PointOnB;
	btVector3 const endPoint = PointOnB + normalOnB * distance;
	
	drawLine( startPoint, endPoint, color );
}

void BulletDebugDrawer::reportErrorWarning(const char* warningString)
{
     
}

void BulletDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{
	// FUTURE WORK - BulletDebugDrawer::draw3dText needs an implementation
}



void BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& lineColor)
{
	CommonStates states(m_pGraphics->getDevice());
	m_pGraphics->getContext()->OMSetBlendState(states.Opaque(), nullptr, 0xFFFFFFFF);
	m_pGraphics->getContext()->OMSetDepthStencilState(states.DepthNone(), 0);
 	m_pGraphics->getContext()->RSSetState(states.CullCounterClockwise());

	basicEffect->Apply(m_pGraphics->getContext());
	m_pGraphics->getContext()->IASetInputLayout(inputLayout.Get());
	basicEffect->SetView(m_pGraphics->getViewMatrix());
	basicEffect->SetProjection(m_pGraphics->getProjectionMatrix());
	line->Begin();
	line->DrawLine(VertexPositionColor(XMFLOAT3(from.x(), from.y(), from.z()), XMFLOAT4(lineColor.getX(), lineColor.getY(), lineColor.getZ(), 1)),
		VertexPositionColor(XMFLOAT3(to.x(), to.y(), to.z()), XMFLOAT4(lineColor.getX(), lineColor.getY(), lineColor.getZ(), 1)));
 
	line->DrawLine(VertexPositionColor(XMVectorSet(120, 50, 215, 1), Colors::Red),
		VertexPositionColor(XMVectorSet(40, 30, 10, 1), Colors::Red));

	line->End(); 
 
}

void BulletDebugDrawer::setDebugMode(int debugMode)
{
	m_DebugModes = (DebugDrawModes)debugMode;
}

int BulletDebugDrawer::getDebugMode() const
{
	return m_DebugModes;
}
BulletDebugDrawer::BulletDebugDrawer(cGraphics *graphics)
{
	if (graphics != nullptr)
	{
		m_pGraphics = graphics;
	}

	line = std::unique_ptr<PrimitiveBatch<VertexPositionColor>>(new PrimitiveBatch<VertexPositionColor>(m_pGraphics->getContext()));
	basicEffect = std::unique_ptr<BasicEffect>(new BasicEffect(m_pGraphics->getDevice()));
	basicEffect->SetVertexColorEnabled(true);
	void const* shaderByteCode;
	size_t byteCodeLength;

	basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);

	m_pGraphics->getDevice()->CreateInputLayout(VertexPositionColor::InputElements,
		VertexPositionColor::InputElementCount,
		shaderByteCode, byteCodeLength,
		inputLayout.GetAddressOf());

}

BulletDebugDrawer::~BulletDebugDrawer(void)
{
	 
}



