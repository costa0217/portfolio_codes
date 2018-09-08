#include "StdAfx.h"
#include "Hart.h"

CHart::CHart(void)
{
}

CHart::~CHart(void)
{
}
void CHart::Initialize(void)
{
	m_iHP		= 2;
	m_tInfo.fCX = 31.f;
	m_tInfo.fCY = 27.f;
	m_tFrame	= FRAME(0, 2, 0, 100);
}
int CHart::Progress(void)
{
	/*if(m_iHP <= 0)
		m_iHP = 0;*/

	return 0;	
}
void CHart::Render(HDC hdc)
{
	TransparentBlt(hdc,						//ÇÏÆ®
		int(m_tInfo.fX - m_tInfo.fCX / 2.f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		(*m_pBitMap)["Heart"]->GetMemDC(),
		int(31 * m_iHP),	
		int(27 * m_tFrame.iScene),
		31, 27,
		RGB(61, 16, 123));
}
void CHart::Release(void)
{
}
