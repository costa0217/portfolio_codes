#include "stdafx.h"
#include "SSRF.h"

bool CSSRF::Begin() {
	D3D11_BLEND_DESC descBlend;
	descBlend.AlphaToCoverageEnable = FALSE;
	descBlend.IndependentBlendEnable = FALSE;
	const D3D11_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc =
	{
		TRUE,
		D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD,
		D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP_ADD,
		D3D11_COLOR_WRITE_ENABLE_ALL,
	};
	for (UINT i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; ++i)
		descBlend.RenderTarget[i] = defaultRenderTargetBlendDesc;
	GLOBALVALUEMGR->GetDevice()->CreateBlendState(&descBlend, &m_pd3dBlendState);

	m_pSSReflectionVSConstants = RESOURCEMGR->CreateConstantBuffer("SSReflectionVSConstants", 1, sizeof(SSReflectionVSConstants), 8, BIND_VS | BIND_GS | BIND_DS, 0);//rsmgr
	m_pSSReflectionPSConstants = RESOURCEMGR->CreateConstantBuffer("SSReflectionPSConstants", 1, sizeof(SSReflectionPSConstants), 8, BIND_PS, 0);//rsmgr

	return true;
}

bool CSSRF::End() {
	m_pd3dBlendState->Release();
	m_pSSReflectionVSConstants = nullptr;
	m_pSSReflectionPSConstants = nullptr;
	//end할것 없음
	return true;
}

void CSSRF::SetShaderState() {

}

void CSSRF::CleanShaderState() {

}

void CSSRF::UpdateShaderState() {

}

void CSSRF::Excute( CCamera* pCamera, ID3D11RenderTargetView* prtvHDR, ID3D11DepthStencilView* pdsvReadOnlyDepthStencil, ID3D11ShaderResourceView* psrvHDR, ID3D11ShaderResourceView* psrvDepth, ID3D11ShaderResourceView* psrvNormal) {
	auto mapSSRFRC = RCSELLER->GetTagRenderContainer()[tag::TAG_REFLECTION];
	bool bSSRF = false;
	for (auto vecSSRF : mapSSRFRC) {
		if ( vecSSRF.second->GetObjectList().size() > 0) {
			bSSRF = true;
			break;
		}
	}
	if (false == bSSRF)return;

	if (RCSELLER->GetTagRenderContainer()[tag::TAG_REFLECTION].size() == 0) return;

	FLOAT clearColor[4] = { 0,0,0,0 };
	GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pd3drtvRef, clearColor);

	SSReflectionVSConstants* pVSData = (SSReflectionVSConstants*)m_pSSReflectionVSConstants->Map();
	pVSData->View = XMMatrixTranspose(pCamera->GetViewMtx());
	pVSData->ViewProjection = XMMatrixTranspose(pCamera->GetProjectionMtx());
	m_pSSReflectionVSConstants->Unmap();

	UPDATER->GetSkyBoxContainer()->GetSkyBoxTexture()->SetShaderState();;

	//1. vs, ps, 처음 랜더에 필요한 vs, ps 버퍼, set한다.
	m_pSSReflectionVSConstants->SetShaderState();
	m_pSSReflectionPSConstants->SetShaderState();

	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, &m_pd3drtvRef, pdsvReadOnlyDepthStencil);//rtv set
	//vObjectRendererResult texture도 set해준다.
	ID3D11ShaderResourceView* ppSRV[] = { psrvHDR, psrvDepth, psrvNormal };
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(0, 3, ppSRV);

	//2. 모든 메시를 랜더한다. s
	for (auto pObject : RCSELLER->GetTagRenderContainer()[tag::TAG_REFLECTION]) {
		pObject.second->Render(pCamera);
		pObject.second->ClearObjectList();
	}//이녀석들은 shader가 필요없다. mesh랑 instance buff만 채워줄뿐..
	 //3. 정리 후
	//clear

	ID3D11BlendState* pPrevBlendState;
	FLOAT prevBlendFactor[4];
	UINT prevSampleMask;
	GLOBALVALUEMGR->GetDeviceContext()->OMGetBlendState(&pPrevBlendState, prevBlendFactor, &prevSampleMask);
	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(m_pd3dBlendState, prevBlendFactor, prevSampleMask);


	//4. hdr rt를 set한다. 
	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, &prtvHDR, nullptr);
	//5. hdr rt에 ref image를 blending에서 그린다.
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(0, 1, &m_pd3dsrvRef);
	RCSELLER->GetTagRenderContainer()[tag::TAG_SSRF]["SSRF"]->RenderWithOutObject(pCamera);

	GLOBALVALUEMGR->GetDeviceContext()->OMSetBlendState(pPrevBlendState, prevBlendFactor, prevSampleMask);

	DEBUGER->AddTexture(XMFLOAT2(300, 300), XMFLOAT2(400, 400), m_pd3dsrvRef);
}

