#include "WireTex.h"

Engine::CWireTex::CWireTex( LPDIRECT3DDEVICE9 pDevice )
: CVIBuffer(pDevice)
{

}

Engine::CWireTex::~CWireTex( void )
{

}

Engine::CResources* Engine::CWireTex::CloneResource( void )
{
	++(*m_pwRefCnt);

	return new CWireTex(*this);
}

HRESULT Engine::CWireTex::CreateBuffer( void )
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 4;
	m_dwFVF = VTXTEX_FVF;

	m_dwIdxSize = sizeof(INDEX16);
	m_dwTriCnt = 2;
	m_IdxFmt = D3DFMT_INDEX16;

	FAILED_CHECK(CVIBuffer::CreateBuffer());

	VTXTEX*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = D3DXVECTOR3(-0.4f, 1.f, 0.f);
	pVertex[0].vTexUV = D3DXVECTOR2(0.f, 0.f);

	pVertex[1].vPos = D3DXVECTOR3(0.4f, 1.f, 0.f);
	pVertex[1].vTexUV = D3DXVECTOR2(1.f, 0.f);

	pVertex[2].vPos = D3DXVECTOR3(0.4f, -1.f, 0.f);
	pVertex[2].vTexUV = D3DXVECTOR2(1.f, 1.f);

	pVertex[3].vPos = D3DXVECTOR3(-0.4f, -1.f, 0.f);
	pVertex[3].vTexUV = D3DXVECTOR2(0.f, 1.f);

	m_pVB->Unlock();


	INDEX16*		pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._1 = 0;
	pIndex[0]._2 = 1;
	pIndex[0]._3 = 3;

	pIndex[1]._1 = 1;
	pIndex[1]._2 = 2;
	pIndex[1]._3 = 3;

	m_pIB->Unlock();

	return S_OK;
}

Engine::CWireTex* Engine::CWireTex::Create( LPDIRECT3DDEVICE9 pDevice )
{
	CWireTex* pBuffer = new CWireTex(pDevice);
	if(FAILED(pBuffer->CreateBuffer()))
		Engine::Safe_Delete(pBuffer);

	return pBuffer;
}

