#include "StdAfx.h"
#include "SkillMeteo.h"

#include "Device.h"

CSkillMeteo::CSkillMeteo(void)
{
}

CSkillMeteo::~CSkillMeteo(void)
{
	Release();
}
void CSkillMeteo::Initialize(void)
{
	m_eBulletType	= BUL_SKILLMETEO;
	m_iHP			= 6;		//공격력

	m_fSpeedX		= 10;		//속도
	m_fSpeedY		= 10;
	m_tInfo.fCX		= 64.f;
	m_tInfo.fCY		= 64.f;

	m_tFrame		= FRAME(0, 3, 0, 60);
	m_tExpFrame		= FRAME(0, 15, 0, 100);
	m_tTargetInfo	= INFO(m_tInfo.fX, m_tInfo.fY, 50.f, 50.f);
	m_tMeteoInfo	= INFO(m_tInfo.fX, m_tInfo.fY - float(rand()%2500 + 1000.f), m_tInfo.fCX, m_tInfo.fCY);

	m_dwExpTime		= GetTickCount();
	m_dwMeteoTime	= GetTickCount();

	m_bExplosion	= false;

}
int CSkillMeteo::Progress(void)
{
	
	if(m_dwMeteoTime + m_tFrame.dwTime < GetTickCount())
	{
		m_dwMeteoTime = GetTickCount();
		++m_tFrame.iStart;
		if( m_tFrame.iStart > m_tFrame.iLast)
			m_tFrame.iStart = 0;
	}
	if(m_bExplosion)
	{
		if(m_dwExpTime + m_tExpFrame.dwTime < GetTickCount())
		{
			m_dwExpTime = GetTickCount();
			if( m_tExpFrame.iStart == 1)
				CDevice::GetInstance()->SoundPlay(BOOM, 0);
			++m_tExpFrame.iStart;
			if( m_tExpFrame.iStart > m_tExpFrame.iLast)
				return 1;
		}
	}
	if(m_tMeteoInfo.fY + 100.f < m_tTargetInfo.fY)
	{
		m_fSpeedY += 0.5f;
		m_tMeteoInfo.fY += m_fSpeedY;
	}
	else
	{
		m_bExplosion = true;
	}

	return 0;
}
void CSkillMeteo::Render(HDC hdc)
{
	/*Rectangle(hdc,
		int(m_tMeteoInfo.fX - m_tTargetInfo.fCX / 1.4f),
		int(m_tMeteoInfo.fY - m_tTargetInfo.fCY / 2.f),
		int(m_tMeteoInfo.fX + m_tTargetInfo.fCX / 1.4f),
		int(m_tMeteoInfo.fY + m_tTargetInfo.fCY / 2.f));*/


	TransparentBlt(hdc,										//메테오
		int(m_tMeteoInfo.fX - m_tMeteoInfo.fCX / 2.f),
		int(m_tMeteoInfo.fY - m_tMeteoInfo.fCY / 2.f),
		int(m_tMeteoInfo.fCX), int(m_tMeteoInfo.fCY),
		(*m_pBitMap)["MeteoEffect"]->GetMemDC(),
		0,	0,
		int(m_tMeteoInfo.fCX), int(m_tMeteoInfo.fCY),
		RGB(63, 16, 123));

	if(m_bExplosion)
	{
		TransparentBlt(hdc,						//폭팔이펙트
			int(m_tInfo.fX - 43.f),
			int(m_tInfo.fY - 130.f),
			100, 100,
			(*m_pBitMap)["explosion"]->GetMemDC(),
			int(96 * m_tExpFrame.iStart),			//이미지크기 96x96
			int(96 * m_tExpFrame.iScene),
			96, 96,
			RGB(61, 16, 123));
	}
}
void CSkillMeteo::Release(void)
{
}
RECT CSkillMeteo::GetCOlRect()
{
	if(m_bExplosion)
	{
		RECT rc = {
			int(m_tMeteoInfo.fX - m_tTargetInfo.fCX / 1.4f),
			int(m_tMeteoInfo.fY - m_tTargetInfo.fCY / 2.f),
			int(m_tMeteoInfo.fX + m_tTargetInfo.fCX / 1.4f),
			int(m_tMeteoInfo.fY + m_tTargetInfo.fCY / 2.f)};
	
		return rc;
	}
	else
	{
		RECT rc = {
			0,
			0,
			0,
			0};
	
		return rc;
	}
}