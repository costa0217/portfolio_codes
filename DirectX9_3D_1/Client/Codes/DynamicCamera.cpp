#include "stdafx.h"
#include "DynamicCamera.h"

#include "Export_Function.h"

#include "Player.h"
#include "ControlPointMgr.h"

#include "Line2D.h"

#include "StaticObject.h"
#include "CollisionMgr.h"

IMPLEMENT_SINGLETON(CDynamicCamera)

CDynamicCamera::CDynamicCamera(void)
: Engine::CCamera(Engine::Get_GraphicDev())
, m_bFixCamera(true)
, m_bFixEnable(false)
, m_bFovChange(false)
, m_fTarget_SC_EyeDistance(-10.f)
, m_fCurrent_SC_EyeDistance(-10.f)
, m_fPlayerTargetAngle(270.f)
, m_fPlayerCurrentAngle(270.f)
, m_fTarget_SC_YPos(5.f)
, m_fCurrent_SC_YPos(5.f)
, m_bVibrationStart(false)
, m_fVibrationY(0.f)
, m_fVibrationMaxTime(10.f)
, m_fVibrationTime(0.f)
, m_fVibrationYValue(1.f)
, m_fVibrationSpeed(2.f)
, m_pNaviCell(NULL)
, m_fTimeDelta(0.f)
, m_vPivotPos(0.f, 0.f, 0.f)
, m_vRayDir(0.f, 0.f, 0.f)
, m_bCollisionMoveCheck(false)
, m_vCollisionEyePos(0.f, 0.f, 0.f)
{
	Initialize();

	D3DXMatrixIdentity(&m_matWorld);

	ZeroMemory(m_fTargetAngle, sizeof(_float) * 2);
	ZeroMemory(m_fCurrentAngle, sizeof(_float) * 2);

	m_fTargetAngle[ANGLE_Y] = D3DXToRadian(180);
	m_fCurrentAngle[ANGLE_Y] = D3DXToRadian(180);

	m_fTargetDistance	= 10.f;
	m_fCurrentDistance	= 10.f;

	m_bMaxDistanceOn = false;
	m_fMaxDistance = 0.f;
	m_fOriginalDistance	= 10.f;
	m_iDistance = 0;

	m_bModeCheck					= false;
	m_bCameraModeInfo_Initialize	= true;
	m_iPlayModeIndex				= 0;
	m_iDYListIndex					= 0;//DynamicCameraList 인덱스
	m_iCurrentIndex					= 0;//카메라의 현재 리스트 위치
	m_iMaxIndex						= 0;//카메라 모드에서 가능한 리스트 위치

	m_pCameraTargetTransCom			= NULL;

}

CDynamicCamera::~CDynamicCamera(void)
{

}

HRESULT CDynamicCamera::Initialize(void)
{
	m_fCurrent_SC_YPos = 5.f;
	m_fCurrent_SC_EyeDistance	= -10.f;
	m_vAt	= D3DXVECTOR3(0.f, m_fCurrent_SC_YPos, 0.f);
	m_vEye	= D3DXVECTOR3(0.f, m_fCurrent_SC_YPos, m_fCurrent_SC_EyeDistance);
	m_vUp	= D3DXVECTOR3(0.f, 1.f, 0.f);



	Invalidate_ViewMatrix();

	m_fFovY = D3DXToRadian(60.f);
	m_fAspect = float(WINSIZEX) / WINSIZEY;
	m_fNear = 0.1f;
	m_fFar = 1000.f;
	Invalidate_ProjMatrix();

	return S_OK;
}

_int CDynamicCamera::Update(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;

	if(Engine::GetDIKeyState(DIK_ESCAPE) & 0x80)
	{
		if(!m_bFixEnable)
		{
			m_bFixCamera ^= true; //다르면 true로 변환 같으면 false로 대입 xor 연산자
		}
		m_bFixEnable = true;
	}
	else
		m_bFixEnable = false;

	if(CPlayer::GetInstance()->GetInGame())
	{
		if(m_bModeCheck)
		{
			ModeCamera(fTimeDelta);
		}
		else
		{
			if(m_bFixCamera)
			{
				FixMouse();	
				PlayerCamera(fTimeDelta);
				CameraInput(fTimeDelta);
				CameraVibration(fTimeDelta);

				m_vAfterPos	= m_vEye;

				Check_Navigation();
			}
			else
			{
				PlayerCamera(fTimeDelta);

				_matrix matRotX, matRotY;
				D3DXMatrixRotationX(&matRotX, m_fCurrentAngle[ANGLE_X]);
				D3DXMatrixRotationY(&matRotY, m_fCurrentAngle[ANGLE_Y]);
				m_matWorld = matRotX * matRotY;

				_vec3 vTempEyePos;
				D3DXVec3TransformCoord(&vTempEyePos, &_vec3(0.f, 0.f, m_fCurrentDistance), &m_matWorld);

				m_vEye = m_vAt + vTempEyePos;

				CameraVibration(fTimeDelta);
				m_vAfterPos	= m_vEye;

				Check_Navigation();
			}
		}
	}
	else
	{
		if(StartingScreenCheck())
		{
			StartingScreenCamera(fTimeDelta);
		}
	}

	Engine::CCamera::Update(fTimeDelta);

	return 0;
}

