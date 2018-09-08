#include "stdafx.h"
#include "CullingMgr.h"

#include "Export_Function.h"



IMPLEMENT_SINGLETON(CCullingMgr)

CCullingMgr::CCullingMgr()
{

}

CCullingMgr::~CCullingMgr()
{

}

bool CCullingMgr::CullingCamera(const D3DXVECTOR3* pPos, float fRadius)
{
	m_Vtx[0] = D3DXVECTOR3(-1.f, -1.f, 0.f);
	m_Vtx[1] = D3DXVECTOR3(1.f, -1.f, 0.f);
	m_Vtx[2] = D3DXVECTOR3(1.f, -1.f, 1.f);
	m_Vtx[3] = D3DXVECTOR3(-1.f, -1.f, 1.f);
	m_Vtx[4] = D3DXVECTOR3(-1.f, 1.f, 0.f);
	m_Vtx[5] = D3DXVECTOR3(1.f, 1.f, 0.f);
	m_Vtx[6] = D3DXVECTOR3(1.f, 1.f, 1.f);
	m_Vtx[7] = D3DXVECTOR3(-1.f, 1.f, 1.f);

	D3DXMATRIX matView;
	D3DXMATRIX matProj;

	//현재 디바이스의 뷰 행렬과 투영 행렬을 갖고 온다.
	Engine::Get_GraphicDev()->GetTransform(D3DTS_VIEW, &matView);
	Engine::Get_GraphicDev()->GetTransform(D3DTS_PROJECTION, &matProj);

	//얻어온 행렬을 곱하고 역행렬을 구한다.
	D3DXMATRIX ViewProj = (matView) * (matProj);
	D3DXMatrixInverse(&ViewProj, NULL, &ViewProj);

	for(int i = 0; i < 8; ++i)	//8개의 점을 뷰 포트 상의 위치로 옮긴다.
		D3DXVec3TransformCoord(&m_Vtx[i], &m_Vtx[i], &ViewProj);

	//뷰 포트 상의 8개의 점을 이용하여 절두체의 좌우상하원근 평면을 구한다.
	D3DXPlaneFromPoints(&m_Plane[0], &m_Vtx[4], &m_Vtx[7], &m_Vtx[6]);
	D3DXPlaneFromPoints(&m_Plane[1], &m_Vtx[0], &m_Vtx[1], &m_Vtx[2]);
	D3DXPlaneFromPoints(&m_Plane[2], &m_Vtx[0], &m_Vtx[4], &m_Vtx[5]);
	D3DXPlaneFromPoints(&m_Plane[3], &m_Vtx[2], &m_Vtx[6], &m_Vtx[7]);
	D3DXPlaneFromPoints(&m_Plane[4], &m_Vtx[0], &m_Vtx[3], &m_Vtx[7]);
	D3DXPlaneFromPoints(&m_Plane[5], &m_Vtx[1], &m_Vtx[5], &m_Vtx[6]);

	float fDest;

	/*
	점과 평면의 거리를 구하고 반지름과 비교를 하여 평면의 안쪽에 위치하는지
	밖에 위치하는지 비교를 하여 하나라도 만족하지 않으면 경계구가 밖에 있다고
	판단한다.
	*/
	for(int i = 0; i < 6; ++i)
	{
		fDest = D3DXPlaneDotCoord(&m_Plane[i], pPos);
		if(fDest > fRadius)
			return false;
	}

	return true;
}

void CCullingMgr::Release(void)
{
	delete this;
}
