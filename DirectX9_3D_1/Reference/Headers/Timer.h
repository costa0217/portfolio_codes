/*!
 * \file Timer.h
 * \date 2015/11/06 21:48
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief
 *
 * TODO: CPU�� �ʴ� �������� ���� �ð��� ����ϴ� Ŭ������ CTimeMgr�� �����Ͽ� �ð��� ���õ� ������, �ӵ��� ����Ѵ�.
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
	//LARGE_INTEGER => 64bit�� Signed Integer. ���� 32��Ʈ DWORD LowPart�� ���� 32��Ʈ LONG ������ ���� �Ǿ� �ִ�.
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