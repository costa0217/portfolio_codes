#include "StdAfx.h"
#include "FamFly.h"

CFamFly::CFamFly(void)
{
}

CFamFly::~CFamFly(void)
{
	Release();
}
void CFamFly::Initialize(void)
{
	m_tInfo.fCX = 30.f;
	m_tInfo.fCY = 30.f;
	m_fAngle	= 0.0f;	
	m_tFrame	= FRAME(0, 3, 0, 60);

	m_dwFamFlyTime	= GetTickCount();
}
int CFamFly::Progress(void)
{

	if(m_dwFamFlyTime + m_tFrame.dwTime < GetTickCount())
	{
		m_dwFamFlyTime = GetTickCount();
		++m_tFrame.iStart;
		if(m_tFrame.iStart >= m_tFrame.iLast)
			m_tFrame.iStart = 0;
	}
	m_fAngle -= 0.1f;

	m_tPlayInfo.fCX = m_pPlayer->GetInfo().fCX;
	m_tPlayInfo.fCY = m_pPlayer->GetInfo().fCY;
	m_tPlayInfo.fX	= m_pPlayer->GetInfo().fX;
	m_tPlayInfo.fY	= m_pPlayer->GetInfo().fY;


	return 0;
}
void CFamFly::Render(HDC hdc)
{
	POINT rotIST1;
	rotIST1 = Rot( m_tPlayInfo.fX - m_tPlayInfo.fCX/1.5f , m_tPlayInfo.fY - m_tPlayInfo.fCY/1.5f, m_tPlayInfo.fX, m_tPlayInfo.fY, m_fAngle);

	/*Ellipse(hdc,
		int(rotIST1.x - m_tInfo.fCX / 2.f),
		int(rotIST1.y - m_tInfo.fCX / 2.f),
		int(rotIST1.x + m_tInfo.fCX / 2.f),
		int(rotIST1.y + m_tInfo.fCX / 2.f));*/

	TransparentBlt(hdc,								
			int(rotIST1.x - m_tInfo.fCX / 2.f),
			int(rotIST1.y - m_tInfo.fCX / 2.f),
			int(m_tInfo.fCX), int(m_tInfo.fCY),
			(*m_pBitMap)["BarriorImage"]->GetMemDC(),
			int(32 * m_tFrame.iStart),	
			0,
			32, 32,
			RGB(61, 16, 123));
}
void CFamFly::Release(void)
{
}

