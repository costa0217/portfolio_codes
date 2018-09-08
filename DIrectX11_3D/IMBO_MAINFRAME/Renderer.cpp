
#include "stdafx.h"
#include "Renderer.h"

static bool bSSAO{ false }; 
bool CRenderer::Begin() {
	m_pCSGlobalBuffer = RESOURCEMGR->CreateConstantBuffer("CSGlobalBuffer", 1, sizeof(stCSGlobalBufferData), 13, BIND_CS);

	//layer
	m_pObjectRenderer = new CObjectRenderer();
	m_pObjectRenderer->Begin();
	m_pAORenderer = new CAORenderer();
	m_pAORenderer->Begin();

	m_pLightRenderer = new CLightRenderer();
	m_pLightRenderer->Begin();

	//post processing
	m_pBloomDownScale = new CBloomDownScale();	m_pBloomDownScale->Begin();
	m_pBloom = new CBloom();					m_pBloom->Begin();
	m_p16to1Blur = new CBlur();					m_p16to1Blur->Begin();
	m_p4to1Blur = new CBlur();					m_p4to1Blur->Begin();
	m_pPostProcessingFinalPass = new CPostProcessingFinalPass();	m_pPostProcessingFinalPass->Begin();
	m_pSSLR = new CSSLR();						m_pSSLR->Begin();
	m_pRefrectionRenderer = new CSSRF();		m_pRefrectionRenderer->Begin();
	//layer

	//shadow
	m_pShadow = new CShadow();
	m_pShadow->Begin();

	//UIRenderer 
	m_pUIRederer = new CUIRenderer();
	m_pUIRederer->Initialize();
	m_pBigWaterRenderer = new CBigWaterRenderer();
	m_pBigWaterRenderer->Begin();

	m_pWaterRenderer = new CWaterRenderer();
	m_pWaterRenderer->Begin();

	m_pEffectRenderer = new CEffectRenderCont();
	m_pEffectRenderer->Initialize();

	m_pFinalRenderer = new CFinalRenderer();
	m_pFinalRenderer->Initialize();

	// render target과 depth-stencil buffer 생성/ deferred texture 생성
	if (!CreateRenderTargetView()) {
		MessageBox(GLOBALVALUEMGR->GethWnd(), TEXT("RenderTarget이나 Depth-Stencil 버퍼 생성이 실패했습니다. 프로그램을 종료합니다."), TEXT("프로그램 구동 실패"), MB_OK);
		return false;
	}
	return true;
}

bool CRenderer::End() {
	m_pCSGlobalBuffer = nullptr;

	//if (m_pdxgiSwapChain) m_pdxgiSwapChain->Release();
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3ddsvReadOnlyDepthStencil) m_pd3ddsvReadOnlyDepthStencil->Release();

	ReleaseForwardRenderTargets();

	if (m_pd3dDSSEarlyZRead) m_pd3dDSSEarlyZRead->Release();
	m_pd3dDSSEarlyZRead = nullptr;

	if (m_pd3dDSSEarlyZWrite) m_pd3dDSSEarlyZWrite->Release();
	m_pd3dDSSEarlyZWrite = nullptr;

	//layer
	Safe_EndDelete(m_pObjectRenderer);
	Safe_EndDelete(m_pBigWaterRenderer);
	Safe_EndDelete(m_pAORenderer);
	Safe_EndDelete(m_pLightRenderer);
	Safe_EndDelete(m_pBloomDownScale);
	Safe_EndDelete(m_pBloom);
	Safe_EndDelete(m_p16to1Blur);
	Safe_EndDelete(m_p4to1Blur);
	Safe_EndDelete(m_pPostProcessingFinalPass);
	Safe_EndDelete(m_pSSLR);
	Safe_EndDelete(m_pShadow);
	Safe_EndDelete(m_pWaterRenderer);
	Safe_EndDelete(m_pRefrectionRenderer);
	//Safe_Delete(m_pUIRederer);
	Safe_Delete(m_pEffectRenderer);
	Safe_Delete(m_pFinalRenderer);

	if (m_pUIRederer)
		delete m_pUIRederer;

	return true;
}

void CRenderer::PreRender()
{
	//CLEAR
	RESOURCEMGR->GetSampler("WRAP_LINEAR")->SetShaderState();
	RESOURCEMGR->GetSampler("WRAP_POINT")->SetShaderState();
	RESOURCEMGR->GetSampler("CLAMP_LINEAR")->SetShaderState();

	//CLEAR
	ClearDepthStencilView(m_pd3ddsvDepthStencil);
	float fClearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	if (m_pd3dRenderTargetView) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3dRenderTargetView, fClearColor);


	SetMainRenderTargetView();
	if (nullptr != m_pUIRederer) {
		m_pUIRederer->RenderUI();
	}

	HRESULT hr = GLOBALVALUEMGR->GetSwapChain()->Present(0, 0);
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
		DEBUGER->DebugMessageBox("explosion", "d");	return;
	}
}

