#include "StdAfx.h"
#include "SkillAll.h"

CSkillAll::CSkillAll(void)
{
}

CSkillAll::~CSkillAll(void)
{
	Release();
}

void CSkillAll::Initialize(void)
{
	m_eBulletType	= BUL_SKILLALL;
	m_iHP			= 100;		//°ø°Ý·Â

	m_fSpeedX		= 10;		//¼Óµµ
	m_fSpeedY		= 10;
	//m_tInfo		= INFO(WINCX /2.f, 100.f , 256.f, 256.f);
	m_tInfo.fCX		= 256.f;
	m_tInfo.fCY		= 256.f;

	m_tFrame		= FRAME(0, 0, 0, 80);

	m_dwSkillAllTime= GetTickCount();

	//m_bExplosion	= false;

}
int CSkillAll::Progress(void)
{
	if(m_tFrame.iScene < 4 && m_dwSkillAllTime + m_tFrame.dwTime < GetTickCount())
	{
		m_dwSkillAllTime = GetTickCount();
		++m_tFrame.iScene;
		//if( m_tFrame.iScene > 4)
		//	return 1;
	}
	if(m_tFrame.iScene == 4)
	{
		m_fSpeedY += 3;
		m_tInfo.fY += m_fSpeedY;

		
	}
	if(m_tInfo.fY > 1000)
			return 1;

	

	//if(m_bExplosion)
	//{
	//	if(m_dwExpTime + m_tExpFrame.dwTime < GetTickCount())
	//	{
	//		m_dwExpTime = GetTickCount();
	//		++m_tExpFrame.iStart;
	//		if( m_tExpFrame.iStart > m_tExpFrame.iLast)
	//			return 1;
	//	}
	//}
	//if(m_tLegInfo.fY + 100.f < m_tTargetInfo.fY)
	//{
	//	m_fSpeedY += 2.f;
	//	m_tLegInfo.fY += m_fSpeedY;
	//}
	//else
	//{
	//	m_bExplosion = true;
	//}

	return 0;
}
void CSkillAll::Render(HDC hdc)
{
		//Rectangle(hdc,
		//int(m_tInfo.fX - m_tInfo.fCX / 2.f),
		//int(m_tInfo.fY - m_tInfo.fCY / 2.f),
		//int(m_tInfo.fX + m_tInfo.fCX / 2.f),
		//int(m_tInfo.fY + m_tInfo.fCY / 2.f));



	TransparentBlt(hdc,								
		int(m_tInfo.fX - m_tInfo.fCX / 2.f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		(*m_pBitMap)["AllAttackImage"]->GetMemDC(),
		0,
		int(m_tInfo.fCY * m_tFrame.iScene),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		RGB(63, 16, 123));

	//if(m_bExplosion)
	//{
	//	TransparentBlt(hdc,						//ÆøÆÈÀÌÆåÆ®
	//		int(m_tInfo.fX - 90.f),
	//		int(m_tInfo.fY - 150.f),
	//		200, 200,
	//		(*m_pBitMap)["explosion"]->GetMemDC(),
	//		int(96 * m_tExpFrame.iStart),			//ÀÌ¹ÌÁöÅ©±â 96x96
	//		int(96 * m_tExpFrame.iScene),
	//		96, 96,
	//		RGB(61, 16, 123));
	//}
}
void CSkillAll::Release(void)
{
}
RECT CSkillAll::GetCOlRect()
{

	{
		RECT rc = {
			int(m_tInfo.fX - m_tInfo.fCX /2.f),
			int(m_tInfo.fY - m_tInfo.fCY /4.f),
			int(m_tInfo.fX + m_tInfo.fCX /2.f),
			int(m_tInfo.fY + m_tInfo.fCY /4.f)};
	
		return rc;
	}
}