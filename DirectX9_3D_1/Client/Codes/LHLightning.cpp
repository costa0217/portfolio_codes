#include "stdafx.h"
#include "LHLightning.h"

#include "Player.h"

CLHLightning::CLHLightning(LPDIRECT3DDEVICE9 pGraphicDev)
: CEffect(pGraphicDev)
{

}

CLHLightning::~CLHLightning(void)
{

}

_int CLHLightning::Update(const _float& fTimeDelta)
{
	m_fDeleteTime	+= fTimeDelta;
	m_fTime			+= fTimeDelta;

	if(true == m_bBullet) // 날리는 이펙트일 때
	{
		if(m_bCollsion)
		{
			_vec3 vDir = _vec3(0.f, 0.f, -1.f);

			_int iNum = 8;
			for(_int i = 0; i < iNum; ++i)
			{
				_matrix		matRotY;
				_vec3		vEffectDir(0.f, 0.f, 0.f);
				float		fAngle = (360.f / iNum) * (float)i;

				D3DXMatrixIdentity(&matRotY);
				D3DXMatrixRotationY(&matRotY, D3DXToRadian(fAngle));

				D3DXVec3TransformNormal(&vEffectDir, &vDir, &matRotY);
				D3DXVec3Normalize(&vEffectDir, &vEffectDir);

				_float fTime = 1.f;
				if(i == 1 || i == 4)
				{
					m_pTransCom->m_vPosition.y -= 2.f;
					fTime = 2.f;
				}
				else if(i == 3 || i == 7)
				{
					m_pTransCom->m_vPosition.y -= 4.f;
					fTime = 0.5f;
				}
				else 
					fTime = 1.f;
				CEffectMgr::GetInstance()->Play_Effect(L"Body_Lightning2", m_pTransCom->m_vPosition + (vEffectDir * 4.f), false, true, _vec3(1.f, 1.f, 1.f), _vec3(0.f, 0.f, 0.f), 0.f, true, fTime, NULL);
			}
			
			
			return -1;
		}
		if(m_fDeleteTime > 4.f)
				return -1;

		CGameObject::Update(fTimeDelta);

		m_Matrix =  *m_pBoneMatrix * *m_pWorldMatrix;

		//초기화때 정해진 방향으로 날리고 y값은 줄여준다.
		//m_Matrix 이거 그대로 쓰면 사정거리가 짧다.
		m_pTransCom->m_vPosition += m_vDir * fTimeDelta * 80.f;
		m_pTransCom->m_vPosition.y -= fTimeDelta * 20.f;
	}
	else // 특정 뼈에 붙는 이펙트일 때
	{
		if(m_bDelete)
		{
			printf("CLHLightning Delete=============\n");
			printf("%f\n", m_fDeleteTime);
			return -1;
		}

		CGameObject::Update(fTimeDelta);

		m_Matrix = *m_pBoneMatrix * *m_pWorldMatrix;
		m_pTransCom->m_vPosition = _vec3(m_Matrix._41, m_Matrix._42, m_Matrix._43);

	}
 	
	return 0;
}

void CLHLightning::Render(void)
{
#ifdef _DEBUG
	if(CPlayer::GetInstance()->m_bShowBounding)
	{
		CEffect::Render_BoudingSphere();
	}
#endif
}

HRESULT CLHLightning::Initialize(const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, _bool bBullet)
{
	if(FAILED(Add_Component()))
		return E_FAIL;


	m_pWorldMatrix = pWorldMatrix;
	m_pBoneMatrix = pBoneMatrix;
	m_Matrix = *pBoneMatrix * *pWorldMatrix;

	m_pTransCom->m_vPosition = _vec3(m_Matrix._41, m_Matrix._42, m_Matrix._43);

	m_fDeleteTime = 0.f;

	//공격력
	m_fAttackValue = 222.f;

	//플레이어 행렬로 던질 방향을 구한다.
	memcpy( &m_vDir, &(pWorldMatrix->m[2][0]), sizeof(_vec3) );	
	D3DXVec3Normalize(&m_vDir, &m_vDir);
	m_vDir *= -1.f;

	m_bBullet = bBullet;

	_vec3 vRevolDir = _vec3(m_vDir.x, 0.f, m_vDir.z);
	D3DXVec3Normalize(&vRevolDir, &vRevolDir);
	_float fDirAngle = acos(D3DXVec3Dot(&vRevolDir , &g_vLook));
	if(vRevolDir.z > g_vLook.z)
		fDirAngle = D3DX_PI * 2.f - fDirAngle;

	if(true == m_bBullet)
	{
		m_fRadius = 2.f;
	
		CEffectMgr::GetInstance()->Play_Effect(L"Test_Lightning", _vec3(0.f, 0.f, 0.f)/*m_pTransCom->m_vPosition*/, false, true, _vec3(0.f,0.f,0.f), _vec3(0.f,0.f,0.f), fDirAngle, true, 2.f, &m_pTransCom->m_vPosition);
		
		//CEffectMgr::GetInstance()->Play_Effect(L"Tail_Tail", _vec3(0.f, 0.f, 0.f)/*m_pTransCom->m_vPosition*/, false, true, _vec3(0.f,0.f,0.f), _vec3(0.f,0.f,0.f), fDirAngle, true, 1.f, &m_pTransCom->m_vPosition);
	}
	else
	{
		m_fRadius = 0.f;
		CEffectMgr::GetInstance()->Play_Effect(L"Test_Lightning", _vec3(0.f, 0.f, 0.f)/*m_pTransCom->m_vPosition*/, false, true, _vec3(0.f,0.f,0.f), _vec3(0.f,0.f,0.f), fDirAngle, true, 4.f, &m_pTransCom->m_vPosition);
		//CEffectMgr::GetInstance()->Play_Effect(L"Test_Lightning", m_pTransCom->m_vPosition/*, false, _vec3(0.f,0.f,0.f), _vec3(0.f,0.f,0.f), 0.f, true, 10.f, &m_pTransCom->m_vPosition*/);
	}

	return S_OK;
}

HRESULT CLHLightning::Add_Component(void)
{
	Engine::CComponent*		pComponent = NULL;

	//Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));
	
	return S_OK;
}

void CLHLightning::Set_ConstantTable(void)
{

}

CEffect* CLHLightning::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _matrix* pWorldMatrix, const _matrix* pBoneMatrix, _bool bBullet)
{
	CLHLightning* pEffect = new CLHLightning(pGraphicDev);

	if( FAILED(pEffect->Initialize(pWorldMatrix, pBoneMatrix, bBullet)))
	{
		MSG_BOX("LHLightning Create Failed");
		::Safe_Release(pEffect);
	}

	return pEffect;
}

_ulong CLHLightning::Release(void)
{
	_ulong dwRefCnt = CEffect::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}