void CRenderer::NoPostProcessRender( CCamera* pCamera)
{
	bool bDebug = INPUTMGR->GetDebugMode();
	pCamera->SetShaderState();
	RESOURCEMGR->GetSampler("WRAP_LINEAR")->SetShaderState();
	RESOURCEMGR->GetSampler("WRAP_POINT")->SetShaderState();
	RESOURCEMGR->GetSampler("CLAMP_LINEAR")->SetShaderState();
	RESOURCEMGR->GetSampler("CLAMP_POINT")->SetShaderState();
	RESOURCEMGR->GetSampler("SHADOW")->SetShaderState();


	UPDATER->GetSpaceContainer()->PrepareRender(pCamera);
	UPDATER->GetSkyBoxContainer()->GetSkyBox()->RegistToContainer();

	// skybox
	pCamera->SetShaderState();

	ClearDepthStencilView(m_pd3ddsvDepthStencil);

	// object render
	////=> pre-z pass
	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pd3dDSSEarlyZWrite, 1);
	ID3D11RenderTargetView* pRTV = nullptr;
	SetRenderTargetViews(1, &pRTV, m_pd3ddsvDepthStencil);
	m_pObjectRenderer->Excute(pCamera);

	UPDATER->GetSpaceContainer()->PrepareRender(pCamera);
	UPDATER->GetSkyBoxContainer()->GetSkyBox()->RegistToContainer();

	////=> render pass
	if (true == bDebug) DEBUGER->start_Timemeasurement();
	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pd3dDSSEarlyZRead, 1);
	SetForwardRenderTargets(m_pd3ddsvReadOnlyDepthStencil);//gbuff가 될 rtv/ dsv set
	m_pObjectRenderer->Excute(pCamera);
	m_pObjectRenderer->CleanShaderState();

	
	if (true == bDebug) DEBUGER->end_Timemeasurement(L"object_render");
	GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pd3dDSSEarlyZWrite, 1);

	//m_pWaterRenderer->RenderWater(pCamera, m_pd3dsrvDepthStencil);

	// SSAO render
	if (true == bDebug) DEBUGER->start_Timemeasurement();
	SetRenderTargetViews(1, &m_pd3drtvLight, m_pd3ddsvReadOnlyDepthStencil);
	SetMainRenderTargetView(); 
	size_t iVecSize = m_vObjectLayerResultTexture.size();
	for (size_t i = 0; i < iVecSize; ++i) {
		m_vObjectLayerResultTexture[i]->SetShaderState();
	}

	ID3D11ShaderResourceView* pAmbientOcclution = m_pAORenderer->Excute(pCamera);
	pAmbientOcclution = m_p4to1Blur->Excute(pAmbientOcclution);
	if (true == bDebug) DEBUGER->end_Timemeasurement(L"ssao");
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(4, 1, &pAmbientOcclution);
	if (true == bDebug) DEBUGER->AddDepthTexture(XMFLOAT2(500, 500), XMFLOAT2(600, 600), pAmbientOcclution);

	// LIGHT RENDER
	if (true == bDebug) DEBUGER->start_Timemeasurement();
	m_pLightRenderer->Excute(pCamera, m_pShadow);
	if (true == bDebug) DEBUGER->end_Timemeasurement(L"light_render");


	if (true == bDebug) { DEBUGER->DebugRender(pCamera); }
	for (size_t i = 0; i < iVecSize; ++i) {
		m_vObjectLayerResultTexture[i]->CleanShaderState();
	}

	//water
	//m_pBigWaterRenderer->RenderBigWater(pCamera, m_pd3drtvLight, m_pd3ddsvReadOnlyDepthStencil, m_vObjectLayerResultTexture);
	//water

	m_pObjectRenderer->RenderSkyBox();
	m_pObjectRenderer->ClearSkyBox();


	if (nullptr != m_pUIRederer) {
		m_pUIRederer->RenderUI();
	}


	//DEBUG
	if (true == bDebug) {
		DEBUGER->AddDepthTexture(XMFLOAT2(500, 0), XMFLOAT2(750, 150), m_pd3dsrvDepthStencil);
		//	DEBUGER->AddTexture(XMFLOAT2(100, 0), XMFLOAT2(350, 250), pRFL);
		DEBUGER->AddTexture(XMFLOAT2(0, 0), XMFLOAT2(200, 200), m_vObjectLayerResultTexture[1]->GetShaderResourceView());
		DEBUGER->AddTexture(XMFLOAT2(0, 200), XMFLOAT2(200, 400), m_vObjectLayerResultTexture[2]->GetShaderResourceView());
		DEBUGER->AddTexture(XMFLOAT2(0, 400), XMFLOAT2(200, 600), m_vObjectLayerResultTexture[3]->GetShaderResourceView());
		DEBUGER->AddTexture(XMFLOAT2(0, 600), XMFLOAT2(200, 800), m_vLightLayerResultTexture[0]->GetShaderResourceView());
		DEBUGER->AddTexture(XMFLOAT2(200, 400), XMFLOAT2(400, 600), m_pd3dsrvPostProcess);

		//이건 꼭 여기서 해줘야함.
		DEBUGER->RenderTexture();
		DEBUGER->RenderText();
	}
	//DEBUGER->AddGameText(100, 100, 100, YT_Color(), L"TEST");
	DEBUGER->RenderGameText();
	DEBUGER->ClearDebuger();

	//PRESENT
	if (true == bDebug) DEBUGER->start_Timemeasurement();
	HRESULT hr = GLOBALVALUEMGR->GetSwapChain()->Present(0, 0);
	if (true == bDebug) DEBUGER->end_Timemeasurement(L"present");
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
		DEBUGER->DebugMessageBox("explosion", "d");
		return;
	}
}


