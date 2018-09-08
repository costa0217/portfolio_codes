#include "QuadTree.h"
#include "Frustum.h"

IMPLEMENT_SINGLETON2(Engine::CQuadTree)

Engine::CQuadTree::CQuadTree(void)
: m_iCenter(0)
, m_fRadius(0.f)
, m_isIn(false)
{
	ZeroMemory(m_pChild, sizeof(CQuadTree*) * CHILD_END);
	ZeroMemory(m_iCorner, sizeof(int) * CORNER_END);
	ZeroMemory(m_pNeighbor, sizeof(CQuadTree*) * NEIGHBOR_END);
}

Engine::CQuadTree::~CQuadTree(void)
{
	Release();
}

HRESULT Engine::CQuadTree::InitQuadTree(const VTXTEX* pTerrainVtx, const int& iCntX, const int& iCntZ)
{
	m_iCorner[CORNER_LT] = 0;
	m_iCorner[CORNER_RT] = iCntX - 1;
	m_iCorner[CORNER_LB] = iCntX * (iCntZ - 1);
	m_iCorner[CORNER_RB] = iCntX * iCntZ - 1;

	for(int i = 0; i < CORNER_END; ++i)
		m_iCenter += m_iCorner[i];

	m_iCenter = m_iCenter >> 2;
	SetChild(pTerrainVtx);
	return S_OK;
}

void Engine::CQuadTree::IsinFrustum(const VTXTEX* pTerrainVtx, const CFrustum* pFrustum)
{
	if(m_pChild[CHILD_LT] == NULL)
	{
		m_isIn = true;
		return;
	}

	m_isIn = pFrustum->SphereInFrustum(&pTerrainVtx[m_iCenter].vPos, m_fRadius);

	if(m_isIn == true)
	{
		for(int i = 0; i < CHILD_END; ++i)
		{
			if(m_pChild[i] != NULL)
				m_pChild[i]->IsinFrustum(pTerrainVtx, pFrustum);
		}
	}
}

