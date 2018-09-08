#include "StdAfx.h"
#include "Fire.h"

#include "Device.h"

CFire::CFire(void)
:m_dwFireTime(GetTickCount())
,m_dwFirePoofTime(GetTickCount())
{
}

CFire::~CFire(void)
{
	Release();
}
void CFire::Initialize(void)
{
	m_tInfo.fCX		= 50;
	m_tInfo.fCY		= 50;
	m_tFrame		= FRAME(rand()%6, 6, 0, 100);
	m_tPoofFrame	= FRAME(0, 19, 0, 100);
	m_tCOlInfo		= INFO(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX /2.f, m_tInfo.fCY /2.f);

	//CDevice::GetInstance()->LoadWave(L"../Sound/FireEnd.wav");  //6
	

	m_bFireFade		= false;
	m_bFirePoof		= false;
}
int CFire::Progress(void)
{
	if(m_bFireFade)
	{
		m_tFrame.iStart = 0;
		m_tFrame.iScene = 1;
		
		m_bFirePoof = true;
		if(m_dwFirePoofTime + m_tPoofFrame.dwTime < GetTickCount())
		{
			m_dwFirePoofTime = GetTickCount();

			if(m_tPoofFrame.iStart == 0)
				CDevice::GetInstance()->SoundPlay(FIREEND, 0);
			if(m_tPoofFrame.iStart >= m_tPoofFrame.iLast)
			{
				m_tPoofFrame.iStart = m_tPoofFrame.iLast;
				m_bFirePoof = false;
			}
			++m_tPoofFrame.iStart;
		}
		
	}
	else if(m_dwFireTime + m_tFrame.dwTime < GetTickCount())
	{
		m_dwFireTime = GetTickCount();
		++m_tFrame.iStart;
		if(m_tFrame.iStart >= m_tFrame.iLast)
		{
			m_tFrame.iStart = 0;
		}
	}
	return 0;
}
void CFire::Render(HDC hdc)
{
	TransparentBlt(hdc,						
		int(m_tInfo.fX - m_tInfo.fCX / 2.f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		(*m_pBitMap)["fire"]->GetMemDC(),
		int(32 * m_tFrame.iStart),	
		int(32 * m_tFrame.iScene),
		32,32,
		RGB(61, 16, 123));

	if(m_bFireFade && m_bFirePoof)
	{
		TransparentBlt(hdc,						
			int(m_tInfo.fX - 100 / 2.f),
			int(m_tInfo.fY - 140 / 2.f) ,
			95, 95,
			(*m_pBitMap)["poof2"]->GetMemDC(),
			int(95 * m_tPoofFrame.iStart),	
			int(95 * m_tPoofFrame.iScene),
			95,95,
			RGB(61, 16, 123));
	}
}
void CFire::Release(void)
{
}