void CRenderer::Render( CCamera* pCamera) {
	if (true == m_bRadialBlur)
	{
		m_fRBlurAccTime += m_fTimeDelta;
		//m_fBlurStart = 0.2f * (m_fRBlurAccTime / m_fRBlurMaxTime) + 0.8f;
		m_fBlurStart = 1.f;
		m_fBlurWitdh = 0.2f * (m_fRBlurAccTime / m_fRBlurMaxTime) - 0.2f;
		if (m_fRBlurAccTime > m_fRBlurMaxTime)
		{
			m_bRadialBlur = false;
			m_fRBlurAccTime = 0.f;
			m_fBlurStart = 0.f;
			m_fBlurWitdh = 0.f;
		}
	}

	bool bDebug = INPUTMGR->GetDebugMode();
	pCamera->SetShaderState();
	RESOURCEMGR->GetSampler("WRAP_LINEAR")->SetShaderState();
	RESOURCEMGR->GetSampler("WRAP_POINT")->SetShaderState();
	RESOURCEMGR->GetSampler("CLAMP_LINEAR")->SetShaderState();
	RESOURCEMGR->GetSampler("CLAMP_POINT")->SetShaderState();
	RESOURCEMGR->GetSampler("SHADOW")->SetShaderState();


	// shadow render
	m_pShadow->RenderShadowMap(pCamera);

	UPDATER->GetSpaceContainer()->PrepareRender(pCamera);
	UPDATER->GetSkyBoxContainer()->GetSkyBox()->RegistToContainer();

	// skybox
	pCamera->SetShaderState();
	ClearDepthStencilView(m_pd3ddsvDepthStencil);
	SetRenderTargetViews(1, &m_pd3drtvSkyBox, m_pd3ddsvDepthStencil);
	m_pObjectRenderer->RenderSkyBox();
	m_pObjectRenderer->ClearSkyBox();
	ClearDepthStencilView(m_pd3ddsvDepthStencil);

	// object render
	////=> pre-z pass
	//GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pd3dDSSEarlyZWrite, 1);
	//ID3D11RenderTargetView* pRTV = nullptr;
	//SetRenderTargetViews(1, &pRTV, m_pd3ddsvDepthStencil);
	//m_pObjectRenderer->Excute(pCamera);

	////=> render pass
	if (true == bDebug) DEBUGER->start_Timemeasurement();
	//GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pd3dDSSEarlyZRead, 1);
	SetForwardRenderTargets(m_pd3ddsvDepthStencil);//gbuff가 될 rtv/ dsv set
	m_pObjectRenderer->Excute(pCamera);
	m_pObjectRenderer->CleanShaderState();

	//m_pEffectRenderer->RenderEffect();

	//naviobject render
	if (true == bDebug) DEBUGER->end_Timemeasurement(L"object_render");
	//GLOBALVALUEMGR->GetDeviceContext()->OMSetDepthStencilState(m_pd3dDSSEarlyZWrite, 1);

	//m_pWaterRenderer->RenderWater(pCamera, m_pd3dsrvDepthStencil);

	// SSAO render 
	if (true == bDebug) DEBUGER->start_Timemeasurement(); 
	SetRenderTargetViews(1, &m_pd3drtvLight, m_pd3ddsvReadOnlyDepthStencil);

	size_t iVecSize = m_vObjectLayerResultTexture.size();
	for (size_t i = 0; i < iVecSize; ++i) {
		m_vObjectLayerResultTexture[i]->SetShaderState();
	}
	ID3D11ShaderResourceView* pAmbientOcclution;
	if (m_bAO) {
		pAmbientOcclution = m_pAORenderer->Excute(pCamera);
		pAmbientOcclution = m_p4to1Blur->Excute(pAmbientOcclution);
		GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(4, 1, &pAmbientOcclution);
	}
	else {
		pAmbientOcclution = RESOURCEMGR->GetTexture("DEFAULT")->GetShaderResourceView();
		GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(4, 1, &pAmbientOcclution);
	}
	if (true == bDebug) DEBUGER->AddDepthTexture(XMFLOAT2(500, 500), XMFLOAT2(600, 600), pAmbientOcclution);

	// LIGHT RENDER
	if (true == bDebug) DEBUGER->start_Timemeasurement();
	m_pLightRenderer->Excute(pCamera, m_pShadow);
	if (true == bDebug) DEBUGER->end_Timemeasurement(L"light_render");

	//// Effect render
	m_pEffectRenderer->RenderEffect();

	ID3D11ShaderResourceView* srvDistortion = m_pEffectRenderer->RenderDistortionEffect(m_pd3ddsvReadOnlyDepthStencil);

	// debug object redner
	SetRenderTargetViews(1, &m_pd3drtvLight, m_pd3ddsvReadOnlyDepthStencil);
	if (true == bDebug) {
		if (false == m_bDIFFUSE && false == m_bLIGHTMAP) {
			{ DEBUGER->DebugRender(pCamera); }
		}
	}
	for (size_t i = 0; i < iVecSize; ++i) {
		m_vObjectLayerResultTexture[i]->CleanShaderState();
	}
	//water
	m_pBigWaterRenderer->RenderBigWater(pCamera, m_pd3drtvLight, m_pd3ddsvReadOnlyDepthStencil, m_vObjectLayerResultTexture);
	//water

	//// water render
	DEBUGER->start_Timemeasurement();
	m_pRefrectionRenderer->Excute(pCamera, m_pd3drtvLight, m_pd3ddsvReadOnlyDepthStencil, m_pd3dsrvLight, m_pd3dsrvDepthStencil, m_pd3dsrvNormal);
	DEBUGER->end_Timemeasurement(L"ssrf");

	//// SSLR render
	if (m_bSSLR) {
		if (true == bDebug) DEBUGER->start_Timemeasurement();
		m_pSSLR->Excute(pCamera, m_pd3drtvLight, pAmbientOcclution);
		if (true == bDebug) DEBUGER->end_Timemeasurement(L"sslr");
	}

	if (false == m_bLIGHTMAP) {

		/*DEBUGER->start_Timemeasurement();
		m_pRefrectionRenderer->Excute(pCamera, m_pd3dRenderTargetView, m_pd3ddsvReadOnlyDepthStencil, m_pd3dsrvLight, m_pd3dsrvDepthStencil, m_pd3dsrvNormal);
		DEBUGER->end_Timemeasurement(L"ssrf");*/

		// effect blend
		ID3D11ShaderResourceView* pBlendSrv = m_pEffectRenderer->BlendingDistortion(m_pd3dsrvLight, m_pd3dsrvDepthStencil);

		// POST PROCESSING
		GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, &m_pd3drtvPostProcess, m_pd3ddsvReadOnlyDepthStencil);
		if (true == bDebug) DEBUGER->start_Timemeasurement();
		GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(0, 1, &pBlendSrv);
		GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(0, 1, &pBlendSrv);
		PostProcessing(pCamera);
		size_t iLightVecSize = m_vLightLayerResultTexture.size();

		ID3D11ShaderResourceView* pNull = nullptr;
		GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(0, 1, &pNull);
		GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(0, 1, &pNull);
		if (true == bDebug) DEBUGER->end_Timemeasurement(L"postprocessing");
		pCamera->SetShaderState();

		// final
		SetMainRenderTargetView();
		m_pFinalRenderer->RenderFinalPass(m_pd3dsrvPostProcess, m_pEffectRenderer->GetAlphaSRV(), m_pd3dsrvSkyBox, srvDistortion,
			m_bRadialBlur, m_fBlurStart, m_fBlurWitdh);
	}
	else {
		SetMainRenderTargetView();
	}
	if (nullptr != m_pUIRederer) {
		m_pUIRederer->RenderUI();
	}

	//DEBUG
	if (true == bDebug) {
		DEBUGER->AddDepthTexture(XMFLOAT2(500, 0), XMFLOAT2(750, 150), m_pd3dsrvDepthStencil);
		//	DEBUGER->AddTexture(XMFLOAT2(100, 0), XMFLOAT2(350, 250), pRFL);
		DEBUGER->AddTexture(XMFLOAT2(0, 0), XMFLOAT2(200, 200), m_vObjectLayerResultTexture[1]->GetShaderResourceView());
		DEBUGER->AddTexture(XMFLOAT2(0, 200), XMFLOAT2(200, 400), m_vObjectLayerResultTexture[2]->GetShaderResourceView());
		DEBUGER->AddTexture(XMFLOAT2(0, 400), XMFLOAT2(200, 600), m_vObjectLayerResultTexture[3]->GetShaderResourceView());
		DEBUGER->AddTexture(XMFLOAT2(0, 600), XMFLOAT2(200, 800), m_vLightLayerResultTexture[0]->GetShaderResourceView());
	
		DEBUGER->AddTexture(XMFLOAT2(200, 400), XMFLOAT2(400, 600), m_pd3dsrvPostProcess);

		if (m_bDIFFUSE) {
			RECT rcClient = GLOBALVALUEMGR->GetrcClient();
			DEBUGER->AddTexture(XMFLOAT2(0, 0), XMFLOAT2(rcClient.right, rcClient.bottom), m_vObjectLayerResultTexture[1]->GetShaderResourceView());
		}
		if (m_bLIGHTMAP) {
			RECT rcClient = GLOBALVALUEMGR->GetrcClient();
			DEBUGER->AddTexture(XMFLOAT2(0, 0), XMFLOAT2(rcClient.right, rcClient.bottom), m_vLightLayerResultTexture[0]->GetShaderResourceView());
		}

		DEBUGER->RenderTexture();
		if (false == m_bDIFFUSE && false == m_bLIGHTMAP) {
			DEBUGER->RenderText();
		}

	}
	//DEBUGER->AddGameText(100, 100, 100, YT_Color(), L"TEST");
	DEBUGER->RenderGameText();
	DEBUGER->ClearDebuger();

	//PRESENT
	if (true == bDebug) DEBUGER->start_Timemeasurement();
	HRESULT hr = GLOBALVALUEMGR->GetSwapChain()->Present(0, 0);
	if (true == bDebug) DEBUGER->end_Timemeasurement(L"present");
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
		DEBUGER->DebugMessageBox("explosion", "d");
		return;
	}
}
void CRenderer::Update(float fTimeElapsed) {
	if (m_bPresentaition) {
		switch (m_nPresentaition) {
		case 0:
			//difuse
			INPUTMGR->SetDebugMode(true);
			m_bDIFFUSE = true;
			m_bLIGHTMAP = false;
			m_bAO = false;
			m_pShadow->SetbShadow(false);
			m_bSSLR = false;
			m_bBLOOM = false;
			break;
		case 1:
			//light
			INPUTMGR->SetDebugMode(true);
			m_bDIFFUSE = false;
			m_bLIGHTMAP = true;
			m_bAO = false;
			m_pShadow->SetbShadow(false);
			m_bSSLR = false;
			m_bBLOOM = false;
			break;
		case 2:
			//light + ssao
			INPUTMGR->SetDebugMode(true);
			m_bDIFFUSE = false;
			m_bLIGHTMAP = true;
			m_bAO = true;
			m_pShadow->SetbShadow(false);
			m_bSSLR = false;
			m_bBLOOM = false;
			break;
		case 3:
			//light + ssao + shadow
			INPUTMGR->SetDebugMode(true);
			m_bDIFFUSE = false;
			m_bLIGHTMAP = true;
			m_bAO = true;
			m_pShadow->SetbShadow(true);
			m_bSSLR = false;
			m_bBLOOM = false;
			break;
		case 4:
			//light + ssao + shadow + sslr
			INPUTMGR->SetDebugMode(true);
			m_bDIFFUSE = false;
			m_bLIGHTMAP = true;
			m_bAO = true;
			m_pShadow->SetbShadow(true);
			m_bSSLR = true;
			m_bBLOOM = false;
			break;
		case 5:
			//light + ssao + shadow + sslr + bloom
			INPUTMGR->SetDebugMode(false);
			m_bDIFFUSE = false;
			m_bLIGHTMAP = false;
			m_bAO = true;
			m_pShadow->SetbShadow(true);
			m_bSSLR = true;
			m_bBLOOM = true;
			break;
		default:
			break;
		}
	}
	if (INPUTMGR->KeyDown(VK_MULTIPLY_)) {
		m_bPresentaition = (m_bPresentaition + 1) % 2;
		if (m_bPresentaition) {
			//시연 on
			INPUTMGR->SetDebugMode(true);
			m_nPresentaition = 0;
		}
		else {
			//시연 off
			INPUTMGR->SetDebugMode(false);
			m_bDIFFUSE	= false;
			m_bLIGHTMAP	= false;
			m_bAO		= true;
			m_pShadow->SetbShadow(true);
			m_bSSLR		= true;
			m_bBLOOM	= true;
		}
	}
	else if (INPUTMGR->KeyDown(VK_ADD_)) {
		m_nPresentaition = MIN(5, m_nPresentaition + 1);
	}
	else if (INPUTMGR->KeyDown(VK_SUBTRACT_)) {
		m_nPresentaition = MAX(0, m_nPresentaition - 1);
	}
	m_pBloomDownScale->SetAdaptation(fTimeElapsed);
	m_fTimeDelta = fTimeElapsed;
}
void CRenderer::PostProcessing( CCamera* pCamera) {

	m_pBloomDownScale->Excute();
	ID3D11ShaderResourceView* pBloomImage = m_pBloom->Excute(pCamera);
	pBloomImage = m_p16to1Blur->Excute(pBloomImage);
	if (true == INPUTMGR->GetDebugMode() && m_iOption == 2)
	{
		DEBUGER->AddTexture(XMFLOAT2(0, 0), XMFLOAT2(GLOBALVALUEMGR->GetrcClient().right, GLOBALVALUEMGR->GetrcClient().bottom), pBloomImage);
	}

	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(2, 1, &pBloomImage);

	//rtv에 풀스크린 드로우 
	m_pPostProcessingFinalPass->Excute(pCamera);

	//적응을 하기위한 이전avgLum과 지금 계산한 avgLum을 교환
	m_pBloomDownScale->SwapAdaptationBuff();
}
void CRenderer::ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView) {
	GLOBALVALUEMGR->GetDeviceContext()->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}
