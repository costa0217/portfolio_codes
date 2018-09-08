/*!
 * \file DynamicCamera.h
 * \date 2015/11/23 21:09
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 모든 Scene
 *
 * TODO: 모든 Scene에서 사용할 다이나믹 카메라 class.
 *
 * \note
*/

#ifndef DynamicCamera_h__
#define DynamicCamera_h__

#include "Defines.h"
#include "Camera.h"

namespace Engine
{
	class CTransform;
	class CNaviCell;
}

class CControlPointMgr;
class CDynamicCamera : public Engine::CCamera
{
	DECLARE_SINGLETON(CDynamicCamera)

private:
	explicit CDynamicCamera(void);
	virtual ~CDynamicCamera(void);

public:
	virtual _int Update(const _float& fTimeDelta);

public:
	CDynamicCamera* ReturnThisPoint(void) { return this; }	//카메라의 주소 반환
	_float	GetCurrentDegreeAngleY(void) { return D3DXToDegree(m_fCurrentAngle[ANGLE_Y]); }
	_float	GetCurrentDistance(void) { return m_fCurrentDistance; }
	_vec3	GetEye(void) { return m_vEye; }
	_bool	GetModeCheck(void) { return m_bModeCheck; }
	_bool	GetbFixMouse(void) { return m_bFixCamera; }
	void	SetbFixMouse(_bool SetBool) { m_bFixCamera = SetBool; }

private:
	enum ANGLE { ANGLE_X, ANGLE_Y, ANGLE_END };

private:
	_bool	m_bFixCamera;
	_bool	m_bFixEnable;
	_bool	m_bFovChange;

#pragma region 카메라 인풋에 따른 움직임에 필요한 변수들
	_matrix m_matWorld;				//카메라의 눈의 행렬
	_float	m_fTargetAngle[ANGLE_END];	//카메라 눈의 회전값(목표)
	_float	m_fCurrentAngle[ANGLE_END];	//카메라 눈의 회전값(현재)

	_float	m_fTargetDistance;		//플레이어와 카메라의 거리(목표)
	_float	m_fCurrentDistance;		//플레이어와 카메라의 거리(현재)

	_bool	m_bMaxDistanceOn;
	_float	m_fMaxDistance;			//X축 각도에따른 최대거리를 제한하는 변수
	_float	m_fOriginalDistance;	//X축 각도에 따른 치대거리로 인해 변경이 되기전 원본 오리지널
	_int	m_iDistance;			//예외처리를 하기위해 선언
#pragma endregion

#pragma region 다이나믹 카메라에 필요한 변수와 함수들
	vector<CControlPointMgr*>	m_ControlPointMgrVector[3];
	typedef	vector<CControlPointMgr*> CPMgrVector;

	CControlPointMgr* m_pControlPointMgr;

	_bool	m_bModeCheck;					//모드 카메라가 On되어는지 확인하는 변수
	_bool	m_bCameraModeInfo_Initialize;	//카메라 모드가 On이 되면 처음 위치들을 초기화 시켜주기 위한 변수
	
	_int	m_iPlayModeIndex; //플레이할 인덱스 번호
	_int	m_iDYListIndex;		//DynamicCameraList 인덱스
	_int	m_iCurrentIndex;	//카메라의 현재 리스트 위치
	_int	m_iMaxIndex;		//카메라 모드에서 가능한 리스트 위치

	//선형보간을 하기 위해 필요한 카메라
	_float	m_fAtDistance;
	_float	m_fEyeDistance;
	_float	m_fAtLefpTime;
	_float	m_fEyeLefpTime;

	_float	m_fCamModeSpeed;	//카메라 모드에서의 속도
	_float	m_fDistance;

	_vec3	m_vCurrentAt;
	_vec3	m_vCurrentEye;

	Engine::CTransform*		m_pCameraTargetTransCom;

public:
	void SetCameraTargetTransCom(Engine::CTransform* pTransCom) { m_pCameraTargetTransCom = pTransCom; }

	void ControlPointMgrInfoLoad(_int iNumber, TCHAR* pPath);	//몇번째에 넣을지, 파일 경로

	void CameraModePlayer(_int iPlayModeIndex);

private:
	void ModeCamera(const _float& fTimeDelta);

	void CameraModeMove(const _float& fTimeDelta, int iNumber);

	void CameraLinearInterpolation(const _float& fTimeDelta);
	void CameraLinearInterpolation_TargetMode(const _float& fTimeDelta);
	void CameraLinearInterpolation_ObserverMode(const _float& fTimeDelta);

#pragma endregion

#pragma region 스타트 카메라 관련
private:
	_float m_fPlayerTargetAngle;
	_float m_fPlayerCurrentAngle;

	_float m_fTarget_SC_YPos;
	_float m_fCurrent_SC_YPos;

	_float m_fTarget_SC_EyeDistance;
	_float m_fCurrent_SC_EyeDistance;

	_bool StartingScreenCheck(void);
	void StartingScreenCamera(const _float& fTimeDelta);
#pragma endregion

#pragma region 카메라 진동 관련
private:
	_bool	m_bVibrationStart;	//카메라 진동 시작
	_float	m_fVibrationYValue;	//카메리 Y 진동 수치
	_float	m_fVibrationSpeed;	//카메라 진동 시간 => (10/m_fVibrationSpeed) 초

	_float	m_fVibrationY;		//현재 진동된 Y값
	_float	m_fVibrationMaxTime;//카메라 x축 최대 시간 
	_float	m_fVibrationTime;	//카메라 현재 x축 시간

public:
	void CameraStartVibration(_float fYValue = 1.f, _float fSpeed = 1.f);
private:
	void CameraVibration(const _float& fTimeDelta);

#pragma endregion

#pragma region 카메라 오브젝트 충돌, 네비게이션 관련
private:
	Engine::CNaviCell*		m_pNaviCell;
	_vec3					m_vOriginalPos;
	_vec3					m_vAfterPos;

	_float					m_fTimeDelta;

	_vec3					m_vPivotPos;	//Ray의 시작점
	_vec3					m_vRayDir;		//Ray의 방향

	_bool					m_bCollisionMoveCheck;	//네비 밖으로 갔을때 제일 처음 체크
	_vec3					m_vCollisionEyePos;

public:
	void SetNaviCell(Engine::CNaviCell* SetPoint) { m_pNaviCell = SetPoint; NavigationInitialize(); }
private:
	void AtPos_Translation_ViewSpace(void);
	void Translation_Local(const D3DXMATRIX* pWorld);
	_bool PickingObjectPos(LPD3DXMESH& pMehs, D3DXMATRIX* pMat, _vec3* SearchPos);
	void Check_Navigation(void);
	void NavigationInitialize(void);
#pragma endregion


public:
		virtual HRESULT Initialize(void);
private:
	void FixMouse(void);

	void CameraInput(const _float& fTimeDelta);		//인풋에 따라 카메라의 값을 바꾸어 준다.
	void CameraInputLerp(const _float& fTimeDelta);	//바뀐 값에 따라 카메라의 값들을 보간한다.
	
	_bool PlayerAnimationCheck(Player::PLAYERANIMATION ePlayerAnimation);
	void CameraFovChange(void);	//플레이어 애니메이션을 체크하여 FOV의 값을 변하시킨다.
	void PlayerCamera(const _float& fTimeDelta);			//플레이어의 애니메이션과 상태 타입에 따라 At의 행렬과 FOV의 값을 바꾸어 준다.

public:
	virtual _ulong Release(void);
};
#endif // DynamicCamera_h__