#include "stdafx.h"
#include "EffectRenderCont.h"
//#include "ResourceMgr.h"


//static ID3D11SamplerState* pSampler{ nullptr };

CEffectRenderCont::CEffectRenderCont()
{

}
CEffectRenderCont::~CEffectRenderCont()
{
	if (m_pd3dtxtDistortion) m_pd3dtxtDistortion->Release();
	if (m_pd3dsrvDistortion) m_pd3dsrvDistortion->Release();
	if (m_pd3drtvDistortion) m_pd3drtvDistortion->Release();

	if (m_pd3dtxtBlend) m_pd3dtxtBlend->Release();
	if (m_pd3dsrvBlend) m_pd3dsrvBlend->Release();
	if (m_pd3drtvBlend) m_pd3drtvBlend->Release();
	//if (m_pd3ddsvEffect) m_pd3ddsvEffect->Release();

	if (m_pd3dtxtEffect) m_pd3dtxtEffect->Release();
	if (m_pd3dsrvEffect) m_pd3dsrvEffect->Release();
	if (m_pd3drtvEffect) m_pd3drtvEffect->Release();

	if (m_pd3dtxtEffectDepth) m_pd3dtxtEffectDepth->Release();
	if (m_pd3ddsvEffectDepth) m_pd3ddsvEffectDepth->Release();
	if (m_pd3dsrvEffectDepth) m_pd3dsrvEffectDepth->Release();

}
HRESULT CEffectRenderCont::Initialize()
{
	ResizeBuffer();
	m_pRenderShader = RESOURCEMGR->GetRenderShader("Effect");
	m_pDistRShader = RESOURCEMGR->GetRenderShader("Blend");
	m_pTrailShader = RESOURCEMGR->GetRenderShader("Trail");

	D3D11_RASTERIZER_DESC	descRasterizer;
	ZeroMemory(&descRasterizer, sizeof(D3D11_RASTERIZER_DESC));
	descRasterizer.FillMode = D3D11_FILL_SOLID;
	descRasterizer.CullMode = D3D11_CULL_NONE;
	GLOBALVALUEMGR->GetDevice()->CreateRasterizerState(&descRasterizer, &m_EffectRasterizerState);
	//GLOBALVALUEMGR->GetDeviceContext()->RSSetState(m_EffectRasterizerState);

	D3D11_BLEND_DESC descBlend;
	ZeroMemory(&descBlend, sizeof(D3D11_BLEND_DESC));
	descBlend.AlphaToCoverageEnable = TRUE;
	descBlend.IndependentBlendEnable = FALSE;
	descBlend.RenderTarget[0].BlendEnable = TRUE;
	descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_GREEN | D3D11_COLOR_WRITE_ENABLE_BLUE;
	//blend state
	GLOBALVALUEMGR->GetDevice()->CreateBlendState(&descBlend, &m_pAlphaBlendState);

	m_pMesh = RESOURCEMGR->GetMesh("DirectionalLight");

	//GLOBALVALUEMGR->GetDevice()->CreateSamplerState(&d3dSamplerDesc, &pSampler);

	return S_OK;
}
//렌더러에서 호출.
void CEffectRenderCont::RenderEffect()
{
	//render target set
	ID3D11RenderTargetView *pd3dRTVs[] = { m_pd3drtvEffect, m_pd3drtvEffectAlpha };
	float fClearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	if (m_pd3drtvEffect) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvEffect, fClearColor);
	if (m_pd3drtvEffectAlpha) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvEffectAlpha, fClearColor);
	GLOBALVALUEMGR->GetDeviceContext()->ClearDepthStencilView(m_pd3ddsvEffectDepth, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(2, pd3dRTVs, m_pd3ddsvEffectDepth);

	

	ID3D11RasterizerState*	preRasterizerState;
	GLOBALVALUEMGR->GetDeviceContext()->RSGetState(&preRasterizerState);
	GLOBALVALUEMGR->GetDeviceContext()->RSSetState(m_EffectRasterizerState);

	//객체 렌더
	GLOBALVALUEMGR->GetDeviceContext()->OMGetBlendState(&m_pPreBlendState, m_pPreBlendFactor, &m_PreSampleMask);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pAlphaBlendState, nullptr, 0xffffffff);
	//GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(6, 1, &pSRVDepth);

	if (false == m_mapSkill.empty()) {
		////공통 set
		multimap<float, CGameObject*>::iterator iter = m_mapSkill.begin();
		multimap<float, CGameObject*>::iterator iter_End = m_mapSkill.end();
		for (; iter != iter_End; ++iter)
		{
			//iter->second->r
		}
		m_mapSkill.clear();		// 다그리고 제거e
	}
	

	if (false == m_mapEffect.empty()) {
		////공통 set
		m_pRenderShader->UpdateShaderState();
		m_pRenderShader->SetShaderState();
		multimap<float, CMyEffect*>::iterator iter = m_mapEffect.begin();
		multimap<float, CMyEffect*>::iterator iter_End = m_mapEffect.end();
		for (; iter != iter_End; ++iter)
		{
			iter->second->Render();
		}
		m_pRenderShader->CleanShaderState();
		m_mapEffect.clear();		// 다그리고 제거e
	}

	if (false == m_mapTrail.empty()) {

		m_pTrailShader->UpdateShaderState();
		m_pTrailShader->SetShaderState();
		multimap<float, CTrail*>::iterator iter = m_mapTrail.begin();
		multimap<float, CTrail*>::iterator iter_End = m_mapTrail.end();
		for (; iter != iter_End; ++iter)
		{
			iter->second->Render();
		}
		m_pTrailShader->CleanShaderState();
		m_mapTrail.clear();
	}

	GLOBALVALUEMGR->GetDeviceContext()->RSSetState(preRasterizerState);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pPreBlendState, m_pPreBlendFactor, m_PreSampleMask);

	//DEBUGER->AddTexture(XMFLOAT2(750, 0), XMFLOAT2(1000, 150), m_pd3dsrvEffect);
	//DEBUGER->AddTexture(XMFLOAT2(750, 300), XMFLOAT2(1000, 450), m_pd3dsrvEffectAlpha);
}

