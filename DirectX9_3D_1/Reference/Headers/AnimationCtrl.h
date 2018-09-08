/*!
 * \file AnimationCtrl.h
 * \date 2015/11/07 1:27
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief CAnimationCtrl 클래스
 *
 * TODO: CDynamicMesh의 애니메이션과 관련된 class
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
	bool Set_AnimationSet(const _uint& iAniIdx);	//다른 애니메이션만 재생
	bool Set_AnimationSetTwo(const _uint& iAniIdx);	//같은 애니메이션을 재생
	bool Set_AnimationSetSameFinish(const _uint& iAniIdx); //다른 애니메이션은 바로 재생하고 같은 애니메이션은 끝났을때만 초기화를 하면서 재생
	bool Set_AnimationSetThree(const _uint& iAniIdx);	//시작화면용

	void Move_Frame(const _float& fTimeDelta);		// 애니메이션을 동작시킨다.
	_bool Check_EndPeriod(void);
	_bool Check_EndPeriodMulTime(_float fMultiplication);	// 특정 프레임을 지나는지 안지나는지 검사한다.
	_uint Get_CurrentAniIdx(void) { return m_iCurrentAniIdx; }
	_uint Get_MaxAniIdx(void) { return m_iMaxAniSet; }
	_double Get_Period(void) { return m_Period; }
	
	_ulong Get_CurrentTrack(void) { return m_dwCurrentTrack; }	//애니메이션 싱크를 맞추기 위해 현재 트랙을 반환 한다.
	_bool Get_LerpCheckTrack(void);

	_bool Get_AnimationChange(void) { return m_bAnimationChange; }
	void Set_AnimationChange(_bool SetBool) { m_bAnimationChange = SetBool; }
private:
	//애니메이션을 관리하기위한 객체
	//메시의 애니메이션정보를 대표한다.
	//메시의 애니메이션 정보를 대표하면 애니메이션을 관리하기 위한 변수
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