void CDynamicCamera::FixMouse()
{
	POINT		ptMouse = {1600 >> 1, 900 >> 1};
	ClientToScreen(g_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

_bool CDynamicCamera::PlayerAnimationCheck(Player::PLAYERANIMATION ePlayerAnimation)
{
	if(CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() == ePlayerAnimation && !CPlayer::GetInstance()->Get_MeshCom()->Check_EndPeriodMulTime(0.9f))
		return true;
	else
		return false;	
}

void CDynamicCamera::CameraFovChange(void)
{
	if(PlayerAnimationCheck(Player::battle_attack_break_l)
		|| PlayerAnimationCheck(Player::battle_attack_break_r)
		|| PlayerAnimationCheck(Player::battle_attack_break_speedymove_l)
		|| PlayerAnimationCheck(Player::battle_attack_break_speedymove_r)
		|| PlayerAnimationCheck(Player::battle_attack_strong_01)
		|| PlayerAnimationCheck(Player::battle_attack_strong_02_a)
		|| PlayerAnimationCheck(Player::battle_attack_strong_03)
		|| PlayerAnimationCheck(Player::battle_attack_strong_04_a)
		|| PlayerAnimationCheck(Player::battle_attack_strong_04_b)
		|| PlayerAnimationCheck(Player::battle_attack_strong_run_sprint))
	{
		m_bFovChange = true;
	}
	else
		m_bFovChange = false;
}

void CDynamicCamera::PlayerCamera(const _float& fTimeDelta)
{
	CameraFovChange();

	if(m_bFovChange)
	{
		m_fFovY -= D3DXToRadian(50.f * fTimeDelta);
		if(m_fFovY <= D3DXToRadian(45.f))
			m_fFovY = D3DXToRadian(45.f);
	}
	else
	{
		m_fFovY += D3DXToRadian(50.f * fTimeDelta);
		if(m_fFovY >= D3DXToRadian(55.f))
			m_fFovY = D3DXToRadian(55.f);
	}

	D3DXVec3TransformCoord(&m_vAt, &_vec3(0.f, 0.f, 0.f), &(*CPlayer::GetInstance()->Get_MeshCom()->Get_FrameMatrix("ValveBiped_Bip01_Spine2") * CPlayer::GetInstance()->Get_TransCom()->m_matWorld));
}

void CDynamicCamera::CameraInput(const _float& fTimeDelta)
{
	m_iDistance = 0;

	if(m_iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_X))
	{
		m_fTargetAngle[ANGLE_Y] += D3DXToRadian(m_iDistance * fTimeDelta * 10.f);
	}

	if(m_iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_Y))
	{
		if(D3DXToDegree(m_fTargetAngle[ANGLE_X]) >= -45.f && D3DXToDegree(m_fTargetAngle[ANGLE_X]) <= 45.f)
			m_fTargetAngle[ANGLE_X] -= D3DXToRadian( m_iDistance * fTimeDelta * 10.f);

		if(m_iDistance < 0)
		{
			m_bMaxDistanceOn = true;
		}
	}

	if(m_iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_Z))
	{
		m_fTargetDistance -= m_iDistance * fTimeDelta * 2.0f;
		if(D3DXToDegree(m_fTargetAngle[ANGLE_X]) <= 0.f)
			m_fOriginalDistance = m_fTargetDistance;

		if(m_iDistance > 0)
		{
			m_bMaxDistanceOn = false;
			m_fOriginalDistance = m_fTargetDistance;
		}
	}

	CameraInputLerp(fTimeDelta);
}

