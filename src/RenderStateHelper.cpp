#include "RenderStateHelper.h"
 
 
    RenderStateHelper::RenderStateHelper(cGraphics *graphics)
		: m_Graphics(graphics), m_RasterizerState(nullptr), m_BlendState(nullptr), m_BlendFactor(new FLOAT[4]), m_SampleMask(UINT_MAX), m_DepthStencilState(nullptr), m_StencilRef(UINT_MAX)
    {
    }

    RenderStateHelper::~RenderStateHelper()
    {
	
    }

	void RenderStateHelper::Release()
	{
		ReleaseCOM(m_RasterizerState);
		ReleaseCOM(m_BlendState);
		ReleaseCOM(m_DepthStencilState);
		DeleteObjects(m_BlendFactor);
	}

    void RenderStateHelper::ResetAll(ID3D11DeviceContext* deviceContext)
    {
        deviceContext->RSSetState(nullptr);
        deviceContext->OMSetBlendState(nullptr, nullptr, UINT_MAX);
        deviceContext->OMSetDepthStencilState(nullptr, UINT_MAX);
    }

	ID3D11RasterizerState* RenderStateHelper::RasterizerState()
	{
		return m_RasterizerState;
	}

    ID3D11BlendState* RenderStateHelper::BlendState()
	{
		return m_BlendState;
	}

    ID3D11DepthStencilState* RenderStateHelper::DepthStencilState()
	{
		return m_DepthStencilState;
	}

    void RenderStateHelper::SaveRasterizerState()
    {
		ReleaseCOM(m_RasterizerState);
        m_Graphics->getContext()->RSGetState(&m_RasterizerState);
    }

    void RenderStateHelper::RestoreRasterizerState() const
    {
		m_Graphics->getContext()->RSSetState(m_RasterizerState);
    }

    void RenderStateHelper::SaveBlendState()
    {
		ReleaseCOM(m_BlendState);
		m_Graphics->getContext()->OMGetBlendState(&m_BlendState, m_BlendFactor, &m_SampleMask);
    }

    void RenderStateHelper::RestoreBlendState() const
    {
		m_Graphics->getContext()->OMSetBlendState(m_BlendState, m_BlendFactor, m_SampleMask);
    }

    void RenderStateHelper::SaveDepthStencilState()
    {
		ReleaseCOM(m_DepthStencilState);
		m_Graphics->getContext()->OMGetDepthStencilState(&m_DepthStencilState, &m_StencilRef);
    }

    void RenderStateHelper::RestoreDepthStencilState() const
    {
		m_Graphics->getContext()->OMSetDepthStencilState(m_DepthStencilState, m_StencilRef);
    }

    void RenderStateHelper::SaveAll()
    {
        SaveRasterizerState();
        SaveBlendState();
        SaveDepthStencilState();
    }

    void RenderStateHelper::RestoreAll() const
    {
        RestoreRasterizerState();
        RestoreBlendState();
        RestoreDepthStencilState();
    }
 
