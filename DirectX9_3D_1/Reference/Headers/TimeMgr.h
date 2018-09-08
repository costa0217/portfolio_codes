/*!
 * \file TimeMgr.h
 * \date 2015/11/06 21:50
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: Map STL�� ���ؼ� CTime Class�� �����ϴ� Singleton Manager clsss.
 *
 * \note
*/

#ifndef TimeMgr_h__
#define TimeMgr_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CTimer;
class ENGINE_DLL CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr)

private:
	explicit CTimeMgr(void);
	~CTimeMgr(void);

public:
	_float Get_TimeDelta(const TCHAR* pTimerTag); //�ð��� ��� ���� �Լ�

public:
	void Set_TimeDelta(const TCHAR* pTimerTag);	//pTimerTag�� �ش��ϴ� CTimeŬ������ �ð� ������ ȣ�� �ϴ� �Լ�

public:
	HRESULT	Ready_Timer(const TCHAR* pTimerTag);

private:
	boost::unordered_map<const TCHAR*, CTimer*>			m_mapTimer;
	typedef boost::unordered_map<const TCHAR*, CTimer*>	MAPTIMER;
private:
	CTimer*	Find_Timer(const TCHAR* pTimerTag);

public:
	void Release(void);
public:
};

END

#endif // TimeMgr_h__