void CDynamicCamera::CameraInputLerp(const _float& fTimeDelta)
{
	//X 각도 예외처리
	if(m_fTargetAngle[ANGLE_X] != m_fCurrentAngle[ANGLE_X])
	{
		m_fCurrentAngle[ANGLE_X] += (m_fTargetAngle[ANGLE_X] - m_fCurrentAngle[ANGLE_X]) * fTimeDelta * 5.f;
	}

	if(D3DXToDegree(m_fTargetAngle[ANGLE_X]) <= -45.f)
		m_fTargetAngle[ANGLE_X] = D3DXToRadian(-45.f);
	else if(D3DXToDegree(m_fTargetAngle[ANGLE_X]) >= 45.f)
		m_fTargetAngle[ANGLE_X] = D3DXToRadian(45.f);

	if(D3DXToDegree(m_fTargetAngle[ANGLE_X]) > 0.f)
		m_fMaxDistance = 0.4f * (D3DXToDegree(m_fTargetAngle[ANGLE_X]));

	//Y 각도 예외처리
	if(m_fTargetAngle[ANGLE_Y] != m_fCurrentAngle[ANGLE_Y])
	{
		m_fCurrentAngle[ANGLE_Y] += (m_fTargetAngle[ANGLE_Y] - m_fCurrentAngle[ANGLE_Y]) * fTimeDelta * 5.f;
	}

	//거리 예외처리
	if(m_fTargetDistance <= 5.f)
		m_fTargetDistance = 5.f;
	else if(m_fTargetDistance >= (20.f - m_fMaxDistance))
	{
		m_fTargetDistance = (20.f - m_fMaxDistance);
	}
	else if(m_iDistance < 0)
	{
		m_fOriginalDistance = m_fTargetDistance;
	}

	if(m_fTargetDistance != m_fCurrentDistance)
	{
		m_fCurrentDistance += (m_fTargetDistance - m_fCurrentDistance) * fTimeDelta * 5.f;
	}

	if(abs((m_fTargetDistance - m_fCurrentDistance)) > 5.f)
	{
		if(!(m_fTargetDistance >= (20.f - m_fMaxDistance)))
			Engine::CRenderer::m_bRadialBlurRender = true;
	}

	if(m_bMaxDistanceOn)
		m_fTargetDistance = m_fOriginalDistance;

	_matrix matRotX, matRotY;
	D3DXMatrixRotationX(&matRotX, m_fCurrentAngle[ANGLE_X]);
	D3DXMatrixRotationY(&matRotY, m_fCurrentAngle[ANGLE_Y]);
	m_matWorld = matRotX * matRotY;

	_vec3 vTempEyePos;
	D3DXVec3TransformCoord(&vTempEyePos, &_vec3(0.f, 0.f, m_fCurrentDistance), &m_matWorld);

	m_vEye = m_vAt + vTempEyePos;
}

