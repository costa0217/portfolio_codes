#pragma once

#include "MyEffect.h"
#include "Trail.h"

#include "RenderShader.h"
#include "Texture.h"
#include "Buffer.h"
#include "Material.h"


class CMyParticle;

class CEffectRenderCont
{
public:
	CEffectRenderCont();
	~CEffectRenderCont();

public:
	HRESULT Initialize();
	void SetRenderContainer(float fZ, CMyEffect* pEffect, bool bDistortion = false) {
		if (bDistortion == false) m_mapEffect.insert(make_pair(fZ, pEffect));
		else m_mapDistortEffect.insert(make_pair(fZ, pEffect));
	}
	void SetParticleRenderContainer(float fZ, CMyParticle* pParticle) { m_mapParticle.insert(make_pair(fZ, pParticle)); }
	void SetTrailRenderContainer(float fZ, CTrail* pTrail) { m_mapTrail.insert(make_pair(fZ, pTrail)); }
	void RenderEffect();
	void RenderParticle();
	ID3D11ShaderResourceView* RenderDistortionEffect(ID3D11DepthStencilView* pDepthStencilView);
	ID3D11ShaderResourceView* BlendingDistortion(ID3D11ShaderResourceView* pSRV, ID3D11ShaderResourceView* pDepthSRV);

public:
	ID3D11ShaderResourceView* GetAlphaSRV() { return m_pd3dsrvBlendAlpha; }
	ID3D11ShaderResourceView* GetDistortionSRV() { return m_pd3dsrvDistortion; }

private:
	multimap<float, CMyEffect*, greater<float>> m_mapEffect;
	multimap<float, CMyEffect*, greater<float>> m_mapDistortEffect;
	multimap<float, CMyParticle*, greater<float>> m_mapParticle;
	multimap<float, CTrail*, greater<float>> m_mapTrail;
	multimap<float, CGameObject*, greater<float>> m_mapSkill;

	CRenderShader*	m_pRenderShader{ nullptr };
	CRenderShader*	m_pTrailShader{ nullptr };
	CRenderShader*	m_pDistRShader{ nullptr };
	CMesh*			m_pMesh{ nullptr };
	//CBuffer*		m_pBuffer{ nullptr };

	ID3D11RasterizerState*	m_EffectRasterizerState;

	ID3D11BlendState*	m_pAlphaBlendState;
	ID3D11BlendState*	m_pPreBlendState{ nullptr };
	float* m_pPreBlendFactor{ nullptr };
	UINT m_PreSampleMask{ 0 };

public:
	void	ResizeBuffer();

private:
	ID3D11Texture2D			 *m_pd3dtxtDistortion{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvDistortion{ nullptr };
	ID3D11RenderTargetView	 *m_pd3drtvDistortion{ nullptr };

	ID3D11Texture2D			 *m_pd3dtxtBlend{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvBlend{ nullptr };
	ID3D11RenderTargetView	 *m_pd3drtvBlend{ nullptr };

	ID3D11Texture2D			 *m_pd3dtxtEffectDepth{ nullptr };
	ID3D11DepthStencilView	 *m_pd3ddsvEffectDepth{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvEffectDepth{ nullptr };

	ID3D11Texture2D			 *m_pd3dtxtEffect{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvEffect{ nullptr };
	ID3D11RenderTargetView	 *m_pd3drtvEffect{ nullptr };

	ID3D11Texture2D			 *m_pd3dtxtEffectAlpha{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvEffectAlpha{ nullptr };
	ID3D11RenderTargetView	 *m_pd3drtvEffectAlpha{ nullptr };

	ID3D11Texture2D			 *m_pd3dtxtBlendAlpha{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvBlendAlpha{ nullptr };
	ID3D11RenderTargetView	 *m_pd3drtvBlendAlpha{ nullptr };

public:
	CCamera*		m_pCamera{ nullptr };
};

