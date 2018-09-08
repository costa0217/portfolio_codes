#include "StdAfx.h"
#include "PlayerSnake.h"

#include "Device.h"

CPlayerSnake::CPlayerSnake(void)
{
}

CPlayerSnake::~CPlayerSnake(void)
{
}
void CPlayerSnake::Initialize(void)
{
	m_eBulletType = BUL_PLAYERSNAKE;
	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 64.f;
	m_bPoof		= false;

	m_fAngle		= 0;
	m_iHP			= 2;		//공격력
	m_fSpeedX		= 5.f;		//속도
	m_fSpeedY		= 5.f;
	m_fMyAngle		= 0.f;

	m_tFrame		= FRAME(0, 15, 0, 25);

	m_tShadowInfo	= m_tInfo;
	m_dwBulletTime	= GetTickCount();

}
int CPlayerSnake::Progress(void)
{
	if(m_tFrame.iStart == m_tFrame.iLast)		
		return 1;

	if(!m_bPoof)
	{
		m_fMyAngle += 0.2f;

		if(m_fAngle == 0.f)
		{
			m_tInfo.fX +=  10 * cos(m_fMyAngle);
			m_tInfo.fY -=  m_fSpeedY;

			m_tShadowInfo.fX = m_tInfo.fX;
			m_tShadowInfo.fY = m_tInfo.fY;
		}
		else if(m_fAngle == 1.57f)
		{
			m_tInfo.fX +=  m_fSpeedX;
			m_tInfo.fY +=  10 * cos(m_fMyAngle);

			m_tShadowInfo.fX = m_tInfo.fX;
			m_tShadowInfo.fY = m_tInfo.fY;
		}
		else if(m_fAngle == 3.14f)
		{
			m_tInfo.fX -=  10 * cos(m_fMyAngle);
			m_tInfo.fY +=  m_fSpeedY;

			m_tShadowInfo.fX = m_tInfo.fX;
			m_tShadowInfo.fY = m_tInfo.fY;
		}
		else if(m_fAngle == 4.71f)
		{
			m_tInfo.fX -=  m_fSpeedX;
			m_tInfo.fY -=  10 * cos(m_fMyAngle);

			m_tShadowInfo.fX = m_tInfo.fX;
			m_tShadowInfo.fY = m_tInfo.fY;
		}
	}
	return 0;
}
void CPlayerSnake::Render(HDC hdc)
{
	if(m_bPoof)
	{
		if(m_dwBulletTime + m_tFrame.dwTime < GetTickCount())
		{
			if(m_tFrame.iStart == 0)
				CDevice::GetInstance()->SoundPlay(BULLETEND, 0);

			m_dwBulletTime = GetTickCount();
			++m_tFrame.iStart;
		}
	}
		//Rectangle(hdc,
		//int(m_tInfo.fX - m_tInfo.fCX / 6.f),
		//	int(m_tInfo.fY - m_tInfo.fCY / 6.f),
		//	int(m_tInfo.fX + m_tInfo.fCX / 6.f),
		//	int(m_tInfo.fY + m_tInfo.fCY / 6.f));


	if(!m_bPoof)
	{
	TransparentBlt(hdc,						//그림자
			int(m_tShadowInfo.fX - m_tInfo.fCX / 8.f),
			int(m_tShadowInfo.fY - m_tInfo.fCY / 2.f) + 50,
			int(m_tInfo.fCX / 4.f), int(m_tInfo.fCY / 6.f),
			(*m_pBitMap)["shadow"]->GetMemDC(),
			0, 0,
			120, 49,
			RGB(61, 16, 123));
	}

	TransparentBlt(hdc,						//총알
		int(m_tInfo.fX - m_tInfo.fCX / 2.f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		(*m_pBitMap)["PlayerTear"]->GetMemDC(),
		int(m_tInfo.fCX * m_tFrame.iStart),	
		int(m_tInfo.fCY * m_tFrame.iScene),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		RGB(61, 16, 123));




}
void CPlayerSnake::Release(void)
{
}

RECT CPlayerSnake::GetCOlRect()
{

	if(!m_bPoof)
	{
		RECT rc = {
			int(m_tInfo.fX - m_tInfo.fCX / 6.f),
			int(m_tInfo.fY - m_tInfo.fCY / 6.f),
			int(m_tInfo.fX + m_tInfo.fCX / 6.f),
			int(m_tInfo.fY + m_tInfo.fCY / 6.f)};
			return rc;
	}
	else
	{
		RECT rc = {
			int(m_tInfo.fX),
			int(m_tInfo.fY),
			int(m_tInfo.fX),
			int(m_tInfo.fY)};
			return rc;
	}
}