void CDynamicCamera::ControlPointMgrInfoLoad(_int iNumber, TCHAR* pPath)
{
	HANDLE	hFile = CreateFile(pPath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD		dwByte;

	//DynamicCameraMgr에 저장해야 하는 ControlPointMgr의 개수를 읽어온다.
	_int iControlPointMgrSize = 0;
	ReadFile(hFile, &iControlPointMgrSize, sizeof(_int), &dwByte, NULL);

	//로드해야 하는 ControlPointMgr의 수 만큼 돌면서 정보를 로드한다.
	for(int iCPMSize = 0; iCPMSize < iControlPointMgrSize; ++iCPMSize)
	{
		//CControlPointMgr에 있는 list<ControlPoint*>의Size와CameraModeType, StandardType에 대한 정보를 읽어온다.
		_int iControlPointSize	= 0;
		_int iCamerModeType		= 0;
		_int iStandardType		= 0;

		ReadFile(hFile, &iControlPointSize, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, &iCamerModeType, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, &iStandardType, sizeof(_int), &dwByte, NULL);

		//읽어온 정보를 토대로 CControlPointMgr을 만들고 푸쉬한다.
		CControlPointMgr* pControlPointMgr = CControlPointMgr::Create(CameraModeType(iCamerModeType), StandardType(iStandardType));
		m_ControlPointMgrVector[iNumber].push_back(pControlPointMgr);

		//동적 할당한 pControlPointMgr의 list<ControlPoint*>에 넣을 정보를 읽어온다.
		for(int iCPSize = 0; iCPSize < iControlPointSize; ++iCPSize)
		{
			//읽어온 정보를 토대로 생성하고 Push한다.
			ControlPointInfo CPInfo;
			ReadFile(hFile, &CPInfo, sizeof(ControlPointInfo), &dwByte, NULL);
			m_ControlPointMgrVector[iNumber][iCPMSize]->GetpControlPointVector()->push_back(CPInfo);
		}
	}

	CloseHandle(hFile);
}

void CDynamicCamera::CameraModePlayer(_int iPlayModeIndex)
{
	Engine::CRenderer::m_bRadialBlurRender = false;
	m_bModeCheck		= true;
	m_bCameraModeInfo_Initialize = true;

	m_iPlayModeIndex	= iPlayModeIndex;
	m_iDYListIndex		= 0;
	m_iCurrentIndex		= 0;
}

void CDynamicCamera::ModeCamera(const _float& fTimeDelta)
{
	if(m_bCameraModeInfo_Initialize)
	{
		if(m_ControlPointMgrVector[m_iPlayModeIndex].empty() || m_ControlPointMgrVector[m_iPlayModeIndex][m_iDYListIndex]->GetpControlPointVector()->size() < 3)
		{
			m_bModeCheck	= false;
			return;
		}

		//카메라 모드가 On되어 들어왔을때 맨 처음 카메라에 대한 정보를 초기화 시켜주는 곳이다.
		//여기서 카메라의 모드와 기준 위치에 따라 초기화를 해준다.
		m_bCameraModeInfo_Initialize = false;	
		m_pControlPointMgr	= m_ControlPointMgrVector[m_iPlayModeIndex][m_iDYListIndex];

		m_iMaxIndex			= m_pControlPointMgr->GetpControlPointVector()->size();
		m_iCurrentIndex		= 0;

		m_fAtDistance		= D3DXVec3Length(&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vAtPos - m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vAtPos));
		m_fAtLefpTime		= 0.f;

		m_fEyeDistance		= D3DXVec3Length(&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vEyePos - m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vEyePos));
		m_fEyeLefpTime		= 0.f;
		m_fDistance = m_fAtDistance > m_fEyeDistance ? m_fAtDistance : m_fEyeDistance; //거리가 가장 먼것을 기준으로 이동하게 한다.
	}

	if(m_pControlPointMgr->GetCameraMode() == FREEMODE)
	{
		CameraModeMove(fTimeDelta, 0);
	}
	else if(m_pControlPointMgr->GetCameraMode() == TARGETMODE)
	{
		if(m_pControlPointMgr->GetStandardType() == LASTCP)
		{
			CameraModeMove(fTimeDelta, 1);
		}
		else if(m_pControlPointMgr->GetStandardType() == SPECIFIC)
		{
			CameraModeMove(fTimeDelta, 3);
		}
	}
	else if(m_pControlPointMgr->GetCameraMode() == OBSERVERMODE)
	{
		if(m_pControlPointMgr->GetStandardType() == LASTCP)
		{
			CameraModeMove(fTimeDelta, 2);
		}
		else if(m_pControlPointMgr->GetStandardType() == SPECIFIC)
		{
			CameraModeMove(fTimeDelta, 4);
		}
	}
}

void CDynamicCamera::CameraModeMove(const _float& fTimeDelta, int iNumber)
{
	if(m_fAtLefpTime >= 1.f && m_fEyeLefpTime >= 1.f)
	{
		++m_iCurrentIndex;

		if(m_iCurrentIndex == (m_iMaxIndex - 1))
		{
			if(m_iDYListIndex + 1 < (int)m_ControlPointMgrVector[m_iPlayModeIndex].size())
			{
				m_bCameraModeInfo_Initialize = true;
				m_iDYListIndex += 1;
				return;
			}

			m_bModeCheck	= false;
			return;
		}

		m_fAtDistance		= D3DXVec3Length(&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vAtPos - m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vAtPos));
		m_fAtLefpTime		= 0.f;

		m_fEyeDistance		= D3DXVec3Length(&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vEyePos - m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vEyePos));
		m_fEyeLefpTime		= 0.f;

		m_fDistance = m_fAtDistance > m_fEyeDistance ? m_fAtDistance : m_fEyeDistance;
	}

	if(iNumber == 0 || iNumber == 1 || iNumber == 2)
		CameraLinearInterpolation(fTimeDelta);
	else if(iNumber == 3)
		CameraLinearInterpolation_TargetMode(fTimeDelta);
	else if(iNumber == 4)
		CameraLinearInterpolation_ObserverMode(fTimeDelta);

	m_vAt	= m_vCurrentAt;
	m_vEye	= m_vCurrentEye;
}

