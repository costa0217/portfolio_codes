#include "StdAfx.h"
#include "FamBaby.h"

CFamBaby::CFamBaby(void)
{
}

CFamBaby::~CFamBaby(void)
{
}
void CFamBaby::Initialize(void)
{
	m_tInfo.fCX = 35.f;
	m_tInfo.fCY = 35.f;
	m_fAngle	= 0.0f;	
	m_tFrame	= FRAME(0, 7, 0, 50);

	m_dwFamBabyTime	= GetTickCount();
}
int CFamBaby::Progress(void)
{
	if(m_dwFamBabyTime + m_tFrame.dwTime < GetTickCount())
	{
		m_dwFamBabyTime = GetTickCount();
		++m_tFrame.iStart;
		if(m_tFrame.iStart >= m_tFrame.iLast)
			m_tFrame.iStart = 0;
	}
	m_fAngle += 0.1f;

	m_tPlayInfo.fCX = m_pPlayer->GetInfo().fCX;
	m_tPlayInfo.fCY = m_pPlayer->GetInfo().fCY;
	m_tPlayInfo.fX	= m_pPlayer->GetInfo().fX;
	m_tPlayInfo.fY	= m_pPlayer->GetInfo().fY;

	return 0;
}
void CFamBaby::Render(HDC hdc)
{
	POINT rotIST1;
	rotIST1 = Rot( m_tPlayInfo.fX - m_tPlayInfo.fCX/1.2f , m_tPlayInfo.fY - m_tPlayInfo.fCY/1.2f, m_tPlayInfo.fX, m_tPlayInfo.fY, m_fAngle);


	TransparentBlt(hdc,								
			int(rotIST1.x - m_tInfo.fCX / 2.f),
			int(rotIST1.y - m_tInfo.fCX / 2.f),
			int(m_tInfo.fCX), int(m_tInfo.fCY),
			(*m_pBitMap)["FamilyerImage"]->GetMemDC(),
			int(32 * m_tFrame.iStart),	
			0,
			32, 32,
			RGB(61, 16, 123));
}
void CFamBaby::Release(void)
{
}

