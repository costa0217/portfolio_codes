#include "stdafx.h"
#include "Shadow.h"


bool CShadow::Begin() {
	ReleaseBuffer();
	//m_pShadowVPBuffer = CBuffer::CreateConstantBuffer(1, sizeof(XMFLOAT4X4), 0, BIND_VS | BIND_DS , NULL);
	m_pCamera = new CFreeCamera();
	//------------------------------------------카메라 제작--------------------------------------
	m_pCamera->Begin();
	m_pCamera->GenerateProjectionMatrix(60.0f * XM_PI / 180.0f
		, float(GLOBALVALUEMGR->GetrcClient().right) / float(GLOBALVALUEMGR->GetrcClient().bottom)
		, 5.f
		, 3000.0f);

	//viewprojection행렬 제작 / 광원
	XMVECTOR eye = { 0.0f, 500.0f, -500.0f, 0.0f };
	XMVECTOR at = { 0, -1.0f, 1.0, 0.0f };
	XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };
	m_pCamera->SetLookAt(eye, at, up);

	D3D11_RASTERIZER_DESC descRasterizer;
	ZeroMemory(&descRasterizer, sizeof(D3D11_RASTERIZER_DESC));
	descRasterizer.FillMode = D3D11_FILL_SOLID;
	descRasterizer.CullMode = D3D11_CULL_NONE;
	GLOBALVALUEMGR->GetDevice()->CreateRasterizerState(&descRasterizer, &m_pd3dRSShader);

	m_pShadowBuf = RESOURCEMGR->CreateConstantBuffer("ShadowInfoBuffer", 1, sizeof(stShadowInfo), PS_CB_SHADOW, BIND_PS);
	m_pStaticShadowBuf = RESOURCEMGR->CreateConstantBuffer("StaticShadowBuffer", 1, sizeof(XMFLOAT4X4), 5, BIND_PS);
	m_pShadowInfo = new stShadowInfo;

	return true;
}

bool CShadow::End()
{
	if (m_pCamera) {
		m_pCamera->End();
		delete m_pCamera;
	}
	m_pShadowBuf = nullptr;
	m_pStaticShadowBuf = nullptr;
	delete m_pShadowInfo;
	return true;
}

void CShadow::SetShaderState() {



	//resourece
	if (m_bShadow) {
		ID3D11ShaderResourceView* pSRV[] = { m_pd3dsrvShadow };
		GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(5, 1, pSRV);
		GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(6, 1, &m_pd3dsrvStaticShadow);
	}
	else {
		ID3D11ShaderResourceView* pSRV[] = { RESOURCEMGR->GetTexture("DEFAULT")->GetShaderResourceView() };
		GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(5, 1, pSRV);
		GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(6, 1, pSRV);
	}
	stShadowInfo* pData = (stShadowInfo*)m_pShadowBuf->Map();
	//이건 동적 그림자 카메라
	pData->xmmtxViewProj = XMMatrixTranspose(m_pCamera->GetViewMtx()*m_pCamera->GetProjectionMtx());
	pData->bias = m_pShadowInfo->bias * 0.00001;
	pData->bias_offset = m_pShadowInfo->bias_offset * 0.00001;
	pData->kernelhalf = m_pShadowInfo->kernelhalf;
	pData->dist_sum = m_pShadowInfo->dist_sum;

	m_pShadowBuf->Unmap();
	m_pShadowBuf->SetShaderState();

	XMFLOAT4X4* pStaticData = (XMFLOAT4X4*)m_pStaticShadowBuf->Map();
	XMStoreFloat4x4(pStaticData, XMMatrixTranspose(m_xmmtxStaticShadowVP));
	m_pStaticShadowBuf->Unmap();
	m_pStaticShadowBuf->SetShaderState();
	//(*pSRV)->Release();
}

void CShadow::CleanShaderState() {
	ID3D11ShaderResourceView* pSRV[] = { nullptr };
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(5, 1, pSRV);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(6, 1, pSRV);
	m_pShadowBuf->CleanShaderState();
	m_pStaticShadowBuf->CleanShaderState();
}

void CShadow::UpdateShaderState() {

}