void CDynamicCamera::CameraLinearInterpolation(const _float& fTimeDelta)
{
#pragma  region D3DXVec3CatmullRom 방법

	m_fCamModeSpeed	= m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).fSpeed + (m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).fSpeed - m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).fSpeed) * m_fAtLefpTime;
	m_fAtLefpTime 	+= (m_fCamModeSpeed * fTimeDelta) / m_fDistance; //속도 * 시간 / 거리
	m_fEyeLefpTime	+= (m_fCamModeSpeed * fTimeDelta) / m_fDistance; //속도 * 시간 / 거리

	if(m_iCurrentIndex == 0)//처음 인덱스
	{
		D3DXVec3CatmullRom(&m_vCurrentAt,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 2).vAtPos,
			m_fAtLefpTime);

		D3DXVec3CatmullRom(&m_vCurrentEye,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 2).vEyePos,
			m_fEyeLefpTime);
	}
	else if(m_iCurrentIndex == m_iMaxIndex - 2)//마지막에서 2번째
	{
		D3DXVec3CatmullRom(&m_vCurrentAt,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex - 1).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vAtPos,
			m_fAtLefpTime);

		D3DXVec3CatmullRom(&m_vCurrentEye,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex - 1).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vEyePos,
			m_fEyeLefpTime);
	}
	else
	{
		D3DXVec3CatmullRom(&m_vCurrentAt,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex - 1).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 2).vAtPos,
			m_fAtLefpTime);

		D3DXVec3CatmullRom(&m_vCurrentEye,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex - 1).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 2).vEyePos,
			m_fEyeLefpTime);		
	}
#pragma endregion
}

void CDynamicCamera::CameraLinearInterpolation_TargetMode(const _float& fTimeDelta)
{
#pragma  region D3DXVec3CatmullRom 방법

	m_fCamModeSpeed	= m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).fSpeed + (m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).fSpeed - m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).fSpeed) * m_fAtLefpTime;
	m_fAtLefpTime 	+= (m_fCamModeSpeed * fTimeDelta) / m_fDistance; //속도 * 시간 / 거리
	m_fEyeLefpTime	+= (m_fCamModeSpeed * fTimeDelta) / m_fDistance; //속도 * 시간 / 거리

	_vec3 vDifferenceDistance = m_pCameraTargetTransCom->m_vPosition - StandardPos;

	if(m_iCurrentIndex == 0)//처음 인덱스
	{
		D3DXVec3CatmullRom(&m_vCurrentAt,
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vAtPos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vAtPos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vAtPos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 2).vAtPos + vDifferenceDistance),
			m_fAtLefpTime);

		D3DXVec3CatmullRom(&m_vCurrentEye,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 2).vEyePos,
			m_fEyeLefpTime);
	}
	else if(m_iCurrentIndex == m_iMaxIndex - 2)//마지막에서 2번째
	{
		D3DXVec3CatmullRom(&m_vCurrentAt,
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex - 1).vAtPos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vAtPos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vAtPos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vAtPos + vDifferenceDistance),
			m_fAtLefpTime);

		D3DXVec3CatmullRom(&m_vCurrentEye,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex - 1).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vEyePos,
			m_fEyeLefpTime);
	}
	else
	{
		D3DXVec3CatmullRom(&m_vCurrentAt,
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex - 1).vAtPos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vAtPos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vAtPos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 2).vAtPos + vDifferenceDistance),
			m_fAtLefpTime);

		D3DXVec3CatmullRom(&m_vCurrentEye,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex - 1).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vEyePos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 2).vEyePos,
			m_fEyeLefpTime);		
	}

	_matrix matRotationY;
	D3DXMatrixIdentity(&matRotationY);
	D3DXMatrixRotationY(&matRotationY, m_pCameraTargetTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);

	m_vCurrentEye -= TargetAtPos;
	D3DXVec3TransformCoord(&m_vCurrentEye, &m_vCurrentEye, &matRotationY);
	m_vCurrentEye += TargetAtPos;
	m_vCurrentEye += vDifferenceDistance;
#pragma endregion
}

