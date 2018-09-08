#include "StdAfx.h"
#include "Poop.h"

CPoop::CPoop(void)
{
}

CPoop::~CPoop(void)
{
}
void CPoop::Initialize(void)
{
	m_tInfo.fCX		= 50;
	m_tInfo.fCY		= 50;
	m_tFrame		= FRAME(0, 4, 0, 200);
	m_bPoopCrush	= false;
	m_bPoopFade		= false;
	m_tCOlInfo		= INFO(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY);
}
int CPoop::Progress(void)
{

	if(m_bPoopCrush)
	{
		++m_tFrame.iStart;
		if(m_tFrame.iStart == m_tFrame.iLast -1)
			m_bPoopFade = true;
		else if(m_tFrame.iStart >= m_tFrame.iLast)
		{
			m_tFrame.iStart = m_tFrame.iLast;
		}
		m_bPoopCrush = false;
	}
	return 0;
}
void CPoop::Render(HDC hdc)
{
		TransparentBlt(hdc,						
		int(m_tInfo.fX - m_tInfo.fCX / 2.f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		(*m_pBitMap)["poop"]->GetMemDC(),
		int(32 * m_tFrame.iStart),	
		int(32 * m_tFrame.iScene),
		32,32,
		RGB(61, 16, 123));
}
void CPoop::Release(void)
{
}
