/*!
 * \file TimeMgr.h
 * \date 2015/11/06 21:50
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: Map STL을 통해서 CTime Class를 관리하는 Singleton Manager clsss.
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
	_float Get_TimeDelta(const TCHAR* pTimerTag); //시간을 얻어 오는 함수

public:
	void Set_TimeDelta(const TCHAR* pTimerTag);	//pTimerTag에 해당하는 CTime클래스의 시간 셋팅을 호출 하는 함수

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