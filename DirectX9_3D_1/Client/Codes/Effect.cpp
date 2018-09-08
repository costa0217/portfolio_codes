#include "stdafx.h"
#include "Effect.h"

#include "CollisionMgr.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev)
: CGameObject(pGraphicDev)
, m_pTransCom(NULL)
, m_fTime(0.f)
, m_bCollsion(false)
, m_fDeleteTime(0.f)
, m_pWorldMatrix(NULL)
, m_pBoneMatrix(NULL)
, m_fAttackValue(0.f)
, m_fDownValue(10.f)
, m_bDelete(false)
{
	CCollisionMgr::GetInstance()->Get_EffectList()->push_back(this);
}

CEffect::~CEffect(void)
{

}

_ulong CEffect::Release(void)
{
	_ulong dwRefCnt = CGameObject::Release();

	if(0 == dwRefCnt)
	{	

	}

	return dwRefCnt;
}

#ifdef DEBUG
void CEffect::Render_BoudingSphere()
{
	//피격 스피어
	D3DXCreateSphere(m_pGraphicDev, m_fRadius, 20, 10, &m_pSphereMesh, &m_pSphereBuffer );

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &(m_pTransCom->m_matWorld) );
	m_pSphereMesh->DrawSubset(0);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	Engine::Safe_Release(m_pSphereMesh);
	Engine::Safe_Release(m_pSphereBuffer);
}
#endif