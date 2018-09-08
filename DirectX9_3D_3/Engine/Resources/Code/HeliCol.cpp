#include "HeliCol.h"

Engine::CHeliCol::CHeliCol(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
{

}

Engine::CHeliCol::~CHeliCol(void)
{

}

Engine::CResources* Engine::CHeliCol::CloneResource(void)
{
	++(*m_pwRefCnt);

	return new CHeliCol(*this);
}

HRESULT Engine::CHeliCol::CreateBuffer(int iAllVtxCnt, vector<VTXCOL>& vecVTX)
{
	if(vecVTX.empty())
		return E_FAIL;

	m_dwVtxSize = sizeof(VTXCOL);
	m_dwVtxCnt	= iAllVtxCnt;
	m_dwFVF		= VTXCOL_FVF;

	m_dwIdxSize = sizeof(INDEX16);
	m_dwTriCnt	= iAllVtxCnt / 2;
	m_IdxFmt	= D3DFMT_INDEX16;

	FAILED_CHECK(CVIBuffer::CreateBuffer());

	VTXCOL*		pVertex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	for(int i = 0; i < iAllVtxCnt; ++i)
	{
		D3DXVECTOR3	vPos	= vecVTX[i].vPos;
		DWORD		dwColor	= vecVTX[i].dwColor;

		pVertex[i].vPos		= vPos;
		pVertex[i].dwColor	= dwColor;
	}

	m_pVB->Unlock();

	int		iTriCnt = 0;

	INDEX16*		pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	
	for(int j = 0; j < iAllVtxCnt / 4; ++j)
	{
		pIndex[iTriCnt]._1 = 0 + iTriCnt * 2;
		pIndex[iTriCnt]._2 = 1 + iTriCnt * 2;
		pIndex[iTriCnt]._3 = 3 + iTriCnt * 2;
		++iTriCnt;

		pIndex[iTriCnt]._1 = 1 + ((iTriCnt- 1) * 2);
		pIndex[iTriCnt]._2 = 2 + ((iTriCnt- 1) * 2);
		pIndex[iTriCnt]._3 = 3 + ((iTriCnt- 1) * 2);
		++iTriCnt;
	}	

	m_pIB->Unlock();




	return S_OK;
}

Engine::CHeliCol* Engine::CHeliCol::Create(LPDIRECT3DDEVICE9 pDevice 
										   , int iAllVtxCnt 
										   , vector<VTXCOL>& vecVTX)
{
	CHeliCol* pBuffer = new CHeliCol(pDevice);
	if(FAILED(pBuffer->CreateBuffer(iAllVtxCnt, vecVTX)))
		Engine::Safe_Delete(pBuffer);

	return pBuffer;
}
