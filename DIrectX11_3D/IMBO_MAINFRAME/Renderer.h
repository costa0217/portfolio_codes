#pragma once

#include "SingleTon.h"

//
#include "DirectionalLight.h"
#include "TerrainContainer.h"
#include "SkyBoxContainer.h"
#include "ObjectRenderer.h"
#include "BigWaterRenderer.h"
#include "AORenderer.h"
#include "LightRenderer.h"
#include "BloomDownScale.h"
#include "Bloom.h"
#include "Blur.h"
#include "PostProcessingFinalPass.h"
#include "SSLR.h"
#include "SSRF.h"

#include "Shadow.h"
#include "UIRenderer.h"
#include "WaterRenderer.h"
#include "EffectRenderCont.h"
#include "FinalRenderer.h"

class CDirectXFramework;

struct stCSGlobalBufferData {
	UINT nWidth{ 0 };//1
	UINT nHeight{ 0 };
	UINT nHalfWidth{ 0 };//1/2
	UINT nHalfHeight{ 0 };
	UINT nQuadWidth{ 0 };//1/4
	UINT nQuadHeight{ 0 };
	UINT pad[2];
};
class CRenderer : public CSingleTonBase<CRenderer> {

public:
	bool Begin();
	bool End();


	void PreRender();
	void NoPostProcessRender( CCamera* pCamera);
	void Render( CCamera* pCamera);
	void PostProcessing( CCamera* pCamera);
	void Update(float fTimeElapsed);

	void SetForwardRenderTargets(ID3D11DepthStencilView* pDepthStencil);
	void SetMainRenderTargetView();
	void SetRenderTargetViews(UINT nRenderTarget, ID3D11RenderTargetView** pd3dRTVs, ID3D11DepthStencilView* pd3ddsvDepthStencil);

	//bool CreateSwapChain();
	bool CreateRenderTargetView();
	void ReleaseForwardRenderTargets();
	void ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView);

	bool ResizeBuffer();
	void SetFramework(CDirectXFramework* pFramework) { m_pFramework = pFramework; }

	CRenderContainer*	GetTerrainRenderContainer() { return m_pObjectRenderer->GetTerrainRenderContainer(); }
	CRenderContainer*	GetSkyBoxRenderContainer() { return m_pObjectRenderer->GetSkyBoxRenderContainer(); }
	CObjectRenderer*	GetObjectRenderer() { return m_pObjectRenderer; }
	CShadow*			GetShadow() { return m_pShadow; }
	CUIRenderer*		GetUIRenderer() { return m_pUIRederer; }
	CEffectRenderCont*	GetEffectRenderer() { return m_pEffectRenderer; }


	void LoadEffectInfo(wstring wsOutputPath, wstring wsSceneName);
