#include "stdafx.h"
#include "MouseCol.h"

#include "Include.h"
#include "Export_Function.h"
#include "CameraObserver.h"

CMouseCol::CMouseCol(void)
: m_pInfoSubject(Engine::Get_InfoSubject())
, m_pCameraObserver(NULL)
, m_vPivotPos(0.f, 0.f, 0.f)
, m_vRayDir(0.f, 0.f, 0.f)
{

}

CMouseCol::~CMouseCol(void)
{
	Release();
}

Engine::CCollision* CMouseCol::Clone(void)
{
	++(*m_pwRefCnt);

	return new CMouseCol(*this);
}

POINT CMouseCol::GetMousePos(void)
{
	POINT	pt;

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return pt;
}

void CMouseCol::PickTerrain(D3DXVECTOR3* pOut, const Engine::VTXTEX* pTerrainVtx)
{
	//Translation_ViewSpace();

	//D3DXMATRIX		matWorld;
	//D3DXMatrixIdentity(&matWorld);
	//Translation_Local(&matWorld);

	//const Engine::VTXTEX*	pVertex = pTerrainVtx;

	//float		fU, fV, fDist;

	//for(int z = 0; z < VTXCNTZ - 1; ++z)
	//{
	//	for(int x = 0; x < VTXCNTX - 1; ++x)
	//	{
	//		int		iIndex = z * VTXCNTX + x;

	//		if(D3DXIntersectTri(&pVertex[iIndex + VTXCNTX + 1].vPos
	//			, &pVertex[iIndex + VTXCNTX].vPos
	//			, &pVertex[iIndex + 1].vPos
	//			, &m_vPivotPos, &m_vRayDir, &fU, &fV, &fDist))
	//		{
	//			*pOut = pVertex[iIndex + VTXCNTX + 1].vPos
	//				+ (pVertex[iIndex + VTXCNTX].vPos - pVertex[iIndex + VTXCNTX + 1].vPos) * fU
	//				+ (pVertex[iIndex + 1].vPos - pVertex[iIndex + VTXCNTX + 1].vPos) * fV;
	//			return ;
	//		}
	//		if(D3DXIntersectTri(&pVertex[iIndex].vPos
	//			, &pVertex[iIndex + 1].vPos
	//			, &pVertex[iIndex + VTXCNTX].vPos
	//			, &m_vPivotPos, &m_vRayDir, &fU, &fV, &fDist))
	//		{
	//			*pOut = pVertex[iIndex].vPos
	//				+ (pVertex[iIndex + 1].vPos - pVertex[iIndex].vPos) * fU
	//				+ (pVertex[iIndex + VTXCNTX].vPos - pVertex[iIndex].vPos) * fV;
	//			return ;
	//		}
	//	}
	//}
}

CMouseCol* CMouseCol::Create(void)
{
	CMouseCol* pCollision = new CMouseCol;
	if(FAILED(pCollision->Initialize()))
		Engine::Safe_Delete(pCollision);

	return pCollision;
}

HRESULT CMouseCol::Initialize(void)
{
	m_pCameraObserver = CCameraObserver::Create();
	m_pInfoSubject->Subscribe(m_pCameraObserver);

	return S_OK;
}

void CMouseCol::Release(void)
{
	m_pInfoSubject->UnSubscribe(m_pCameraObserver);

	Engine::Safe_Delete(m_pCameraObserver);

	if( (*m_pwRefCnt) == 0 )
		Engine::Safe_Delete(m_pwRefCnt);
	else
		--(*m_pwRefCnt);
}

void CMouseCol::Translation_ViewSpace(void)
{
	POINT		ptMouse = GetMousePos();
	const D3DXMATRIX*	pmatProj = m_pCameraObserver->GetProj();

	//	0 -> -1		, 800 -> 1
	//  0 ->1		, 600 - > -1

	D3DXVECTOR3		vTemp;
	vTemp.x = ((float(ptMouse.x) / (WINCX >> 1)) - 1.f) / pmatProj->_11;
	vTemp.y = ((float(-ptMouse.y) / (WINCY >> 1)) + 1.f) / pmatProj->_22;
	vTemp.z = 1.f;

	/*D3DXMATRIX		matView;
	D3DXMatrixInverse(&matView, NULL, pmatProj);
	D3DXVec3TransformCoord(&vTemp, &vTemp, &matView);*/

	m_vPivotPos = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vRayDir = vTemp - m_vPivotPos;
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);
}

void CMouseCol::PickObject(D3DXVECTOR3* pOut , const Engine::VTXTEX* pVertex , const D3DXMATRIX* pmatWorld)
{
	
}

void CMouseCol::Translation_Local(const D3DXMATRIX* pWorld)
{
	D3DXMATRIX		matView = *m_pCameraObserver->GetView();

	D3DXMatrixInverse(&matView, NULL, &matView);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matView);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matView);

	D3DXMATRIX		matWorldInv;

	D3DXMatrixInverse(&matWorldInv, NULL, pWorld);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matWorldInv);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matWorldInv);
}