void CRenderer::SetForwardRenderTargets(ID3D11DepthStencilView* pDepthStencil) {
	ID3D11RenderTargetView *pd3dRTVs[RENDER_TARGET_NUMBER] = { m_pd3drtvColorSpecInt, m_pd3drtvNormal, m_pd3drtvSpecPow };
	//float fClearColor[4] = { xmf4Xolor.x, xmf4Xolor.y, xmf4Xolor.z, xmf4Xolor.w };
	float fClearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	if (m_pd3drtvColorSpecInt) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvColorSpecInt, fClearColor);
	if (m_pd3drtvNormal) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvNormal, fClearColor);
	if (m_pd3drtvSpecPow) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvSpecPow, fClearColor);

	SetRenderTargetViews(RENDER_TARGET_NUMBER, pd3dRTVs, pDepthStencil);
}
void CRenderer::SetMainRenderTargetView() {
	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, &m_pd3dRenderTargetView, m_pd3ddsvReadOnlyDepthStencil);
}
void CRenderer::SetRenderTargetViews(UINT nRenderTarget, ID3D11RenderTargetView** pd3dRTVs, ID3D11DepthStencilView* pd3ddsvDepthStencil) {
	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(nRenderTarget, pd3dRTVs, pd3ddsvDepthStencil);
}

