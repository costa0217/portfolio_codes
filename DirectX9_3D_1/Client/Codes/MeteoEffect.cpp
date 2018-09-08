#include "stdafx.h"
#include "MeteoEffect.h"

#include "Player.h"
#include "SoundMgr.h"

_float CMeteoEffect::m_fSize = 2.f;

CMeteoEffect::CMeteoEffect(LPDIRECT3DDEVICE9 pGraphicDev)
: CEffect(pGraphicDev)
, m_bPlay(false)
, m_bPlaySound(false)
, m_fPlayTime(0.f)
{

}

CMeteoEffect::~CMeteoEffect(void)
{

}

_int CMeteoEffect::Update(const _float& fTimeDelta)
{
	m_fDeleteTime	+= fTimeDelta;
	m_fTime			+= fTimeDelta;

	if( m_fTime >= m_fPlayTime && !m_bPlay)
	{
		m_bPlay = true;
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin", m_pTransCom->m_vPosition, false, false, _vec3(m_fSize, m_fSize, m_fSize));
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin2", m_pTransCom->m_vPosition, false, false, _vec3(m_fSize, m_fSize, m_fSize), _vec3(0.f, 0.f, 0.f), 0.f, false, 0.f, NULL, true);
	}
	if( m_fTime >= 0.4f + m_fPlayTime&& !m_bPlaySound)
	{
		m_bPlaySound = true;
		CSoundMgr::GetInstance()->PlayEffect(SOUND::explosion_lv4, m_pTransCom->m_vPosition, 0.9f);
	}

	if( m_fDeleteTime > 1.f )
		m_bDelete = true;

	if(m_bDelete)
	{
		m_fTime = 0.f;
		printf("CMeteoEffect Delete=============\n");
		printf("%f\n", m_fDeleteTime);
		return -1;
	}

	CGameObject::Update(fTimeDelta);

	return 0;
}

void CMeteoEffect::Render(void)
{
#ifdef _DEBUG
	if(CPlayer::GetInstance()->m_bShowBounding)
	{
		CEffect::Render_BoudingSphere();
	}
#endif
}

HRESULT CMeteoEffect::Initialize(_vec3 vCreatePos)
{
	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vPosition = vCreatePos;
	m_fRadius = 10.f;

	m_fAttackValue = 350.f + rand()%150;

	m_fPlayTime = rand()%3 * 0.1f;

	return S_OK;
}

HRESULT CMeteoEffect::Add_Component(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CMeteoEffect::Set_ConstantTable(void)
{

}

CEffect* CMeteoEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vCreatePos)
{
	CMeteoEffect* pEffect = new CMeteoEffect(pGraphicDev);

	if( FAILED(pEffect->Initialize(vCreatePos)))
	{
		MSG_BOX("BreathEffect Create Failed");
		::Safe_Release(pEffect);
	}

	return pEffect;
}

_ulong CMeteoEffect::Release(void)
{
	_ulong dwRefCnt = CEffect::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}