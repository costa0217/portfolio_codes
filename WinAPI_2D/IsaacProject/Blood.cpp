#include "StdAfx.h"
#include "Blood.h"

CBlood::CBlood(void)
{
}

CBlood::~CBlood(void)
{
}

void CBlood::Initialize(void)
{
	m_iNum = rand()%6;
	m_tInfo.fCX = float(rand()%50 + 70);
	m_tInfo.fCY = float(rand()%50 + 40);
}
int CBlood::Progress(void)
{
	return 0;
}
void CBlood::Render(HDC hdc)
{
	TransparentBlt(hdc,						
		int(m_tInfo.fX - m_tInfo.fCX / 2.f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		(*m_pBitMap)["blood"]->GetMemDC(),
		0,
		int(35 * m_iNum),
		50, 35,
		RGB(61, 16, 123));
}
void CBlood::Release(void)
{
}