bool CRenderer::CreateRenderTargetView() {

	HRESULT hResult = S_OK;
	ID3D11Texture2D *pd3dBackBuffer{ nullptr };
	if (FAILED(hResult = GLOBALVALUEMGR->GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID *)&pd3dBackBuffer))) return(false);
	if (FAILED(hResult = GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(pd3dBackBuffer, NULL, &m_pd3dRenderTargetView))) return(false);
	//m_pd3dRenderTargetView->


	if (pd3dBackBuffer) pd3dBackBuffer->Release();
	{
		//----------------------------------------Resource Desc-----------------------------------------//
		D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc;
		::ZeroMemory(&d3dSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
		d3dSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		d3dSRVDesc.Texture2D.MipLevels = 1;
		//d3dSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
		//----------------------------------------Resource Desc-----------------------------------------//
		//----------------------------------------TextUre Desc-----------------------------------------//
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
		d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		//d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		//----------------------------------------TextUre Desc-----------------------------------------//
		//----------------------------------------Render Desc-----------------------------------------//
		D3D11_RENDER_TARGET_VIEW_DESC d3dRTVDesc;
		::ZeroMemory(&d3dRTVDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		d3dRTVDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		d3dRTVDesc.Texture2D.MipSlice = 0;
		//----------------------------------------TextUre Desc-----------------------------------------//

		// Create the depth stencil view 
		D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
		ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
		d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;

		ReleaseForwardRenderTargets();


		D3D11_DEPTH_STENCIL_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(D3D11_DEPTH_STENCIL_DESC));
		descDepth.DepthEnable = TRUE;
		descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		descDepth.DepthFunc = D3D11_COMPARISON_LESS;
		descDepth.StencilEnable = FALSE;
		GLOBALVALUEMGR->GetDevice()->CreateDepthStencilState(&descDepth, &m_pd3dDSSEarlyZWrite);

		descDepth.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		descDepth.DepthFunc = D3D11_COMPARISON_EQUAL;
		GLOBALVALUEMGR->GetDevice()->CreateDepthStencilState(&descDepth, &m_pd3dDSSEarlyZRead);


		//--------------------------------------Scene0 DSV Create-----------------------------------------//
		//d3dTexture2DDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		//d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		//d3dSRVDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		d3dTexture2DDesc.Format = DXGI_FORMAT_R32G8X24_TYPELESS;
		d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
		d3dSRVDesc.Format = DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS;

		d3dTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtDepthStencil);
		GLOBALVALUEMGR->GetDevice()->CreateDepthStencilView(m_pd3dtxtDepthStencil, &d3dDepthStencilViewDesc, &m_pd3ddsvDepthStencil);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtDepthStencil, &d3dSRVDesc, &m_pd3dsrvDepthStencil);
		//real depth stencil
		d3dDepthStencilViewDesc.Flags = D3D11_DSV_READ_ONLY_DEPTH;
		if (FAILED(hResult = GLOBALVALUEMGR->GetDevice()->CreateDepthStencilView(m_pd3dtxtDepthStencil, &d3dDepthStencilViewDesc, &m_pd3ddsvReadOnlyDepthStencil))) return(false);
		//--------------------------------------Scene0 DSV Create-----------------------------------------//

		d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dRTVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

		//--------------------------------------Scene1 RTV Create-----------------------------------------//
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtColorSpecInt);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtColorSpecInt, &d3dSRVDesc, &m_pd3dsrvColorSpecInt);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtColorSpecInt, &d3dRTVDesc, &m_pd3drtvColorSpecInt);
		//--------------------------------------Scene1 RTV Create-----------------------------------------//

		//--------------------------------------Scene2 RTV Create-----------------------------------------//
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtNormal);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtNormal, &d3dRTVDesc, &m_pd3drtvNormal);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtNormal, &d3dSRVDesc, &m_pd3dsrvNormal);
		//--------------------------------------Scene2 RTV Create-----------------------------------------//

		//--------------------------------------Scene3 RTV Create-----------------------------------------//
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtSpecPow);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtSpecPow, &d3dRTVDesc, &m_pd3drtvSpecPow);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtSpecPow, &d3dSRVDesc, &m_pd3dsrvSpecPow);
		//--------------------------------------Scene3 RTV Create-----------------------------------------//

		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtSkyBox);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtSkyBox, &d3dRTVDesc, &m_pd3drtvSkyBox);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtSkyBox, &d3dSRVDesc, &m_pd3dsrvSkyBox);


		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtPostProcess);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtPostProcess, &d3dRTVDesc, &m_pd3drtvPostProcess);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtPostProcess, &d3dSRVDesc, &m_pd3dsrvPostProcess);


		//자기 texture set -> sampler set위함
		//---------------------make texture---------------------
		//texture set to light rendercontainer

		ID3D11ShaderResourceView *pd3dSRV = { m_pd3dsrvDepthStencil };
		UINT Slot = { 0 };
		UINT BindFlag = { BIND_PS | BIND_CS };
		 CTexture* pTexture = CTexture::CreateTexture(pd3dSRV, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);

		pd3dSRV = { m_pd3dsrvColorSpecInt };
		Slot = { 1 };
		BindFlag = { BIND_PS | BIND_CS };
		pTexture = CTexture::CreateTexture(pd3dSRV, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);

		pd3dSRV = { m_pd3dsrvNormal };
		Slot = { 2 };
		BindFlag = { BIND_PS | BIND_CS };
		pTexture = CTexture::CreateTexture(pd3dSRV, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);

		pd3dSRV = { m_pd3dsrvSpecPow };
		Slot = { 3 };
		BindFlag = { BIND_PS | BIND_CS };
		pTexture = CTexture::CreateTexture(pd3dSRV, Slot, BindFlag);
		m_vObjectLayerResultTexture.push_back(pTexture);
		//---------------------make texture---------------------


		//light texture제작
		GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtLight);
		GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtLight, &d3dRTVDesc, &m_pd3drtvLight);
		GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtLight, &d3dSRVDesc, &m_pd3dsrvLight);

		//make texture
		UINT LightTexSlot = { 0 };
		UINT LightTexBindFlag = { BIND_PS | BIND_CS };
		pTexture = CTexture::CreateTexture(m_pd3dsrvLight, LightTexSlot, LightTexBindFlag);
		m_vLightLayerResultTexture.push_back(pTexture);
		//light texture제작
	}
	m_pAORenderer->ResizeBuffer();
	m_pBigWaterRenderer->ResizeBuffer();
	m_pRefrectionRenderer->ResizeBuffer();
	m_pBloomDownScale->ResizeBuffer();
	m_pBloom->ResizeBuffer();
	m_p16to1Blur->ResizeBuffer(4);
	m_p4to1Blur->ResizeBuffer(2);
	m_pSSLR->ResizeBuffer();
	m_pShadow->ResizeBuffer();
	m_pEffectRenderer->ResizeBuffer();
	//m_pWaterRenderer->ResizeBuffer();
	m_pFinalRenderer->ResizeBuffer();

	return true;
}

