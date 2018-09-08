#include "StdAfx.h"
#include "None.h"

CNone::CNone(void)
{
}

CNone::~CNone(void)
{
}
void CNone::Initialize(void)
{
	m_tInfo.fCX = 50;
	m_tInfo.fCY = 50;
	m_tCOlInfo		= INFO(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY);
}
int CNone::Progress(void)
{
	return 0;
}
void CNone::Render(HDC hdc)
{
	//Rectangle(hdc,
	//	int(m_tInfo.fX - m_tInfo.fCX / 2.f),
	//	int(m_tInfo.fY - m_tInfo.fCY / 2.f),
	//	int(m_tInfo.fX + m_tInfo.fCX / 2.f),
	//	int(m_tInfo.fY + m_tInfo.fCY / 2.f));
}
void CNone::Release(void)
{
}