void CDynamicCamera::CameraLinearInterpolation_ObserverMode(const _float& fTimeDelta)
{
#pragma  region D3DXVec3CatmullRom 방법

	m_fCamModeSpeed	= m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).fSpeed + (m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).fSpeed - m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).fSpeed) * m_fAtLefpTime;
	m_fAtLefpTime 	+= (m_fCamModeSpeed * fTimeDelta) / m_fDistance; //속도 * 시간 / 거리
	m_fEyeLefpTime	+= (m_fCamModeSpeed * fTimeDelta) / m_fDistance; //속도 * 시간 / 거리

	_vec3 vDifferenceDistance = m_pCameraTargetTransCom->m_vPosition - StandardPos;

	if(m_iCurrentIndex == 0)//처음 인덱스
	{
		D3DXVec3CatmullRom(&m_vCurrentAt,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 2).vAtPos,
			m_fAtLefpTime);

		D3DXVec3CatmullRom(&m_vCurrentEye,
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vEyePos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vEyePos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vEyePos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 2).vEyePos + vDifferenceDistance),
			m_fEyeLefpTime);
	}
	else if(m_iCurrentIndex == m_iMaxIndex - 2)//마지막에서 2번째
	{
		D3DXVec3CatmullRom(&m_vCurrentAt,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex - 1).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vAtPos,
			m_fAtLefpTime);

		D3DXVec3CatmullRom(&m_vCurrentEye,
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex - 1).vEyePos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vEyePos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vEyePos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vEyePos + vDifferenceDistance),
			m_fEyeLefpTime);
	}
	else
	{
		D3DXVec3CatmullRom(&m_vCurrentAt,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex - 1).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vAtPos,
			&m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 2).vAtPos,
			m_fAtLefpTime);

		D3DXVec3CatmullRom(&m_vCurrentEye,
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex - 1).vEyePos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vEyePos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vEyePos + vDifferenceDistance),
			&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 2).vEyePos + vDifferenceDistance),
			m_fEyeLefpTime);		
	}

	_matrix matRotationY;
	D3DXMatrixIdentity(&matRotationY);
	D3DXMatrixRotationY(&matRotationY, m_pCameraTargetTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);

	m_vCurrentAt -= ObserverEyePos;
	D3DXVec3TransformCoord(&m_vCurrentAt, &m_vCurrentAt, &matRotationY);
	m_vCurrentAt += ObserverEyePos;
	m_vCurrentAt += vDifferenceDistance;
#pragma endregion
}

_bool CDynamicCamera::StartingScreenCheck(void)
{
	POINT	ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	{
		//의상 선택
		RECT			rcUI;
		SetRect(&rcUI, 10, 10, 275, 440);

		if(PtInRect(&rcUI, ptMouse))
		{
			return false;
		}
	}

	{
		//염색 색상 선정
		RECT			rcUI;
		SetRect(&rcUI, 10, 470, 460, 890);

		if(PtInRect(&rcUI, ptMouse))
		{
			return false;
		}
	}

	{
		//염색 부위 선정
		RECT			rcUI;
		SetRect(&rcUI, 1270, 10, 1580, 470);

		if(PtInRect(&rcUI, ptMouse))
		{
			return false;
		}
	}

	return true;
}

void CDynamicCamera::StartingScreenCamera(const _float& fTimeDelta)
{
	m_iDistance = 0;

	if(Engine::GetDIMouseState(Engine::CInput::DIM_LBUTTON) || Engine::GetDIMouseState(Engine::CInput::DIM_RBUTTON))
	{
		if(m_iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_X))
		{
			m_fPlayerTargetAngle += m_iDistance * fTimeDelta * -40.f;
		}

		if(m_iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_Y))
		{
			m_fTarget_SC_YPos += m_iDistance * fTimeDelta / 2.f;

			if(m_fTarget_SC_YPos <= 2.5f)
				m_fTarget_SC_YPos = 2.5f;

			if(m_fTarget_SC_YPos >= 7.5f)
				m_fTarget_SC_YPos = 7.5f;
		}
	}

	if(m_iDistance = Engine::GetDIMouseMove(Engine::CInput::DIM_Z))
	{
		m_fTarget_SC_EyeDistance += m_iDistance * fTimeDelta;

		if(m_fTarget_SC_EyeDistance <= -10.f)
			m_fTarget_SC_EyeDistance = -10.f;

		if(m_fTarget_SC_EyeDistance >= -2.5f)
			m_fTarget_SC_EyeDistance = -2.5f;
	}

	//플레이어 Y 각도
	if(m_fPlayerTargetAngle != m_fPlayerCurrentAngle)
	{
		m_fPlayerCurrentAngle += (m_fPlayerTargetAngle - m_fPlayerCurrentAngle) * fTimeDelta * 6.0f;
	}

	//Y값
	if(m_fTarget_SC_YPos != m_fCurrent_SC_YPos)
	{
		m_fCurrent_SC_YPos += (m_fTarget_SC_YPos - m_fCurrent_SC_YPos) * fTimeDelta * 6.0f;
	}

	//거리
	if(m_fTarget_SC_EyeDistance != m_fCurrent_SC_EyeDistance)
	{
		m_fCurrent_SC_EyeDistance += (m_fTarget_SC_EyeDistance - m_fCurrent_SC_EyeDistance) * fTimeDelta * 6.0f;
	}

	if(abs((m_fTarget_SC_EyeDistance - m_fCurrent_SC_EyeDistance)) > 3.f)
	{
		Engine::CRenderer::m_bRadialBlurRender = true;
	}
	else
	{
		Engine::CRenderer::m_bRadialBlurRender = false;
	}

	CPlayer::GetInstance()->SetAngleY(D3DXToRadian(m_fPlayerCurrentAngle));
	m_vAt	= D3DXVECTOR3(0.f, m_fCurrent_SC_YPos, 0.f);
	m_vEye	= D3DXVECTOR3(0.f, m_fCurrent_SC_YPos, m_fCurrent_SC_EyeDistance);
}

