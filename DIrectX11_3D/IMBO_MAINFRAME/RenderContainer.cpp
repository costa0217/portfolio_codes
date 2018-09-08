#include "stdafx.h"
#include "RenderContainer.h"



//---------------------------dxobject---------------------------------
bool CRenderContainer::Begin() {

	return true;
}
bool CRenderContainer::End() {
	//m_lpObjects.clear();
	m_lpObjects.clear();
	m_vpTexture.clear();
	m_vpBuffer.clear();
	m_vpMesh.clear();

	delete[] m_ppBufferData;

	return true;
}
//---------------------------dxobject---------------------------------

//--------------------------container---------------------------------
void CRenderContainer::UpdateShaderState( CCamera* pCamera) {

	//----------------------------update instance buffer--------------------------
	if (m_vpBuffer.empty()) return;
	int nInstance = 0;
	int nBuffer = 0;
	//map
	size_t vecBufferSize = m_vpBuffer.size();
	for (size_t i = 0; i < vecBufferSize; ++i)
	{
		m_ppBufferData[nBuffer++] = m_vpBuffer[i]->Map();
	}
	/*for (auto p : m_vpBuffer) {
		m_ppBufferData[nBuffer++] = p->Map();
	}*/

	bool bIsDebug = INPUTMGR->GetDebugMode();
	size_t vecSize = m_lpObjects.size();
	for (size_t i = 0; i < vecSize; ++i)
	{
		if (true == bIsDebug)
		{
			DEBUGER->RegistToDebugRenderContainer(m_lpObjects[i]);
			m_lpObjects[i]->RegistToDebuger();
		}
		m_lpObjects[i]->SetBufferInfo(m_ppBufferData, nInstance, pCamera);
		nInstance++;
	}
	/*for (auto pObject : m_lpObjects) {
		if (true == bIsDebug)
		{
			DEBUGER->RegistToDebugRenderContainer(pObject);
			pObject->RegistToDebuger();
		}
		pObject->SetBufferInfo(m_ppBufferData, nInstance, pCamera);
		nInstance++;
	}*/

	//unmap
	for (size_t i = 0; i < vecBufferSize; ++i)
	{
		m_vpBuffer[i]->Unmap();
	}
	/*for (auto p : m_vpBuffer) {
		p->Unmap();
	}*/
	//----------------------------update instance buffer--------------------------


	//if (m_pAnimater) m_pAnimater->Update(TIMEMGR->GetTimeElapsed());

	m_pShader->UpdateShaderState();
	size_t vecTexSize = m_vpTexture.size();
	for (size_t i = 0; i < vecTexSize; ++i)
	{
		m_vpTexture[i]->UpdateShaderState();
	}
	size_t vecMatSize = m_vpMaterial.size();
	for (size_t i = 0; i < vecMatSize; ++i)
	{
		m_vpMaterial[i]->UpdateShaderState();
	}
	for (size_t i = 0; i < vecBufferSize; ++i)
	{
		m_vpBuffer[i]->UpdateShaderState();
	}
	/*for (auto p : m_vpBuffer) {
		p->UpdateShaderState();
	}*/

	//if (m_pGlobalBuffer) m_pGlobalBuffer->UpdateShaderState();
	if(m_pRimColorBuffer) m_pRimColorBuffer->UpdateShaderState();

}
void CRenderContainer::SetShaderState() {
	m_pShader->SetShaderState();

	for (auto p : m_vpTexture) {
		p->SetShaderState();
	}
	for (auto p : m_vpVolatileTexture) {
		p->SetShaderState();
	}
	for (auto p : m_vpMaterial) {
		p->SetShaderState();
	}
	for (auto p : m_vpBuffer) {
		p->SetShaderState();
	}
	for (auto p : m_vpVolatileBuffer) {
		p->SetShaderState();
	}

	if (m_pAnimater)m_pAnimater->SetShaderState();
	if (m_pRimColorBuffer) m_pRimColorBuffer->SetShaderState();
}

void CRenderContainer::ClearVolatileResources() {
	m_vpVolatileTexture.clear();
	m_vpVolatileBuffer.clear();
}

