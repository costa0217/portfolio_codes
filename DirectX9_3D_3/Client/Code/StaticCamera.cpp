#include "stdafx.h"
#include "StaticCamera.h"

#include "Include.h"
#include "Export_Function.h"
#include "Transform.h"

CStaticCamera::CStaticCamera(LPDIRECT3DDEVICE9 pDevice)
: Engine::CCamera(pDevice)
, m_pTimeMgr(Engine::Get_TimeMgr())
, m_pInfoSubject(Engine::Get_InfoSubject())
, m_fTargetDistance(0.f)
, m_fAngleRight(0.f)
, m_fAngleUp(0.f)
, m_fCamSpeed(0.f)
, m_bBombMove(false)
, m_bBombCreate(false)
, m_bEyeFix(false)
, m_bKeyCheck(true)
, m_bBombSecondMove(true)
, m_bGameStart(false)
{

}

CStaticCamera::~CStaticCamera(void)
{
	// 이거 한거야 리무브해야지 로고에서 스테이지로 깔끔하게 넘어간다. 질척거리지않고
	m_pInfoSubject->RemoveData(D3DTS_VIEW, &m_matView);
	m_pInfoSubject->RemoveData(D3DTS_PROJECTION, &m_matProj);
	m_pInfoSubject->RemoveData(D3DTS_ORTHOGONAL, &m_matOrtho);
}

void CStaticCamera::SetCameraTarget(const Engine::CTransform* pTargetInfo)
{
	m_pTargetInfo = pTargetInfo;
}

HRESULT CStaticCamera::Initialize(void)
{
	m_fTargetDistance = 10.f;
	m_fAngleRight = D3DXToRadian(30.f);
	m_fAngleUp	  = D3DXToRadian(0.f);
	m_fCamSpeed = 15.f;

	m_pInfoSubject->AddData(D3DTS_VIEW, &m_matView);
	m_pInfoSubject->AddData(D3DTS_PROJECTION, &m_matProj);
	m_pInfoSubject->AddData(D3DTS_ORTHOGONAL, &m_matOrtho);

	return S_OK;
}

int CStaticCamera::Update(void)
{
	
	if(m_bBombMove == false)
	{
		if(m_bKeyCheck)
			KeyCheck();
		TargetRenewal();
	}
	else if(m_bBombMove)
	{
		m_fTargetDistance -= 1.f;
		if(BombMove())
		{
			m_bBombMove = false;
			m_bBombCreate = true;
		}
	}

	m_pInfoSubject->Notify(D3DTS_VIEW);
	m_pInfoSubject->Notify(D3DTS_PROJECTION);
	m_pInfoSubject->Notify(D3DTS_ORTHOGONAL);

	return 0;
}

CStaticCamera* CStaticCamera::Create(LPDIRECT3DDEVICE9 pDevice
									 , const Engine::CTransform* pTargetInfo)
{
	CStaticCamera* pCamera = new CStaticCamera(pDevice);
	if(FAILED(pCamera->Initialize()))
		Engine::Safe_Delete(pCamera);

	pCamera->SetCameraTarget(pTargetInfo);
	pCamera->SetProjectionMatrix(D3DXToRadian(45.f), float(WINCX) / WINCY, 1.f, 2000.f);
	pCamera->SetOrthogonalMatrix(D3DXToRadian(45.f), float(WINCX) / WINCY, 1.f, 2000.f);

	return pCamera;
}

