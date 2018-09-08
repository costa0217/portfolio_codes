#include "stdafx.h"
#include "BloomDownScale.h"

bool CBloomDownScale::Begin() {
	//cs shader resource CB
	m_pBloomStaticCB = RESOURCEMGR->CreateConstantBuffer("BloomStaticBuffer", 1, sizeof(stBloomStaticData), 12, BIND_CS);
	m_pBloomDynamicCB = RESOURCEMGR->CreateConstantBuffer("BloomDynamicBuffer", 1, sizeof(stBloomDynamicData), 0, BIND_CS);
	m_pFirstPassComputeShader = CComputeShader::CreateComputeShader(TEXT("CSDownScaleFirstPass.cso"));


	//tone mappint second pass shader resource
	//------------------------------second pass--------------------------
	//buffer
	D3D11_BUFFER_DESC descBuffer;
	ZeroMemory(&descBuffer, sizeof(D3D11_BUFFER_DESC));
	descBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	descBuffer.StructureByteStride = sizeof(float);
	descBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	descBuffer.ByteWidth = sizeof(float);
	GLOBALVALUEMGR->GetDevice()->CreateBuffer(&descBuffer, nullptr, &m_pSecondPassBuffer);
	GLOBALVALUEMGR->GetDevice()->CreateBuffer(&descBuffer, nullptr, &m_pPrevAvgLumBuffer);//이전 평균휘도 값
																						  //uav 
	D3D11_UNORDERED_ACCESS_VIEW_DESC descUAV;
	ZeroMemory(&descUAV, sizeof(descUAV));
	descUAV.Format = DXGI_FORMAT_UNKNOWN;
	descUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	descUAV.Buffer.NumElements = 1;
	GLOBALVALUEMGR->GetDevice()->CreateUnorderedAccessView(m_pSecondPassBuffer, &descUAV, &m_pSecondPassUAV);
	GLOBALVALUEMGR->GetDevice()->CreateUnorderedAccessView(m_pPrevAvgLumBuffer, &descUAV, &m_pPrevAvgLumUAV);

	//srv
	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	ZeroMemory(&descSRV, sizeof(descSRV));
	descSRV.Format = DXGI_FORMAT_UNKNOWN;
	descSRV.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
	descSRV.Buffer.NumElements = 1;
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pSecondPassBuffer, &descSRV, &m_pSecondPassSRV);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pPrevAvgLumBuffer, &descSRV, &m_pPrevAvgLumSRV);

	m_pSecondPassComputeShader = CComputeShader::CreateComputeShader(TEXT("CSDownScaleSecondPass.cso"));
	return true;
}

bool CBloomDownScale::End() {
	//if (m_pFirstPassCB) m_pFirstPassCB->Release();
	
	m_pBloomStaticCB = nullptr;
	m_pBloomDynamicCB = nullptr;

	if (m_pFirstPassBuffer) m_pFirstPassBuffer->Release();
	m_pFirstPassBuffer = nullptr;
	if (m_pFirstPassAvgLumSRV) m_pFirstPassAvgLumSRV->Release();
	m_pFirstPassAvgLumSRV = nullptr;
	if (m_pFirstPassAvgLumUAV) m_pFirstPassAvgLumUAV->Release();
	m_pFirstPassAvgLumUAV = nullptr;

	//1
	if (m_pd3dtxtDownCasting) m_pd3dtxtDownCasting->Release();
	m_pd3dtxtDownCasting = nullptr;
	if (m_pd3dsrvDownCasting) m_pd3dsrvDownCasting->Release();
	m_pd3dsrvDownCasting = nullptr;
	if (m_pd3duavDownCasting) m_pd3duavDownCasting->Release();
	m_pd3duavDownCasting = nullptr;


	//second pass
	if (m_pSecondPassBuffer) m_pSecondPassBuffer->Release();
	m_pSecondPassBuffer = nullptr;
	if (m_pSecondPassUAV) m_pSecondPassUAV->Release();
	m_pSecondPassUAV = nullptr;
	if (m_pSecondPassSRV) m_pSecondPassSRV->Release();
	m_pSecondPassSRV = nullptr;

	m_pFirstPassComputeShader->End();
	delete m_pFirstPassComputeShader;
	m_pFirstPassComputeShader = nullptr;
	m_pSecondPassComputeShader->End();
	delete m_pSecondPassComputeShader;
	m_pSecondPassComputeShader = nullptr;
	return true;
}

void CBloomDownScale::SetShaderState() {
	
}

void CBloomDownScale::CleanShaderState() {
	
}

void CBloomDownScale::UpdateShaderState() {

}

