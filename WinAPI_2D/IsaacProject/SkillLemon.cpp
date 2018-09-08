#include "StdAfx.h"
#include "SkillLemon.h"

CSkillLemon::CSkillLemon(void)
{
}

CSkillLemon::~CSkillLemon(void)
{
}
void CSkillLemon::Initialize(void)
{
	m_eBulletType	= BUL_SKILLLEMON;
	m_iHP			= 6;		//공격력

	m_fSpeedX		= 2;		//속도
	m_fSpeedY		= 2;
	m_tInfo.fCX		= 28.8f;
	m_tInfo.fCY		= 16.f;

	m_dwLemonTime	= GetTickCount();
	m_bPlusSize		= false;

}
int CSkillLemon::Progress(void)
{
	if(m_dwLemonTime + 100 < GetTickCount())
	{	
		m_fSpeedX -= 0.1f; 
		m_fSpeedY -= 0.1f;

		m_dwLemonTime = GetTickCount();
		m_tInfo.fCX += 9.f * m_fSpeedX;	
		m_tInfo.fCY += 5.f * m_fSpeedY;

		if( m_tInfo.fCX < 0 )
			return 1;
	}
	return 0;
}
void CSkillLemon::Render(HDC hdc)
{
	//
	//Rectangle(hdc,
	//	int(m_tInfo.fX - m_tInfo.fCX / 2.4f),
	//	int(m_tInfo.fY - m_tInfo.fCY / 2.4f),
	//	int(m_tInfo.fX + m_tInfo.fCX / 2.4f),
	//	int(m_tInfo.fY + m_tInfo.fCY / 2.4f));

	TransparentBlt(hdc,										//메테오
		int(m_tInfo.fX - m_tInfo.fCX / 2.f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		(*m_pBitMap)["LemonImage"]->GetMemDC(),
		0,	0,
		int(288.f), int(160.f),
		RGB(63, 16, 123));


}
void CSkillLemon::Release(void)
{
}
RECT CSkillLemon::GetCOlRect()
{
	RECT rc = {
		int(m_tInfo.fX - m_tInfo.fCX / 2.2f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.2f),
		int(m_tInfo.fX + m_tInfo.fCX / 2.2f),
		int(m_tInfo.fY + m_tInfo.fCY / 2.2f)};

		return rc;

}
RECT CSkillLemon::GetRect()
{
	RECT rc = {
			0,
			0,
			0,
			-2000};

			return rc;
}