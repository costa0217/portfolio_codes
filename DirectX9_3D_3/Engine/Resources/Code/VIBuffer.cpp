#include "VIBuffer.h"
#include "Engine_Include.h"

Engine::CVIBuffer::CVIBuffer(LPDIRECT3DDEVICE9 pDevice)
: CResources(pDevice)
{

}

Engine::CVIBuffer::~CVIBuffer(void)
{
	Release();
}

void Engine::CVIBuffer::GetVtxInfo(void* pVertex)
{
	void* pOriVertex = NULL;
	m_pVB->Lock(0, 0, &pOriVertex, 0);
	memcpy(pVertex, pOriVertex, m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();
}

void Engine::CVIBuffer::SetVtxInfo(void* pVertex)
{
	void* pOriVertex = NULL;
	m_pVB->Lock(0, 0, &pOriVertex, 0);
	memcpy(pOriVertex, pVertex, m_dwVtxSize * m_dwVtxCnt);
	m_pVB->Unlock();
}

HRESULT Engine::CVIBuffer::CreateBuffer(void)
{
	HRESULT		hr = NULL;

	hr = m_pDevice->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, 0, m_dwFVF, D3DPOOL_MANAGED
		, &m_pVB, NULL);
	FAILED_CHECK_MSG(hr, L"버텍스 버퍼 생성 실패");

	hr = m_pDevice->CreateIndexBuffer(m_dwIdxSize * m_dwTriCnt, 0, m_IdxFmt, D3DPOOL_MANAGED
		, &m_pIB, NULL);
	FAILED_CHECK_MSG(hr, L"버텍스 버퍼 생성 실패");
	return S_OK;
}

void Engine::CVIBuffer::Render(void)
{
	m_pDevice->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pDevice->SetFVF(m_dwFVF);
	m_pDevice->SetIndices(m_pIB);
	m_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

void Engine::CVIBuffer::Release(void)
{
	if((*m_pwRefCnt) == 0 )
	{
		Engine::Safe_Release(m_pVB);
		Engine::Safe_Release(m_pIB);
		CResources::Release();
	}
	else
		--(*m_pwRefCnt);
}