void CSSRF::ResizeBuffer() {
	ReleaseBuffer();

	// Create the HDR render target
	D3D11_TEXTURE2D_DESC dtd = {
		GLOBALVALUEMGR->GetrcClient().right, //UINT Width;
		GLOBALVALUEMGR->GetrcClient().bottom, //UINT Height;
		1, //UINT MipLevels;
		1, //UINT ArraySize;
		DXGI_FORMAT_R16G16B16A16_TYPELESS, //DXGI_FORMAT Format;
		1, //DXGI_SAMPLE_DESC SampleDesc;
		0,
		D3D11_USAGE_DEFAULT,//D3D11_USAGE Usage;
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,//UINT BindFlags;
		0,//UINT CPUAccessFlags;
		0//UINT MiscFlags;    
	};
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&dtd, NULL, &m_pd3dtxtRef);

	D3D11_RENDER_TARGET_VIEW_DESC rtsvd =
	{
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		D3D11_RTV_DIMENSION_TEXTURE2D
	};
	GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pd3dtxtRef, &rtsvd, &m_pd3drtvRef);

	D3D11_SHADER_RESOURCE_VIEW_DESC dsrvd =
	{
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		D3D11_SRV_DIMENSION_TEXTURE2D,
		0,
		0
	};
	dsrvd.Texture2D.MipLevels = 1;
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtRef, &dsrvd, &m_pd3dsrvRef);

}

void CSSRF::ReleaseBuffer() {
	if (m_pd3dtxtRef) m_pd3dtxtRef->Release();
	if (m_pd3drtvRef)m_pd3drtvRef->Release();
	if (m_pd3dsrvRef)m_pd3dsrvRef->Release();
}

void CSSRF::SetSSRFValues(float fMinDepthBias, float fMaxDepthBias, float fEdgeDistThreshold, float fReflectionScale, float fViewAngleThreshold, float fPixelScale, float fNumStepScale){
	SetMinDepthBias(fMinDepthBias);
	SetMaxDepthBias(fMaxDepthBias);
	SetEdgeDistThreshold(fEdgeDistThreshold);
	SetReflectionScale(fReflectionScale);
	SetViewAngleThreshold(fViewAngleThreshold);
	SetPixelScale(fPixelScale);
	NumStepScale(fNumStepScale);
							
	ChangeStaticData();
}

void CSSRF::ChangeStaticData(){
	SSReflectionPSConstants* pPSData = (SSReflectionPSConstants*)m_pSSReflectionPSConstants->Map();
	pPSData->minDepthBias = m_fMinDepthBias;
	pPSData->maxDepthBias = m_fMaxDepthBias;
	pPSData->EdgeDistThreshold = m_fEdgeDistThreshold;
	pPSData->ReflectionScale = m_fReflectionScale;
	pPSData->ViewAngleThreshold = m_fViewAngleThreshold;
	float fPixelSize = 1.f / (float)GLOBALVALUEMGR->GetrcClient().bottom;
	pPSData->PixelSize = fPixelSize * m_fPixelScale;
	pPSData->nNumSteps = GLOBALVALUEMGR->GetrcClient().right * m_fNumStepScale;

	m_pSSReflectionPSConstants->Unmap();
	m_pSSReflectionPSConstants->SetShaderState();
}


CSSRF::CSSRF() : DXObject("ssrf") {

}


CSSRF::~CSSRF() {

}
