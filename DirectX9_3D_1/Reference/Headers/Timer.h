/*!
 * \file Timer.h
 * \date 2015/11/06 21:48
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief
 *
 * TODO: CPU의 초당 진동수를 통해 시간을 계산하는 클래스로 CTimeMgr와 연계하여 시간관 관련된 프레임, 속도에 사용한다.
 *
 * \note
*/

#ifndef Timer_h__
#define Timer_h__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CTimer : public Engine::CBase
{
private:
	explicit CTimer(void);
	virtual ~CTimer(void);

public:	//getter
	_float Get_TimeDelta(void) { return m_fTimeDelta; }

public: //setter
	void Set_TimeDelta(void);

public:
	HRESULT Init_Timer(void);

private:
	//LARGE_INTEGER => 64bit의 Signed Integer. 하위 32비트 DWORD LowPart와 상위 32비트 LONG 형으로 구성 되어 있다.
	LARGE_INTEGER		m_FrameTime;
	LARGE_INTEGER		m_FixTime;
	LARGE_INTEGER		m_LastTime;
	LARGE_INTEGER		m_CpuTick;

	_float				m_fTimeDelta;
	
public:
	static CTimer* Create(void);

public:
	virtual _ulong Release(void);
};

END

#endif // Timer_h__