void CEffectRenderCont::RenderParticle()
{
	if (m_mapParticle.empty()) return;

	////공통 set
	//m_pRenderShader->UpdateShaderState();
	//m_pRenderShader->SetShaderState();

	////객체 렌더
	//GLOBALVALUEMGR->GetDeviceContext()->OMGetBlendState(&m_pPreBlendState, m_pPreBlendFactor, &m_PreSampleMask);
	//GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pAlphaBlendState, nullptr, 0xffffffff);
	//multimap<float, CMyParticle*>::iterator iter = m_mapParticle.begin();
	//multimap<float, CMyParticle*>::iterator iter_End = m_mapParticle.end();
	//for (; iter != iter_End; ++iter)
	//{
	//	iter->second->Render();
	//}
	//m_pRenderShader->CleanShaderState();

	//m_mapParticle.clear();		// 다그리고 제거e

	//GLOBALVALUEMGR->GetDeviceContext()->CSSetSamplers(0, 1, &pSampler);
	//GLOBALVALUEMGR->GetDeviceContext()->PSSetSamplers(0, 1, &pSampler);

	//GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pPreBlendState, nullptr, 0xffffffff);
}

ID3D11ShaderResourceView* CEffectRenderCont::RenderDistortionEffect(ID3D11DepthStencilView* pDepthStencilView)
{
	ID3D11RenderTargetView *pd3dRTVs[1] = { m_pd3drtvDistortion };
	float fClearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	if (m_pd3drtvDistortion) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvDistortion, fClearColor);
	if (m_mapDistortEffect.empty()) return nullptr;


	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, pd3dRTVs, pDepthStencilView);

	m_pRenderShader->UpdateShaderState();
	m_pRenderShader->SetShaderState();

	//객체 렌더
	GLOBALVALUEMGR->GetDeviceContext()->OMGetBlendState(&m_pPreBlendState, m_pPreBlendFactor, &m_PreSampleMask);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pAlphaBlendState, nullptr, 0xffffffff);
	multimap<float, CMyEffect*>::iterator iter = m_mapDistortEffect.begin();
	multimap<float, CMyEffect*>::iterator iter_End = m_mapDistortEffect.end();
	for (; iter != iter_End; ++iter)
	{
		iter->second->Render();
	}

	m_pRenderShader->CleanShaderState();
	m_mapDistortEffect.clear();		// 다그리고 제거
	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pPreBlendState, m_pPreBlendFactor, m_PreSampleMask);


	//GLOBALVALUEMGR->GetDeviceContext()->CSSetSamplers(0, 1, &pSampler);
	//GLOBALVALUEMGR->GetDeviceContext()->PSSetSamplers(0, 1, &pSampler);

	//DEBUGER->AddTexture(XMFLOAT2(1000, 0), XMFLOAT2(1250, 150), m_pd3dsrvDistortion);
	return m_pd3dsrvDistortion;
}