private:
	 CBuffer* m_pCSGlobalBuffer{ nullptr };
	CDirectXFramework* m_pFramework{ nullptr };
	CRenderContainer* m_pTerrainRenderContainer{ nullptr };
	CRenderContainer* m_pSkyBoxRenderContainer{ nullptr };

	//IDXGISwapChain			*	m_pdxgiSwapChain{ nullptr };
	ID3D11RenderTargetView	*	m_pd3dRenderTargetView{ nullptr };

	ID3D11DepthStencilState*	m_pd3dDSSEarlyZWrite{ nullptr };
	ID3D11DepthStencilState*	m_pd3dDSSEarlyZRead{ nullptr };
	ID3D11DepthStencilView		*m_pd3ddsvReadOnlyDepthStencil{ nullptr };

	//---------------------------deferred rendering----------------------
	ID3D11Texture2D			 *m_pd3dtxtDepthStencil{ nullptr };
	ID3D11Texture2D			 *m_pd3dtxtColorSpecInt{ nullptr };//0
	ID3D11Texture2D			 *m_pd3dtxtNormal{ nullptr };//1
	ID3D11Texture2D			 *m_pd3dtxtSpecPow{ nullptr };//2
	ID3D11Texture2D			 *m_pd3dtxtSkyBox{ nullptr };//2
	ID3D11Texture2D			 *m_pd3dtxtPostProcess{ nullptr };//2

	ID3D11ShaderResourceView *m_pd3dsrvDepthStencil{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvColorSpecInt{ nullptr };//0
	ID3D11ShaderResourceView *m_pd3dsrvNormal{ nullptr };//1
	ID3D11ShaderResourceView *m_pd3dsrvSpecPow{ nullptr };//2
	ID3D11ShaderResourceView *m_pd3dsrvSkyBox{ nullptr };//2
	ID3D11ShaderResourceView *m_pd3dsrvPostProcess{ nullptr };//2

	ID3D11DepthStencilView	 *m_pd3ddsvDepthStencil{ nullptr };
	ID3D11RenderTargetView   *m_pd3drtvColorSpecInt{ nullptr };//0
	ID3D11RenderTargetView   *m_pd3drtvNormal{ nullptr };//1
	ID3D11RenderTargetView   *m_pd3drtvSpecPow{ nullptr };//2
	ID3D11RenderTargetView   *m_pd3drtvSkyBox{ nullptr };//2
	ID3D11RenderTargetView	 *m_pd3drtvPostProcess{ nullptr };//2


	//vector< CTexture>> m_vObjectLayerResultTexture;
	//vector< CTexture>> m_vLightLayerResultTexture;
	vector< CTexture*> m_vObjectLayerResultTexture;
	vector< CTexture*> m_vLightLayerResultTexture;

	//--------------------------light render target----------------
	ID3D11Texture2D			 *m_pd3dtxtLight{ nullptr };
	ID3D11ShaderResourceView *m_pd3dsrvLight{ nullptr };
	ID3D11RenderTargetView   *m_pd3drtvLight{ nullptr };
	//--------------------------light render target----------------
	//-------------------------layer-------------------------
	CObjectRenderer*	m_pObjectRenderer{ nullptr };
	CBigWaterRenderer* m_pBigWaterRenderer{ nullptr };
	CAORenderer*		m_pAORenderer{ nullptr };
	CLightRenderer*		m_pLightRenderer{ nullptr };
	CSSRF*				m_pRefrectionRenderer{ nullptr };
	CBloomDownScale*	m_pBloomDownScale{ nullptr };
	CBloom*				m_pBloom{ nullptr };
	CBlur*				m_p16to1Blur{ nullptr };
	CBlur*				m_p4to1Blur{ nullptr };
	CPostProcessingFinalPass* m_pPostProcessingFinalPass{ nullptr };
	CSSLR*				m_pSSLR{ nullptr };
	CShadow*			m_pShadow{ nullptr };

	CUIRenderer*		m_pUIRederer{ nullptr };
	CWaterRenderer*		m_pWaterRenderer{ nullptr };
	CEffectRenderCont*	m_pEffectRenderer{ nullptr };
	CFinalRenderer*		m_pFinalRenderer{ nullptr };
	//-------------------------layer-------------------------

private:
	bool	m_bRadialBlur{ false };
	float	m_fRBlurMaxTime{ 0.f };
	float	m_fRBlurAccTime{ 0.f };
	float	m_fTimeDelta{ 0.f };
	float	m_fBlurStart{ 0.f };
	float	m_fBlurWitdh{ 0.f };

	bool m_bDIFFUSE{ false };
	bool m_bLIGHTMAP{ false };
	bool m_bAO{ true };
	bool m_bShadow{ true };
	bool m_bSSLR{ true };
	bool m_bBLOOM{ true };

	bool m_bPresentaition{ false };
	UINT m_nPresentaition{ 0 };

public:
	void	SetRadialBlurTime(bool bBlurSwitch, float fTime) { 
		m_bRadialBlur = bBlurSwitch; m_fRBlurMaxTime = fTime;
		m_fRBlurAccTime = 0.f;
	}
	void	SetMissionFail();
	void	SetMissionSuccess();

	//effects
	//back  buffer color
	float m_fBackBufferClearColor[4];

	// temp
	int m_iOption{ 0 };

public:
	CRenderer();
	virtual ~CRenderer();
};