void CBloomDownScale::Excute() {
	//first pass
	stBloomDynamicData* pDynamicData = (stBloomDynamicData*)m_pBloomDynamicCB->Map();
	pDynamicData->fAdaptation = m_fAdaptation;
	m_pBloomDynamicCB->Unmap();

	//input은 이미 set되어 있음 srv..

	//uav
	//bloom
	//첫 패스에서 블룸 1/16scale texture를 set한다.
	ID3D11UnorderedAccessView* arrUAVs[2] = { m_pd3duavDownCasting, m_pFirstPassAvgLumUAV };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 2, arrUAVs, (UINT*)(&arrUAVs));

	//constant
	m_pBloomStaticCB->SetShaderState();
	m_pBloomDynamicCB->SetShaderState();
	//cs도 set되어 있음
	//이후 dispatch

	m_pFirstPassComputeShader->ExcuteShaderState(m_nDownScaleGroup, 1, 1);

	//uav set nullptr
	ID3D11UnorderedAccessView* pUAVs[2] = { nullptr, nullptr };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 2, pUAVs, (UINT*)(&pUAVs));

	//set next pass input
	//down scale 다 함 그럼 1/16 scale texture가 생성됨
	//이건 있다는 것이 확인됨

	ID3D11ShaderResourceView* pSRVs[2] = { m_pd3dsrvDownCasting, m_pFirstPassAvgLumSRV };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(0, 2, pSRVs);
//first pass
//second pass
//cs는 자동으로 clean up 됨 shader resource 들만 정리하면 됨

//ID3D11ShaderResourceView* pSRVs[1] = { nullptr };
//m_pd3dDeviceContext->PSSetShaderResources(1, 1, pSRVs);


//input, constant buffer는 이미 set되어 있다.
	ID3D11UnorderedAccessView* arrSecondPassUAVs[1] = { m_pSecondPassUAV };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, arrSecondPassUAVs, (UINT*)(&arrSecondPassUAVs));

	ID3D11ShaderResourceView* pPrevAvgLumSRVs[1] = { m_pPrevAvgLumSRV };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(2, 1, pPrevAvgLumSRVs);
	//m_pd3dDeviceContext->PSSetShaderResources(5, 1, pSRVs);

	m_pSecondPassComputeShader->ExcuteShaderState(1, 1, 1);

	ID3D11UnorderedAccessView* pClearSecondPassUAVs[1] = { nullptr };
	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 1, pClearSecondPassUAVs, (UINT*)(&pClearSecondPassUAVs));

	//그리고 이제 ps에 hdr texture는 이미 set되어 있다.
	//0번에 1개가 set되어 있다.
	//1번에 지금껏 구한 평균 휘도 값을 set한다.
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(1, 1, &m_pSecondPassSRV);//tone mapping
	GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(1, 1, &m_pSecondPassSRV);//temp bloom tex 제작
}

void CBloomDownScale::ResizeBuffer(){
	ReleaseBuffer();
	
	m_nWidth = GLOBALVALUEMGR->GetrcClient().right/4;
	m_nHeight = GLOBALVALUEMGR->GetrcClient().bottom/4;
	m_nDownScaleGroup = (UINT)ceil((float)(m_nWidth*m_nHeight) / 1024);

	ChangeStaticData();
	//----------------------------------------UAV Desc----------------------------------------------//
	D3D11_UNORDERED_ACCESS_VIEW_DESC d3dUAVDesc;
	ZeroMemory(&d3dUAVDesc, sizeof(D3D11_UNORDERED_ACCESS_VIEW_DESC));
	d3dUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	d3dUAVDesc.Buffer.FirstElement = 0;
	d3dUAVDesc.Buffer.NumElements = static_cast<UINT>(m_nWidth*m_nHeight);
	//----------------------------------------UAV Desc----------------------------------------------//
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
	d3dTexture2DDesc.Width = static_cast<UINT>(m_nWidth);
	d3dTexture2DDesc.Height = static_cast<UINT>(m_nHeight);
	d3dTexture2DDesc.MipLevels = 1;
	d3dTexture2DDesc.ArraySize = 1;
	d3dTexture2DDesc.SampleDesc.Count = 1;
	d3dTexture2DDesc.SampleDesc.Quality = 0;
	d3dTexture2DDesc.Usage = D3D11_USAGE_DEFAULT;
	d3dTexture2DDesc.CPUAccessFlags = 0;
	d3dTexture2DDesc.MiscFlags = 0;
	d3dTexture2DDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	//d3dTexture2DDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	//----------------------------------------TextUre Desc-----------------------------------------//
	d3dTexture2DDesc.Format = d3dSRVDesc.Format = d3dUAVDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;

	//1.
	GLOBALVALUEMGR->GetDevice()->CreateTexture2D(&d3dTexture2DDesc, nullptr, &m_pd3dtxtDownCasting);
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pd3dtxtDownCasting, &d3dSRVDesc, &m_pd3dsrvDownCasting);
	GLOBALVALUEMGR->GetDevice()->CreateUnorderedAccessView(m_pd3dtxtDownCasting, &d3dUAVDesc, &m_pd3duavDownCasting);


	//-------------------------------first pass----------------------
	//buffer
	D3D11_BUFFER_DESC descBuffer;
	ZeroMemory(&descBuffer, sizeof(D3D11_BUFFER_DESC));
	descBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	descBuffer.StructureByteStride = sizeof(float);
	descBuffer.ByteWidth = m_nDownScaleGroup * descBuffer.StructureByteStride;
	descBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	GLOBALVALUEMGR->GetDevice()->CreateBuffer(&descBuffer, nullptr, &m_pFirstPassBuffer);

	//uav
	
	D3D11_UNORDERED_ACCESS_VIEW_DESC descUAV;
	ZeroMemory(&descUAV, sizeof(descUAV));
	descUAV.Format = DXGI_FORMAT_UNKNOWN;
	descUAV.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	descUAV.Buffer.NumElements = m_nDownScaleGroup;
	GLOBALVALUEMGR->GetDevice()->CreateUnorderedAccessView(m_pFirstPassBuffer, &descUAV, &m_pFirstPassAvgLumUAV);

	//srv
	
	D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
	ZeroMemory(&descSRV, sizeof(descSRV));
	descSRV.Format = DXGI_FORMAT_UNKNOWN;
	descSRV.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
	descSRV.Buffer.NumElements = m_nDownScaleGroup;
	GLOBALVALUEMGR->GetDevice()->CreateShaderResourceView(m_pFirstPassBuffer, &descSRV, &m_pFirstPassAvgLumSRV);

}

