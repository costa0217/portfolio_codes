#pragma once

#include "Include.h"

class CTimeMgr
{
	DECLARE_SINGLETON(CTimeMgr)

private:
	LARGE_INTEGER			m_FrameTime;
	LARGE_INTEGER			m_FixTime;
	LARGE_INTEGER			m_LastTime;
	LARGE_INTEGER			m_CpuTick;

private:
	float					m_fTime;
	float					m_fTimeControl;
	float					m_fTimeControl2;
	float					m_fTimeControl3;

public:
	void	SetTimeControl3(float _fTimeControl3){ m_fTimeControl3 = _fTimeControl3; }
	void	SetTimeControl2(float _fTimeControl2){ m_fTimeControl2 = _fTimeControl2; }
	void	SetTimeControl(float _fTimeControl){ m_fTimeControl = _fTimeControl; }
	float	GetTimeControl(){ return m_fTimeControl; }
public:
	float					GetTime(void);
	void					InitTime(void);
	void					SetTime(void);

private:
	CTimeMgr(void);
	~CTimeMgr(void);
};
