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
	m_iDYListIndex					= 0;//DynamicCameraList �ε���
	m_iCurrentIndex					= 0;//ī�޶��� ���� ����Ʈ ��ġ
	m_iMaxIndex						= 0;//ī�޶� ��忡�� ������ ����Ʈ ��ġ

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
			m_bFixCamera ^= true; //�ٸ��� true�� ��ȯ ������ false�� ���� xor ������
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
	//X ���� ����ó��
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

	//Y ���� ����ó��
	if(m_fTargetAngle[ANGLE_Y] != m_fCurrentAngle[ANGLE_Y])
	{
		m_fCurrentAngle[ANGLE_Y] += (m_fTargetAngle[ANGLE_Y] - m_fCurrentAngle[ANGLE_Y]) * fTimeDelta * 5.f;
	}

	//�Ÿ� ����ó��
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

	//DynamicCameraMgr�� �����ؾ� �ϴ� ControlPointMgr�� ������ �о�´�.
	_int iControlPointMgrSize = 0;
	ReadFile(hFile, &iControlPointMgrSize, sizeof(_int), &dwByte, NULL);

	//�ε��ؾ� �ϴ� ControlPointMgr�� �� ��ŭ ���鼭 ������ �ε��Ѵ�.
	for(int iCPMSize = 0; iCPMSize < iControlPointMgrSize; ++iCPMSize)
	{
		//CControlPointMgr�� �ִ� list<ControlPoint*>��Size��CameraModeType, StandardType�� ���� ������ �о�´�.
		_int iControlPointSize	= 0;
		_int iCamerModeType		= 0;
		_int iStandardType		= 0;

		ReadFile(hFile, &iControlPointSize, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, &iCamerModeType, sizeof(_int), &dwByte, NULL);
		ReadFile(hFile, &iStandardType, sizeof(_int), &dwByte, NULL);

		//�о�� ������ ���� CControlPointMgr�� ����� Ǫ���Ѵ�.
		CControlPointMgr* pControlPointMgr = CControlPointMgr::Create(CameraModeType(iCamerModeType), StandardType(iStandardType));
		m_ControlPointMgrVector[iNumber].push_back(pControlPointMgr);

		//���� �Ҵ��� pControlPointMgr�� list<ControlPoint*>�� ���� ������ �о�´�.
		for(int iCPSize = 0; iCPSize < iControlPointSize; ++iCPSize)
		{
			//�о�� ������ ���� �����ϰ� Push�Ѵ�.
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

		//ī�޶� ��尡 On�Ǿ� �������� �� ó�� ī�޶� ���� ������ �ʱ�ȭ �����ִ� ���̴�.
		//���⼭ ī�޶��� ���� ���� ��ġ�� ���� �ʱ�ȭ�� ���ش�.
		m_bCameraModeInfo_Initialize = false;	
		m_pControlPointMgr	= m_ControlPointMgrVector[m_iPlayModeIndex][m_iDYListIndex];

		m_iMaxIndex			= m_pControlPointMgr->GetpControlPointVector()->size();
		m_iCurrentIndex		= 0;

		m_fAtDistance		= D3DXVec3Length(&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vAtPos - m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vAtPos));
		m_fAtLefpTime		= 0.f;

		m_fEyeDistance		= D3DXVec3Length(&(m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).vEyePos - m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).vEyePos));
		m_fEyeLefpTime		= 0.f;
		m_fDistance = m_fAtDistance > m_fEyeDistance ? m_fAtDistance : m_fEyeDistance; //�Ÿ��� ���� �հ��� �������� �̵��ϰ� �Ѵ�.
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
#pragma  region D3DXVec3CatmullRom ���

	m_fCamModeSpeed	= m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).fSpeed + (m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).fSpeed - m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).fSpeed) * m_fAtLefpTime;
	m_fAtLefpTime 	+= (m_fCamModeSpeed * fTimeDelta) / m_fDistance; //�ӵ� * �ð� / �Ÿ�
	m_fEyeLefpTime	+= (m_fCamModeSpeed * fTimeDelta) / m_fDistance; //�ӵ� * �ð� / �Ÿ�

	if(m_iCurrentIndex == 0)//ó�� �ε���
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
	else if(m_iCurrentIndex == m_iMaxIndex - 2)//���������� 2��°
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
#pragma  region D3DXVec3CatmullRom ���

	m_fCamModeSpeed	= m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).fSpeed + (m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).fSpeed - m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).fSpeed) * m_fAtLefpTime;
	m_fAtLefpTime 	+= (m_fCamModeSpeed * fTimeDelta) / m_fDistance; //�ӵ� * �ð� / �Ÿ�
	m_fEyeLefpTime	+= (m_fCamModeSpeed * fTimeDelta) / m_fDistance; //�ӵ� * �ð� / �Ÿ�

	_vec3 vDifferenceDistance = m_pCameraTargetTransCom->m_vPosition - StandardPos;

	if(m_iCurrentIndex == 0)//ó�� �ε���
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
	else if(m_iCurrentIndex == m_iMaxIndex - 2)//���������� 2��°
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
#pragma  region D3DXVec3CatmullRom ���

	m_fCamModeSpeed	= m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).fSpeed + (m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex + 1).fSpeed - m_pControlPointMgr->GetControlPointInfo(m_iCurrentIndex).fSpeed) * m_fAtLefpTime;
	m_fAtLefpTime 	+= (m_fCamModeSpeed * fTimeDelta) / m_fDistance; //�ӵ� * �ð� / �Ÿ�
	m_fEyeLefpTime	+= (m_fCamModeSpeed * fTimeDelta) / m_fDistance; //�ӵ� * �ð� / �Ÿ�

	_vec3 vDifferenceDistance = m_pCameraTargetTransCom->m_vPosition - StandardPos;

	if(m_iCurrentIndex == 0)//ó�� �ε���
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
	else if(m_iCurrentIndex == m_iMaxIndex - 2)//���������� 2��°
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
		//�ǻ� ����
		RECT			rcUI;
		SetRect(&rcUI, 10, 10, 275, 440);

		if(PtInRect(&rcUI, ptMouse))
		{
			return false;
		}
	}

	{
		//���� ���� ����
		RECT			rcUI;
		SetRect(&rcUI, 10, 470, 460, 890);

		if(PtInRect(&rcUI, ptMouse))
		{
			return false;
		}
	}

	{
		//���� ���� ����
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

	//�÷��̾� Y ����
	if(m_fPlayerTargetAngle != m_fPlayerCurrentAngle)
	{
		m_fPlayerCurrentAngle += (m_fPlayerTargetAngle - m_fPlayerCurrentAngle) * fTimeDelta * 6.0f;
	}

	//Y��
	if(m_fTarget_SC_YPos != m_fCurrent_SC_YPos)
	{
		m_fCurrent_SC_YPos += (m_fTarget_SC_YPos - m_fCurrent_SC_YPos) * fTimeDelta * 6.0f;
	}

	//�Ÿ�
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
	m_bVibrationStart	= true;		//ī�޶� Update���� CameraVibration�� ȣ�� �Ͽ� true�� �� ������ �ϵ��� ��
	m_fVibrationTime	= 0.f;		//������ ���������� CameraStartVibration�� ȣ�� �Ҷ��� ���� �ʱ�ȭ�Ѵ�.
	m_fVibrationYValue	= fYValue;	//�Ű������� ������ ���� �����Ѵ�.
	m_fVibrationSpeed	= fSpeed;	//�Ű������� �����ð�(10/m_fVibrationSpeed)�� �����Ѵ�.
}

