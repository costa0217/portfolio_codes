#include "TerrainTex.h"

Engine::CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pDevice)
: CVIBuffer(pDevice)
, m_dwVtxCntX(0)
, m_dwVtxCntZ(0)
{

}

Engine::CTerrainTex::~CTerrainTex(void)
{

}

Engine::CResources* Engine::CTerrainTex::CloneResource(void)
{
	++(*m_pwRefCnt);

	return new CTerrainTex(*this);
}

HRESULT Engine::CTerrainTex::CreateBuffer(const wstring& wstrPath/*const WORD& wCntX, const WORD& wCntZ, const WORD& wItv*/)
{
	HANDLE hFile = CreateFile(wstrPath.c_str(), GENERIC_READ, 0, NULL, 
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte = 0;

	DWORD	wCntX = 0;
	DWORD	wCntZ = 0;

	ReadFile(hFile, &wCntX, sizeof(DWORD), &dwByte, NULL);
	ReadFile(hFile, &wCntZ, sizeof(DWORD), &dwByte, NULL);

	DWORD dwVtxCnt = wCntX * wCntZ;

	m_dwVtxCntX = wCntX;
	m_dwVtxCntZ = wCntZ;

	Engine::VTXTEX* pLoadVertex = new Engine::VTXTEX[wCntX * wCntZ];
	Engine::VTXTEX* pTerrainVtx = new Engine::VTXTEX;

	for (DWORD i = 0; i < dwVtxCnt; ++i)
	{
		ReadFile(hFile, pTerrainVtx, sizeof(Engine::VTXTEX), &dwByte, NULL);

		pLoadVertex[i] = (*pTerrainVtx);
	}

	CloseHandle(hFile);

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = dwVtxCnt;
	m_dwFVF = VTXTEX_FVF;

	m_dwIdxSize = sizeof(INDEX32);
	m_dwTriCnt = (wCntX - 1) * (wCntZ - 1) * 2;
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK(CVIBuffer::CreateBuffer());

	CVIBuffer::SetVtxInfo(pLoadVertex);

	Safe_Delete(pLoadVertex);
	Safe_Delete(pTerrainVtx);

	INDEX32*		pIndex = NULL;
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	int		iTriCnt = 0;
	int		iIndex = 0;

	for(DWORD z = 0; z < wCntZ - 1; ++z)
	{
		for(DWORD x = 0; x < wCntX - 1; ++x)
		{
			iIndex = z * wCntX + x;

			pIndex[iTriCnt]._1 = iIndex + wCntX;
			pIndex[iTriCnt]._2 = iIndex + wCntX + 1;
			pIndex[iTriCnt]._3 = iIndex + 1;
			++iTriCnt;

			pIndex[iTriCnt]._1 = iIndex + wCntX;
			pIndex[iTriCnt]._2 = iIndex + 1;
			pIndex[iTriCnt]._3 = iIndex;
			++iTriCnt;
		}
	}
	m_pIB->Unlock();

	return S_OK;
}

Engine::CTerrainTex* Engine::CTerrainTex::Create(LPDIRECT3DDEVICE9 pDevice, const wstring& wstrPath
												 /*, const WORD& wCntX, const WORD& wCntZ, const WORD& wItv*/)
{
	CTerrainTex* pBuffer = new CTerrainTex(pDevice);
	if(FAILED(pBuffer->CreateBuffer(wstrPath/*wCntX, wCntZ, wItv*/)))
		Engine::Safe_Delete(pBuffer);

	return pBuffer;
}

void Engine::CTerrainTex::GetVtxCnt(DWORD* pdwCntX, DWORD* pdwCntZ)
{
	*pdwCntX = m_dwVtxCntX;
	*pdwCntZ = m_dwVtxCntZ;
}

