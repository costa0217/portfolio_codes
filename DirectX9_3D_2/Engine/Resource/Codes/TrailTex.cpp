#include "TrailTex.h"

USING(Engine)

Engine::CTrailTex::CTrailTex(LPDIRECT3DDEVICE9 pGraphicDev)
: CVIBuffer(pGraphicDev)
{

}

Engine::CTrailTex::~CTrailTex(void)
{

}

HRESULT Engine::CTrailTex::Create_Buffer(void)
{
	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxCnt = 40;
	m_dwVtxFVF = VTXFVF_TEX;

	m_dwIdxSize = sizeof(INDEX16);
	m_dwTriCnt = 38;
	m_IdxFmt = D3DFMT_INDEX16;

	if(FAILED(CVIBuffer::Create_Buffer()))
		return E_FAIL;

	VTXTEX*			pVtxTex = NULL;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	for(int i = 0; i < 40; ++i)
	{
		if(i < 20)
		{
			pVtxTex[i].vPos = _vec3(0.f, 0.f, 0.f);
			pVtxTex[i].vNormal = _vec3(0.f, 0.f, 0.f);
			pVtxTex[i].vTexUV = _vec2(i * 0.05f, 0.f);
		}
		else
		{
			pVtxTex[i].vPos = _vec3(0.f, 0.f, 0.f);
			pVtxTex[i].vNormal = _vec3(0.f, 0.f, 0.f);
			pVtxTex[i].vTexUV = _vec2(i * 0.05f - 1.f, 1.f);
		}
	}

	m_pVB->Unlock();

	INDEX16*			pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	int iCnt = 0;
	for(int i = 0; i < 19; ++i)
	{
		pIndex[iCnt]._1 = i;
		pIndex[iCnt]._2 = i + 1;
		pIndex[iCnt]._3 = i + 20;
		++iCnt;

		pIndex[iCnt]._1 = i + 1;
		pIndex[iCnt]._2 = i + 20 + 1;
		pIndex[iCnt]._3 = i + 20;
		++iCnt;
	}
	m_pIB->Unlock();

	return S_OK;
}

Engine::CResources* Engine::CTrailTex::Clone_Resource(void)
{
	CResources* pResource = new CTrailTex(*this);

	++(*m_pwRefCnt);

	return pResource;
}

Engine::CTrailTex* Engine::CTrailTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrailTex*		pBuffer = new CTrailTex(pGraphicDev);

	if(FAILED(pBuffer->Create_Buffer()))	
		Engine::Safe_Release(pBuffer);

	return pBuffer;
}


_ulong Engine::CTrailTex::Release(void)
{
	_ulong dwRefCnt = Engine::CVIBuffer::Release();

	if(0 == dwRefCnt)
	{
		if(NULL == m_pwRefCnt)
		{

		}
		delete this;
	}

	return dwRefCnt;
}