void CDynamicCamera::CameraVibration(const _float& fTimeDelta)
{
	if(m_bVibrationStart)
	{
		//�ð��� �帧�� ���� m_fVibrationY�� ���� �ٲ۴�.
		m_fVibrationY		= sin(m_fVibrationTime * 10.f) * pow(0.5f, m_fVibrationTime) * m_fVibrationYValue;
		m_vEye.y			+= m_fVibrationY;	//�� ������ ���� ���� m_vEye�� ��ġ�� �������� �����ش�.
		m_fVibrationTime	+= fTimeDelta * m_fVibrationSpeed;
	}

	if(m_fVibrationTime >= m_fVibrationMaxTime) //�ִ������ð��� 10���ε� 10�ʺ��� ũ��
	{
		m_fVibrationTime = 0.f;		//���� ������ ���� �ʱ�ȭ�� ���ְ�
		m_bVibrationStart = false;	//������ �����.
	}
}


void CDynamicCamera::NavigationInitialize(void)
{
	if(NULL == m_pNaviCell)
	{
		list<Engine::CNaviCell*>::iterator iter = Engine::Get_CameraNaviMesh()->Get_listNaviCell()->begin();
		list<Engine::CNaviCell*>::iterator iter_end = Engine::Get_CameraNaviMesh()->Get_listNaviCell()->end();
		for(; iter != iter_end; ++iter)	//iter�� ���鼭 ���ǿ� �����ϴ� �׺���̼� �޽��� �ּҸ� ã�´�.
		{
			_bool bInside = (*iter)->Inside_Point(&m_vAfterPos);
			if(true == bInside)
			{
				m_pNaviCell = (*iter);
				break;	//�ش��ϴ� �ּҸ� ã���� break�� Ż���Ѵ�.
			}
		}
	}
}