ID3D11ShaderResourceView* CEffectRenderCont::BlendingDistortion(ID3D11ShaderResourceView* pSRV, ID3D11ShaderResourceView* pDepthSRV)
{
	ID3D11RenderTargetView *pd3dRTVs[2] = { m_pd3drtvBlend, m_pd3drtvBlendAlpha };
	float fClearColor[4] = { 1.f, 1.f, 1.f, 1.f };
	if (m_pd3drtvBlend) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvBlend, fClearColor);

	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(2, pd3dRTVs, nullptr);

	m_pDistRShader->UpdateShaderState();
	m_pDistRShader->SetShaderState();

	ID3D11ShaderResourceView* pLightSRV[1] = { pSRV };
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(1, 1, &m_pd3dsrvEffectAlpha);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(2, 1, &m_pd3dsrvEffect);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(3, 1, pLightSRV);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(4, 1, &m_pd3dsrvDistortion);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(5, 1, &pDepthSRV);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(6, 1, &m_pd3dsrvEffectDepth);

	// screen mesh render
	m_pMesh->Render(1);
	m_pDistRShader->CleanShaderState();


	ID3D11ShaderResourceView* pSRVnull[1] = { nullptr };
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(1, 1, pSRVnull);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(2, 1, pSRVnull);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(3, 1, pSRVnull);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(4, 1, pSRVnull);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(5, 1, pSRVnull);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(6, 1, pSRVnull);

	//DEBUGER->AddTexture(XMFLOAT2(1000, 0), XMFLOAT2(1250, 150), m_pd3dsrvBlend);
	//DEBUGER->AddTexture(XMFLOAT2(1000, 150), XMFLOAT2(1250, 300), m_pd3dsrvBlendAlpha);
	//DEBUGER->AddDepthTexture(XMFLOAT2(1000, 300), XMFLOAT2(1250, 450), m_pd3dsrvEffectDepth);
	return m_pd3dsrvBlend;
}

void CEffectRenderCont::ResizeBuffer()
{
	D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
	::ZeroMemory(&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dTexture2DDesc.Width = GLOBALVALUEMGR->GetrcClient().right;
	d3dTexture2DDesc.Height = GLOBALVALUEMGR->GetrcClient().bottom;
	d3dTexture2DDesc.MipLevels = 1;
	d3dTexture2DDesc.ArraySize = 1;
	d3dTexture2DDesc.SampleDesc.Count = 1;
	d3dTexture2DDesc.SampleDesc.Quality = 0;
	d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DDesc.CPUAccessFlags = 0;
	d3dTexture2DDesc.MiscFlags = 0;


	D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc;
	::ZeroMemory(&d3dSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	d3dSRVDesc.Texture2D.MipLevels = 1;

	D3D11_RENDER_TARGET_VIEW_DESC d3dRTVDesc;
	::ZeroMemory(&d3dRTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
	d3dRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	d3dRTVDesc.Texture2D.MipSlice = 0;
	d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtDistortion);
	GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtDistortion, &d3dRTVDesc, &m_pd3drtvDistortion);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtDistortion, &d3dSRVDesc, &m_pd3dsrvDistortion);

	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtBlend);
	GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtBlend, &d3dRTVDesc, &m_pd3drtvBlend);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtBlend, &d3dSRVDesc, &m_pd3dsrvBlend);

	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtEffect);
	GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtEffect, &d3dRTVDesc, &m_pd3drtvEffect);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtEffect, &d3dSRVDesc, &m_pd3dsrvEffect);

	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtEffectAlpha);
	GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtEffectAlpha, &d3dRTVDesc, &m_pd3drtvEffectAlpha);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtEffectAlpha, &d3dSRVDesc, &m_pd3dsrvEffectAlpha);

	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtBlendAlpha);
	GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtBlendAlpha, &d3dRTVDesc, &m_pd3drtvBlendAlpha);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtBlendAlpha, &d3dSRVDesc, &m_pd3dsrvBlendAlpha);

	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;
	d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;

	D3D11_TEXTURE2D_DESC d3dTexture2DDescDepth;
	::ZeroMemory(&d3dTexture2DDescDepth, sizeof(D3D11_TEXTURE2D_DESC));
	d3dTexture2DDescDepth.Width = GLOBALVALUEMGR->GetrcClient().right;
	d3dTexture2DDescDepth.Height = GLOBALVALUEMGR->GetrcClient().bottom;
	d3dTexture2DDescDepth.MipLevels = 1;
	d3dTexture2DDescDepth.ArraySize = 1;
	d3dTexture2DDescDepth.SampleDesc.Count = 1;
	d3dTexture2DDescDepth.SampleDesc.Quality = 0;
	d3dTexture2DDescDepth.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DDescDepth.CPUAccessFlags = 0;
	d3dTexture2DDescDepth.MiscFlags = 0;
	d3dTexture2DDescDepth.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	d3dTexture2DDescDepth.Format = DXGI_FORMAT_R32_TYPELESS;

	D3D11_SHADER_RESOURCE_VIEW_DESC d3dSDepthRVDesc;
	::ZeroMemory(&d3dSDepthRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dSDepthRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	d3dSDepthRVDesc.Texture2D.MipLevels = 1;
	d3dSDepthRVDesc.Format = DXGI_FORMAT_R32_FLOAT;

	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDescDepth, nullptr, &m_pd3dtxtEffectDepth);
	GLOBALVALUEMGR->GetDevice()->CreateDepthStencilView(m_pd3dtxtEffectDepth, &d3dDepthStencilViewDesc, &m_pd3ddsvEffectDepth);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtEffectDepth, &d3dSDepthRVDesc, &m_pd3dsrvEffectDepth);
}
