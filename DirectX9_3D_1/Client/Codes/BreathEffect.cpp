#include "stdafx.h"
#include "BreathEffect.h"

#include "Player.h"

_vec3 CBreathEffect::m_vDir = _vec3(0.f, 0.f, 0.f);

CBreathEffect::CBreathEffect(LPDIRECT3DDEVICE9 pGraphicDev)
: CEffect(pGraphicDev)
, m_fSize(0.3f)
{

}

CBreathEffect::~CBreathEffect(void)
{

}

_int CBreathEffect::Update(const _float& fTimeDelta)
{
	m_fDeleteTime	+= fTimeDelta;
	m_fTime += fTimeDelta;

	m_pTransCom->m_vPosition += m_vDir * 50.f * fTimeDelta; 
	if(m_fTime > 0.1f)
	{
		m_fSize += 0.2f;
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Missile", m_pTransCom->m_vPosition + (m_vDir * 4.f), false, false, _vec3(m_fSize, m_fSize, m_fSize), _vec3(0.f, 0.f, 0.f));
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Missile", m_pTransCom->m_vPosition + (m_vDir * 4.f), false, false, _vec3(m_fSize, m_fSize, m_fSize), _vec3(0.f, 90.f, 0.f));
		m_fTime = 0.f;
	}

	if(m_bCollsion || m_fDeleteTime > 1.f)
		return -1;

	CGameObject::Update(fTimeDelta);
	return 0;
}

void CBreathEffect::Render(void)
{
#ifdef _DEBUG
	if(CPlayer::GetInstance()->m_bShowBounding)
	{
		CEffect::Render_BoudingSphere();
	}
#endif
}

HRESULT CBreathEffect::Initialize(_vec3 vCreatePos)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vPosition = vCreatePos;
	m_fRadius = 8.f;
	m_fAttackValue = 550 + rand()%150;

	CEffectMgr::GetInstance()->Play_Effect(L"Blood_Missile", m_pTransCom->m_vPosition, false, false, _vec3(m_fSize, m_fSize, m_fSize), _vec3(0.f, 0.f, 0.f), 0.f, true, 2.f);
	CEffectMgr::GetInstance()->Play_Effect(L"Blood_Missile", m_pTransCom->m_vPosition, false, false, _vec3(m_fSize, m_fSize, m_fSize), _vec3(0.f, 90.f, 0.f), 0.f, true, 2.f);

	return S_OK;
}

HRESULT CBreathEffect::Add_Component(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CBreathEffect::Set_ConstantTable(void)
{

}

CEffect* CBreathEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vCreatePos)
{
	CBreathEffect* pEffect = new CBreathEffect(pGraphicDev);

	if( FAILED(pEffect->Initialize(vCreatePos)))
	{
		MSG_BOX("BreathEffect Create Failed");
		::Safe_Release(pEffect);
	}

	return pEffect;
}

void CBreathEffect::SetDir( _vec3 vDir )
{
	m_vDir = vDir;
}

_ulong CBreathEffect::Release(void)
{
	_ulong dwRefCnt = CEffect::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}