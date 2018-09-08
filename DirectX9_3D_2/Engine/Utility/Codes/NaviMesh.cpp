#include "NaviMesh.h"
#include "NaviCell.h"

IMPLEMENT_SINGLETON(Engine::CNaviMesh)

Engine::CNaviMesh::CNaviMesh(void)
: m_dwIdxCnt(0)
, m_dwReserveSize(0)
, m_iPickCnt(0)
, m_iFindNeviIdx(-1)
, m_dwPlayerIdx(-1) // 임시
, m_bJump(false)
{
	ZeroMemory(m_vPickPos, sizeof(D3DXVECTOR3));
}

Engine::CNaviMesh::~CNaviMesh(void)
{
}

void Engine::CNaviMesh::Reserve_CellContainerSize(LPDIRECT3DDEVICE9 pGraphicDev, const DWORD& dwSize)
{
	m_pGraphicDev = pGraphicDev;
	m_dwReserveSize = dwSize;
	m_vecNaviCell.reserve(dwSize);
}

HRESULT Engine::CNaviMesh::Add_Cell(const D3DXVECTOR3* pPointA
									, const D3DXVECTOR3* pPointB
									, const D3DXVECTOR3* pPointC)
{
	NULL_CHECK_RETURN(m_dwReserveSize, E_FAIL);

	CNaviCell*	pCell = CNaviCell::Create(m_pGraphicDev, pPointA, pPointB, pPointC, m_dwIdxCnt);
	++m_dwIdxCnt;

	m_vecNaviCell.push_back(pCell);
	return S_OK;
}


void Engine::CNaviMesh::Link_Cell(void)
{
	VECCELL::iterator		iter = m_vecNaviCell.begin();

	if(iter == m_vecNaviCell.end())
		return;

	for( ; iter != m_vecNaviCell.end() ; ++iter)	//널 초기화
	{
		(*iter)->Set_Neighbor(CNaviCell::NEIGHBOR_AB, NULL);
		(*iter)->Set_Neighbor(CNaviCell::NEIGHBOR_BC, NULL);
		(*iter)->Set_Neighbor(CNaviCell::NEIGHBOR_CA, NULL);
	}


	for( VECCELL::iterator	iter = m_vecNaviCell.begin();
		iter != m_vecNaviCell.end() ; ++iter)
	{
		VECCELL::iterator		iter_Target = m_vecNaviCell.begin();

		while( iter_Target != m_vecNaviCell.end())
		{
			if(iter == iter_Target)
			{
				++iter_Target;
				continue;
			}

			if((*iter_Target)->Compare_Point((*iter)->Get_Point(CNaviCell::POINT_A)
				, (*iter)->Get_Point(CNaviCell::POINT_B), (*iter)))
			{
				(*iter)->Set_Neighbor(CNaviCell::NEIGHBOR_AB, (*iter_Target));
			}

			else if((*iter_Target)->Compare_Point((*iter)->Get_Point(CNaviCell::POINT_B)
				, (*iter)->Get_Point(CNaviCell::POINT_C), (*iter)))
			{
				(*iter)->Set_Neighbor(CNaviCell::NEIGHBOR_BC, (*iter_Target));
			}

			else if((*iter_Target)->Compare_Point((*iter)->Get_Point(CNaviCell::POINT_C)
				, (*iter)->Get_Point(CNaviCell::POINT_A), (*iter)))
			{
				(*iter)->Set_Neighbor(CNaviCell::NEIGHBOR_CA, (*iter_Target));
			}


			for(int i = 0; i < 3; ++i)
			{
				for(int j = 0; j < 3; ++j)
				{
					if((*iter)->m_vPoint[i] == (*iter_Target)->m_vPoint[j])
					{
						(*iter)->m_VecPoint[i].push_back((*iter_Target));
					}
				}
			}

			++iter_Target;
		}
	}
}

