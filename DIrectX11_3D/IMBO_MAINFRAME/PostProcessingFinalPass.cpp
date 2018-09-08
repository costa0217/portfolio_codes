#include "stdafx.h"
#include "PostProcessingFinalPass.h"

bool CPostProcessingFinalPass::Begin() {
	for (auto RenderContainer : RCSELLER->GetTagRenderContainer()[tag::TAG_POSTPROCESSING]) {
		m_mRenderContainer[RenderContainer.first] = RenderContainer.second;
	}

	m_pFinalPassCB = RESOURCEMGR->CreateConstantBuffer("BloomFinalpassBuffer", 1, sizeof(stFinalPassCB), 13, BIND_PS);
	ChangeStaticData();
	return true;
}

bool CPostProcessingFinalPass::End() {
	m_mRenderContainer.clear();
	//m_mRenderContainer.RemoveAll();
	m_pFinalPassCB = nullptr;

	return true;
}

void CPostProcessingFinalPass::SetShaderState() {

}

void CPostProcessingFinalPass::CleanShaderState() {

}

void CPostProcessingFinalPass::UpdateShaderState() {

}

void CPostProcessingFinalPass::Excute( CCamera* pCamera) {
	//그것을 이용하여
	//풀 스크린 드로우를 실행 한다.
	//객체없는 랜더
	//CString csName =  ("postprocessing");
	m_mRenderContainer["postprocessing"]->RenderWithOutObject(pCamera);

	ID3D11Buffer* pBuffer[4] = { nullptr,nullptr,nullptr,nullptr };
	ID3D11ShaderResourceView* pSRVs[4] = { nullptr, nullptr,nullptr,nullptr };
	ID3D11UnorderedAccessView* pUAVs[4] = { nullptr,nullptr ,nullptr ,nullptr };

	GLOBALVALUEMGR->GetDeviceContext()->VSSetConstantBuffers(0, 4, pBuffer);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetConstantBuffers(0, 4, pBuffer);
	GLOBALVALUEMGR->GetDeviceContext()->CSSetConstantBuffers(0, 4, pBuffer);

	GLOBALVALUEMGR->GetDeviceContext()->VSSetShaderResources(0, 4, pSRVs);
	GLOBALVALUEMGR->GetDeviceContext()->PSSetShaderResources(0, 4, pSRVs);
	GLOBALVALUEMGR->GetDeviceContext()->CSSetShaderResources(0, 4, pSRVs);

	GLOBALVALUEMGR->GetDeviceContext()->CSSetUnorderedAccessViews(0, 4, pUAVs, (UINT*)(&pUAVs));

}

void CPostProcessingFinalPass::ChangeStaticData(){
	stFinalPassCB* pData = (stFinalPassCB*)m_pFinalPassCB->Map();
	pData->fMiddleGrey = m_fMeddleGrey;
	pData->fLumWhiteSqr = m_fWhite;
	pData->fLumWhiteSqr *= pData->fMiddleGrey;//Scale by the middle grey value
	pData->fLumWhiteSqr *= pData->fLumWhiteSqr;// Squre
	pData->fBloomScale = m_fBloomScale;
	m_pFinalPassCB->Unmap();
	m_pFinalPassCB->SetShaderState();
}

void CPostProcessingFinalPass::ResizeBuffer() {
	ReleaseBuffer();

}

void CPostProcessingFinalPass::ReleaseBuffer(){
}

CPostProcessingFinalPass::CPostProcessingFinalPass() : DXObject("postprocessingfinalpass") {

}

CPostProcessingFinalPass::~CPostProcessingFinalPass() {

}

void CPostProcessingFinalPass::SetBloomFinalPassValues(float fMeddleGrey, float fWhite, float fBloomScale){
	SetMiddleGrey(fMeddleGrey);
	SetWhite(fWhite);
	SetBloomScale(fBloomScale);
	ChangeStaticData();
}
