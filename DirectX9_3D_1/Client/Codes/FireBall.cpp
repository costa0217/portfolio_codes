#include "stdafx.h"
#include "FireBall.h"

#include "Player.h"

CFireBall::CFireBall(LPDIRECT3DDEVICE9 pGraphicDev)
: CEffect(pGraphicDev)
{

}

CFireBall::~CFireBall(void)
{

}

_int CFireBall::Update(const _float& fTimeDelta)
{
	m_fDeleteTime	+= fTimeDelta;
	m_fTime			+= fTimeDelta;

	m_pTransCom->m_vPosition.z += 100.f * fTimeDelta; 

	if(m_fTime > 0.1f)
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Fire_Test", m_pTransCom->m_vPosition);
		m_fTime = 0.f;
	}

	if(m_bCollsion || m_fDeleteTime > 1.f)
		return -1;

	CGameObject::Update(fTimeDelta);
	return 0;
}

void CFireBall::Render(void)
{
#ifdef _DEBUG
	if(CPlayer::GetInstance()->m_bShowBounding)
	{
		CEffect::Render_BoudingSphere();
	}
#endif
}

HRESULT CFireBall::Initialize(_vec3 vCreatePos)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vPosition = vCreatePos;
	m_fRadius = 0.5f;

	CEffectMgr::GetInstance()->Play_Effect(L"Fire_Test", m_pTransCom->m_vPosition);

	return S_OK;
}

HRESULT CFireBall::Add_Component(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CFireBall::Set_ConstantTable(void)
{

}

CEffect* CFireBall::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vCreatePos)
{
	CFireBall* pEffect = new CFireBall(pGraphicDev);

	if( FAILED(pEffect->Initialize(vCreatePos)))
	{
		MSG_BOX("FireBall Create Failed");
		::Safe_Release(pEffect);
	}

	return pEffect;
}

_ulong CFireBall::Release(void)
{
	_ulong dwRefCnt = CEffect::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}