void CRenderer::ReleaseForwardRenderTargets() {
	//texture end
	for (auto pTexture : m_vObjectLayerResultTexture) {
		//pTexture->End();
		delete pTexture;
	}
	m_vObjectLayerResultTexture.clear();
	for (auto pTexture : m_vLightLayerResultTexture) {
		//pTexture->End();
		delete pTexture;
	}
	m_vLightLayerResultTexture.clear();

	if (m_pd3dtxtColorSpecInt) m_pd3dtxtColorSpecInt->Release();//0
	m_pd3dtxtColorSpecInt = nullptr;

	if (m_pd3dtxtNormal) m_pd3dtxtNormal->Release();//1
	m_pd3dtxtNormal = nullptr;

	if (m_pd3dtxtSpecPow) m_pd3dtxtSpecPow->Release();//2
	m_pd3dtxtSpecPow = nullptr;

	if (m_pd3dtxtDepthStencil) m_pd3dtxtDepthStencil->Release();
	m_pd3dtxtDepthStencil = nullptr;

	if (m_pd3dsrvColorSpecInt) m_pd3dsrvColorSpecInt->Release();//0
	m_pd3dsrvColorSpecInt = nullptr;

	if (m_pd3dsrvNormal) m_pd3dsrvNormal->Release();//1
	m_pd3dsrvNormal = nullptr;

	if (m_pd3dsrvSpecPow) m_pd3dsrvSpecPow->Release();//2
	m_pd3dsrvSpecPow = nullptr;

	if (m_pd3dsrvDepthStencil) m_pd3dsrvDepthStencil->Release();
	m_pd3dsrvDepthStencil = nullptr;

	if (m_pd3drtvColorSpecInt) m_pd3drtvColorSpecInt->Release();//0
	m_pd3drtvColorSpecInt = nullptr;

	if (m_pd3drtvNormal) m_pd3drtvNormal->Release();//1
	m_pd3drtvNormal = nullptr;

	if (m_pd3drtvSpecPow) m_pd3drtvSpecPow->Release();//2
	m_pd3drtvSpecPow = nullptr;

	if (m_pd3ddsvDepthStencil) m_pd3ddsvDepthStencil->Release();
	m_pd3ddsvDepthStencil = nullptr;

	if (m_pd3drtvLight) m_pd3drtvLight->Release();
	m_pd3drtvLight = nullptr;

	if (m_pd3dtxtSkyBox) m_pd3dtxtSkyBox->Release();//2
	m_pd3dtxtSkyBox = nullptr;

	if (m_pd3dtxtPostProcess) m_pd3dtxtPostProcess->Release();
	m_pd3dtxtPostProcess = nullptr;

	if (m_pd3dsrvSkyBox) m_pd3dsrvSkyBox->Release();
	m_pd3dsrvSkyBox = nullptr;

	if (m_pd3dsrvPostProcess) m_pd3dsrvPostProcess->Release();
	m_pd3dsrvPostProcess = nullptr;

	if (m_pd3drtvSkyBox) m_pd3drtvSkyBox->Release();
	m_pd3drtvSkyBox = nullptr;

	if (m_pd3drtvPostProcess) m_pd3drtvPostProcess->Release();
	m_pd3drtvPostProcess = nullptr;

	//if (m_pd3dDSSEarlyZRead) m_pd3dDSSEarlyZRead->Release();
	//m_pd3dDSSEarlyZRead = nullptr;

	//if (m_pd3dDSSEarlyZWrite) m_pd3dDSSEarlyZWrite->Release();
	//m_pd3dDSSEarlyZWrite = nullptr;
}