void Engine::CNaviMesh::Render_NaviMesh(void)
{
	UINT		iSize = m_vecNaviCell.size();

	if(iSize == 0)
		return;

	for(UINT i = 0; i < iSize; ++i)
	{
		if(m_dwPlayerIdx == i)
			continue;

		m_vecNaviCell[i]->Render(D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
	}

	if(m_dwPlayerIdx != -1)
		m_vecNaviCell[m_dwPlayerIdx]->Render(D3DXCOLOR(0.f, 1.f, 0.f, 1.f));
}

void Engine::CNaviMesh::Release(void)
{
	for_each(m_vecNaviCell.begin(), m_vecNaviCell.end(), Engine::CDeleteObj());
	m_vecNaviCell.clear();

	delete this;
}

float Engine::CNaviMesh::Search_StraightLine(D3DXVECTOR3 vLine1, D3DXVECTOR3 vLine2, D3DXVECTOR3 vDestLine)
{
	return ((vLine2.z - vLine1.z) / (vLine2.x - vLine1.x)) * (vDestLine.x - vLine1.x) + vLine1.z - vDestLine.z;
}

bool Engine::CNaviMesh::LineCollsion(D3DXVECTOR3 vCurrentPos, D3DXVECTOR3 vDestPos, D3DXVECTOR3 pPoint1, D3DXVECTOR3 pPoint2)
{

	float fFstLine = 0.f;
	float fSndLine = 0.f;

	fFstLine = Search_StraightLine(pPoint1, pPoint2, vCurrentPos);
	fSndLine = Search_StraightLine(pPoint1, pPoint2, vDestPos);

	if(fFstLine * fSndLine > 0.f)
		return false;

	fFstLine = Search_StraightLine(vCurrentPos, vDestPos, pPoint1);
	fSndLine = Search_StraightLine(vCurrentPos, vDestPos, pPoint2);

	if(fFstLine * fSndLine > 0.f)
		return false;

	return true;
}


D3DXVECTOR3 Engine::CNaviMesh::Search_SlideVec(CNaviCell* pNaviCell, D3DXVECTOR3* pPos, const D3DXVECTOR3* pDir, int& rLineIndex)
{
	D3DXVECTOR3 vSlide = D3DXVECTOR3(0.f, 0.f, 0.f);

	D3DXVECTOR3 vDestPos	= (*pPos) + (*pDir);
	D3DXVECTOR3 vCurrentPos = (*pPos);


	if(LineCollsion(vCurrentPos, vDestPos, pNaviCell->m_vPoint[0], pNaviCell->m_vPoint[1]))
	{
		rLineIndex = 0;
	}
	else if(LineCollsion(vCurrentPos, vDestPos, pNaviCell->m_vPoint[1], pNaviCell->m_vPoint[2]))
	{
		rLineIndex = 1;
	}
	else if(LineCollsion(vCurrentPos, vDestPos, pNaviCell->m_vPoint[2], pNaviCell->m_vPoint[0]))
	{
		rLineIndex = 2;
	}
	D3DXVECTOR3 vNormal = D3DXVECTOR3(pNaviCell->m_vEdge[rLineIndex].z * -1, 0.f, pNaviCell->m_vEdge[rLineIndex].x);
	D3DXVec3Normalize(&vNormal, &vNormal);
	D3DXVECTOR3 vDestDir = vDestPos - vCurrentPos;
	D3DXVec3Normalize(&vDestDir, &vDestDir);

	vSlide = vDestDir - vNormal * (D3DXVec3Dot(&vDestDir, &vNormal));

	return vSlide * 0.5f;
}

D3DXVECTOR3 Engine::CNaviMesh::Search_ReflcVec(D3DXVECTOR3* pSlideVec, D3DXVECTOR3* pDir)
{
	D3DXVECTOR3 vReflect = D3DXVECTOR3(0.f, 0.f, 0.f);

	return vReflect;
}

bool Engine::CNaviMesh::FindNeighbor(D3DXVECTOR3 vMiraclePos, DWORD dwCurrentIdx, DWORD& dwNeighborIdx)
{
	for(int i = 0; i < 3; ++i)
	{
		if(m_vecNaviCell[dwCurrentIdx]->m_pNeighbor[i] != NULL
			&&m_vecNaviCell[dwCurrentIdx]->m_pNeighbor[i]->Check_IsIn(vMiraclePos))
		{
			dwNeighborIdx = i;
			return true;
		}
	}
	return false;
}
DWORD Engine::CNaviMesh::FindDotIndex(D3DXVECTOR3 vPos, CNaviCell* pNaviCell)
{
	DWORD dwIndex = 4;

	float fDest = 100.f;
	float fMinDest = 100.f;

	for(int i = 0; i < 3; ++i)
	{
		fDest = D3DXVec3Length(&(vPos - pNaviCell->m_vPoint[i]));
		if(fDest < fMinDest)
		{
			fMinDest = fDest;
			dwIndex = i;
		}
	}
	return dwIndex;
}


DWORD Engine::CNaviMesh::MovingNaviMesh(D3DXVECTOR3* pPos, const D3DXVECTOR3* pDir
										, const DWORD& dwCurrentIdx
										, const float& fSpeed
										, const float& fTimeDelta)
{
	D3DXVECTOR3	vMiracleDir = (*pDir);
	D3DXVECTOR3 vMiraclePos = (*pPos) + vMiracleDir;

	DWORD		dwNeighborIdx = 4;
	m_dwPlayerIdx = dwCurrentIdx;

	// 다음위치가 현재지역이라면
	if(m_vecNaviCell[dwCurrentIdx]->Check_IsIn(vMiraclePos))
	{
		(*pPos) += vMiracleDir * fSpeed * fTimeDelta;	
		if(!m_bJump) (*pPos).y = Compute_Height((*pPos), dwCurrentIdx);
	}
	else
	{
		// 다음지역에 이웃이있다면 -> 이동, 인덱스 체인지
		if(FindNeighbor(vMiraclePos, dwCurrentIdx, dwNeighborIdx))	
		{
			(*pPos) += vMiracleDir * fSpeed * fTimeDelta;	
			if(!m_bJump) (*pPos).y = Compute_Height((*pPos), m_vecNaviCell[dwCurrentIdx]->m_pNeighbor[dwNeighborIdx]->m_dwIndex);
			return m_vecNaviCell[dwCurrentIdx]->m_pNeighbor[dwNeighborIdx]->m_dwIndex;
		}
		// 다음지역에 이웃이없다면 -> 슬라이딩 벡터로 이동
		else
		{
			int iLineIndex = -1;

			vMiracleDir = Search_SlideVec(m_vecNaviCell[dwCurrentIdx], pPos, pDir, iLineIndex);
			if(m_vecNaviCell[dwCurrentIdx]->Check_IsIn((*pPos) + vMiracleDir))
			{
				(*pPos) += vMiracleDir * fSpeed * fTimeDelta;	
				if(!m_bJump) (*pPos).y = Compute_Height((*pPos), dwCurrentIdx);
			}
			else
			{
				// 꼭지점에 닿을때
				int iDotIdx = FindDotIndex((*pPos), m_vecNaviCell[dwCurrentIdx]);

				size_t iSize = m_vecNaviCell[dwCurrentIdx]->m_VecPoint[iDotIdx].size();
				for(size_t i = 0; i < iSize; ++i)
				{
					if(m_vecNaviCell[dwCurrentIdx]->m_VecPoint[iDotIdx][i]->Check_IsIn((*pPos) + vMiracleDir))
					{
						(*pPos) += vMiracleDir * fSpeed * fTimeDelta;	
						if(!m_bJump) (*pPos).y = Compute_Height((*pPos), m_vecNaviCell[dwCurrentIdx]->m_VecPoint[iDotIdx][i]->m_dwIndex);
						return m_vecNaviCell[dwCurrentIdx]->m_VecPoint[iDotIdx][i]->m_dwIndex;
					}
				}						
			}
		}
	}	
	return dwCurrentIdx;
}


void Engine::CNaviMesh::CheckPickThird(D3DXVECTOR3 vPickPos)
{
	if(!m_vecNaviCell.empty())
		return;

	if(vPickPos == D3DXVECTOR3(0.f, 0.f, 0.f))
		return;

	m_vPickPos[m_iPickCnt] = vPickPos;

	int i = m_iPickCnt;

	++m_iPickCnt;
	if(m_iPickCnt == 3)
	{
		Add_Cell(&m_vPickPos[0], &m_vPickPos[1], &m_vPickPos[2]);

		m_iPickCnt = 0;
		ZeroMemory(m_vPickPos, sizeof(D3DXVECTOR3));
	}
}

void Engine::CNaviMesh::NextPickingNevi(D3DXVECTOR3 vPickPos, float fAttachLevel)
{
	if(m_vecNaviCell.empty())
		return;

	if(vPickPos == D3DXVECTOR3(0.f, 0.f, 0.f))
	{
		m_iFindNeviIdx = -1;
		return;
	}

	if(m_iFindNeviIdx == -1)
	{
		m_iFindNeviIdx = FindIndex(vPickPos);
		return;
	}
	else
	{
		D3DXVECTOR3 vDir;
		D3DXVECTOR3 vNormal[3];
		for(int i = 0; i < 3; ++i)
		{
			vNormal[i] = D3DXVECTOR3(m_vecNaviCell[m_iFindNeviIdx]->m_vEdge[i].z * -1, 0.f, m_vecNaviCell[m_iFindNeviIdx]->m_vEdge[i].x);
			D3DXVec3Normalize(&vNormal[i], &vNormal[i]);
		}

		vDir = vPickPos - m_vecNaviCell[m_iFindNeviIdx]->m_vCenterPos;
		D3DXVec3Normalize(&vDir, &vDir);

		float	fMinAngle = 100.f;
		int		iEdgeIndex = -1;

		for(int i = 0; i < 3; ++i)
		{
			if(acos(D3DXVec3Dot(&vNormal[i], &vDir)) < fMinAngle)
			{
				fMinAngle	= acos(D3DXVec3Dot(&vNormal[i], &vDir));
				iEdgeIndex	= i;
			}
		}

		bool bCheck = false;
		size_t vecCellSize = m_vecNaviCell.size();
		for(size_t i = 0; i < vecCellSize; ++i)
		{

			for(int j = 0; j < 3; ++j)
			{
				if(D3DXVec3Length(& (m_vecNaviCell[i]->m_vPoint[j] - vPickPos)) < fAttachLevel)
				{
					vPickPos	= m_vecNaviCell[i]->m_vPoint[j];
					bCheck		= true;
					break;
				}
			}
			if(bCheck)
				break;
		}

		switch(iEdgeIndex)
		{
		case 0:		// AB
			Add_Cell(&m_vecNaviCell[m_iFindNeviIdx]->m_vPoint[0], &vPickPos, &m_vecNaviCell[m_iFindNeviIdx]->m_vPoint[1]);
			break;
		case 1:		// BC
			Add_Cell(&m_vecNaviCell[m_iFindNeviIdx]->m_vPoint[1], &vPickPos, &m_vecNaviCell[m_iFindNeviIdx]->m_vPoint[2]);
			break;
		case 2:		// CA
			Add_Cell(&m_vecNaviCell[m_iFindNeviIdx]->m_vPoint[2], &vPickPos, &m_vecNaviCell[m_iFindNeviIdx]->m_vPoint[0]);
			break;
		}

		m_iFindNeviIdx = -1;
	}

}

DWORD Engine::CNaviMesh::FindIndex(D3DXVECTOR3 vPickPos)
{
	size_t vecNeviCellSize = m_vecNaviCell.size();
	for(size_t i = 0; i < vecNeviCellSize; ++i)
	{
		if(m_vecNaviCell[i]->Check_IsIn(vPickPos))
		{
			return i;
		}
	}
	return -1;
}

void Engine::CNaviMesh::TurnBack(void)
{
	if(m_vecNaviCell.empty())
		return;

	size_t vecNeviCellSize = m_vecNaviCell.size();

	Engine::Safe_Delete(m_vecNaviCell[vecNeviCellSize - 1]);
	m_vecNaviCell.pop_back();

	--m_dwIdxCnt;
}

void Engine::CNaviMesh::AllDelete(void)
{
	for_each(m_vecNaviCell.begin(), m_vecNaviCell.end(), Engine::CDeleteObj());
	m_vecNaviCell.clear();

	m_vecNaviCell.swap(VECCELL());

	m_dwIdxCnt = 0;
}

void Engine::CNaviMesh::Refresh_Data(void)
{
	m_iFindNeviIdx  = -1;
	m_iPickCnt		= 0;
	ZeroMemory(m_vPickPos, sizeof(D3DXVECTOR3));
}

D3DXVECTOR3 Engine::CNaviMesh::Find_NaviPoint(D3DXVECTOR3 vPickPos)
{
	if(m_vecNaviCell.empty())
		return D3DXVECTOR3(0.f, 0.f, 0.f);

	m_SamePosPointsList.clear();
	m_ResetInfoList.clear();

	D3DXVECTOR3	vNearPointPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	bool	bFind	= false;

	size_t vecNeviCellSize = m_vecNaviCell.size();
	for(size_t i = 0; i < vecNeviCellSize; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			if(D3DXVec3Length(&(vPickPos - m_vecNaviCell[i]->m_vPoint[j])) < 1.f)
			{
				vNearPointPos = m_vecNaviCell[i]->m_vPoint[j];
				bFind = true;
				break;
			}
		}
		if(bFind) break;
	}

	for(size_t i = 0; i < vecNeviCellSize; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			if( vNearPointPos == m_vecNaviCell[i]->m_vPoint[j])
			{
				m_SamePosPointsList.push_back(&m_vecNaviCell[i]->m_vPoint[j]);
				m_ResetInfoList.push_back(m_vecNaviCell[i]);
			}
		}
	}

	return vNearPointPos;
}