void CBloomDownScale::ReleaseBuffer(){
	//bloom
	if (m_pd3dtxtDownCasting) m_pd3dtxtDownCasting->Release();
	m_pd3dtxtDownCasting = nullptr;
	if (m_pd3dsrvDownCasting) m_pd3dsrvDownCasting->Release();
	m_pd3dsrvDownCasting = nullptr;
	if (m_pd3duavDownCasting) m_pd3duavDownCasting->Release();
	m_pd3duavDownCasting = nullptr;

	if (m_pFirstPassBuffer) m_pFirstPassBuffer->Release();
	m_pFirstPassBuffer = nullptr;
	if (m_pFirstPassAvgLumUAV) m_pFirstPassAvgLumUAV->Release();
	m_pFirstPassAvgLumUAV = nullptr;
	if (m_pFirstPassAvgLumSRV) m_pFirstPassAvgLumSRV->Release();
	m_pFirstPassAvgLumSRV = nullptr;
}

void CBloomDownScale::SetAdaptation(float fTimeElapsed){
	float fAdaptationNorm;
	static float fAdaptation = 1.0f;
	static bool s_bFirstTime = true;
	if (s_bFirstTime)
	{
		// On the first frame we want to fully adapt the new value so use 0
		fAdaptationNorm = 0.0f;
		s_bFirstTime = false;
	}
	else
	{
		// Normalize the adaptation time with the frame time (all in seconds)
		// Never use a value higher or equal to 1 since that means no adaptation at all (keeps the old value)
		fAdaptationNorm = min(fAdaptation < 0.0001f ? 1.0f : fTimeElapsed / fAdaptation, 0.9999f);
	}
	m_fAdaptation = fAdaptationNorm;
}

void CBloomDownScale::SwapAdaptationBuff() {
	//적응
	//계산 이 끝난 후에 두개의 버퍼의 값을 바꾸어 준다.
	ID3D11Buffer* pTempBuff = m_pPrevAvgLumBuffer;
	ID3D11ShaderResourceView* pTempSRV = m_pPrevAvgLumSRV;
	ID3D11UnorderedAccessView* pTempUAV = m_pPrevAvgLumUAV;

	m_pPrevAvgLumSRV = m_pSecondPassSRV;
	m_pPrevAvgLumUAV = m_pSecondPassUAV;
	m_pPrevAvgLumBuffer = m_pSecondPassBuffer;

	m_pSecondPassSRV = pTempSRV;
	m_pSecondPassUAV = pTempUAV;
	m_pSecondPassBuffer = pTempBuff;


	//계산 이 끝난 후에 두개의 버퍼의 값을 바꾸어 준다.

}

void CBloomDownScale::SetBloomThreshold(float fBloomThreshold){
	m_fBloomThreshold = fBloomThreshold;
	ChangeStaticData();
}

void CBloomDownScale::ChangeStaticData(){
	stBloomStaticData* pStaticData = (stBloomStaticData*)m_pBloomStaticCB->Map();
	pStaticData->nGroupSize = m_nDownScaleGroup;
	pStaticData->nTotalPixels = m_nWidth * m_nHeight;
	pStaticData->fBloomThreshold = m_fBloomThreshold;
	m_pBloomStaticCB->Unmap();
	m_pBloomStaticCB->SetShaderState();
}

CBloomDownScale::CBloomDownScale() : DXObject("bloomdownscale") {

}

CBloomDownScale::~CBloomDownScale() {

}
