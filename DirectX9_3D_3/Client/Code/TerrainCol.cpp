#include "stdafx.h"
#include "TerrainCol.h"

#include "Include.h"

CTerrainCol::CTerrainCol(void)
: m_pPos(NULL)
, m_pTerrainVtx(NULL)
, m_bYfix(true)
, m_dwVtxCntX(0)
, m_dwVtxCntZ(0)
, m_pLeftLegPos(NULL)
, m_pRightLegPos(NULL)
{

}

CTerrainCol::~CTerrainCol(void)
{
	Release();
}

Engine::CCollision* CTerrainCol::Clone(void)
{
	++(*m_pwRefCnt);

	return new CTerrainCol(*this);
}

void CTerrainCol::SetColInfo(D3DXVECTOR3* pPos, const Engine::VTXTEX* pTerrainVtx, DWORD dwCntX, DWORD dwCntZ)
{
	m_pPos = pPos;
	m_pTerrainVtx = pTerrainVtx;

	m_dwVtxCntX = dwCntX;
	m_dwVtxCntZ = dwCntZ;
}


void CTerrainCol::Update(void)
{
	if(m_pPos != NULL)
	{

		m_iIndex = (int(m_pPos->z) / VTXITV) * m_dwVtxCntX + (int(m_pPos->x) / VTXITV);

		if(m_iIndex < 0 || m_iIndex >= int(m_dwVtxCntX * m_dwVtxCntZ))
		{
			m_pPos->y = 1.f;
			return;
		}

		float	fRatioX = m_pPos->x - m_pTerrainVtx[m_iIndex + m_dwVtxCntX].vPos.x / VTXITV;
		float	fRatioZ = m_pTerrainVtx[m_iIndex + m_dwVtxCntX].vPos.z - m_pPos->z / VTXITV;

		D3DXPLANE		Plane;

		if(fRatioX > fRatioZ) //오른쪽 위
		{
			D3DXPlaneFromPoints(&Plane, &m_pTerrainVtx[m_iIndex + m_dwVtxCntX].vPos
				, &m_pTerrainVtx[m_iIndex + m_dwVtxCntX + 1].vPos
				, &m_pTerrainVtx[m_iIndex + 1].vPos);
		}
		else	//왼쪽아래
		{
			D3DXPlaneFromPoints(&Plane, &m_pTerrainVtx[m_iIndex + m_dwVtxCntX].vPos
				, &m_pTerrainVtx[m_iIndex + 1].vPos
				, &m_pTerrainVtx[m_iIndex].vPos);
		}

		//ax + by + cz + d = 0
		//by = -ax -cz - d
		//y = (-ax -cz - d) / b

		float PlaneY = (-Plane.a * m_pPos->x - Plane.c * m_pPos->z - Plane.d) / Plane.b;
		if(m_pPos->y - PlaneY < 5 && m_bYfix)
			m_pPos->y = PlaneY  + 5.f;
		//m_pPos->y += 1.f;
	}
	////////////////////////////////////////////
	///////////////////////////////////////////
	if(m_pLeftLegPos != NULL)
	{

		//왼쪽
		m_iIndex = (int(m_pLeftLegPos->z) / VTXITV) * m_dwVtxCntX + (int(m_pLeftLegPos->x) / VTXITV);

		if(m_iIndex < 0 || m_iIndex >= int(m_dwVtxCntX * m_dwVtxCntZ))
		{
			m_pLeftLegPos->y = 1.f;
			return;
		}

		float fRatioX = m_pLeftLegPos->x - m_pTerrainVtx[m_iIndex + m_dwVtxCntX].vPos.x / VTXITV;
		float fRatioZ = m_pTerrainVtx[m_iIndex + m_dwVtxCntX].vPos.z - m_pLeftLegPos->z / VTXITV;
		D3DXPLANE		Plane;

		if(fRatioX > fRatioZ) //오른쪽 위
		{
			D3DXPlaneFromPoints(&Plane, &m_pTerrainVtx[m_iIndex + m_dwVtxCntX].vPos
				, &m_pTerrainVtx[m_iIndex + m_dwVtxCntX + 1].vPos
				, &m_pTerrainVtx[m_iIndex + 1].vPos);
		}
		else	//왼쪽아래
		{
			D3DXPlaneFromPoints(&Plane, &m_pTerrainVtx[m_iIndex + m_dwVtxCntX].vPos
				, &m_pTerrainVtx[m_iIndex + 1].vPos
				, &m_pTerrainVtx[m_iIndex].vPos);
		}

		//ax + by + cz + d = 0
		//by = -ax -cz - d
		//y = (-ax -cz - d) / b

		float PlaneY = (-Plane.a * m_pLeftLegPos->x - Plane.c * m_pLeftLegPos->z - Plane.d) / Plane.b;
		if(m_pLeftLegPos->y - PlaneY < 5 && m_bYfix)
		{
			m_pHeliPos->y = PlaneY  + 5.f;
			*m_bHeliCol = true;
		}
		//임시 (좀더생각)
		if(m_pHeliPos->y < 0)
			m_pHeliPos->y = PlaneY  -m_pHeliPos->y;

	}
	///////////////////////
	if(m_pRightLegPos != NULL)
	{

		//오른쪽
		m_iIndex = (int(m_pRightLegPos->z) / VTXITV) * m_dwVtxCntX + (int(m_pRightLegPos->x) / VTXITV);

		if(m_iIndex < 0 || m_iIndex >= int(m_dwVtxCntX * m_dwVtxCntZ))
		{
			m_pRightLegPos->y = 1.f;
			return;
		}

		float fRatioX = m_pRightLegPos->x - m_pTerrainVtx[m_iIndex + m_dwVtxCntX].vPos.x / VTXITV;
		float fRatioZ = m_pTerrainVtx[m_iIndex + m_dwVtxCntX].vPos.z - m_pRightLegPos->z / VTXITV;
		D3DXPLANE		Plane;

		if(fRatioX > fRatioZ) //오른쪽 위
		{
			D3DXPlaneFromPoints(&Plane, &m_pTerrainVtx[m_iIndex + m_dwVtxCntX].vPos
				, &m_pTerrainVtx[m_iIndex + m_dwVtxCntX + 1].vPos
				, &m_pTerrainVtx[m_iIndex + 1].vPos);
		}
		else	//왼쪽아래
		{
			D3DXPlaneFromPoints(&Plane, &m_pTerrainVtx[m_iIndex + m_dwVtxCntX].vPos
				, &m_pTerrainVtx[m_iIndex + 1].vPos
				, &m_pTerrainVtx[m_iIndex].vPos);
		}

		//ax + by + cz + d = 0
		//by = -ax -cz - d
		//y = (-ax -cz - d) / b

		float PlaneY = (-Plane.a * m_pRightLegPos->x - Plane.c * m_pRightLegPos->z - Plane.d) / Plane.b;
		if(m_pRightLegPos->y - PlaneY < 5 && m_bYfix)
		{
			m_pHeliPos->y = PlaneY  + 5.f;
			*m_bHeliCol = true;

		}
		//임시 (좀더생각)
		if(m_pHeliPos->y < 0)
			m_pHeliPos->y = PlaneY  -m_pHeliPos->y;
	}
}

CTerrainCol* CTerrainCol::Create(void)
{
	return new CTerrainCol;
}

void CTerrainCol::Release(void)
{
	if( (*m_pwRefCnt) == 0 )
		Engine::Safe_Delete(m_pwRefCnt);
	else
		--(*m_pwRefCnt);
}

void CTerrainCol::SetHeliColInfo( D3DXVECTOR3* pLeftLegPos,D3DXVECTOR3* pRightLegPos,D3DXVECTOR3* pHeliPos ,const Engine::VTXTEX* pTerrainVtx, DWORD dwCntX, DWORD dwCntZ )
{
	m_pHeliPos = pHeliPos;
	m_pLeftLegPos = pLeftLegPos;
	m_pRightLegPos = pRightLegPos;

	m_pTerrainVtx = pTerrainVtx;

	m_dwVtxCntX = dwCntX;
	m_dwVtxCntZ = dwCntZ;
}

