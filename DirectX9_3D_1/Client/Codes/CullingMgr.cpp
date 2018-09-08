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

	//���� ����̽��� �� ��İ� ���� ����� ���� �´�.
	Engine::Get_GraphicDev()->GetTransform(D3DTS_VIEW, &matView);
	Engine::Get_GraphicDev()->GetTransform(D3DTS_PROJECTION, &matProj);

	//���� ����� ���ϰ� ������� ���Ѵ�.
	D3DXMATRIX ViewProj = (matView) * (matProj);
	D3DXMatrixInverse(&ViewProj, NULL, &ViewProj);

	for(int i = 0; i < 8; ++i)	//8���� ���� �� ��Ʈ ���� ��ġ�� �ű��.
		D3DXVec3TransformCoord(&m_Vtx[i], &m_Vtx[i], &ViewProj);

	//�� ��Ʈ ���� 8���� ���� �̿��Ͽ� ����ü�� �¿���Ͽ��� ����� ���Ѵ�.
	D3DXPlaneFromPoints(&m_Plane[0], &m_Vtx[4], &m_Vtx[7], &m_Vtx[6]);
	D3DXPlaneFromPoints(&m_Plane[1], &m_Vtx[0], &m_Vtx[1], &m_Vtx[2]);
	D3DXPlaneFromPoints(&m_Plane[2], &m_Vtx[0], &m_Vtx[4], &m_Vtx[5]);
	D3DXPlaneFromPoints(&m_Plane[3], &m_Vtx[2], &m_Vtx[6], &m_Vtx[7]);
	D3DXPlaneFromPoints(&m_Plane[4], &m_Vtx[0], &m_Vtx[3], &m_Vtx[7]);
	D3DXPlaneFromPoints(&m_Plane[5], &m_Vtx[1], &m_Vtx[5], &m_Vtx[6]);

	float fDest;

	/*
	���� ����� �Ÿ��� ���ϰ� �������� �񱳸� �Ͽ� ����� ���ʿ� ��ġ�ϴ���
	�ۿ� ��ġ�ϴ��� �񱳸� �Ͽ� �ϳ��� �������� ������ ��豸�� �ۿ� �ִٰ�
	�Ǵ��Ѵ�.
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