void CDynamicCamera::CameraStartVibration(_float fYValue, _float fSpeed)
{
	m_bVibrationStart	= true;		//카메라 Update에서 CameraVibration을 호출 하여 true일 때 진동을 하도록 함
	m_fVibrationTime	= 0.f;		//진동이 끝나기전에 CameraStartVibration을 호출 할때를 위해 초기화한다.
	m_fVibrationYValue	= fYValue;	//매개변수로 진동의 폭을 조절한다.
	m_fVibrationSpeed	= fSpeed;	//매개변수로 진동시간(10/m_fVibrationSpeed)을 조절한다.
}

void CDynamicCamera::CameraVibration(const _float& fTimeDelta)
{
	if(m_bVibrationStart)
	{
		//시간의 흐름에 따라 m_fVibrationY의 값을 바꾼다.
		m_fVibrationY		= sin(m_fVibrationTime * 10.f) * pow(0.5f, m_fVibrationTime) * m_fVibrationYValue;
		m_vEye.y			+= m_fVibrationY;	//매 프레임 마다 최종 m_vEye의 위치에 진동값을 더해준다.
		m_fVibrationTime	+= fTimeDelta * m_fVibrationSpeed;
	}

	if(m_fVibrationTime >= m_fVibrationMaxTime) //최대진동시간은 10초인데 10초보다 크면
	{
		m_fVibrationTime = 0.f;		//다음 진동을 위해 초기화를 해주고
		m_bVibrationStart = false;	//진동을 멈춘다.
	}
}


void CDynamicCamera::NavigationInitialize(void)
{
	if(NULL == m_pNaviCell)
	{
		list<Engine::CNaviCell*>::iterator iter = Engine::Get_CameraNaviMesh()->Get_listNaviCell()->begin();
		list<Engine::CNaviCell*>::iterator iter_end = Engine::Get_CameraNaviMesh()->Get_listNaviCell()->end();
		for(; iter != iter_end; ++iter)	//iter를 돌면서 조건에 만족하는 네비게이션 메쉬의 주소를 찾는다.
		{
			_bool bInside = (*iter)->Inside_Point(&m_vAfterPos);
			if(true == bInside)
			{
				m_pNaviCell = (*iter);
				break;	//해당하는 주소를 찾으면 break로 탈출한다.
			}
		}
	}
}

