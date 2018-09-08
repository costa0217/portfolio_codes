#include "StdAfx.h"
#include "Bomb.h"
#include "Device.h"

CBomb::CBomb(void)
:m_dwBombTime(GetTickCount())
,m_dwExpTime(GetTickCount())
,m_bExplosion(false)
,m_bExpColBox(false)
,m_bBombitSelf(true)
{
}

CBomb::~CBomb(void)
{
}

void CBomb::Initialize(void)
{
	m_eBulletType	= BUL_BOMB;
	m_tInfo.fCX		= 32.f;
	m_tInfo.fCY		= 32.f;
	m_tFrame		= FRAME(0, 11, 0, 150);
	m_tExpFrame		= FRAME(0, 15, 0, 100);
	m_iHP			= 4;
	//m_tColInfo		= INFO(m_tInfo.fX, m_tInfo.fY, 40.f, 70.f);

	m_fSpeedX		= 0;
	m_fSpeedY		= 0;
	m_fMaxSpeedX	= 7.f;
	m_fMaxSpeedY	= 7.f;
}
int CBomb::Progress(void)
{
	

	if(m_tExpFrame.iStart == m_tExpFrame.iLast)
		return 1;

	if(m_dwBombTime + m_tFrame.dwTime < GetTickCount())
	{
		m_dwBombTime = GetTickCount();
		++m_tFrame.iStart;
		if( m_tFrame.iStart == 2)
			CDevice::GetInstance()->SoundPlay(BOMBBEEB, 1);
		
		if( m_tFrame.iStart >= m_tFrame.iLast )
		{
			m_tFrame.iStart = 12;
			m_bExplosion	= true;
			m_bBombitSelf	= false;
			m_fSpeedX		= 0;
			m_fSpeedY		= 0;
			
		}
	}


	m_tInfo.fX += m_fSpeedX;	
	m_tInfo.fY += m_fSpeedY;

	if(m_fSpeedX > 0)
		m_fSpeedX -= 0.5;
	else if(m_fSpeedX < 0)
		m_fSpeedX += 0.5;
	if(m_fSpeedY > 0)
		m_fSpeedY -= 0.5;
	else if(m_fSpeedY < 0)
		m_fSpeedY += 0.5;

	if(m_bExplosion)
	{
		CDevice::GetInstance()->SoundStop(BOMBBEEB);
		if(m_dwExpTime + m_tExpFrame.dwTime < GetTickCount())
		{
			m_dwExpTime = GetTickCount();
			if(m_tExpFrame.iStart == 0)
				CDevice::GetInstance()->SoundPlay(BOOM, 0);

			++m_tExpFrame.iStart;
			if( m_tExpFrame.iStart == 2)
				m_bExpColBox = true;
		}
	}
	return 0;
}
void CBomb::Render(HDC hdc)
{
	/*Rectangle(hdc,
			int(m_tInfo.fX - 50.f),
			int(m_tInfo.fY - 50.f),
			int(m_tInfo.fX + 80.f),
			int(m_tInfo.fY + 80.f));*/
	TransparentBlt(hdc,						//∆¯≈∫
		int(m_tInfo.fX - m_tInfo.fCX / 2.f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f),
		60, 60,
		(*m_pBitMap)["Bomb"]->GetMemDC(),
		int(m_tInfo.fCX * m_tFrame.iStart),	
		int(m_tInfo.fCY * m_tFrame.iScene),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		RGB(61, 16, 123));

	if(m_bExplosion)
	{
		TransparentBlt(hdc,						//∆¯∆»¿Ã∆Â∆Æ
			int(m_tInfo.fX - 85.f),
			int(m_tInfo.fY - 150.f),
			200, 200,
			(*m_pBitMap)["explosion"]->GetMemDC(),
			int(96 * m_tExpFrame.iStart),			//¿ÃπÃ¡ˆ≈©±‚ 96x96
			int(96 * m_tExpFrame.iScene),
			96, 96,
			RGB(61, 16, 123));
	}
}
void CBomb::Release(void)
{
}
RECT CBomb::GetCOlRect()
{
	if(m_bExpColBox)
	{
		RECT rc = {
			int(m_tInfo.fX),
			int(m_tInfo.fY),
			int(m_tInfo.fX),
			int(m_tInfo.fY)};
			return rc;
	}
	else if(m_bBombitSelf && !m_bExplosion)
	{
		RECT rc = {
			int(m_tInfo.fX - m_tInfo.fCX / 4.f),
			int(m_tInfo.fY - m_tInfo.fCY / 4.f),
			int(m_tInfo.fX - (m_tInfo.fCX / 3.f) + 48),
			int(m_tInfo.fY - (m_tInfo.fCY / 3.f) + 48)};
			return rc;
	}
	else if(m_bExplosion && !m_bBombitSelf)
	{
		RECT rc = {
			int(m_tInfo.fX - 50.f),
			int(m_tInfo.fY - 50.f),
			int(m_tInfo.fX + 80.f),
			int(m_tInfo.fY + 80.f)};
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
