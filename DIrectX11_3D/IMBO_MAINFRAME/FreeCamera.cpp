#pragma once
#include "stdafx.h"
#include "FreeCamera.h"

#include "stdafx.h"
#include "Camera.h"

//DI
bool CFreeCamera::Begin() {

	return CCamera::Begin();
}
bool CFreeCamera::End() {
	
	return CCamera::End();
}

void CFreeCamera::Rotate(float x, float y, float z) {

	//m_fAngleX += x;
	//m_fAngleY += y;

	////XMStoreFloat3(&m_xmf3At, XMVector4Transform(XMLoadFloat4(&XMFLOAT4(0.f, 0.f, 1.f, 0.f)), XMMatrixRotationRollPitchYaw(m_fLinearY, m_fLinearX, 0)));
	////XMStoreFloat3(&m_xmf3At, XMVector3Normalize(XMLoadFloat3(&m_xmf3At)));

	//if (nullptr != m_pTarget)
	//{
	//	m_pTarget->SetRotation(XMMatrixRotationY(m_fLinearX));
	//}
}

void CFreeCamera::Update(float fTimeElapsed) {
	float fTimeDelta = TIMEMGR->GetTimeElapsed();
	CalcultateMouseMoveValue();
	FixCamera();
	if (nullptr != m_pTarget)
	{
		AttackZoomInOut(fTimeDelta);
		CameraInputProcess(fTimeDelta);
		CameraVibration(fTimeDelta);
	}

	UpdateShaderState();
	if (true == m_bFix && MODE_FIX == m_eMode)
	{
		INPUTMGR->SetCheckMouse(false);
		POINT		ptMouse = { GLOBALVALUEMGR->GetrcClient().right >> 1, GLOBALVALUEMGR->GetrcClient().bottom >> 1 };
		ClientToScreen(GLOBALVALUEMGR->GethWnd(), &ptMouse);
		SetCursorPos(ptMouse.x, ptMouse.y);
		m_ptOldMousePos = ptMouse;
	}
}

//상수버퍼 갱신
void CFreeCamera::UpdateShaderState() {
	//정보 갱신

	
	CCamera::UpdateShaderState();

	
}

void CFreeCamera::ProcessInput(float fTimeElapsed) {
	//DWORD dwDirection = 0;
	////UINT iMoveState;
	//float fDeltaMdis;
	//float fMouseSpeed = 10.f;
	////float cyDelta = INPUTMGR->GetcyDelta();
	//
	//if (fDeltaMdis = INPUTMGR->GetcxDelta())
	//{
	//	m_fTargetAngle[ANGLE_Y] += XMConvertToRadians(fDeltaMdis * fTimeElapsed * fMouseSpeed);
	//}

	//if (fDeltaMdis = INPUTMGR->GetcyDelta())
	//{
	//	if(XMConvertToDegrees(m_fTargetAngle[ANGLE_X]) >= -45.f && XMConvertToDegrees(m_fTargetAngle[ANGLE_X]) <= 45.f)
	//		m_fTargetAngle[ANGLE_X] += XMConvertToRadians(fDeltaMdis * fTimeElapsed * fMouseSpeed);
	//}
	//if()
}

void CFreeCamera::CameraInputProcess(float fTimeDelta)
{
	DWORD dwDirection = 0;
	//UINT iMoveState;
	//float fDeltaMdis;
	float fMouseSpeed = 20.f;
	//float cyDelta = INPUTMGR->GetcyDelta();

	if (nullptr != m_pTarget)
	{
		XMMATRIX xmmtxFrame = m_pTarget->GetAnimater()->GetCurAnimationInfo()->GetCurFrameMtx(m_pTarget->GetAnimater()->GetHead());
		XMMATRIX xmmtxFinal = xmmtxFrame * m_pTarget->GetAnimater()->GetMeshOffsetMtx() 
			* XMMatrixTranslation(0.f, 0.5f, 0.f) * m_pTarget->GetWorldMtx();
		//DEBUGER->RegistCoordinateSys(xmmtxFinal);
		XMFLOAT4X4 xmf4x4Final;
		XMStoreFloat4x4(&xmf4x4Final, xmmtxFinal);

		//XMStoreFloat3(&m_xmf3At, XMVectorAdd(m_pTarget->GetPosition(), XMLoadFloat3(&XMFLOAT3(0.f, 0.f, 0.f))));
		XMStoreFloat3(&m_xmf3At, XMLoadFloat3(&XMFLOAT3(xmf4x4Final._41, xmf4x4Final._42, xmf4x4Final._43)));
	}
		

	if (0.f != m_cxDelta)
	{
		m_fTargetAngle[ANGLE_Y] += XMConvertToRadians(m_cxDelta * fTimeDelta * fMouseSpeed);
	}

	if (0.f != m_cyDelta)
	{
		if (XMConvertToDegrees(m_fTargetAngle[ANGLE_X]) >= -45.f && XMConvertToDegrees(m_fTargetAngle[ANGLE_X]) <= 45.f)
			m_fTargetAngle[ANGLE_X] -= XMConvertToRadians(m_cyDelta * fTimeDelta * fMouseSpeed);
	}


	float fCamZoomSpd = 2.f;
	float fTagetDis = m_fTgtDistance + INPUTMGR->GetcurWheelDalta();

	//Lerp
	if (m_fTargetAngle[ANGLE_X] != m_fCurrentAngle[ANGLE_X])
	{
		m_fCurrentAngle[ANGLE_X] += (m_fTargetAngle[ANGLE_X] - m_fCurrentAngle[ANGLE_X]) * fTimeDelta * 5.f;
	}

	if (XMConvertToDegrees(m_fTargetAngle[ANGLE_X]) <= -45.f)
		m_fTargetAngle[ANGLE_X] = XMConvertToRadians(-45.f);
	else if (XMConvertToDegrees(m_fTargetAngle[ANGLE_X]) >= 45.f)
		m_fTargetAngle[ANGLE_X] = XMConvertToRadians(45.f);

	if (m_fTargetAngle[ANGLE_Y] != m_fCurrentAngle[ANGLE_Y])
	{
		m_fCurrentAngle[ANGLE_Y] += (m_fTargetAngle[ANGLE_Y] - m_fCurrentAngle[ANGLE_Y]) * fTimeDelta * 5.f;
	}

	if (fTagetDis != m_fCurDistance)
	{
		m_fCurDistance += (fTagetDis - m_fCurDistance) * fTimeDelta * 5.f;
	}


	XMMATRIX matRotX, matRotY, matWorld;
	matRotX = XMMatrixRotationX(m_fCurrentAngle[ANGLE_X]);
	matRotY = XMMatrixRotationY(m_fCurrentAngle[ANGLE_Y]);
	matWorld = XMMatrixMultiply(matRotX, matRotY);

	XMVECTOR vTempEyePos;
	vTempEyePos = XMVector4Transform(XMLoadFloat3(&XMFLOAT3(0.f, 0.f, m_fCurDistance)), matWorld);

	
	//m_vEye = m_vAt + vTempEyePos;
	XMStoreFloat3(&m_xmf3Pos, XMVectorAdd(XMLoadFloat3(&m_xmf3At), vTempEyePos));

	if (nullptr != m_pTarget)
	{
		//if(false == INPUTMGR->MouseRightDown() || false == m_pTarget->GetIdleState())
		//	m_pTarget->SetRotation(XMMatrixRotationY(m_fCurrentAngle[ANGLE_Y] + XM_PI));
	}
}

