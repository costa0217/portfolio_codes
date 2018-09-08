/*!
 * \file AnimationCtrl.h
 * \date 2015/11/07 1:27
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief CAnimationCtrl Ŭ����
 *
 * TODO: CDynamicMesh�� �ִϸ��̼ǰ� ���õ� class
 *
 * \note
*/

#ifndef AnimationCtrl_h__
#define AnimationCtrl_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CAnimationCtrl
{
private:
	explicit CAnimationCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAnimationCtrl(const CAnimationCtrl& Instance);
	~CAnimationCtrl(void);

public:
	static CAnimationCtrl* Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static CAnimationCtrl* Clone(const CAnimationCtrl& Instance);

public:
	bool Set_AnimationSet(const _uint& iAniIdx);	//�ٸ� �ִϸ��̼Ǹ� ���
	bool Set_AnimationSetTwo(const _uint& iAniIdx);	//���� �ִϸ��̼��� ���
	bool Set_AnimationSetSameFinish(const _uint& iAniIdx); //�ٸ� �ִϸ��̼��� �ٷ� ����ϰ� ���� �ִϸ��̼��� ���������� �ʱ�ȭ�� �ϸ鼭 ���
	bool Set_AnimationSetThree(const _uint& iAniIdx);	//����ȭ���

	void Move_Frame(const _float& fTimeDelta);		// �ִϸ��̼��� ���۽�Ų��.
	_bool Check_EndPeriod(void);
	_bool Check_EndPeriodMulTime(_float fMultiplication);	// Ư�� �������� �������� ���������� �˻��Ѵ�.
	_uint Get_CurrentAniIdx(void) { return m_iCurrentAniIdx; }
	_uint Get_MaxAniIdx(void) { return m_iMaxAniSet; }
	_double Get_Period(void) { return m_Period; }
	
	_ulong Get_CurrentTrack(void) { return m_dwCurrentTrack; }	//�ִϸ��̼� ��ũ�� ���߱� ���� ���� Ʈ���� ��ȯ �Ѵ�.
	_bool Get_LerpCheckTrack(void);

	_bool Get_AnimationChange(void) { return m_bAnimationChange; }
	void Set_AnimationChange(_bool SetBool) { m_bAnimationChange = SetBool; }
private:
	//�ִϸ��̼��� �����ϱ����� ��ü
	//�޽��� �ִϸ��̼������� ��ǥ�Ѵ�.
	//�޽��� �ִϸ��̼� ������ ��ǥ�ϸ� �ִϸ��̼��� �����ϱ� ���� ����
	LPD3DXANIMATIONCONTROLLER		m_pAniCtrl;

private:
	_uint							m_iMaxAniSet;
	_uint							m_iCurrentAniIdx;

	_ulong							m_dwCurrentTrack;
	_ulong							m_dwNewTrack;
	_ulong							m_dwLerpCheckTrack;
	_bool							m_bAnimationChange;

	_float							m_fAccTime;
	_double							m_Period;

public:
	_ulong Release(void);
};

END

#endif // AnimationCtrl_h__