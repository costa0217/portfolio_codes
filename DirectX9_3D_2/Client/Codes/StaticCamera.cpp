#include "stdafx.h"
#include "StaticCamera.h"

#include "Defines.h"
#include "Export_Function.h"
#include "Transform.h"
#include "Player.h"
#include "Boss.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pDevice, Engine::CGameObject* pPlayer)
: Engine::CCamera(pDevice)
, m_fTargetDistance(0.f)
, m_fAngle(0.f)
, m_fCamSpeed(0.f)
, m_pTimeMgr(NULL)
, m_pTargetInfo(NULL)
, m_pPlayer(pPlayer)
, m_vCameraDirX(0.f, 0.f, 0.f)
, m_vCameraDirY(0.f, 0.f, 0.f)
, m_vResultCamDir(0.f, 0.f, 0.f)
, m_bClickMouse(false)
, m_bDirChange(true)
, m_fRotScreenTime(0.f)

, m_fSaveDist(0.f)
, m_bFstLoop(false)
, m_fTimeDelta(0.f)
, m_fZoomTime(0.f)
, m_vShakeAt(0.f, 0.f, 0.f)
, m_bKeyPress(false)
, m_vSaveAt(0.f, 0.f, 0.f)

, m_pBoss(NULL)
{
	m_ClickMousePoint.x = 0;
	m_ClickMousePoint.y = 0;
}

CStaticCamera::~CStaticCamera(void)
{

}

void CStaticCamera::SetCameraTarget(Engine::CTransform* pTargetInfo)
{
	m_pTargetInfo = pTargetInfo;

	m_vEye	= m_pTargetInfo->m_vDir * -1.f * m_fTargetDistance;

	D3DXVECTOR3		vRight;

	D3DXVec3Cross(&vRight, &m_vUp, &m_pTargetInfo->m_vDir);
	D3DXVec3Normalize(&vRight, &vRight);


	D3DXMATRIX		matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, m_fAngle);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	m_vCameraDirX = m_vEye;
	m_vCameraDirY = m_vEye;

	m_vAt = m_pTargetInfo->m_vPosition;
	m_vEye += m_pTargetInfo->m_vPosition;

	Invalidate_ViewMatrix();
}

HRESULT CStaticCamera::Initialize(void)
{
	m_fTargetDistance = 6.f;
	m_fAngle = D3DXToRadian(45.f);
	m_fCamSpeed = 15.f;

	m_vEye = _vec3(0.f, 13.f, -8.f);
	m_vAt = _vec3(0.f, 0.f, 0.f);
	m_vUp = _vec3(0.f, 1.f, 0.f);

	Invalidate_ViewMatrix();

	m_fFovY		= D3DXToRadian(45.f);
	m_fAspect	= float(WINSIZEX) / WINSIZEY;
	m_fNear		= 0.1f;
	m_fFar		= 1000.f;

	Invalidate_ProjMatrix();

	return S_OK;
}

_int CStaticCamera::Update(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	if( ((CBoss*)m_pBoss)->Get_Openning() == false)
	{
		if(NULL == m_pTargetInfo)
		{
			m_pTargetInfo = (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Player");
			if(NULL == m_pTargetInfo)
				return -1;

			SetCameraTarget(m_pTargetInfo);
		}

		Zoom_InOut();
		if(!m_bFstLoop)
			KeyCheck(fTimeDelta);

		Engine::CCamera::Update(fTimeDelta);

		TargetRenewal();
	}
		

	return 0;
}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pDevice, Engine::CGameObject* pPlayer/* , const Engine::CTransform* pTargetInfo*/)
{
	CStaticCamera* pCamera = new CStaticCamera(pDevice, pPlayer);
	if(FAILED(pCamera->Initialize()))
		Engine::Safe_Release(pCamera);

	return pCamera;
}