bool CRenderContainer::CheckPickMesh(XMVECTOR xmvModelCameraStartPos, XMVECTOR xmvModelRayDir, float & distance) {
	bool bResult = false;
	float fHitDistance = FLT_MAX;
	//	distance = fHitDistance;
	float fNearHitDistance = FLT_MAX;

	for (auto pMesh : m_vpMesh) {
		if (pMesh->CheckPickMesh(xmvModelCameraStartPos, xmvModelRayDir, fHitDistance)) {
			if (fNearHitDistance > fHitDistance) {
				fNearHitDistance = fHitDistance;
			}
			bResult = true;
			//break;
		}
	}
	distance = fNearHitDistance;
	return bResult;
}
void CRenderContainer::RenderExcute() {
	for (auto p : m_vpMesh) {
		p->Render(m_lpObjects.size());
	}
}
void CRenderContainer::RenderExcuteWithOutObject() {
	for (auto p : m_vpMesh) {
		p->Render(1);
	}
}
void CRenderContainer::CleanShaderState() {
	m_pShader->CleanShaderState();

	for (auto p : m_vpTexture) {
		p->CleanShaderState();
	}
	for (auto p : m_vpVolatileTexture) {
		p->CleanShaderState();
	}
	for (auto p : m_vpMaterial) {
		p->CleanShaderState();
	}
	for (auto p : m_vpBuffer) {
		p->CleanShaderState();
	}
	for (auto p : m_vpVolatileBuffer) {
		p->CleanShaderState();
	}
	if (m_pAnimater)m_pAnimater->CleanShaderState();
	if (m_pRimColorBuffer) m_pRimColorBuffer->CleanShaderState();
	//if (m_pGlobalBuffer) m_pGlobalBuffer->CleanShaderState();//global buffer
}


//--------------------------container 불변 함수---------------------------------
void CRenderContainer::Render( CCamera* pCamera) {
	if (m_lpObjects.empty()) return;

	UpdateShaderState(pCamera);
	SetShaderState();
	RenderExcute();			//Render;
	//CleanShaderState();
	m_pShader->CleanShaderState();
	if (m_pAnimater)m_pAnimater->CleanShaderState();
}

void CRenderContainer::SetShaderState_NoPS(){
	m_pShader->SetShaderState_NoPS();

	for (auto p : m_vpTexture) {
		p->SetShaderState();
	}
	for (auto p : m_vpVolatileTexture) {
		p->SetShaderState();
	}
	for (auto p : m_vpMaterial) {
		p->SetShaderState();
	}
	for (auto p : m_vpBuffer) {
		p->SetShaderState();
	}
	for (auto p : m_vpVolatileBuffer) {
		p->SetShaderState();
	}

	if (m_pAnimater)m_pAnimater->SetShaderState();

}

void CRenderContainer::Render_NoPS(CCamera * pCamera){
	if (m_lpObjects.empty()) return;

	UpdateShaderState(pCamera);
	SetShaderState_NoPS();
	RenderExcute();			//Render;
							//CleanShaderState();
	m_pShader->CleanShaderState();
	if (m_pAnimater)m_pAnimater->CleanShaderState();
}

void CRenderContainer::RenderWithOutObject( CCamera* pCamera) {
	//shader State Update/ Instancing Buffet Update
	UpdateShaderState(pCamera);
	SetShaderState();
	RenderExcuteWithOutObject();
	//CleanShaderState();
	m_pShader->CleanShaderState();
	if (m_pAnimater)m_pAnimater->CleanShaderState();
}

void CRenderContainer::ClearMesh() {
	if (m_vpMesh.empty())return;
	m_vpMesh.clear();
}

void CRenderContainer::ClearBuffer() {
	if (m_vpBuffer.empty())return;
	m_vpBuffer.clear();
	m_nBuffer = 0;
}
void CRenderContainer::ClearAnimater() {
	m_pAnimater = nullptr;
	//animater를 지울일이 없음.. 사용하는거지 내가 관리할 녀석ㅇ ㅣ아니거든..
}

