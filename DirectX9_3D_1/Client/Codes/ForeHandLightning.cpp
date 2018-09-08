#include "stdafx.h"
#include "ForeHandLightning.h"

#include "Player.h"

CForeHandLightning::CForeHandLightning(LPDIRECT3DDEVICE9 pGraphicDev)
: CEffect(pGraphicDev)
{

}

CForeHandLightning::~CForeHandLightning(void)
{

}

_int CForeHandLightning::Update(const _float& fTimeDelta)
{
	m_fDeleteTime	+= fTimeDelta;
	m_fTime			+= fTimeDelta;

	if(m_bDelete)
	{
		return -1;
	}

	CGameObject::Update(fTimeDelta);

	m_Matrix = *m_pBoneMatrix * *m_pWorldMatrix;

	m_pTransCom->m_vPosition = _vec3(m_Matrix._41, m_Matrix._42, m_Matrix._43);

	return 0;
}

void CForeHandLightning::Render(void)
{
#ifdef _DEBUG
	if(CPlayer::GetInstance()->m_bShowBounding)
	{
		CEffect::Render_BoudingSphere();
	}
#endif
}

HRESULT CForeHandLightning::Initialize(const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, TCHAR* pEffectName)
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
	m_fAttackValue = 418.f;

	//플레이어 행렬로 던질 방향을 구한다.
	_vec3 vDir;
	memcpy( &vDir, &(pWorldMatrix->m[2][0]), sizeof(_vec3) );	
	D3DXVec3Normalize(&vDir, &vDir);
	vDir *= -1.f;

	_vec3 vRevolDir = _vec3(vDir.x, 0.f, vDir.z);
	D3DXVec3Normalize(&vRevolDir, &vRevolDir);
	_float fDirAngle = acos(D3DXVec3Dot(&vRevolDir , &g_vLook));
	if(vRevolDir.z > g_vLook.z)
		fDirAngle = D3DX_PI * 2.f - fDirAngle;

	if( !lstrcmp(pEffectName, L"Thunder_storm2"))
		CEffectMgr::GetInstance()->Play_Effect(pEffectName, vRevolDir * 10.f/*m_pTransCom->m_vPosition*/, false, true, _vec3(0.f,0.f,0.f), _vec3(0.f,0.f,0.f), fDirAngle, true, 1.f, &m_pTransCom->m_vPosition);
	else
		CEffectMgr::GetInstance()->Play_Effect(pEffectName, _vec3(0.f, 0.f, 0.f)/*m_pTransCom->m_vPosition*/, false, true, _vec3(0.f,0.f,0.f), _vec3(0.f,0.f,0.f), fDirAngle, true, 3.6f, &m_pTransCom->m_vPosition);
	
	return S_OK;
}

HRESULT CForeHandLightning::Add_Component(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CForeHandLightning::Set_ConstantTable(void)
{

}

CEffect* CForeHandLightning::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, TCHAR* pEffectName)
{
	CForeHandLightning* pEffect = new CForeHandLightning(pGraphicDev);

	if( FAILED(pEffect->Initialize(pWorldMatrix, pBoneMatrix, pEffectName)))
	{
		MSG_BOX("ForeHandLightning Create Failed");
		::Safe_Release(pEffect);
	}

	return pEffect;
}

_ulong CForeHandLightning::Release(void)
{
	_ulong dwRefCnt = CEffect::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}