void Engine::CQuadTree::CullingToQuadTree(LPDIRECT3DDEVICE9 pDevice, const VTXTEX* pTerrainVtx, const CFrustum* pFrustum , INDEX32* pIndex, DWORD* pdwTriCnt)
{
	if(m_isIn == true)
	{
		if(m_pChild[CHILD_LT] == NULL || LevelofDetail(pDevice, pTerrainVtx))
		{
			bool		isRender[4] = {true, true, true, true};

			for(int i = 0; i < NEIGHBOR_END; ++i)
			{
				if(NULL != m_pNeighbor[i])
					isRender[i] = m_pNeighbor[i]->LevelofDetail(pDevice, pTerrainVtx);
			}

			if(isRender[NEIGHBOR_LEFT] && isRender[NEIGHBOR_TOP]
			&& isRender[NEIGHBOR_RIGHT] && isRender[NEIGHBOR_BOTTOM])
			{
				pIndex[*pdwTriCnt]._1 = m_iCorner[CORNER_LB];
				pIndex[*pdwTriCnt]._2 = m_iCorner[CORNER_RB];
				pIndex[*pdwTriCnt]._3 = m_iCorner[CORNER_RT];
				++(*pdwTriCnt);

				pIndex[*pdwTriCnt]._1 = m_iCorner[CORNER_LB];
				pIndex[*pdwTriCnt]._2 = m_iCorner[CORNER_RT];
				pIndex[*pdwTriCnt]._3 = m_iCorner[CORNER_LT];
				++(*pdwTriCnt);

				return;
			}

			if(false == isRender[NEIGHBOR_LEFT])
			{
				int		iLC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_LB]) >> 1;

				pIndex[*pdwTriCnt]._1 = m_iCorner[CORNER_LB];
				pIndex[*pdwTriCnt]._2 = m_iCenter;
				pIndex[*pdwTriCnt]._3 = iLC;
				++(*pdwTriCnt);

				pIndex[*pdwTriCnt]._1 = iLC;
				pIndex[*pdwTriCnt]._2 = m_iCenter;
				pIndex[*pdwTriCnt]._3 = m_iCorner[CORNER_LT];
				++(*pdwTriCnt);
			}
			else
			{
				pIndex[*pdwTriCnt]._1 = m_iCorner[CORNER_LB];
				pIndex[*pdwTriCnt]._2 = m_iCenter;
				pIndex[*pdwTriCnt]._3 = m_iCorner[CORNER_LT];
				++(*pdwTriCnt);
			}

			if(false == isRender[NEIGHBOR_TOP])
			{
				int		iTC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_RT]) >> 1;

				pIndex[*pdwTriCnt]._1 = m_iCorner[CORNER_LT];
				pIndex[*pdwTriCnt]._2 = m_iCenter;
				pIndex[*pdwTriCnt]._3 = iTC;
				++(*pdwTriCnt);

				pIndex[*pdwTriCnt]._1 = iTC;
				pIndex[*pdwTriCnt]._2 = m_iCenter;
				pIndex[*pdwTriCnt]._3 = m_iCorner[CORNER_RT];
				++(*pdwTriCnt);
			}
			else
			{
				pIndex[*pdwTriCnt]._1 = m_iCorner[CORNER_LT];
				pIndex[*pdwTriCnt]._2 = m_iCenter;
				pIndex[*pdwTriCnt]._3 = m_iCorner[CORNER_RT];
				++(*pdwTriCnt);
			}

			if(false == isRender[NEIGHBOR_RIGHT])
			{
				int		iRC = (m_iCorner[CORNER_RT] + m_iCorner[CORNER_RB]) >> 1;

				pIndex[*pdwTriCnt]._1 = m_iCenter;
				pIndex[*pdwTriCnt]._2 = m_iCorner[CORNER_RB];
				pIndex[*pdwTriCnt]._3 = iRC;
				++(*pdwTriCnt);

				pIndex[*pdwTriCnt]._1 = iRC;
				pIndex[*pdwTriCnt]._2 = m_iCorner[CORNER_RT];
				pIndex[*pdwTriCnt]._3 = m_iCenter;
				++(*pdwTriCnt);
			}
			else
			{
				pIndex[*pdwTriCnt]._1 = m_iCenter;
				pIndex[*pdwTriCnt]._2 = m_iCorner[CORNER_RB];
				pIndex[*pdwTriCnt]._3 = m_iCorner[CORNER_RT];
				++(*pdwTriCnt);
			}

			if(false == isRender[NEIGHBOR_BOTTOM])
			{
				int		iBC = (m_iCorner[CORNER_RB] + m_iCorner[CORNER_LB]) >> 1;

				pIndex[*pdwTriCnt]._1 = m_iCorner[CORNER_LB];
				pIndex[*pdwTriCnt]._2 = iBC;
				pIndex[*pdwTriCnt]._3 = m_iCenter;
				++(*pdwTriCnt);

				pIndex[*pdwTriCnt]._1 = m_iCenter;
				pIndex[*pdwTriCnt]._2 = iBC;
				pIndex[*pdwTriCnt]._3 = m_iCorner[CORNER_RB];
				++(*pdwTriCnt);
			}
			else
			{
				pIndex[*pdwTriCnt]._1 = m_iCenter;
				pIndex[*pdwTriCnt]._2 = m_iCorner[CORNER_LB];
				pIndex[*pdwTriCnt]._3 = m_iCorner[CORNER_RB];
				++(*pdwTriCnt);
			}
			return;
		}

		for(int i = 0; i < CHILD_END; ++i)
		{
			if(m_pChild[i] != NULL)
				m_pChild[i]->CullingToQuadTree(pDevice, pTerrainVtx, pFrustum, pIndex, pdwTriCnt);
		}
	}
}

void Engine::CQuadTree::SetChild(const VTXTEX* pTerrainVtx)
{
	m_fRadius = D3DXVec3Length(&(pTerrainVtx[m_iCorner[CORNER_LT]].vPos - pTerrainVtx[m_iCenter].vPos));

	for(int i = 0; i < CHILD_END; ++i)
	{
		m_pChild[i] = MakeChild(CHILD(i));

		if(m_pChild[i] != NULL)
			m_pChild[i]->SetChild(pTerrainVtx);
	}
}

