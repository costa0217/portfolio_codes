#include "NaviCell.h"
#include "Line2D.h"

Engine::CNaviCell::CNaviCell(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC)
: m_pLine(NULL)
, m_vCenterPos(D3DXVECTOR3(0.f, 0.f, 0.f))
, m_pGraphicDev(NULL)
{
	m_vPoint[POINT_A] = *pPointA;
	m_vPoint[POINT_B] = *pPointB;
	m_vPoint[POINT_C] = *pPointC;
}

Engine::CNaviCell::~CNaviCell(void)
{
	Release();
}

HRESULT Engine::CNaviCell::Init_Cell(LPDIRECT3DDEVICE9 pDevie, const DWORD& dwIdx)
{
	m_pGraphicDev = pDevie;
	m_dwIndex = dwIdx;

	ZeroMemory(m_pNeighbor, sizeof(CNaviCell*) * NEIGHBOR_END);

	m_pLine2D[LINE_AB] = CLine2D::Create(&m_vPoint[POINT_A], &m_vPoint[POINT_B]);
	m_pLine2D[LINE_BC] = CLine2D::Create(&m_vPoint[POINT_B], &m_vPoint[POINT_C]);
	m_pLine2D[LINE_CA] = CLine2D::Create(&m_vPoint[POINT_C], &m_vPoint[POINT_A]);

	m_vEdge[LINE_AB] = m_vPoint[POINT_B] - m_vPoint[POINT_A]; 
	m_vEdge[LINE_BC] = m_vPoint[POINT_C] - m_vPoint[POINT_B]; 
	m_vEdge[LINE_CA] = m_vPoint[POINT_A] - m_vPoint[POINT_C]; 

	m_vCenterPos = (m_vPoint[POINT_A] + m_vPoint[POINT_B] + m_vPoint[POINT_C]) / 3.f;

	if(FAILED(D3DXCreateLine(m_pGraphicDev, &m_pLine)))	return E_FAIL;


	return S_OK;
}

void Engine::CNaviCell::Render(D3DXCOLOR Color)
{
	D3DXMATRIX		matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXVECTOR3		vPoint[4];
	vPoint[0] = m_vPoint[0];
	vPoint[1] = m_vPoint[1];
	vPoint[2] = m_vPoint[2];
	vPoint[3] = m_vPoint[0];

	for(int i = 0; i < 4; ++i)
	{
		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matView);

		if(vPoint[i].z < 0.1f)
			vPoint[i].z = 0.1f;

		D3DXVec3TransformCoord(&vPoint[i], &vPoint[i], &matProj);
	}

	D3DXMATRIX		matIdentity;
	D3DXMatrixIdentity(&matIdentity);


	m_pLine->SetWidth(2.f);
	m_pLine->Begin();

	m_pLine->DrawTransform(vPoint, POINT_END + 1, &matIdentity, /*D3DXCOLOR(1.f, 0.f, 0.f, 1.f)*/Color);

	m_pLine->End();
}

Engine::CNaviCell* Engine::CNaviCell::Create(LPDIRECT3DDEVICE9 pGraphicDev , const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, const D3DXVECTOR3* pPointC , const DWORD& dwIdx)
{
	CNaviCell*	pNaviCell = new CNaviCell(pPointA, pPointB, pPointC);
	if(FAILED(pNaviCell->Init_Cell(pGraphicDev, dwIdx)))
		Engine::Safe_Delete(pNaviCell);

	return pNaviCell;
}

bool Engine::CNaviCell::Compare_Point(const D3DXVECTOR3* pPointA, const D3DXVECTOR3* pPointB, CNaviCell* pNeighbor)
{
	if(*pPointA == m_vPoint[POINT_A])
	{
		if(*pPointB == m_vPoint[POINT_B])
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return true;
		}
		else if(*pPointB == m_vPoint[POINT_C])
		{
			m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
			return true;
		}
	}

	if(*pPointA == m_vPoint[POINT_B])
	{
		if(*pPointB == m_vPoint[POINT_A])
		{
			m_pNeighbor[NEIGHBOR_AB] = pNeighbor;
			return true;
		}
		else if(*pPointB == m_vPoint[POINT_C])
		{
			m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
			return true;
		}
	}

	if(*pPointA == m_vPoint[POINT_C])
	{
		if(*pPointB == m_vPoint[POINT_A])
		{
			m_pNeighbor[NEIGHBOR_CA] = pNeighbor;
			return true;
		}
		else if(*pPointB == m_vPoint[POINT_B])
		{
			m_pNeighbor[NEIGHBOR_BC] = pNeighbor;
			return true;
		}
	}

	return false;
}

void Engine::CNaviCell::Release(void)
{
	for(int i = 0; i < LINE_END; ++i)
		Engine::Safe_Delete(m_pLine2D[i]);

	Engine::Safe_Release(m_pLine);
}

bool Engine::CNaviCell::Check_IsIn(D3DXVECTOR3 vPickPos)
{
	bool bCheck[3];
	ZeroMemory(bCheck, sizeof(bool) * 3);

	for(int i = 0; i < POINT_END; ++i)
	{
		D3DXVECTOR3 vPickDir = vPickPos - m_vPoint[i];
		vPickDir = D3DXVECTOR3(vPickDir.x, 0.f, vPickDir.z);
		D3DXVec3Normalize(&vPickDir, &vPickDir);

		D3DXVECTOR3 vEdgeNormal = D3DXVECTOR3(m_vEdge[i].z * -1, 0.f, m_vEdge[i].x);
		D3DXVec3Normalize(&vEdgeNormal, &vEdgeNormal);

		float fTemp = D3DXVec3Dot(&vPickDir, &vEdgeNormal);
		if(D3DXVec3Dot(&vPickDir, &vEdgeNormal) < 0.f)	// 다 음수면 삼각형 내부
			bCheck[i] = true;
	}
	if(bCheck[0] && bCheck[1] && bCheck[2]) // 모든변 음수
		return true;	// 트루반환

	return false;
}

void Engine::CNaviCell::Set_Info(void)
{
	m_vEdge[LINE_AB] = m_vPoint[POINT_B] - m_vPoint[POINT_A]; 
	m_vEdge[LINE_BC] = m_vPoint[POINT_C] - m_vPoint[POINT_B]; 
	m_vEdge[LINE_CA] = m_vPoint[POINT_A] - m_vPoint[POINT_C]; 

	m_vCenterPos = (m_vPoint[POINT_A] + m_vPoint[POINT_B] + m_vPoint[POINT_C]) / 3.f;

}
