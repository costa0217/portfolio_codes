#include "stdafx.h"
#include "RHLightning.h"

#include "Player.h"

CRHLightning::CRHLightning(LPDIRECT3DDEVICE9 pGraphicDev)
: CEffect(pGraphicDev)
{

}

CRHLightning::~CRHLightning(void)
{

}

_int CRHLightning::Update(const _float& fTimeDelta)
{
	m_fDeleteTime	+= fTimeDelta;
	m_fTime			-= fTimeDelta;

	if(true == m_bBullet) // 날리는 이펙트일 때
	{
		if(m_bCollsion || m_fDeleteTime > 1.f)
			return -1;

		CGameObject::Update(fTimeDelta);

		m_Matrix = *m_pBoneMatrix * *m_pWorldMatrix;

		//초기화때 정해진 방향으로 날린다.
		m_pTransCom->m_vPosition += m_vDir * fTimeDelta * 50.f;

		if(m_fTime <= 0.f)
		{
			_vec3 vRevolDir = _vec3(m_vDir.x, 0.f, m_vDir.z);
			D3DXVec3Normalize(&vRevolDir, &vRevolDir);
			_float fDirAngle = acos(D3DXVec3Dot(&vRevolDir , &g_vLook));
			if(vRevolDir.z > g_vLook.z)
				fDirAngle = D3DX_PI * 2.f - fDirAngle;

			printf("effect_lightning_shock\n");
			CEffectMgr::GetInstance()->Play_Effect(L"Thunder_storm2", m_pTransCom->m_vPosition - _vec3(0.f, 5.f, 0.f), false, true, _vec3(0.f,0.f,0.f), _vec3(0.f,0.f,0.f), fDirAngle, true, 0.1f, NULL);
			CEffectMgr::GetInstance()->Play_Effect(L"Lightning_Flore", m_pTransCom->m_vPosition - _vec3(0.f, 5.f, 0.f), false, true, _vec3(0.f,0.f,0.f), _vec3(0.f,0.f,0.f), fDirAngle, true, 0.2f, NULL);

			CSoundMgr::GetInstance()->PlayEffect(SOUND::effect_lightning_shock);
			m_fTime = 0.1f;
		}

	}
	else // 특정 뼈에 붙는 이펙트일 때
	{
		if(m_bDelete)
		{
			printf("CRHLightning Delete=============\n");
			printf("%f\n", m_fDeleteTime);
			return -1;
		}

		CGameObject::Update(fTimeDelta);

		m_Matrix = *m_pBoneMatrix * *m_pWorldMatrix;

		m_pTransCom->m_vPosition = _vec3(m_Matrix._41, m_Matrix._42, m_Matrix._43);

	}

	return 0;
}

void CRHLightning::Render(void)
{
#ifdef _DEBUG
	if(CPlayer::GetInstance()->m_bShowBounding)
	{
		CEffect::Render_BoudingSphere();
	}
#endif
}

HRESULT CRHLightning::Initialize(const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, _bool bBullet)
{
	if(FAILED(Add_Component()))
		return E_FAIL;
	m_pWorldMatrix = pWorldMatrix;
	m_pBoneMatrix = pBoneMatrix;
	m_Matrix = *pBoneMatrix * *pWorldMatrix;

	m_pTransCom->m_vPosition = _vec3(m_Matrix._41, m_Matrix._42, m_Matrix._43);

	m_fRadius = 2.f;
	m_fDeleteTime = 0.f;

	//공격력
	m_fAttackValue = 299.f;

	//플레이어 행렬로 던질 방향을 구한다.
	memcpy( &m_vDir, &(pWorldMatrix->m[2][0]), sizeof(_vec3) );	
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vDir *= -1.f;

	_vec3 vRevolDir = _vec3(m_vDir.x, 0.f, m_vDir.z);
	D3DXVec3Normalize(&vRevolDir, &vRevolDir);
	_float fDirAngle = acos(D3DXVec3Dot(&vRevolDir , &g_vLook));
	if(vRevolDir.z > g_vLook.z)
		fDirAngle = D3DX_PI * 2.f - fDirAngle;

	m_bBullet = bBullet;

	if(false == m_bBullet)
	{
		m_fRadius = 0.f;
		CEffectMgr::GetInstance()->Play_Effect(L"Test_Lightning", _vec3(0.f, 0.f, 0.f)/*m_pTransCom->m_vPosition*/, false, true, _vec3(0.f,0.f,0.f), _vec3(0.f,0.f,0.f), fDirAngle, true, 1.5f, &m_pTransCom->m_vPosition);
	}
	else
	{
		m_fTime = 0.2f;
		m_vDir.y = 0.f;
	}

	return S_OK;
}

HRESULT CRHLightning::Add_Component(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CRHLightning::Set_ConstantTable(void)
{

}

CEffect* CRHLightning::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, _bool bBullet)
{
	CRHLightning* pEffect = new CRHLightning(pGraphicDev);

	if( FAILED(pEffect->Initialize(pWorldMatrix, pBoneMatrix, bBullet)))
	{
		MSG_BOX("RHLightning Create Failed");
		::Safe_Release(pEffect);
	}

	return pEffect;
}

_ulong CRHLightning::Release(void)
{
	_ulong dwRefCnt = CEffect::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}