#include "StdAfx.h"
#include "PlayerKnife.h"

#include "Device.h"

CPlayerKnife::CPlayerKnife(void)
{
}

CPlayerKnife::~CPlayerKnife(void)
{
	Release();
}

void CPlayerKnife::Initialize(void)
{
	m_eBulletType = BUL_PLAYERKNIFE;
	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;

	m_fAngle		= 0;
	m_iHP			= 2;		//공격력
	m_fSpeedX		= 10;		//속도
	m_fSpeedY		= 10;

	m_bSting = false;


	m_iDirX	= 1;
	m_iDirY	= 1;	
	m_fTime = 0.f;
}
int CPlayerKnife::Progress(void)
{
	m_fTime += 0.2f;
	if(!m_bSting)
	{
		m_tPlayInfo.fX = m_tInfo.fX;
		m_tPlayInfo.fY = m_tInfo.fY;
		m_bSting = true;
		CDevice::GetInstance()->SoundPlay(SWORD, 0);
	}
	
	if(m_fAngle == 0.f)
	{
		m_iDirKnife = 2;

		m_tInfo.fY +=  -m_fSpeedY + (m_fTime * m_fTime); 

		if(m_tInfo.fY >= m_tPlayInfo.fY - 5)
			return 1;
	}
	else if(m_fAngle == 1.57f)
	{
		m_iDirKnife = 1;

		m_tInfo.fX -=  -m_fSpeedX + (m_fTime * m_fTime); 

		if(m_tInfo.fX <= m_tPlayInfo.fX + 5)
			return 1;
	}
	else if(m_fAngle == 3.14f)
	{
		m_iDirKnife = 0;

		m_tInfo.fY -=  -m_fSpeedY + (m_fTime * m_fTime); 

		if(m_tInfo.fY <= m_tPlayInfo.fY + 5)
			return 1;
	}
	else if(m_fAngle == 4.71f)
	{
		m_iDirKnife = 3;

		m_tInfo.fX +=  -m_fSpeedX + (m_fTime * m_fTime); 

		if(m_tInfo.fX >= m_tPlayInfo.fX - 5)
			return 1;
	}
	return 0;
}
void CPlayerKnife::Render(HDC hdc)
{

	TransparentBlt(hdc,								
		int(m_tInfo.fX - m_tInfo.fCX / 2.f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		(*m_pBitMap)["KnifeImage"]->GetMemDC(),
		int(32 * m_iDirKnife),	
		0,
		32, 32,
		RGB(61, 16, 123));

	//Rectangle(hdc,
	//	int(m_tInfo.fX - m_tInfo.fCX / 2.f),
	//	int(m_tInfo.fY - m_tInfo.fCY / 2.f),
	//	int(m_tInfo.fX + m_tInfo.fCX / 2.f),
	//	int(m_tInfo.fY + m_tInfo.fCY / 2.f));

}
void CPlayerKnife::Release(void)
{
}

RECT CPlayerKnife::GetCOlRect()
{

	RECT rc = {
		int(m_tInfo.fX - m_tInfo.fCX / 2.f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f),
		int(m_tInfo.fX + m_tInfo.fCX / 2.f),
		int(m_tInfo.fY + m_tInfo.fCY / 2.f)};
	
	return rc;

}
