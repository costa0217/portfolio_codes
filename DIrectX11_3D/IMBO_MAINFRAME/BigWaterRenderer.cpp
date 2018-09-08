#include "stdafx.h"
#include "BigWaterRenderer.h"

CBigWaterRenderer::CBigWaterRenderer()
	: DXObject("bigwaterrenderer")
{
}


CBigWaterRenderer::~CBigWaterRenderer()
{
}

bool CBigWaterRenderer::Begin()
{
	ResizeBuffer();

	//rasterizer state
	D3D11_RASTERIZER_DESC descRasterizer;
	ZeroMemory(&descRasterizer, sizeof(D3D11_RASTERIZER_DESC));
	descRasterizer.FillMode = D3D11_FILL_SOLID;
	//descRasterizer.FillMode = D3D11_FILL_WIREFRAME;
	descRasterizer.CullMode = D3D11_CULL_NONE;
	GLOBALVALUEMGR->GetDevice()->CreateRasterizerState(&descRasterizer, &m_pTestRSState);
	//rasterizer state

	//D3D11_BLEND_DESC d3dBlendDesc;
	//ZeroMemory(&d3dBlendDesc, sizeof(D3D11_BLEND_DESC));
	//ZeroMemory(&d3dBlendDesc, sizeof(D3D11_BLEND_DESC));
	//d3dBlendDesc.AlphaToCoverageEnable = false;
	//d3dBlendDesc.IndependentBlendEnable = false;
	//d3dBlendDesc.RenderTarget[0].BlendEnable = true;
	////d3dBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	////d3dBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	////d3dBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//d3dBlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	//d3dBlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	//d3dBlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//d3dBlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	//d3dBlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	//d3dBlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	//d3dBlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//
	//GLOBALVALUEMGR->GetDevice()->CreateBlendState(&d3dBlendDesc, &m_pAlphaBlendState);

	D3D11_BLEND_DESC descBlend;
	ZeroMemory(&descBlend, sizeof(D3D11_BLEND_DESC));
	descBlend.RenderTarget[0].BlendEnable = true;
	descBlend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	descBlend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	descBlend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	descBlend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	descBlend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	descBlend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//blend state
	GLOBALVALUEMGR->GetDevice()->CreateBlendState(&descBlend, &m_pAlphaBlendState);


	//���� ���ٽ� ������Ʈ
	//�ſ��� �������� ���̽��ٽ� ������Ʈ
	D3D11_DEPTH_STENCIL_DESC d3dDepthStencilDesc;
	ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = true;
	//d3dDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	d3dDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	d3dDepthStencilDesc.StencilEnable = true;
	d3dDepthStencilDesc.StencilReadMask = 0xff;
	d3dDepthStencilDesc.StencilWriteMask = 0xff;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	auto pDevice = GLOBALVALUEMGR->GetDevice();
	pDevice->CreateDepthStencilState(&d3dDepthStencilDesc, &m_pd3dMirrorToStencilState);
	//�ſ￡ �ݻ�� ��ü ������ �� ���� ���ٽ� ������Ʈ
	ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = true;
	d3dDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	d3dDepthStencilDesc.StencilEnable = true;
	d3dDepthStencilDesc.StencilReadMask = 0xff;
	d3dDepthStencilDesc.StencilWriteMask = 0xff;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	pDevice->CreateDepthStencilState(&d3dDepthStencilDesc, &m_pd3dReflectDepthStencilState);

	ZeroMemory(&d3dDepthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	d3dDepthStencilDesc.DepthEnable = false;
	d3dDepthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	d3dDepthStencilDesc.DepthFunc = D3D11_COMPARISON_NEVER;
	d3dDepthStencilDesc.StencilEnable = true;
	d3dDepthStencilDesc.StencilReadMask = 0xFF;
	d3dDepthStencilDesc.StencilWriteMask = 0xFF;
	d3dDepthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	d3dDepthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	d3dDepthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	pDevice->CreateDepthStencilState(&d3dDepthStencilDesc, &m_pd3dReflectSkyDepthStencilState);

	m_pWaterSurface = new CGameObject("bigwatersurface");
	m_pWaterSurface->Begin();

	m_pWaterPSBuffer = CBuffer::CreateConstantBuffer(1, sizeof(stRenderSurfaceInfo), 0, BIND_PS, NULL);
	m_pWaterInfoPSBuffer = CBuffer::CreateConstantBuffer(1, sizeof(stBigWaterInfo), 0, BIND_PS, NULL);
	m_pWaterColor = CBuffer::CreateConstantBuffer(1, sizeof(XMFLOAT4), 0, BIND_PS, NULL);

	m_pWaterBaseBumpMap = CTexture::CreateTexture(L"../../Assets/Water/water_bump_map01.jpg", 5, BIND_PS);
	m_pWaterDetailBumpMap = CTexture::CreateTexture(L"../../Assets/Water/water_bump_map03.jpg", 6, BIND_PS);
	return true;
}

bool CBigWaterRenderer::End()
{
	m_pWaterSurface->End();
	delete m_pWaterSurface;

	ReleaseBuffer();

	m_pWaterPSBuffer->End();	delete m_pWaterPSBuffer;  m_pWaterPSBuffer = nullptr;
	m_pWaterInfoPSBuffer->End();	delete m_pWaterInfoPSBuffer;  m_pWaterInfoPSBuffer = nullptr;
	m_pWaterColor->End();	delete m_pWaterColor;	m_pWaterColor = nullptr;

	m_pWaterBaseBumpMap->End();
	delete m_pWaterBaseBumpMap;
	m_pWaterDetailBumpMap->End();
	delete m_pWaterDetailBumpMap;
	//m_pWaterPSBuffer->End();	delete m_pWaterPSBuffer;  m_pWaterPSBuffer = nullptr;
	//m_pReflractionVSBuffer->End();	m_pReflractionVSBuffer = nullptr;

	return true;
}

void CBigWaterRenderer::RenderBigWater(CCamera* pCamera, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV, vector<CTexture*>& vTexture) {
	size_t size = RCSELLER->GetTagRenderContainer()[tag::TAG_BIGWATER]["bigwater"]->GetObjectList().size();
	if (size > 0) {
		GLOBALVALUEMGR->GetDeviceContext()->ClearDepthStencilView(m_pDSVDepthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		RenderWater1(pCamera);//�� �⺻ ���� �׸���
		RenderWaterSurface(pCamera);//�� ǥ�� �׸���
		RenderRefractions(pCamera);//���� �׸���

		for (auto pTexture : vTexture) {
			pTexture->SetShaderState();
		}
		RenderWater2(pCamera, pRTV, pDSV);//�� ǥ��, ���� ���� �̿��� �� ǥ�� ����
		for (auto pTexture : vTexture) {
			pTexture->CleanShaderState();
		}
	}
	else {
		m_bBigWater = false;
	}
}

void CBigWaterRenderer::LoadBigWaterInfo() {
	/*
	//if(GLOBALVALUEMGR->GetPositioningObject()->GetAnimater())
	pObject = new CGameObject(GLOBALVALUEMGR->GetPositioningObject()->GetName(), GLOBALVALUEMGR->GetPositioningObject()->GetTag());
	pObject->Begin();
	pObject->SetWorldMtx(GLOBALVALUEMGR->GetPositioningObject()->GetWorldMtx());
	UPDATER->GetSpaceContainer()->AddObject(pObject);
	*/

	m_bBigWater = IMPORTER->ReadBool();
	if (m_bBigWater) {
		//color
		m_xmf4WaterColor.x = IMPORTER->ReadFloat();
		m_xmf4WaterColor.y = IMPORTER->ReadFloat();
		m_xmf4WaterColor.z = IMPORTER->ReadFloat();
		m_xmf4WaterColor.w = IMPORTER->ReadFloat();

		//fresnel
		m_fresnelMode = IMPORTER->ReadFloat();
		m_xDrawfactor = IMPORTER->ReadFloat();
		m_fPowFactor = IMPORTER->ReadFloat();

		//wind
		m_windDir.x = IMPORTER->ReadFloat();
		m_windDir.y = IMPORTER->ReadFloat();
		m_windForce = IMPORTER->ReadFloat();
		m_BumpMapBaseUVScale = IMPORTER->ReadFloat();

		m_windDir2.x = IMPORTER->ReadFloat();
		m_windDir2.y = IMPORTER->ReadFloat();
		m_windForce2 = IMPORTER->ReadFloat();
		m_BumpMapDetailUVScale = IMPORTER->ReadFloat();
		m_fWaterHeight = IMPORTER->ReadFloat();

		//specular
		m_specExp = IMPORTER->ReadFloat();
		m_specIntensity = IMPORTER->ReadFloat();
	}
}

void CBigWaterRenderer::RenderWater1(CCamera* pCamera) {//big water�� �ϳ��� �������
														//���� �׸� �� depth stencil....�� �Ϸ��� �ߴµ� ����. �̰� stencil�ִ�?
														//stencil������
														//�� �׸� �� stencil ��ŷ
														//2. �ſ��� �׸���. ���ٽ� ���۸� ���� ���� Ÿ�ٿ� ������� ����.!

														//water color 
	XMFLOAT4* pColor = (XMFLOAT4*)m_pWaterColor->Map();
	pColor[0] = m_xmf4WaterColor;
	m_pWaterColor->Unmap();
	m_pWaterColor->SetShaderState();

	ID3D11DepthStencilState* pTmpDSS{ nullptr };
	UINT StencilRef{ 0 };
	auto pDeviceContext = GLOBALVALUEMGR->GetDeviceContext();
	pDeviceContext->OMGetDepthStencilState(&pTmpDSS, &StencilRef);
	pDeviceContext->OMSetDepthStencilState(m_pd3dMirrorToStencilState, 2);

	RCSELLER->GetTagRenderContainer()[tag::TAG_BIGWATER]["bigwater"]->Render(pCamera);

	//state ���� ����
	pDeviceContext->OMSetDepthStencilState(pTmpDSS, StencilRef);
}

void CBigWaterRenderer::RenderWaterSurface(CCamera* pCamera) {
	auto pBigWater = RCSELLER->GetTagRenderContainer()[tag::TAG_BIGWATER]["bigwater"]->GetObjectList().begin();
	stBigWaterInfo* pWaterInfo = (stBigWaterInfo*)m_pWaterInfoPSBuffer->Map();
	pWaterInfo[0].nFlag = 1;
	pWaterInfo[0].fHeight = (*pBigWater)->GetPositionY();
	m_pWaterInfoPSBuffer->Unmap();
	m_pWaterInfoPSBuffer->SetShaderState();

	//ǥ�� ���� �����Ѵ�.
	ID3D11RenderTargetView *pd3dRTVs[1] = { m_pRTVReflection };
	//float fClearColor[4] = { xmf4Xolor.x, xmf4Xolor.y, xmf4Xolor.z, xmf4Xolor.w };
	float fClearColor[4] = { 0.f, 0.f, 0.f, 1.f };
	if (m_pRTVReflection) GLOBALVALUEMGR->GetDeviceContext()->ClearRenderTargetView(m_pRTVReflection, fClearColor);

	GLOBALVALUEMGR->GetDeviceContext()->OMSetRenderTargets(1, pd3dRTVs, m_pDSVDepthStencil);

	//ī�޶��� ����� �ӽ÷� �����Ѵ�.
	XMVECTOR xmvPos = pCamera->GetPosition();
	XMVECTOR xmvLookAt = pCamera->GetLookAt();//���� ��ġ���� look���� �������� 1ũ�⸸ŭ ������ ��Ұ� look at

	//bigwater�� ��ġ�� �̿��� plane�� �����Ѵ�.
	//auto pBigWater = RCSELLER->GetTagRenderContainer()[tag::TAG_BIGWATER]["bigwater"]->GetObjectList().begin();
	XMVECTOR xmvPlane = XMPlaneFromPointNormal((*pBigWater)->GetPosition(), XMVectorSet(0, 1, 0, 0));

	//�� plane�� �̿��ؼ� ī�޶� �ݻ�
	pCamera->UpdateReflectionViewMtx(xmvPlane);
	pCamera->SetShaderState();
	
	//�� ī�޶� �̿��ؼ� �ֵ� ����
	UPDATER->GetSpaceContainer()->PrepareRender(pCamera, TAG_TERRAIN | TAG_STATIC_OBJECT | TAG_DYNAMIC_OBJECT);
	//render
	auto pObjectRenderer = RENDERER->GetObjectRenderer();
	ID3D11RasterizerState* pPrv;
	GLOBALVALUEMGR->GetDeviceContext()->RSGetState(&pPrv);
	GLOBALVALUEMGR->GetDeviceContext()->RSSetState(m_pTestRSState);
	pObjectRenderer->Excute(pCamera);
	GLOBALVALUEMGR->GetDeviceContext()->RSSetState(pPrv);
	UPDATER->GetSkyBoxContainer()->GetSkyBox()->RegistToContainer();
	pObjectRenderer->RenderSkyBox();
	pObjectRenderer->ClearSkyBox();
	//UPDATER->GetSkyBoxContainer()->GetSkyBox()->RegistToContainer();
	//clear
	//�� ī�޶� �̿��ؼ� �ֵ� ����

	//�� �ϰ� ī�޶� ���󺹱�
	pCamera->SetLookAt(xmvPos, xmvLookAt, XMVectorSet(0, 1, 0, 0));
	pCamera->UpdateViewMtx();
	pCamera->UpdateShaderState();
	pCamera->SetShaderState();

	//debug
	DEBUGER->AddTexture(XMFLOAT2(200, 200), XMFLOAT2(400, 400), m_pSRVReflection);
}

void CBigWaterRenderer::RenderRefractions(CCamera* pCamera) {

}

void CBigWaterRenderer::RenderWater2(CCamera* pCamera, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV) {
	auto pDeviceContext = GLOBALVALUEMGR->GetDeviceContext();
	auto pBigWater = RCSELLER->GetTagRenderContainer()[tag::TAG_BIGWATER]["bigwater"]->GetObjectList().begin();

	//rtv dsv set
	ID3D11RenderTargetView *pd3dRTVs[1] = { pRTV };
	pDeviceContext->OMSetRenderTargets(1, pd3dRTVs, pDSV);

	//bump map set
	m_pWaterBaseBumpMap->SetShaderState();
	m_pWaterDetailBumpMap->SetShaderState();

	//Surface render�� �ʿ��� ���� set
	stRenderSurfaceInfo* pRenderSurfaceInfo = (stRenderSurfaceInfo*)m_pWaterPSBuffer->Map();
	pRenderSurfaceInfo[0].ViewProjMatrix = XMMatrixTranspose(pCamera->GetViewMtx() * pCamera->GetProjectionMtx());
	pRenderSurfaceInfo[0].RefViewProjMatrix = XMMatrixTranspose(pCamera->GetReflectionViewMtx() * pCamera->GetProjectionMtx());
	XMStoreFloat3(&pRenderSurfaceInfo[0].eyeVector, pCamera->GetPosition());
	pRenderSurfaceInfo[0].fresnelMode = m_fresnelMode;
	pRenderSurfaceInfo[0].fPowFactor = m_fPowFactor;
	pRenderSurfaceInfo[0].xDrawfactor = m_xDrawfactor;
	pRenderSurfaceInfo[0].windDir = m_windDir;
	pRenderSurfaceInfo[0].windForce = m_windForce;
	pRenderSurfaceInfo[0].windDir2 = m_windDir2;
	pRenderSurfaceInfo[0].windForce2 = m_windForce2;

	m_progress += TIMEMGR->GetTimeElapsed();
	pRenderSurfaceInfo[0].progress = m_progress;
	pRenderSurfaceInfo[0].BumpMapBaseUVScale = m_BumpMapBaseUVScale;
	pRenderSurfaceInfo[0].BumpMapDetailUVScale = m_BumpMapDetailUVScale;
	XMStoreFloat3(&pRenderSurfaceInfo[0].DirToLight, -UPDATER->GetDirectionalLight()->GetLook());
	pRenderSurfaceInfo[0].DirLightColor = UPDATER->GetDirectionalLight()->GetColor();
	pRenderSurfaceInfo[0].specExp = m_specExp;
	pRenderSurfaceInfo[0].specIntensity = m_specIntensity;
	pRenderSurfaceInfo[0].fWaterHeight = m_fWaterHeight;
	m_pWaterPSBuffer->Unmap();
	m_pWaterPSBuffer->SetShaderState();


	ID3D11ShaderResourceView* pSRV[] = { m_pSRVReflection };
	pDeviceContext->PSSetShaderResources(4, 1, pSRV);

	//state set
	ID3D11DepthStencilState* pTmp{ nullptr };
	UINT StencilRef{ 0 };
	pDeviceContext->OMGetDepthStencilState(&pTmp, &StencilRef);
	pDeviceContext->OMSetDepthStencilState(m_pd3dReflectDepthStencilState, 2);
	ID3D11BlendState* pTmpBlendState{ nullptr };
	float pPreBlendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	float pBlendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	UINT TmpBlendSampleMask{ 0 };
	pDeviceContext->OMGetBlendState(&pTmpBlendState, pPreBlendFactor, &TmpBlendSampleMask);
	pDeviceContext->OMSetBlendState(m_pAlphaBlendState, pBlendFactor, 0xffffffff);


	m_pWaterSurface->SetWorldMtx((*pBigWater)->GetWorldMtx());
	m_pWaterSurface->RegistToContainer();//water surface 1�� set�ϰ� �׸� 
	RCSELLER->GetTagRenderContainer()[tag::TAG_BIGWATER]["bigwatersurface"]->Render(pCamera);

	for (auto RenderContainer : RCSELLER->GetTagRenderContainer()[tag::TAG_BIGWATER]) {
		RenderContainer.second->ClearObjectList();
	}

	//state ���� ����
	pDeviceContext->OMSetDepthStencilState(pTmp, StencilRef);
	pDeviceContext->OMSetBlendState(pTmpBlendState, pPreBlendFactor, TmpBlendSampleMask);
}
void CBigWaterRenderer::ResizeBuffer()
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
	//d3dTexture2DDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

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

	// �ݻ��
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pTXTReflection);
	GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pTXTReflection, &d3dRTVDesc, &m_pRTVReflection);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pTXTReflection, &d3dSRVDesc, &m_pSRVReflection);
	// ������
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pTXTRefraction);
	GLOBALVALUEMGR->GetDevice()->CreateRenderTargetView(m_pTXTRefraction, &d3dRTVDesc, &m_pRTVRefraction);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pTXTRefraction, &d3dSRVDesc, &m_pSRVRefraction);

	// Create the depth stencil view 
	D3D11_DEPTH_STENCIL_VIEW_DESC d3dDepthStencilViewDesc;
	ZeroMemory(&d3dDepthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	d3dDepthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	d3dDepthStencilViewDesc.Texture2D.MipSlice = 0;

	d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	d3dDepthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	d3dSRVDesc.Format = DXGI_FORMAT_R32_FLOAT;
	d3dTexture2DDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pTXTDepthStencil);
	GLOBALVALUEMGR->GetDevice()->CreateDepthStencilView(m_pTXTDepthStencil, &d3dDepthStencilViewDesc, &m_pDSVDepthStencil);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pTXTDepthStencil, &d3dSRVDesc, &m_pSRVDepthStencil);
}

void CBigWaterRenderer::ReleaseBuffer()
{
	if (m_pTXTReflection) m_pTXTReflection->Release();
	if (m_pRTVReflection) m_pRTVReflection->Release();
	if (m_pSRVReflection) m_pSRVReflection->Release();

	if (m_pTXTRefraction) m_pTXTRefraction->Release();
	if (m_pRTVRefraction) m_pRTVRefraction->Release();
	if (m_pSRVRefraction) m_pSRVRefraction->Release();
}

void CBigWaterRenderer::SetShaderState()
{
}

void CBigWaterRenderer::CleanShaderState()
{
}

void CBigWaterRenderer::UpdateShaderState()
{
}