Engine::CQuadTree* Engine::CQuadTree::MakeChild(CHILD eChild)
{
	if(m_iCorner[CORNER_RT] - m_iCorner[CORNER_LT] == 1)
	{
		m_fRadius = 0.f;
		return NULL;
	}

	int		iLC, iTC, iRC, iBC;
	iLC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_LB]) >> 1;
	iTC = (m_iCorner[CORNER_LT] + m_iCorner[CORNER_RT]) >> 1;
	iRC = (m_iCorner[CORNER_RT] + m_iCorner[CORNER_RB]) >> 1;
	iBC = (m_iCorner[CORNER_LB] + m_iCorner[CORNER_RB]) >> 1;

	CQuadTree*		pQuadTree = new CQuadTree;

	switch(eChild)
	{
	case CHILD_LT:
		pQuadTree->m_iCorner[CORNER_LT] = m_iCorner[CORNER_LT];
		pQuadTree->m_iCorner[CORNER_RT] = iTC;
		pQuadTree->m_iCorner[CORNER_LB] = iLC;
		pQuadTree->m_iCorner[CORNER_RB] = m_iCenter;
		break;

	case CHILD_RT:
		pQuadTree->m_iCorner[CORNER_LT] = iTC;
		pQuadTree->m_iCorner[CORNER_RT] = m_iCorner[CORNER_RT];
		pQuadTree->m_iCorner[CORNER_LB] = m_iCenter;
		pQuadTree->m_iCorner[CORNER_RB] = iRC;
		break;

	case CHILD_LB:
		pQuadTree->m_iCorner[CORNER_LT] = iLC;
		pQuadTree->m_iCorner[CORNER_RT] = m_iCenter;
		pQuadTree->m_iCorner[CORNER_LB] = m_iCorner[CORNER_LB];
		pQuadTree->m_iCorner[CORNER_RB] = iBC;
		break;

	case CHILD_RB:
		pQuadTree->m_iCorner[CORNER_LT] = m_iCenter;
		pQuadTree->m_iCorner[CORNER_RT] = iRC;
		pQuadTree->m_iCorner[CORNER_LB] = iBC;
		pQuadTree->m_iCorner[CORNER_RB] = m_iCorner[CORNER_RB];
		break;
	}

	for(int i = 0; i < CORNER_END; ++i)
		pQuadTree->m_iCenter += pQuadTree->m_iCorner[i];

	pQuadTree->m_iCenter = pQuadTree->m_iCenter >> 2;
	return pQuadTree;
}

bool Engine::CQuadTree::LevelofDetail(LPDIRECT3DDEVICE9 pDevice, const VTXTEX* pTerrainVtx)
{
	D3DXMATRIX		matView;
	pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, NULL, &matView);

	D3DXVECTOR3		vCamPos = D3DXVECTOR3(matView._41, matView._42, matView._43);

	float	fDistance = D3DXVec3Length( &(vCamPos - pTerrainVtx[m_iCenter].vPos) );

	float	fWidth = pTerrainVtx[m_iCorner[CORNER_RT]].vPos.x - pTerrainVtx[m_iCorner[CORNER_LT]].vPos.x;

	if(fDistance *0.05f > fWidth)
	{
		return true;
	}
	return false;
}

void Engine::CQuadTree::Release(void)
{
	for(int i = 0; i < CHILD_END; ++i)
	{
		Engine::Safe_Delete(m_pChild[i]);
	}
}

HRESULT Engine::CQuadTree::InitNeighbor(const VTXTEX* pTerrainVtxInfo, CQuadTree* pParentQuad , const int& iVtxCntX, const int& iVtxCntZ)
{
	int		iLT, iRT, iLB, iRB;

	for(int i = 0; i < NEIGHBOR_END; ++i)
	{
		if(true == MakeNeighbor(NEIGHBOR(i), &iLT, &iRT, &iLB, &iRB, iVtxCntX, iVtxCntZ))
		{
			int	iCenter = (iLT + iRT + iLB + iRB) >> 2;
			m_pNeighbor[i] = pParentQuad->FindNeighbor(pTerrainVtxInfo, iLT, iRT, iLB, iRB, iCenter);
		}
		else
			m_pNeighbor[i] = NULL;
	}

	for(int i = 0; i < CHILD_END; ++i)
	{
		if(NULL != m_pChild[i])
			m_pChild[i]->InitNeighbor(pTerrainVtxInfo, pParentQuad, iVtxCntX, iVtxCntZ);
	}
	return S_OK;
}

