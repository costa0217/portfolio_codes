#include "stdafx.h"
#include "AroundLightning.h"

#include "Player.h"

CAroundLightning::CAroundLightning(LPDIRECT3DDEVICE9 pGraphicDev)
: CEffect(pGraphicDev)
{

}

CAroundLightning::~CAroundLightning(void)
{

}

_int CAroundLightning::Update(const _float& fTimeDelta)
{
	m_fDeleteTime	+= fTimeDelta;
	m_fTime			+= fTimeDelta;

	if(m_bDelete || m_bCollsion)
	{
		return -1;
	}

	CGameObject::Update(fTimeDelta);

	
	return 0;
}

void CAroundLightning::Render(void)
{
#ifdef _DEBUG
	if(CPlayer::GetInstance()->m_bShowBounding)
	{
		CEffect::Render_BoudingSphere();
	}
#endif
}

HRESULT CAroundLightning::Initialize(const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, _bool bBullet)
{
	if(FAILED(Add_Component()))
		return E_FAIL;
	m_pWorldMatrix = pWorldMatrix;
	m_pBoneMatrix = pBoneMatrix;
	m_Matrix = *pBoneMatrix * *pWorldMatrix;

	m_pTransCom->m_vPosition = _vec3(m_Matrix._41, m_Matrix._42, m_Matrix._43);

	m_bBullet = bBullet;

	m_fDeleteTime = 0.f;

	//공격력
	m_fAttackValue = 572.f;

	//플레이어 행렬로 방향을 구한다.
	_vec3 vDir;
	memcpy( &vDir, &(pWorldMatrix->m[2][0]), sizeof(_vec3) );	
	D3DXVec3Normalize(&vDir, &vDir);
	vDir *= -1.f;

	_vec3 vRevolDir = _vec3(vDir.x, 0.f, vDir.z);
	D3DXVec3Normalize(&vRevolDir, &vRevolDir);
	_float fDirAngle = acos(D3DXVec3Dot(&vRevolDir , &g_vLook));
	if(vRevolDir.z > g_vLook.z)
		fDirAngle = D3DX_PI * 2.f - fDirAngle;

	if(true == m_bBullet)
	{
		m_fRadius = 12.f;
		CEffectMgr::GetInstance()->Play_Effect(L"Thunder_storm2", m_pTransCom->m_vPosition, false, true, _vec3(0.f,0.f,0.f), _vec3(0.f,0.f,0.f), 0.f, true, 1.f, NULL);
		
		m_pTransCom->m_vPosition.y += 15.f;
		_vec3 vDir = _vec3(0.f, 0.f, -1.f);

		_int iNum = 16;
		for(_int i = 0; i < iNum; ++i)
		{
			_matrix		matRotY;
			_vec3		vEffectDir(0.f, 0.f, 0.f);
			float		fAngle = (360.f / iNum) * (float)i;

			D3DXMatrixIdentity(&matRotY);
			D3DXMatrixRotationY(&matRotY, D3DXToRadian(fAngle));

			D3DXVec3TransformNormal(&vEffectDir, &vDir, &matRotY);
			D3DXVec3Normalize(&vEffectDir, &vEffectDir);
			CEffectMgr::GetInstance()->Play_Effect(L"Body_Lightning2", m_pTransCom->m_vPosition + (vEffectDir * m_fRadius), false, true, _vec3(0.f, 15.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, true, 2.f, NULL);
		}
		m_pTransCom->m_vPosition.y -= 15.f;

	}
	else
	{
		m_fRadius = 0.f;
		
	}


	return S_OK;
}

HRESULT CAroundLightning::Add_Component(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CAroundLightning::Set_ConstantTable(void)
{

}

CEffect* CAroundLightning::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, _bool bBullet)
{
	CAroundLightning* pEffect = new CAroundLightning(pGraphicDev);

	if( FAILED(pEffect->Initialize(pWorldMatrix, pBoneMatrix, bBullet)))
	{
		MSG_BOX("AroundLightning Create Failed");
		::Safe_Release(pEffect);
	}

	return pEffect;
}

_ulong CAroundLightning::Release(void)
{
	_ulong dwRefCnt = CEffect::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}