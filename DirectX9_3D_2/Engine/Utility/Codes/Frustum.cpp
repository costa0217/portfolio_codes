#include "Frustum.h"

IMPLEMENT_SINGLETON(Engine::CFrustum)

Engine::CFrustum::CFrustum(void)
{
	ZeroMemory(m_vOriPoint, sizeof(D3DXVECTOR3) * 8);
	ZeroMemory(m_vConPoint, sizeof(D3DXVECTOR3) * 8);
}

Engine::CFrustum::~CFrustum(void)
{

}

HRESULT Engine::CFrustum::InitFrustum(LPDIRECT3DDEVICE9 pDevice)
{
	//¾Õ¸é
	m_vOriPoint[0] = D3DXVECTOR3(-1.f, 1.f, 0.f);
	m_vOriPoint[1] = D3DXVECTOR3(1.f, 1.f, 0.f);
	m_vOriPoint[2] = D3DXVECTOR3(1.f, -1.f, 0.f);
	m_vOriPoint[3] = D3DXVECTOR3(-1.f, -1.f, 0.f);

	//µÞ¸é
	m_vOriPoint[4] = D3DXVECTOR3(-1.f, 1.f, 1.f);
	m_vOriPoint[5] = D3DXVECTOR3(1.f, 1.f, 1.f);
	m_vOriPoint[6] = D3DXVECTOR3(1.f, -1.f, 1.f);
	m_vOriPoint[7] = D3DXVECTOR3(-1.f, -1.f, 1.f);

	D3DXMATRIX		matProj;
	pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, NULL, &matProj);

	for(int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vOriPoint[i], &m_vOriPoint[i], &matProj);
	}
	return S_OK;
}

void Engine::CFrustum::Transform_Frustum_ToWorld(LPDIRECT3DDEVICE9 pDevice)
{
	memcpy(m_vConPoint, m_vOriPoint, sizeof(D3DXVECTOR3) * 8);

	D3DXMATRIX		matView;
	pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	for(int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vConPoint[i], &m_vConPoint[i], &matView);
	}

	// +x
	D3DXPlaneFromPoints(&m_Plane[0], &m_vConPoint[1], &m_vConPoint[5], &m_vConPoint[6]);
	// -x
	D3DXPlaneFromPoints(&m_Plane[1], &m_vConPoint[4], &m_vConPoint[0], &m_vConPoint[3]);
	// +y
	D3DXPlaneFromPoints(&m_Plane[2], &m_vConPoint[4], &m_vConPoint[5], &m_vConPoint[1]);
	// -y
	D3DXPlaneFromPoints(&m_Plane[3], &m_vConPoint[3], &m_vConPoint[2], &m_vConPoint[6]);
	// +z
	D3DXPlaneFromPoints(&m_Plane[4], &m_vConPoint[4], &m_vConPoint[6], &m_vConPoint[5]);
	// -z
	D3DXPlaneFromPoints(&m_Plane[5], &m_vConPoint[0], &m_vConPoint[1], &m_vConPoint[2]);
}

void Engine::CFrustum::Transform_Frustum_Local(const D3DXMATRIX* pWorld)
{
	ZeroMemory(m_Plane, sizeof(D3DXPLANE) * 6);

	D3DXMATRIX		matWorldInv;
	D3DXMatrixInverse(&matWorldInv, NULL, pWorld);

	for(int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&m_vConPoint[i], &m_vConPoint[i], &matWorldInv);
	}

	// +x
	D3DXPlaneFromPoints(&m_Plane[0], &m_vConPoint[1], &m_vConPoint[5], &m_vConPoint[6]);
	// -x
	D3DXPlaneFromPoints(&m_Plane[1], &m_vConPoint[4], &m_vConPoint[0], &m_vConPoint[3]);
	// +y
	D3DXPlaneFromPoints(&m_Plane[2], &m_vConPoint[4], &m_vConPoint[5], &m_vConPoint[1]);
	// -y
	D3DXPlaneFromPoints(&m_Plane[3], &m_vConPoint[3], &m_vConPoint[2], &m_vConPoint[6]);
	// +z
	D3DXPlaneFromPoints(&m_Plane[4], &m_vConPoint[4], &m_vConPoint[6], &m_vConPoint[5]);
	// -z
	D3DXPlaneFromPoints(&m_Plane[5], &m_vConPoint[0], &m_vConPoint[1], &m_vConPoint[2]);
}

void Engine::CFrustum::CullingForTerrain(LPDIRECT3DDEVICE9 pDevice
										 , INDEX16* pIndex
										 , DWORD* pTriCnt 
										 , const VTXTEX* pTerrainVtx
										 , const WORD& wCntX, const WORD& wCntZ)
{
	*pTriCnt = 0;	

	WORD	wIndex = 0;

	bool		isIn[4] = {false};
	for(WORD z = 0;  z < wCntZ - 1; ++z)
	{
		for(WORD x = 0; x < wCntX - 1; ++x)
		{
			wIndex = z * wCntX + x;

			isIn[0] = VertexInFrustum(&pTerrainVtx[wIndex + wCntX].vPos);
			isIn[1] = VertexInFrustum(&pTerrainVtx[wIndex + wCntX + 1].vPos);
			isIn[2] = VertexInFrustum(&pTerrainVtx[wIndex + 1].vPos);
			isIn[3] = VertexInFrustum(&pTerrainVtx[wIndex].vPos);

			if(isIn[0] | isIn[1] | isIn[2])
			{
				pIndex[*pTriCnt]._1 = wIndex + wCntX;
				pIndex[*pTriCnt]._2 = wIndex + wCntX + 1;
				pIndex[*pTriCnt]._3 = wIndex + 1;
				++*pTriCnt;
			}

			if(isIn[0] | isIn[2] | isIn[3])
			{
				pIndex[*pTriCnt]._1 = wIndex + wCntX;
				pIndex[*pTriCnt]._2 = wIndex + 1;
				pIndex[*pTriCnt]._3 = wIndex;
				++*pTriCnt;
			}
		}
	}
}

bool Engine::CFrustum::VertexInFrustum(const D3DXVECTOR3* pPoint) const
{
	for(int i = 0; i < 6; ++i)
	{
		if(D3DXPlaneDotCoord(&m_Plane[i], pPoint) > -2)
			return false;
	}
	return true;
}

bool Engine::CFrustum::SphereInFrustum(const D3DXVECTOR3* pCenter, const float& fRadius) const
{
	for(int i = 0; i < 6; ++i)
	{
		if(D3DXPlaneDotCoord(&m_Plane[i], pCenter) > fRadius)
			return false;
	}
	return true;
}

void Engine::CFrustum::Release(void)
{
	delete this;
}