void Engine::CNaviMesh::Modify_NaviPoint(_ushort iIdx, float fDataDelta)
{
	_vec3 vDataDelta = _vec3(0.f, 0.f, 0.f);

	switch(iIdx)
	{
	case 0:	// X
		vDataDelta.x = fDataDelta;
		break;
	case 1:	// Y
		vDataDelta.y = fDataDelta;
		break;
	case 2:	// Z
		vDataDelta.z = fDataDelta;
		break;
	}

	list<D3DXVECTOR3*>::iterator iter = m_SamePosPointsList.begin();
	list<D3DXVECTOR3*>::iterator iter_end = m_SamePosPointsList.end();

	for(; iter != iter_end; ++iter)
	{
		*(*iter) += vDataDelta;
	}	
	Set_Edge();
}

void Engine::CNaviMesh::Set_Edge(void)
{
	list<CNaviCell*>::iterator iter		= m_ResetInfoList.begin();
	list<CNaviCell*>::iterator iter_end = m_ResetInfoList.end();

	for(; iter != iter_end; ++iter)
	{
		(*iter)->Set_Info();
	}
}

_float Engine::CNaviMesh::Compute_Height(const _vec3& vObjPos, const DWORD& dwCurrentIdx)
{
	_vec3	vPickingPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	_float	fResultY	= 0.f;

	D3DXPLANE		Plane;


	D3DXPlaneFromPoints(&Plane, &m_vecNaviCell[dwCurrentIdx]->m_vPoint[0]
		, &m_vecNaviCell[dwCurrentIdx]->m_vPoint[1]
		, &m_vecNaviCell[dwCurrentIdx]->m_vPoint[2]);
	
	//ax + by + cz + d = 0
	//by = -ax -cz - d
	//y = (-ax -cz - d) / b

	fResultY = (-Plane.a * vObjPos.x - Plane.c * vObjPos.z - Plane.d) / Plane.b;

	return fResultY;
}

void Engine::CNaviMesh::Set_NaviCenterPos(_vec3* pvOutPos, _ulong dwIndex)
{
	*pvOutPos = m_vecNaviCell[dwIndex]->m_vCenterPos;
}