void CStaticCamera::KeyCheck(const _float& fTimeDelta)
{
	int		iDistance = 0;
	if(((CPlayer*)m_pPlayer)->Get_State() == CPlayer::STATE_SKILL) return;

	if(Engine::GetDIKeyState(DIK_TAB) & 0x80)
	{
		if(!m_bKeyPress)
		{
			m_bDirChange ^= true;
			m_bKeyPress = true;
		}
	}	
	else
		m_bKeyPress = false;
	
	if(iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_X))
	{
		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &D3DXVECTOR3(0.f, 1.f, 0.f), D3DXToRadian(iDistance * 0.2f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		m_vCameraDirX = m_vAt - vDir - m_pTargetInfo->m_vPosition;
	}

	if(iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_Y))
	{
		D3DXMATRIX		matViewInv;
		D3DXVECTOR3		vRight;
		D3DXMatrixInverse(&matViewInv, NULL, &m_matView);
		memcpy(&vRight, &matViewInv.m[0][0], sizeof(D3DXVECTOR3));

		D3DXMATRIX		matAxis;
		D3DXMatrixRotationAxis(&matAxis, &vRight, D3DXToRadian(iDistance * 0.2f));

		D3DXVECTOR3		vDir;
		vDir = m_vAt - m_vEye;
		D3DXVec3TransformNormal(&vDir, &vDir, &matAxis);

		_vec3		vDirNormal;
		D3DXVec3Normalize(&vDirNormal, &-vDir);

		//y 꼭대기 아래 예외처리
		if ( D3DXToRadian(10.f) < acos(D3DXVec3Dot(&vDirNormal, &D3DXVECTOR3(0.f, 1.f, 0.f)))
				&& D3DXToRadian(100.f) > acos(D3DXVec3Dot(&vDirNormal, &D3DXVECTOR3(0.f, 1.f, 0.f))))
		{
			m_vCameraDirY = m_vAt - vDir - m_pTargetInfo->m_vPosition;
		}
		
	}
	if(iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_Z))
	{
		if(m_fTargetDistance - iDistance * 0.005f < 2.f 
			|| m_fTargetDistance - iDistance * 0.005f > 10.5f)
			return;

		m_fTargetDistance -= iDistance * 0.005f;
	}
	if( !((CPlayer*)m_pPlayer)->GetKeyPress() 
		&& !((CPlayer*)m_pPlayer)->GetStateJump()
		&& m_bDirChange)
	{
		_vec3 vCamDir = _vec3(-m_vResultCamDir.x, 0.f, -m_vResultCamDir.z);
		D3DXVec3Normalize(&vCamDir, &vCamDir);
		((CPlayer*)m_pPlayer)->SetDirection(vCamDir);
	}
	
}

void CStaticCamera::TargetRenewal(void)
{
	D3DXVec3Normalize(&m_vResultCamDir, &((m_vCameraDirX + m_vCameraDirY) * 0.5f));

	if(((CPlayer*)m_pPlayer)->Get_State() == CPlayer::STATE_SKILL 
		|| ((CPlayer*)m_pPlayer)->Get_State() == CPlayer::STATE_ATTACK)
	{
		_vec3  vAtkCampos	= *((CPlayer*)m_pPlayer)->Get_AtkCamPos();

		D3DXVec3TransformCoord(&vAtkCampos, &vAtkCampos, &m_pTargetInfo->m_matWorld);
		m_vAt	= vAtkCampos;
	}
	else
	{
		_vec3  vDftCampos = *((CPlayer*)m_pPlayer)->Get_DftCamPos();
		D3DXVec3TransformCoord(&vDftCampos, &vDftCampos, &m_pTargetInfo->m_matWorld);
		m_vAt	= vDftCampos;
		//m_vAt	= m_pTargetInfo->m_vPosition + (m_vUp * 1.8f) + m_vShakeAt;
	}

	m_vEye	= m_vResultCamDir * m_fTargetDistance + m_pTargetInfo->m_vPosition;

	FixMouse();

	Invalidate_ViewMatrix();
}

_ulong CStaticCamera::Release(void)
{
	_ulong dwRefCnt = Engine::CCamera::Release();

	if(0 == dwRefCnt)
	{
		delete this;
	}

	return dwRefCnt;
}

void CStaticCamera::FixMouse()
{
	POINT		ptMouse = {WINSIZEX >> 1, WINSIZEY >> 1};;

	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

POINT CStaticCamera::GetMouse(void)
{
	POINT pt;

	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	return pt;
}

void CStaticCamera::Zoom_InOut(void)
{
	if(((CPlayer*)m_pPlayer)->Get_State() == CPlayer::STATE_ATTACK
		|| ((CPlayer*)m_pPlayer)->Get_State() == CPlayer::STATE_SKILL)
	{
		if(!m_bFstLoop)
		{
			m_fSaveDist = m_fTargetDistance;
			m_bFstLoop = true;
		}
		m_fZoomTime += m_fTimeDelta;
		float fHalfDist = m_fSaveDist * 0.5f;
		float fCosTheta = cosf(D3DX_PI /** 2.f*/ * (m_fZoomTime / 0.5f));
		if(1.f <= (m_fZoomTime / 1.f))
		{
			m_fZoomTime = 0.f;
			fCosTheta = cosf(D3DX_PI /** 2.f */* (m_fZoomTime / 0.5f));
		}

		m_fTargetDistance = fHalfDist +  (fHalfDist * 0.5f) +(fHalfDist * 0.2f * fCosTheta);	
		
		//D3DXVec3Normalize(&m_vShakeAt, &m_vShakeAt);
	}

	if(((CPlayer*)m_pPlayer)->Get_State() == CPlayer::STATE_DMG)
	{
		int i = rand()%100;
		if(i > 4)
		{
			m_vShakeAt = _vec3(float(rand()%5 - 2), 0.f, float(rand()%3 - 1));
			m_vShakeAt *= 0.01f;
		}
		else
			m_vShakeAt = _vec3(0.f, 0.f, 0.f);
	}
	
	if( m_bFstLoop && ((CPlayer*)m_pPlayer)->Get_State() != CPlayer::STATE_ATTACK)
	{
		m_fTargetDistance = m_fSaveDist;
		m_bFstLoop = false;
		m_fZoomTime = 0.f;
		m_vShakeAt = _vec3(0.f, 0.f, 0.f);
	}
}


