#include "StdAfx.h"
#include "Fly.h"
#include "Isaac.h"

#include "Device.h"

CFly::CFly(void)
:m_dwFlyTime(GetTickCount())
{
}

CFly::~CFly(void)
{
}

void CFly::Initialize(void)
{
	//m_tInfo		= INFO( WINCX / 3.f, WINCY / 3.f, 64.f, 64.f);
	m_eMonsterType = MON_FLY;

	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 64.f;
	m_tColInfo.fCX = m_tInfo.fCX / 5.f;
	m_tColInfo.fCY = m_tInfo.fCY / 5.f;
	m_tPoofFrame= FRAME(0, 15, 0, 50);
	m_tFrame	= FRAME(rand()%5, 10, 0, 50);

	m_iHP		= 3;		//HP	: 2
	m_fSpeedX	= 2;		//speed : 3
	m_fSpeedY	= 2;
	thetaAngle	= 0.0f;
	PX			= 0;
	PY			= 0;

	m_dwPoofTime	= GetTickCount();
	m_appear		= false;

	m_fAngle = 0;
}
int CFly::Progress(void)
{
	if(m_iHP <= 0)
	{
		m_tFrame.iScene = 1;
		if(m_dwFlyTime + m_tFrame.dwTime < GetTickCount())
		{
			m_dwFlyTime = GetTickCount();
			++m_tFrame.iStart;
			if(m_tFrame.iStart == 3)
				CDevice::GetInstance()->SoundPlay(MONSTERDEAD, 0);
			if(m_tFrame.iStart >= m_tFrame.iLast /*fly모션 마지막장면*/)
			{
				return 1;
			}
		}
	}

	//return 1;

	else
	{
		if(!m_appear)
		{
			if(m_dwPoofTime + m_tPoofFrame.dwTime < GetTickCount())
			{
				m_dwPoofTime = GetTickCount();
				++m_tPoofFrame.iStart;
				if(m_tPoofFrame.iStart >= m_tPoofFrame.iLast)
				{
					CDevice::GetInstance()->SoundPlay(FLY, 0);
					m_appear = true;
				}
			}
		}
		else
		{
			PX	=	m_pPlayer->GetInfo().fX;
			PY	=	m_pPlayer->GetInfo().fY;

			float b = sqrtf(pow(PX - m_tInfo.fX, 2) + pow(PY - m_tInfo.fY, 2));

			thetaAngle = acos((PX - m_tInfo.fX) / b);


			if(!((CIsaac*)m_pPlayer)->GetOnHit())
			{

				if(PY - m_tInfo.fY > 0)
				{
					m_tInfo.fX += m_fSpeedX * cosf(thetaAngle) + 2 * sinf(m_fAngle);
					m_tInfo.fY += m_fSpeedY * sinf(thetaAngle) + 2 * sinf(m_fAngle);
					m_fAngle += 0.05f;
				}
				else
				{
					m_tInfo.fX -= m_fSpeedX * cosf(float(3.141592) - thetaAngle) + 2 * sinf(m_fAngle);
					m_tInfo.fY -= m_fSpeedY * sinf(float(3.141592) - thetaAngle) + 2 * sinf(m_fAngle);
					m_fAngle += 0.05f;
				}
			}
			else
			{
				if(PY - m_tInfo.fY > 0)
				{
					m_tInfo.fX -= (m_fSpeedX * cosf(thetaAngle) - sinf(m_fAngle))/2.f;
					m_tInfo.fY -= (m_fSpeedY * sinf(thetaAngle) - sinf(m_fAngle))/2.f;	
					m_fAngle += 0.05f;
				}
				else
				{
					m_tInfo.fX += (m_fSpeedX * cosf(float(3.141592) - thetaAngle) - sinf(m_fAngle))/2.f;
					m_tInfo.fY += (m_fSpeedY * sinf(float(3.141592) - thetaAngle) - sinf(m_fAngle))/2.f;
					m_fAngle += 0.05f;
				}
			}
		}

		if(m_dwFlyTime + m_tFrame.dwTime < GetTickCount())
		{
			m_dwFlyTime = GetTickCount();
			++m_tFrame.iStart;
			if(m_tFrame.iStart >= 5 /*fly모션 마지막장면*/)
			{
				
				m_tFrame.iStart = 0;
			}
		}
	}
	return 0;
}
void CFly::Render(HDC hdc)
{
	/*Rectangle(hdc,
		int(m_tInfo.fX - m_tInfo.fCX / 5.f),
		int(m_tInfo.fY - m_tInfo.fCY / 5.f),
		int(m_tInfo.fX + m_tInfo.fCX / 5.f),
		int(m_tInfo.fY + m_tInfo.fCY / 5.f));*/

	TransparentBlt(hdc,										//그림자
			int(m_tInfo.fX - m_tInfo.fCX / 8.f),
			int(m_tInfo.fY + m_tInfo.fCY / 3.f),
			int(m_tInfo.fCX / 4.f), int(m_tInfo.fCY / 6.f),
			(*m_pBitMap)["shadow"]->GetMemDC(),
			0, 0,
			120, 49,
			RGB(61, 16, 123));

	if(!m_appear)
	{
		TransparentBlt(hdc,						
			int(m_tInfo.fX - m_tInfo.fCX / 2.f),
			int(m_tInfo.fY - m_tInfo.fCY / 2.f),
			64, 64,
			(*m_pBitMap)["poof2"]->GetMemDC(),
			int(95 * m_tPoofFrame.iStart),	
			int(95 * m_tPoofFrame.iScene),
			95, 95,
			RGB(61, 16, 123));
	}

	TransparentBlt(hdc,						
		int(m_tInfo.fX - m_tInfo.fCX / 2.f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		(*m_pBitMap)["fly"]->GetMemDC(),
		int(m_tInfo.fCX * m_tFrame.iStart),	
		int(m_tInfo.fCY * m_tFrame.iScene),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		RGB(61, 16, 123));

}
void CFly::Release(void)
{

}
void CFly::SetPlayer(CObj*	_pPlayer)
{
	m_pPlayer = _pPlayer;
}

//RECT CFly::GetColRect()
//{
//
//	RECT rc = {
//		int(m_tInfo.fX - m_tInfo.fCX / 7.f),
//		int(m_tInfo.fY - m_tInfo.fCY / 7.f),
//		int(m_tInfo.fX + m_tInfo.fCX / 7.f),
//		int(m_tInfo.fY + m_tInfo.fCY / 7.f)};
//
//		return rc;
//}
