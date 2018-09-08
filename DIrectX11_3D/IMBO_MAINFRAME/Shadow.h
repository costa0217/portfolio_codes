#pragma once
#include "DXObject.h"

struct stShadowInfo {
	float bias{ 0.039f };
	float bias_offset{ 0.0105f };
	//UINT pad[2];
	float kernelhalf{ 1.f };
	float dist_sum{ 0.07f };
	XMMATRIX xmmtxViewProj;
};

class CShadow :
	public DXObject
{
public:
	bool Begin();
	virtual bool End();

	virtual void SetShaderState();
	virtual void CleanShaderState();

	virtual void UpdateShaderState();

	ID3D11ShaderResourceView* RenderShadowMap( CCamera* pCamera = nullptr);

	void ResizeBuffer();
	void ReleaseBuffer();

	void SetBias(float bias) { m_pShadowInfo->bias = bias; }
	void SetBiasOffset(float bias_offset) { m_pShadowInfo->bias_offset = bias_offset; }

	void LoadShadow(wstring wsOutputPath, wstring wsSceneName);

	void SetbShadow(bool b) { m_bShadow = b; }
	bool GetbShadow() { return m_bShadow; }
private:
	 CTexture*	m_pShadowTexture{ nullptr };
	ID3D11Texture2D			 *m_pd3dtxtShadow{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvShadow{ nullptr };
	ID3D11DepthStencilView	 *m_pd3ddsvShadow{ nullptr };

	ID3D11Texture2D			 *m_pd3dtxtStaticShadow{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvStaticShadow{ nullptr };
	ID3D11DepthStencilView	 *m_pd3ddsvStaticShadow{ nullptr };

	ID3D11RasterizerState* m_pd3dRSShader{ nullptr };
	//shadow buffer
	 CBuffer* m_pShadowBuf{ nullptr };
	 CBuffer* m_pStaticShadowBuf{ nullptr };

	stShadowInfo* m_pShadowInfo{ nullptr };

private:
	bool m_bShadow{ true };

	XMFLOAT4X4			m_xmmtxShadowVP;
	XMMATRIX			m_xmmtxStaticShadowVP;
	 CBuffer*	m_pShadowVPBuffer;
	 CCamera* m_pCamera{ nullptr };
public:
	CShadow();
	~CShadow();
};