void CRenderContainer::AddMesh( CMesh* pMesh) {
	if (!pMesh) return;
	m_vpMesh.push_back(pMesh);
	//m_vpMesh.push_back(pMesh);
}
void CRenderContainer::SetMesh( CMesh* pMesh) {
	if (!pMesh) return;
	m_vpMesh.push_back(pMesh);
	//m_vpMesh.push_back(pMesh);
}
void CRenderContainer::SetShader( CRenderShader* pShader) {
	if (!pShader)return;

	m_pShader = pShader;
}
void CRenderContainer::AddTexture( CTexture* pTexture) {
	if (!pTexture)return;
	m_vpTexture.push_back(pTexture);
	//m_vpTexture.emplace_back(pTexture);
}

void CRenderContainer::AddVolatileTexture( CTexture* pTexture) {
	if (!pTexture)return;
	m_vpVolatileTexture.push_back(pTexture);
	//m_vpVolatileTexture.emplace_back(pTexture);
}

//-----------------------------------------buffer-----------------------------------------
void CRenderContainer::AddBuffer( CBuffer* pBuffer) {
	if (!pBuffer) return;


	//void**를 초기화 하기 위해 필요한놈
	m_nBuffer++;
	m_vpBuffer.push_back(pBuffer);
	//m_vpBuffer.emplace_back(pBuffer);

	if (m_ppBufferData) delete[] m_ppBufferData;
	m_ppBufferData = new void*[m_vpBuffer.size()];
}
void CRenderContainer::AddVolatileBuffer( CBuffer* pBuffer) {
	if (!pBuffer) return;
	m_vpVolatileBuffer.push_back(pBuffer);
	//m_vpVolatileBuffer.emplace_back(pBuffer);
}
void CRenderContainer::AddInstanceBuffer( CBuffer* pBuffer) {
	if (!pBuffer) return;

	//void**를 초기화 하기 위해 필요한놈
	m_nBuffer++;
	m_vpBuffer.push_back(pBuffer);
	/*m_vpBuffer.emplace_back(pBuffer);*/

	if (m_ppBufferData) delete[] m_ppBufferData;
	m_ppBufferData = new void*[m_vpBuffer.size()];

	ID3D11Buffer* ppBuffers[1] = { pBuffer->GetBuffer() };
	UINT ppStrides[1] = { pBuffer->GetBufferStride() };
	UINT ppOffset[1] = { pBuffer->GetOffset() };


	size_t vecSize = m_vpMesh.size();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_vpMesh[i]->AddInstancingBuffer(pBuffer);
	}
	//for (auto p : m_vpMesh) {
	//	//p->AssembleToVertexBuffer(1, ppBuffers, ppStrides, ppOffset);
	//	p->AddInstancingBuffer(pBuffer.get());
	//}

}
//-----------------------------------------buffer-----------------------------------------

void CRenderContainer::AddMaterial( CMaterial* pMaterial) {
	if (!pMaterial)return;

	m_nMaterial++;
	m_vpMaterial.push_back(pMaterial);
	//m_vpMaterial.emplace_back(pMaterial);
}

void CRenderContainer::SetObejcts(int n, CGameObject** ppObjects) {
	if (!ppObjects) return;

	for (int i = 0; i < n; ++i) {
		m_lpObjects.push_back(ppObjects[i]);
	}
}

void CRenderContainer::AddObject(CGameObject* pObject) {
	if (!pObject) return;

	m_lpObjects.push_back(pObject);
}
//void CRenderContainer::RemoveObject(CGameObject* pObject) {
//	if (!pObject) return;
//	//if (0 == m_lpObjects.size()) return;
//
//	// 벡터로 다 바꿈
//	/*m_lpObjects.remove_if([&pObject](CGameObject* pO) {
//		return pObject == pO;
//	});*/
//}
//--------------------------container 불변 함수---------------------------------

CRenderContainer::CRenderContainer() : DXObject("rendercontainer") {
}
CRenderContainer::~CRenderContainer() {

}