void CDynamicCamera::Check_Navigation(void)
{
	NavigationInitialize();	//m_pNaviCell�� NULL�϶� �׺���̼��� �ּҸ� ã�´�.
	_bool bInside = false;	
	
	if(m_pNaviCell != NULL)
		bInside = m_pNaviCell->CheckInFindNeighbor(&m_vAfterPos, &m_pNaviCell);

	if(true == bInside)	//������ ���� ��ġ�� �׺���̼� �޽��� ���ԵǾ� ������ ī�޶��� ��ġ ������ ����.
	{
		m_bCollisionMoveCheck = false;
	}
	else				//������ ���� ��ġ�� �׺���̼� �޽��� ������ �ȵǾ� ������ ����ó���� �Ѵ�.
	{
		m_pNaviCell = NULL;
		if(!m_bCollisionMoveCheck)	//ó�� �������� ���� ī�޶� �̵��ؾ� �ϴ� ��ġ�� ���� �޴´�.
		{
			m_bCollisionMoveCheck	= true;
			m_vCollisionEyePos		= m_vAfterPos;
		}

		_vec3 vSearchPos = m_vEye;

		list<CStaticObject*>::iterator iter		= CCollisionMgr::GetInstance()->Get_StaticObjectDefaultList()->begin();
		list<CStaticObject*>::iterator iterEnd	= CCollisionMgr::GetInstance()->Get_StaticObjectDefaultList()->end();

		for( ; iter != iterEnd; ++iter )
		{
			//Sphere - Sphere �浹�� �Ǹ� ���� ������ �ִٴ� ���̴�.
			if(Engine::CCollision::GetInstance()->Collision_Sphere_Sphere((*iter)->Get_WorldCenter(), (*iter)->Get_WorldRadius(), m_vCollisionEyePos, 20.f))
			{
				_vec3 vCheckPos(0.f, 0.f, 0.f);
				
				//D3DXIntersect�Լ��� �̿��Ͽ� ī�޶��� At ���� Eye���� ������ �ش� �޽��� �浹�� �ϸ� ��ġ���� ���Ѵ�.
				if(PickingObjectPos((*iter)->Get_MeshCom()->GetRefereMesh(), &(*iter)->Get_TransCom()->m_matWorld, &vCheckPos))
				{
					//�浹 �Ǵ� ���� ������ �ش� ������ ��ġ�� �ٲ��ش�.
					_float fSearchPosDistance	= D3DXVec3Length(&(vSearchPos - m_vAt));
					_float fCheckPosDistance	= D3DXVec3Length(&(vCheckPos - m_vAt));

					//�Ÿ��� ���� ����� �浹 ������ �˻��Ͽ�
					if(fSearchPosDistance > fCheckPosDistance)
					{
						//���粨���� �� ������ �������ش�.
						vSearchPos = vCheckPos;
					}
				}
			}
		}

		_float fGap = D3DXVec3Length(&(vSearchPos - m_vCollisionEyePos));

		if(fGap >= 1.0f)
		{
			_vec3 vDir = vSearchPos - m_vCollisionEyePos; //�浹 ���� - ���� ��ġ
			D3DXVec3Normalize(&vDir, &vDir);	//������ ���Ѵ�.
			
			//�Ÿ��� ���� �����ӵ��� �ٸ��� �Ѵ�.
			m_vCollisionEyePos += vDir * max(fGap, 3.5f) * 10.f * m_fTimeDelta;

			if(fGap > 3.f)	//�浹 ������ ���� �Ÿ����̰� 3���� ũ�� ���� ���� �Ҵ�.
				Engine::CRenderer::m_bRadialBlurRender = true;
		}
		else
		{
			m_vCollisionEyePos = vSearchPos; //�Ÿ����̰� 1���� ������ �ٷ� �̵����Ѵ�.
		}

		m_vEye = m_vCollisionEyePos;	//���� ��ġ�� �����Ѵ�.
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

	//��ǥ����
	//m_vPivotPos + (m_vRayDir * fDist) //=> �޽��� ���콺�� �浹�� ������ ��ǥ���� ���´�.

	bool j = (bHit != 0); //bHit �� false�̸� 0�̵Ǿ� false�� ��ȯ

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