ID3D11ShaderResourceView * CShadow::RenderShadowMap( CCamera* pCamera) {
	if (m_bShadow) {
		bool bStartScene = SCENEMGR->GetSceneStart();
		if (true == bStartScene)
		{

			float offset = UPDATER->GetDirectionalLight()->GetOffsetLength();
			offset = -1500.f;
			XMVECTOR xmvDirectionalLightDir = UPDATER->GetDirectionalLight()->GetLook();

			UINT nSpace = UPDATER->GetSpaceContainer()->GetSpaceNum();
			XMVECTOR at;
			XMFLOAT3 xmf3Pos;

			float space_size = UPDATER->GetSpaceContainer()->GetSize();
			XMVECTOR xmPos = XMVectorSet(space_size / 2, 0.f, space_size / 2, 0.f);
			XMStoreFloat3(&xmf3Pos, xmPos);
			at = XMVectorSet(xmf3Pos.x, xmf3Pos.y, xmf3Pos.z, 0);

			XMVECTOR eye = at + xmvDirectionalLightDir*offset * 1.5f;
			XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };
			m_pCamera->SetLookAt(eye, xmPos, up);
			m_pCamera->SetViewport(0, 0, 4096, 4096, 0.0f, 1.0f);

			m_pCamera->UpdateShaderState();
			m_pCamera->SetShaderState();

			UPDATER->GetSpaceContainer()->PrepareRender(m_pCamera, TAG_STATIC_OBJECT | TAG_TERRAIN);
			SCENEMGR->SetSceneStart(false);
			GLOBALVALUEMGR->GetDeviceContext()->ClearDepthStencilView(m_pd3ddsvStaticShadow, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		}
		else
			UPDATER->GetSpaceContainer()->PrepareRender(m_pCamera, TAG_DYNAMIC_OBJECT);

		DEBUGER->start_Timemeasurement();
		//if (m_pShadowTexture) m_pShadowTexture->SetShaderState();
		GLOBALVALUEMGR->GetDeviceContext()->ClearDepthStencilView(m_pd3ddsvShadow, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		ID3D11RasterizerState* pPrevRSState;
		GLOBALVALUEMGR->GetDeviceContext()->RSGetState(&pPrevRSState);
		GLOBALVALUEMGR->GetDeviceContext()->RSSetState(m_pd3dRSShader);

		if (true == bStartScene)
		{
			float offset = UPDATER->GetDirectionalLight()->GetOffsetLength();
			XMVECTOR xmvDirectionalLightDir = UPDATER->GetDirectionalLight()->GetLook();

			UINT nSpace = UPDATER->GetSpaceContainer()->GetSpaceNum();
			XMVECTOR at;
			XMFLOAT3 xmf3Pos;

			float space_size = UPDATER->GetSpaceContainer()->GetSize();
			XMVECTOR xmPos = XMVectorSet(space_size / 2, 0.f, space_size / 2, 0.f);
			XMStoreFloat3(&xmf3Pos, xmPos);
			at = XMVectorSet(xmf3Pos.x, xmf3Pos.y, xmf3Pos.z, 0);

			XMVECTOR eye = at + xmvDirectionalLightDir*offset * 1.5f;
			XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };
			m_pCamera->SetLookAt(eye, xmPos, up);
			m_pCamera->SetViewport(0, 0, 4096, 4096, 0.0f, 1.0f);

			m_pCamera->UpdateShaderState();
			m_pCamera->SetShaderState();
			m_xmmtxStaticShadowVP = m_pCamera->GetViewMtx() * m_pCamera->GetProjectionMtx();
			ID3D11RenderTargetView*   rtNULL = nullptr;
			GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, &rtNULL, m_pd3ddsvStaticShadow);

			RENDERER->GetObjectRenderer()->ExcuteShadowRender(m_pCamera);

			m_pCamera->SetViewport(0, 0, GLOBALVALUEMGR->GetrcClient().right, GLOBALVALUEMGR->GetrcClient().bottom, 0.0f, 1.0f);

			GLOBALVALUEMGR->GetDeviceContext()->RSSetState(pPrevRSState);
			DEBUGER->end_Timemeasurement(L"shadow");
		}
		else
		{
			float offset = UPDATER->GetDirectionalLight()->GetOffsetLength();
			offset = -50.f;
			XMVECTOR xmvDirectionalLightDir = UPDATER->GetDirectionalLight()->GetLook();

			UINT nSpace = UPDATER->GetSpaceContainer()->GetSpaceNum();
			XMVECTOR at;
			XMFLOAT3 xmf3Pos;

			float space_size = UPDATER->GetSpaceContainer()->GetSize();
			XMVECTOR xmPos = XMVectorSet(space_size / 2, 0.f, space_size / 2, 0.f);
			if (CCameraMgr::GetInstance()->GetCamera(CAM_FREE)->IsTarget())
				xmPos = CCameraMgr::GetInstance()->GetCamera(CAM_FREE)->GetTarget()->GetPosition();

			XMStoreFloat3(&xmf3Pos, xmPos);
			at = XMVectorSet(xmf3Pos.x, xmf3Pos.y, xmf3Pos.z, 0);

			XMVECTOR eye = at + xmvDirectionalLightDir*offset * 1.5f;
			XMVECTOR up = { 0.0f, 1.0f, 0.0f, 0.0f };
			m_pCamera->SetLookAt(eye, xmPos, up);
			m_pCamera->SetViewport(0, 0, 4096, 4096, 0.0f, 1.0f);

			m_pCamera->UpdateShaderState();
			m_pCamera->SetShaderState();


			ID3D11RenderTargetView*   rtNULL = nullptr;
			GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, &rtNULL, m_pd3ddsvShadow);

			RENDERER->GetObjectRenderer()->ExcuteShadowRender(m_pCamera);

			//DEBUGER->AddDepthTexture(XMFLOAT2(500, 150), XMFLOAT2(750, 300), m_pd3dsrvShadow);
			m_pCamera->SetViewport(0, 0, GLOBALVALUEMGR->GetrcClient().right, GLOBALVALUEMGR->GetrcClient().bottom, 0.0f, 1.0f);

			GLOBALVALUEMGR->GetDeviceContext()->RSSetState(pPrevRSState);
			DEBUGER->end_Timemeasurement(L"shadow");
		}
		//DEBUGER->AddDepthTexture(XMFLOAT2(750, 150), XMFLOAT2(900, 300), m_pd3dsrvStaticShadow);
		return m_pd3dsrvShadow;
	}
	else {
		return RESOURCEMGR->GetTexture("DEFAULT")->GetShaderResourceView();
	}
	
}