void CFreeCamera::FixCamera()
{
	if(INPUTMGR->KeyDown(VK_F1_))
		m_bFix ^= true;
}

void CFreeCamera::CalcultateMouseMoveValue()
{
	POINT ptCursorPos;
	GetCursorPos(&ptCursorPos);
	//마우스 버튼이 눌린 채로 이전 위치에서 현재 마우스 커서의 위치까지 움직인 양을 구한다.
	if (true == m_bFix)
	{
		m_cxDelta = (float)(ptCursorPos.x - m_ptOldMousePos.x) / 3.0f;
		m_cyDelta = (float)(ptCursorPos.y - m_ptOldMousePos.y) / 3.0f;
	}
	else
	{
		m_cxDelta = 0.f;
		m_cyDelta = 0.f;
	}
}

void CFreeCamera::CameraStartVibration(float fVibeYvalue, float fVibeSpeed)
{
	m_bVibrationStart = true;		//카메라 Update에서 CameraVibration을 호출 하여 true일 때 진동을 하도록 함
	m_fVibrationTime = 0.f;			//진동이 끝나기전에 CameraStartVibration을 호출 할때를 위해 초기화한다.
	m_fVibrationYValue = fVibeYvalue;	//매개변수로 진동의 폭을 조절한다.
	m_fVibrationSpeed = fVibeSpeed;		//매개변수로 진동시간(10/m_fVibrationSpeed)을 조절한다.
}

void CFreeCamera::CameraVibration(float fTime)
{
	if (m_bVibrationStart)
	{
		//시간의 흐름에 따라 m_fVibrationY의 값을 바꾼다.
		m_fVibrationY = sin(m_fVibrationTime * 10.f) * pow(0.5f, m_fVibrationTime) * m_fVibrationYValue;
		m_xmf3Pos.y += m_fVibrationY;	//매 프레임 마다 최종 m_vEye의 위치에 진동값을 더해준다.
		m_fVibrationTime += fTime * m_fVibrationSpeed;
	}

	if (m_fVibrationTime >= m_fVibrationMaxTime) //최대진동시간은 10초인데 10초보다 크면
	{
		m_fVibrationTime = 0.f;		//다음 진동을 위해 초기화를 해주고
		m_bVibrationStart = false;	//진동을 멈춘다.
	}
}

void CFreeCamera::AttackZoomInOut(float fTime)
{
	if (m_bFovChange)
	{
		m_fFovY -= XMConvertToRadians(50.f * fTime * 3.f);
		if (m_fFovY <= XMConvertToRadians(40.f))
			m_fFovY = XMConvertToRadians(40.f);
	}
	else
	{
		m_fFovY += XMConvertToRadians(50.f * fTime);
		if (m_fFovY >= XMConvertToRadians(55.f))
			m_fFovY = XMConvertToRadians(55.f);
	}
	GenerateProjectionMatrix(m_fFovY	// FOV Y 값 : 클 수록 멀리까지 볼 수 있다.
		, float(GLOBALVALUEMGR->GetrcClient().right) / float(GLOBALVALUEMGR->GetrcClient().bottom)// 종횡비
		, NEAR_PLANE, FAR_PLANE);// 최대 거리
}

CFreeCamera::CFreeCamera() : CCamera() {
	
}
CFreeCamera::~CFreeCamera() {

}
