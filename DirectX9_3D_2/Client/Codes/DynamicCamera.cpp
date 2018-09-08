#include "stdafx.h"
#include "DynamicCamera.h"
#include "Export_Function.h"
#include "Boss.h"

CDynamicCamera::CDynamicCamera(LPDIRECT3DDEVICE9 pDevice)
: Engine::CCamera(pDevice)
, m_bMouseFix(true)
, m_bClick(false)
, m_pTimeMgr(NULL)
, m_pInput(NULL)
, m_pBoss(NULL)
, m_pPlayer(NULL)
, m_pPlayerTransCom(NULL)
, m_pBossTransCom(NULL)
, m_fAccTime(0.f)
{

}

CDynamicCamera::~CDynamicCamera(void)
{
	
}

HRESULT CDynamicCamera::InitCamera(const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	m_fCamSpeed = 10.f;

	m_vEye = *pEye;
	m_vAt = *pAt;
	m_vUp = D3DXVECTOR3(0.f, 1.f, 0.f);
	Invalidate_ViewMatrix();

	m_fFovY = D3DXToRadian(45.f);
	m_fAspect = float(WINSIZEX) / WINSIZEY;
	m_fNear = 0.1f;
	m_fFar = 1000.f;
	Invalidate_ProjMatrix();
	return S_OK;
}

void CDynamicCamera::KeyCheck(const _float& fTimeDelta)
{
}
_int CDynamicCamera::Update(const _float& fTimeDelta)
{
	if( ((CBoss*)m_pBoss)->Get_Openning() == true)
	{
		m_fAccTime += fTimeDelta;
		if(m_pPlayerTransCom == NULL)
		{
			m_pPlayerTransCom	= (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Player"); 
			m_pBossTransCom		= (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Boss"); 

			m_vEye	= _vec3(m_pPlayerTransCom->m_vPosition.x, 0.f, m_pPlayerTransCom->m_vPosition.z)
				+ (m_vUp * 50.f);
			//m_vAt	= m_pBossTransCom->m_vPosition;
		}
		m_vAt	= m_pBossTransCom->m_vPosition + (m_vUp * 3.f);

		_vec3 vLook(0.f, 0.f, 0.f);
		vLook = m_vAt - m_vEye;
		D3DXVec3Normalize(&vLook, &vLook);

		if(D3DXVec3Length(&(m_vEye - m_vAt)) > 8.f)
			m_vEye += vLook * m_fAccTime * 2.f;

		Engine::CCamera::Update(fTimeDelta);		
	}
	else
	{
		m_fAccTime = 0.f;
	}
	
	return 0;
}

CDynamicCamera* CDynamicCamera::Create(LPDIRECT3DDEVICE9 pDevice , const D3DXVECTOR3* pEye, const D3DXVECTOR3* pAt)
{
	CDynamicCamera*		pCamera = new CDynamicCamera(pDevice);
	if(FAILED(pCamera->InitCamera(pEye, pAt)))
		Engine::Safe_Release(pCamera);

	return pCamera;
}

void CDynamicCamera::FixMouse(void)
{
}

_ulong CDynamicCamera::Release()
{
	_ulong dwRefCnt = Engine::CCamera::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}
	return dwRefCnt;
}

