#include "stdafx.h"
#include "Spin.h"

#include "Player.h"

CSpin::CSpin(LPDIRECT3DDEVICE9 pGraphicDev)
: CEffect(pGraphicDev)
{

}

CSpin::~CSpin(void)
{

}

_int CSpin::Update(const _float& fTimeDelta)
{
	m_fDeleteTime	+= fTimeDelta;
	m_fTime			+= fTimeDelta;

// 	if(m_bDelete)
// 	{
// 		return -1;
// 	}

	CGameObject::Update(fTimeDelta);

	m_Matrix =  *m_pWorldMatrix;
	m_pTransCom->m_vPosition = _vec3(m_Matrix._41, m_Matrix._42, m_Matrix._43);

	if(m_fTime > 0.001f)
	{
		printf("m_iSpinNum   %d\n", m_iSpinNum);
		_vec3 vDir = _vec3(1.f, 0.f, 0.f);

		_matrix		matRotY, matRotY2;
		_vec3		vEffectDir(0.f, 0.f, 0.f);
		float		fAngle = (360.f / 36) * (float)m_iSpinNum;
		D3DXMatrixIdentity(&matRotY);
		D3DXMatrixRotationY(&matRotY, D3DXToRadian(-fAngle));

		D3DXMatrixIdentity(&matRotY2);
		D3DXMatrixRotationY(&matRotY2, D3DXToRadian((fAngle + 180.f) * -1.f));

		D3DXVec3TransformNormal(&vEffectDir, &vDir, &matRotY);
		D3DXVec3Normalize(&vEffectDir, &vEffectDir);
		CEffectMgr::GetInstance()->Play_Effect(L"wind_blade", m_pTransCom->m_vPosition + (vEffectDir * 9.f) + _vec3(0.f, 5.f, 0.f), false, true, _vec3(1.f, 8.f, 0.f), _vec3(0.f, -fAngle, 0.f), 0.f, false, 0.f, NULL);
		
		D3DXVec3TransformNormal(&vEffectDir, &vDir, &matRotY2);
		D3DXVec3Normalize(&vEffectDir, &vEffectDir);
		CEffectMgr::GetInstance()->Play_Effect(L"wind_blade", m_pTransCom->m_vPosition + (vEffectDir * 7.f) + _vec3(0.f, 5.f, 0.f), false, true, _vec3(1.f, 8.f, 0.f), _vec3(0.f, -(fAngle + 180.f), 0.f), 0.f, false, 0.f, NULL);
	
		++m_iSpinNum;
		if(m_iSpinNum >= 36)
		{
			return -1;
		}
		m_fTime = 0.f;

	}
	

	return 0;
}

void CSpin::Render(void)
{
#ifdef _DEBUG
	if(CPlayer::GetInstance()->m_bShowBounding)
	{
		CEffect::Render_BoudingSphere();
	}
#endif
}

HRESULT CSpin::Initialize(const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, _bool bBullet)
{
	if(FAILED(Add_Component()))
		return E_FAIL;
	if(NULL == pBoneMatrix)
	{
		m_pWorldMatrix = pWorldMatrix;
		m_Matrix =  *pWorldMatrix;
	}
	else
	{
		m_pWorldMatrix = pWorldMatrix;
		m_pBoneMatrix = pBoneMatrix;
		m_Matrix = *pBoneMatrix * *pWorldMatrix;
	}
	

	m_pTransCom->m_vPosition = _vec3(m_Matrix._41, m_Matrix._42, m_Matrix._43);

	m_fDeleteTime = 0.f;

	//°ø°Ý·Â
	m_fAttackValue = 318.f;

	m_fRadius = 0.f;

	m_iSpinNum = 0;
		

	return S_OK;
}

HRESULT CSpin::Add_Component(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CSpin::Set_ConstantTable(void)
{

}

CEffect* CSpin::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, _bool bBullet)
{
	CSpin* pEffect = new CSpin(pGraphicDev);

	if( FAILED(pEffect->Initialize(pWorldMatrix, pBoneMatrix, bBullet)))
	{
		MSG_BOX("Spin Create Failed");
		::Safe_Release(pEffect);
	}

	return pEffect;
}

_ulong CSpin::Release(void)
{
	_ulong dwRefCnt = CEffect::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}