void CStaticCamera::KeyCheck(void)
{
	float		fTime = m_pTimeMgr->GetTime();
	
	if(GetAsyncKeyState(VK_LEFT) && GetAsyncKeyState(VK_RIGHT))
	{
		if(m_fAngleUp < 0.f)
		{
			m_fAngleUp += 0.2f * fTime;
			if(m_fAngleUp >= 0.f)
				m_fAngleUp = 0.f;
		}

		else if(m_fAngleUp > 0.f)
		{
			m_fAngleUp -= 0.2f * fTime;
			if(m_fAngleUp <= 0.f)
				m_fAngleUp = 0.f;
		}
	}
	if(GetAsyncKeyState(VK_LEFT))	
	{
		m_fAngleUp -= 0.2f * fTime;
		if(m_fAngleUp <= -0.4f)
			m_fAngleUp = -0.4f;
	}


	else if(GetAsyncKeyState(VK_RIGHT))	
	{
		m_fAngleUp += 0.2f * fTime;
		if(m_fAngleUp >= 0.4f)
			m_fAngleUp = 0.4f;
	}
	else
	{
		if(m_bGameStart)
		{
			if(m_fAngleUp < 0.f)
			{
				m_fAngleUp += 0.2f * fTime;
				if(m_fAngleUp >= 0.f)
					m_fAngleUp = 0.f;
			}

			else if(m_fAngleUp > 0.f)
			{
				m_fAngleUp -= 0.2f * fTime;
				if(m_fAngleUp <= 0.f)
					m_fAngleUp = 0.f;
			}
		}

	}

	/*if(GetAsyncKeyState('O'))
		m_fTargetDistance -= m_fCamSpeed * fTime;

	if(GetAsyncKeyState('P'))
		m_fTargetDistance += m_fCamSpeed * fTime;*/
}

void CStaticCamera::TargetRenewal(void)
{
	
	m_fAngleRight = D3DXToRadian(30.f);

	m_vEye = m_pTargetInfo->m_vDir * -1 * m_fTargetDistance;
	
	if(m_bEyeFix == false)
		m_vEye -= D3DXVECTOR3(0.f,m_pTargetInfo->m_vPos.y+5.f,10.f);

	D3DXVECTOR3		vRight;
	memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

	D3DXVECTOR3		vUp;
	memcpy(&vUp, &m_pTargetInfo->m_matWorld.m[1][0], sizeof(D3DXVECTOR3));


	m_fTargetDistance = 25.f +  m_pTargetInfo->m_vPos.y;

	D3DXMATRIX		matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, m_fAngleRight);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	D3DXMatrixRotationAxis(&matRotAxis, &vUp, m_fAngleUp);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	m_vAt = m_pTargetInfo->m_vPos - D3DXVECTOR3(m_pTargetInfo->m_fAngleNo[Engine::ANGLE_Y]*10.f,m_pTargetInfo->m_vPos.y/2.f,0.f);
	m_vEye += m_pTargetInfo->m_vPos;

	SetViewSpaceMatrix(&m_vEye, &m_vAt, &m_vUp);
}

bool CStaticCamera::BombMove()
{

	m_vEye = m_pTargetInfo->m_vDir * -1 * m_fTargetDistance;

	D3DXVECTOR3		vRight;
	memcpy(&vRight, &m_pTargetInfo->m_matWorld.m[0][0], sizeof(D3DXVECTOR3));

	D3DXVECTOR3		vUp;
	memcpy(&vUp, &m_pTargetInfo->m_matWorld.m[1][0], sizeof(D3DXVECTOR3));


	D3DXMATRIX		matRotAxis;
	D3DXMatrixRotationAxis(&matRotAxis, &vRight, m_fAngleRight);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	D3DXMatrixRotationAxis(&matRotAxis, &vUp, m_fAngleUp);
	D3DXVec3TransformNormal(&m_vEye, &m_vEye, &matRotAxis);

	m_vAt = m_pTargetInfo->m_vPos;
	m_vEye += m_pTargetInfo->m_vPos;

	SetViewSpaceMatrix(&m_vEye, &m_vAt, &m_vUp);

	if(m_fTargetDistance < 3)
	{
		if(m_bBombSecondMove)
		{
			m_bBombSecondMove = false;
			m_fTargetDistance = 200.f;
			m_fAngleUp = D3DXToRadian(50.f);
			m_fAngleRight = D3DXToRadian(30.f);
			return false;
		}
		else
		{
			m_bBombSecondMove = true;
			return true;
		}
		
	}
		
	return false;
}