void CDynamicCamera::Check_Navigation(void)
{
	NavigationInitialize();	//m_pNaviCell이 NULL일때 네비게이션의 주소를 찾는다.
	_bool bInside = false;	
	
	if(m_pNaviCell != NULL)
		bInside = m_pNaviCell->CheckInFindNeighbor(&m_vAfterPos, &m_pNaviCell);

	if(true == bInside)	//움직인 후의 위치가 네비게이션 메쉬에 포함되어 있으면 카메라의 위치 변동이 없다.
	{
		m_bCollisionMoveCheck = false;
	}
	else				//움직인 후의 위치가 네비게이션 메쉬에 포함이 안되어 있으면 예외처리를 한다.
	{
		m_pNaviCell = NULL;
		if(!m_bCollisionMoveCheck)	//처음 들어왔을때 원래 카메라가 이동해야 하는 위치의 값을 받는다.
		{
			m_bCollisionMoveCheck	= true;
			m_vCollisionEyePos		= m_vAfterPos;
		}

		_vec3 vSearchPos = m_vEye;

		list<CStaticObject*>::iterator iter		= CCollisionMgr::GetInstance()->Get_StaticObjectDefaultList()->begin();
		list<CStaticObject*>::iterator iterEnd	= CCollisionMgr::GetInstance()->Get_StaticObjectDefaultList()->end();

		for( ; iter != iterEnd; ++iter )
		{
			//Sphere - Sphere 충돌이 되면 서로 가까이 있다는 말이다.
			if(Engine::CCollision::GetInstance()->Collision_Sphere_Sphere((*iter)->Get_WorldCenter(), (*iter)->Get_WorldRadius(), m_vCollisionEyePos, 20.f))
			{
				_vec3 vCheckPos(0.f, 0.f, 0.f);
				
				//D3DXIntersect함수를 이용하여 카메라의 At 에서 Eye로의 광선이 해당 메쉬와 충돌을 하면 위치값을 구한다.
				if(PickingObjectPos((*iter)->Get_MeshCom()->GetRefereMesh(), &(*iter)->Get_TransCom()->m_matWorld, &vCheckPos))
				{
					//충돌 되는 점이 있으면 해당 점으로 위치를 바꿔준다.
					_float fSearchPosDistance	= D3DXVec3Length(&(vSearchPos - m_vAt));
					_float fCheckPosDistance	= D3DXVec3Length(&(vCheckPos - m_vAt));

					//거리가 가장 가까운 충돌 지점을 검사하여
					if(fSearchPosDistance > fCheckPosDistance)
					{
						//현재꺼보다 더 가까우면 갱신해준다.
						vSearchPos = vCheckPos;
					}
				}
			}
		}

		_float fGap = D3DXVec3Length(&(vSearchPos - m_vCollisionEyePos));

		if(fGap >= 1.0f)
		{
			_vec3 vDir = vSearchPos - m_vCollisionEyePos; //충돌 지점 - 현재 위치
			D3DXVec3Normalize(&vDir, &vDir);	//방향을 구한다.
			
			//거리에 따라 보간속도를 다르게 한다.
			m_vCollisionEyePos += vDir * max(fGap, 3.5f) * 10.f * m_fTimeDelta;

			if(fGap > 3.f)	//충돌 지점과 원래 거리사이가 3보다 크면 라디안 블러를 켠다.
				Engine::CRenderer::m_bRadialBlurRender = true;
		}
		else
		{
			m_vCollisionEyePos = vSearchPos; //거리차이가 1보다 작으면 바로 이동을한다.
		}

		m_vEye = m_vCollisionEyePos;	//눈의 위치를 갱신한다.
	}
}

void CDynamicCamera::AtPos_Translation_ViewSpace(void)
{
	m_vPivotPos = m_vAt;
	m_vRayDir	= m_vEye - m_vAt;
	D3DXVec3Normalize(&m_vRayDir, &m_vRayDir);

}

void CDynamicCamera::Translation_Local(const D3DXMATRIX* pWorld)
{
	D3DXMATRIX	matWorldInverse;

	D3DXMatrixInverse(&matWorldInverse, NULL, pWorld);
	D3DXVec3TransformCoord(&m_vPivotPos, &m_vPivotPos, &matWorldInverse);
	D3DXVec3TransformNormal(&m_vRayDir, &m_vRayDir, &matWorldInverse);
}

_bool CDynamicCamera::PickingObjectPos(LPD3DXMESH& pMehs, D3DXMATRIX* pMat, _vec3* SearchPos)
{
	AtPos_Translation_ViewSpace();

	Translation_Local(pMat);

	BOOL	bHit;
	DWORD	pFaceIndex;
	float	fU, fV, fDist;
	LPD3DXBUFFER ppAllHits;
	DWORD	pCountOfHits;

	D3DXIntersect(pMehs, &m_vPivotPos, &m_vRayDir, &bHit, &pFaceIndex, &fU, &fV, &fDist, &ppAllHits, &pCountOfHits);

	//좌표값은
	//m_vPivotPos + (m_vRayDir * fDist) //=> 메쉬와 마우스과 충돌된 로컬의 좌표값이 나온다.

	bool j = (bHit != 0); //bHit 가 false이면 0이되어 false를 반환

	if(j)
	{
		D3DXVec3TransformCoord(SearchPos, &(m_vPivotPos + (m_vRayDir * fDist)), pMat);
	}

	return j;
}

_ulong CDynamicCamera::Release(void)
{
	_ulong dwRefCnt = Engine::CCamera::Release();

	if(0 == dwRefCnt)
	{
		for(int i = 0; i < 3; ++i)
		{
			for_each(m_ControlPointMgrVector[i].begin(), m_ControlPointMgrVector[i].end(), CRelease_Obj());
			m_ControlPointMgrVector[i].clear();
		}

		delete this;
	}

	return dwRefCnt;
}