bool Engine::CQuadTree::MakeNeighbor(NEIGHBOR eNeighbor, int* pLT, int* pRT, int* pLB, int* pRB 
									 , const int& iVtxCntX, const int& iVtxCntZ)
{
	int		iSizeX = m_iCorner[CORNER_RT] - m_iCorner[CORNER_LT];
	int		iSizeY = m_iCorner[CORNER_LB] - m_iCorner[CORNER_LT];

	switch(eNeighbor)
	{
	case NEIGHBOR_LEFT:
		*pLT = m_iCorner[CORNER_LT] - iSizeX;
		*pRT = m_iCorner[CORNER_LT];
		*pLB = m_iCorner[CORNER_LB] - iSizeX;
		*pRB = m_iCorner[CORNER_LB];
		if(m_iCorner[CORNER_LT] % iVtxCntX == 0)
			return false;
		break;

	case NEIGHBOR_TOP:
		*pLT = m_iCorner[CORNER_LT] - iSizeY;
		*pRT = m_iCorner[CORNER_RT] - iSizeY;
		*pLB = m_iCorner[CORNER_LT];
		*pRB = m_iCorner[CORNER_RT];
		if(m_iCorner[CORNER_RT] < iVtxCntX)
			return false;
		break;

	case NEIGHBOR_RIGHT:
		*pLT = m_iCorner[CORNER_RT];
		*pRT = m_iCorner[CORNER_RT] + iSizeX;
		*pLB = m_iCorner[CORNER_RB];
		*pRB = m_iCorner[CORNER_RB] + iSizeX;
		if(m_iCorner[CORNER_RT] % iVtxCntX == iVtxCntX - 1)
			return false;
		break;

	case NEIGHBOR_BOTTOM:
		*pLT = m_iCorner[CORNER_LB];
		*pRT = m_iCorner[CORNER_RB];
		*pLB = m_iCorner[CORNER_LB] + iSizeY;
		*pRB = m_iCorner[CORNER_RB] + iSizeY;
		if(m_iCorner[CORNER_LB] >= iVtxCntX * (iVtxCntZ - 1))
			return false;
		break;
	}

	return true;
}

Engine::CQuadTree* Engine::CQuadTree::FindNeighbor(const VTXTEX* pTerrainVtxInfo 
												   , const int& iLT, const int& iRT
												   , const int& iLB, const int& iRB
												   ,const int& iCenter)
{
	if(m_iCorner[CORNER_LT] == iLT
		&& m_iCorner[CORNER_RT] == iRT
		&& m_iCorner[CORNER_LB] == iLB
		&& m_iCorner[CORNER_RB] == iRB)
		return this;

	RECT	rcQuad;
	POINT	ptCenter;

	for(int i = 0; i < CHILD_END; ++i)
	{
		if(NULL != m_pChild[i])
		{
			ZeroMemory(&rcQuad, sizeof(RECT));
			ZeroMemory(&ptCenter, sizeof(POINT));

			SetRect(&rcQuad , int( pTerrainVtxInfo[m_pChild[i]->m_iCorner[CORNER_LT]].vPos.x )
				, int( pTerrainVtxInfo[m_pChild[i]->m_iCorner[CORNER_LT]].vPos.z )
				, int( pTerrainVtxInfo[m_pChild[i]->m_iCorner[CORNER_RB]].vPos.x )
				, int( pTerrainVtxInfo[m_pChild[i]->m_iCorner[CORNER_RB]].vPos.z ));

			ptCenter.x = long(pTerrainVtxInfo[iCenter].vPos.x);
			ptCenter.y = long(pTerrainVtxInfo[iCenter].vPos.z);

			if(PtInRect(&rcQuad, ptCenter))
				return m_pChild[i]->FindNeighbor(pTerrainVtxInfo, iLT, iRT, iLB, iRB, iCenter);
		}
	}
	return NULL;
}
