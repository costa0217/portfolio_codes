#include "Mesh.h"

USING(Engine)

Engine::CMesh::CMesh(LPDIRECT3DDEVICE9 pDevice)
: CResources(pDevice)
, m_pVB(NULL)
, m_pIB(NULL)
, m_vMin(0.f, 0.f, 0.f)
, m_vMax(0.f, 0.f, 0.f)
//, m_ptOBBInfo(NULL)
{

}

Engine::CMesh::CMesh(const CMesh& Instance)
: CResources(Instance)
, m_pVB(Instance.m_pVB)
, m_pIB(Instance.m_pIB)
, m_vMin(Instance.m_vMin)
, m_vMax(Instance.m_vMax)
//, m_ptOBBInfo(Instance.m_ptOBBInfo)
{
	memcpy(&m_tOBBInfo, &Instance.m_tOBBInfo, sizeof(OBBINFO));
	
}

Engine::CMesh::~CMesh(void)
{

}

_ulong Engine::CMesh::Release(void)
{
	_ulong dwRefCnt = Engine::CResources::Release();

	if(0 == dwRefCnt)
	{
		if(NULL == m_pwRefCnt)
		{
		
		}
	}
	return dwRefCnt;
}