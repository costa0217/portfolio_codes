/*!
 * \file DynamicCamera.h
 * \date 2015/11/23 21:09
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief ��� Scene
 *
 * TODO: ��� Scene���� ����� ���̳��� ī�޶� class.
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
	CDynamicCamera* ReturnThisPoint(void) { return this; }	//ī�޶��� �ּ� ��ȯ
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

#pragma region ī�޶� ��ǲ�� ���� �����ӿ� �ʿ��� ������
	_matrix m_matWorld;				//ī�޶��� ���� ���
	_float	m_fTargetAngle[ANGLE_END];	//ī�޶� ���� ȸ����(��ǥ)
	_float	m_fCurrentAngle[ANGLE_END];	//ī�޶� ���� ȸ����(����)

	_float	m_fTargetDistance;		//�÷��̾�� ī�޶��� �Ÿ�(��ǥ)
	_float	m_fCurrentDistance;		//�÷��̾�� ī�޶��� �Ÿ�(����)

	_bool	m_bMaxDistanceOn;
	_float	m_fMaxDistance;			//X�� ���������� �ִ�Ÿ��� �����ϴ� ����
	_float	m_fOriginalDistance;	//X�� ������ ���� ġ��Ÿ��� ���� ������ �Ǳ��� ���� ��������
	_int	m_iDistance;			//����ó���� �ϱ����� ����
#pragma endregion

#pragma region ���̳��� ī�޶� �ʿ��� ������ �Լ���
	vector<CControlPointMgr*>	m_ControlPointMgrVector[3];
	typedef	vector<CControlPointMgr*> CPMgrVector;

	CControlPointMgr* m_pControlPointMgr;

	_bool	m_bModeCheck;					//��� ī�޶� On�Ǿ���� Ȯ���ϴ� ����
	_bool	m_bCameraModeInfo_Initialize;	//ī�޶� ��尡 On�� �Ǹ� ó�� ��ġ���� �ʱ�ȭ �����ֱ� ���� ����
	
	_int	m_iPlayModeIndex; //�÷����� �ε��� ��ȣ
	_int	m_iDYListIndex;		//DynamicCameraList �ε���
	_int	m_iCurrentIndex;	//ī�޶��� ���� ����Ʈ ��ġ
	_int	m_iMaxIndex;		//ī�޶� ��忡�� ������ ����Ʈ ��ġ

	//���������� �ϱ� ���� �ʿ��� ī�޶�
	_float	m_fAtDistance;
	_float	m_fEyeDistance;
	_float	m_fAtLefpTime;
	_float	m_fEyeLefpTime;

	_float	m_fCamModeSpeed;	//ī�޶� ��忡���� �ӵ�
	_float	m_fDistance;

	_vec3	m_vCurrentAt;
	_vec3	m_vCurrentEye;

	Engine::CTransform*		m_pCameraTargetTransCom;

public:
	void SetCameraTargetTransCom(Engine::CTransform* pTransCom) { m_pCameraTargetTransCom = pTransCom; }

	void ControlPointMgrInfoLoad(_int iNumber, TCHAR* pPath);	//���°�� ������, ���� ���

	void CameraModePlayer(_int iPlayModeIndex);

private:
	void ModeCamera(const _float& fTimeDelta);

	void CameraModeMove(const _float& fTimeDelta, int iNumber);

	void CameraLinearInterpolation(const _float& fTimeDelta);
	void CameraLinearInterpolation_TargetMode(const _float& fTimeDelta);
	void CameraLinearInterpolation_ObserverMode(const _float& fTimeDelta);

#pragma endregion

#pragma region ��ŸƮ ī�޶� ����
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

#pragma region ī�޶� ���� ����
private:
	_bool	m_bVibrationStart;	//ī�޶� ���� ����
	_float	m_fVibrationYValue;	//ī�޸� Y ���� ��ġ
	_float	m_fVibrationSpeed;	//ī�޶� ���� �ð� => (10/m_fVibrationSpeed) ��

	_float	m_fVibrationY;		//���� ������ Y��
	_float	m_fVibrationMaxTime;//ī�޶� x�� �ִ� �ð� 
	_float	m_fVibrationTime;	//ī�޶� ���� x�� �ð�

public:
	void CameraStartVibration(_float fYValue = 1.f, _float fSpeed = 1.f);
private:
	void CameraVibration(const _float& fTimeDelta);

#pragma endregion

#pragma region ī�޶� ������Ʈ �浹, �׺���̼� ����
private:
	Engine::CNaviCell*		m_pNaviCell;
	_vec3					m_vOriginalPos;
	_vec3					m_vAfterPos;

	_float					m_fTimeDelta;

	_vec3					m_vPivotPos;	//Ray�� ������
	_vec3					m_vRayDir;		//Ray�� ����

	_bool					m_bCollisionMoveCheck;	//�׺� ������ ������ ���� ó�� üũ
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

	void CameraInput(const _float& fTimeDelta);		//��ǲ�� ���� ī�޶��� ���� �ٲپ� �ش�.
	void CameraInputLerp(const _float& fTimeDelta);	//�ٲ� ���� ���� ī�޶��� ������ �����Ѵ�.
	
	_bool PlayerAnimationCheck(Player::PLAYERANIMATION ePlayerAnimation);
	void CameraFovChange(void);	//�÷��̾� �ִϸ��̼��� üũ�Ͽ� FOV�� ���� ���Ͻ�Ų��.
	void PlayerCamera(const _float& fTimeDelta);			//�÷��̾��� �ִϸ��̼ǰ� ���� Ÿ�Կ� ���� At�� ��İ� FOV�� ���� �ٲپ� �ش�.

public:
	virtual _ulong Release(void);
};
#endif // DynamicCamera_h__