bool CRenderer::ResizeBuffer() {
	if (m_pd3dRenderTargetView) m_pd3dRenderTargetView->Release();
	if (m_pd3ddsvReadOnlyDepthStencil) m_pd3ddsvReadOnlyDepthStencil->Release();

	if (FAILED(GLOBALVALUEMGR->GetSwapChain()->ResizeBuffers(1, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0)))
		MessageBox(nullptr, TEXT("바보"), TEXT("바보2"), MB_OK);

	//resize rtv size, deferred texture size
	CreateRenderTargetView();

	stCSGlobalBufferData* pData = (stCSGlobalBufferData*)m_pCSGlobalBuffer->Map();
	pData->nWidth = GLOBALVALUEMGR->GetrcClient().right;
	pData->nHeight = GLOBALVALUEMGR->GetrcClient().bottom;
	pData->nHalfWidth = pData->nWidth / 2;
	pData->nHalfHeight = pData->nHeight / 2;
	pData->nQuadWidth = pData->nWidth / 4;
	pData->nQuadHeight = pData->nHeight / 4;
	m_pCSGlobalBuffer->Unmap();
	m_pCSGlobalBuffer->SetShaderState();

	return true;
}

void CRenderer::LoadEffectInfo(wstring wsOutputPath, wstring wsSceneName) {
	//back buffer color
	m_fBackBufferClearColor[0] = IMPORTER->ReadFloat();
	m_fBackBufferClearColor[1] = IMPORTER->ReadFloat();
	m_fBackBufferClearColor[2] = IMPORTER->ReadFloat();
	m_fBackBufferClearColor[3] = IMPORTER->ReadFloat();
	m_pObjectRenderer->GetSkyBoxRenderContainer()->GetMaterialList()[0]->SetMaterialInfo(MATERIAL_INFO{
		XMFLOAT4(m_fBackBufferClearColor[0],m_fBackBufferClearColor[1],m_fBackBufferClearColor[2],m_fBackBufferClearColor[3]),
		XMFLOAT4(0,0,0,0) });

	//ssao
	float fSSAORadius = IMPORTER->ReadFloat();
	float fSSAOOffsetRadius = IMPORTER->ReadFloat();
	bool bSSAOOnOff = true;
	m_pAORenderer->SetSSAOValues(bSSAOOnOff, fSSAOOffsetRadius, fSSAORadius);

	//bloom
	float fBLOOMThreshold = IMPORTER->ReadFloat();
	m_pBloomDownScale->SetBloomThreshold(fBLOOMThreshold);
	float fBLOOMMiddleGrey = IMPORTER->ReadFloat();
	float fBLOOMWhite = IMPORTER->ReadFloat();
	float fBLOOMScale = IMPORTER->ReadFloat();
	m_pPostProcessingFinalPass->SetBloomFinalPassValues(fBLOOMMiddleGrey, fBLOOMWhite, fBLOOMScale);
	//sslr
	bool bSSLROnOff = IMPORTER->ReadBool();
	float fSSLRMaxSunDist = IMPORTER->ReadFloat();
	float fSSLRInitDecay = IMPORTER->ReadFloat();
	float fSSLRDistDecay = IMPORTER->ReadFloat();
	float fSSLRMaxDeltaLen = IMPORTER->ReadFloat();
	m_pSSLR->SetSSLRValues(bSSLROnOff, fSSLRMaxSunDist, fSSLRInitDecay, fSSLRDistDecay, fSSLRMaxDeltaLen);

	//ssrf
	float fMinDepthBias = IMPORTER->ReadFloat();
	float fMaxDepthBias = IMPORTER->ReadFloat();;
	float fEdgeDistThreshold = IMPORTER->ReadFloat();;
	float fReflectionScale = IMPORTER->ReadFloat();;
	float fViewAngleThreshold = IMPORTER->ReadFloat();;
	float fPixelScale = IMPORTER->ReadFloat();;
	float fNumStepScale = IMPORTER->ReadFloat();;

	m_pRefrectionRenderer->SetSSRFValues(fMinDepthBias, fMaxDepthBias, fEdgeDistThreshold, fReflectionScale, fViewAngleThreshold, fPixelScale, fNumStepScale);
	m_pShadow->LoadShadow(wsOutputPath, wsSceneName);
	m_pBigWaterRenderer->LoadBigWaterInfo();
}

void CRenderer::SetMissionFail()
{
}

void CRenderer::SetMissionSuccess()
{
}

CRenderer::CRenderer() :CSingleTonBase<CRenderer>("rendereringleton") {

}

CRenderer::~CRenderer() {

}
