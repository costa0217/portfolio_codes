#include "stdafx.h"
#include "DebugRenderContainer.h"


//--------------------------container---------------------------------
void CDebugRenderContainer::UpdateShaderState( CCamera* pCamera) {
	if (m_vpBuffer.empty()) return;

	m_vpMesh[0]->UpdateShaderState();
	m_pShader->UpdateShaderState();

	for (auto p : m_vpTexture) {
		p->UpdateShaderState();
	}
	for (auto p : m_vpMaterial) {
		//p->UpdateShaderState();
	}
	for (auto p : m_vpBuffer) {
		p->UpdateShaderState();
	}

	//if (m_pGlobalBuffer) m_pGlobalBuffer->UpdateShaderState();
	//----------------------------update instance buffer--------------------------

	
	int nInstance = 0;

	int nBuffer = 0;
	//map
	size_t vecSize = m_vpBuffer.size();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_ppBufferData[nBuffer++] = m_vpBuffer[i]->Map();
	}
	//for (auto p : m_vpBuffer) {
	//	m_ppBufferData[nBuffer++] = p->Map();
	//}
	vecSize = m_lpObjects.size();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_lpObjects[i]->SetBufferInfo(m_ppBufferData, nInstance, pCamera);
		nInstance++;
	}
	/*for (auto pObject : m_lpObjects) {
		pObject->SetBufferInfo(m_ppBufferData, nInstance, pCamera);
		nInstance++;
	}*/

	//unmap
	vecSize = m_vpBuffer.size();
	for (size_t i = 0; i < vecSize; ++i)
	{
		m_vpBuffer[i]->Unmap();
	}
	//for (auto p : m_vpBuffer) {
	//	p->Unmap();
	//}
	//----------------------------update instance buffer--------------------------


}

CDebugRenderContainer::CDebugRenderContainer() : CRenderContainer() {
	
}
CDebugRenderContainer::~CDebugRenderContainer() {

}