void CShadow::ResizeBuffer()
{
	ReleaseBuffer();

	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;
	d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;

	D3D11_TEXTURE2D_DESC d3dTexture2DDesc;
	::ZeroMemory(&d3dTexture2DDesc, sizeof(D3D11_TEXTURE2D_DESC));
	d3dTexture2DDesc.Width = 4096;
	d3dTexture2DDesc.Height = 4096;
	d3dTexture2DDesc.MipLevels = 1;
	d3dTexture2DDesc.ArraySize = 1;
	d3dTexture2DDesc.SampleDesc.Count = 1;
	d3dTexture2DDesc.SampleDesc.Quality = 0;
	d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DDesc.CPUAccessFlags = 0;
	d3dTexture2DDesc.MiscFlags = 0;
	d3dTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;

	D3D11_SHADER_RESOURCE_VIEW_DESC d3dSRVDesc;
	::ZeroMemory(&d3dSRVDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	d3dSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	d3dSRVDesc.Texture2D.MipLevels = 1;
	d3dSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	//d3dTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtShadow);
	GLOBALVALUEMGR->GetDevice()->CreateDepthStencilView(m_pd3dtxtShadow, &d3dDepthStencilViewDesc, &m_pd3ddsvShadow);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtShadow, &d3dSRVDesc, &m_pd3dsrvShadow);


	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtStaticShadow);
	GLOBALVALUEMGR->GetDevice()->CreateDepthStencilView(m_pd3dtxtStaticShadow, &d3dDepthStencilViewDesc, &m_pd3ddsvStaticShadow);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtStaticShadow, &d3dSRVDesc, &m_pd3dsrvStaticShadow);
}

void CShadow::ReleaseBuffer()
{
	if (m_pd3dtxtShadow) m_pd3dtxtShadow->Release();
	if (m_pd3dsrvShadow) m_pd3dsrvShadow->Release();
	if (m_pd3ddsvShadow) m_pd3ddsvShadow->Release();

	if (m_pd3dtxtStaticShadow) m_pd3dtxtStaticShadow->Release();
	if (m_pd3ddsvStaticShadow) m_pd3ddsvStaticShadow->Release();
	if (m_pd3dsrvStaticShadow) m_pd3dsrvStaticShadow->Release();
}

void CShadow::LoadShadow(wstring wsOutputPath, wstring wsSceneName) {
	m_pShadowInfo->bias = IMPORTER->ReadFloat();
	m_pShadowInfo->bias_offset = IMPORTER->ReadFloat();
	m_pShadowInfo->dist_sum = IMPORTER->ReadFloat();
	m_pShadowInfo->kernelhalf = IMPORTER->ReadFloat();
}

CShadow::CShadow()
	: DXObject("shadow") {

}

CShadow::~CShadow() {

}
