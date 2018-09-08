#include "StdAfx.h"
#include "TimeMgr.h"

IMPLEMENT_SINGLETON(CTimeMgr)

CTimeMgr::CTimeMgr(void)
: m_fTime(0.f)
, m_fTimeControl(1.f)
, m_fTimeControl2(1.f)
, m_fTimeControl3(1.f)
{
}

CTimeMgr::~CTimeMgr(void)
{
}

float CTimeMgr::GetTime( void )
{
	return m_fTime * m_fTimeControl * m_fTimeControl2 * m_fTimeControl3;
}

void CTimeMgr::InitTime( void )
{
	QueryPerformanceCounter(&m_FrameTime);
	QueryPerformanceCounter(&m_FixTime);
	QueryPerformanceCounter(&m_LastTime);
	QueryPerformanceFrequency(&m_CpuTick);

}

void CTimeMgr::SetTime( void )
{
	QueryPerformanceCounter(&m_FrameTime);	//20

	if(m_FrameTime.QuadPart - m_LastTime.QuadPart > m_CpuTick.QuadPart)
	{
		QueryPerformanceFrequency(&m_CpuTick);
		m_LastTime.QuadPart = m_FrameTime.QuadPart;
	}

	m_fTime = float(m_FrameTime.QuadPart - m_FixTime.QuadPart) / m_CpuTick.QuadPart;

	m_FixTime = m_FrameTime;
}
