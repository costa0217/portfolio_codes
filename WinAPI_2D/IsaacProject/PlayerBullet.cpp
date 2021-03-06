#include "StdAfx.h"
#include "PlayerBullet.h"
#include "ObjMgr.h"
#include "Isaac.h"
#include "Device.h"

CPlayerBullet::CPlayerBullet(void)
:m_dwBulletTime(0)
,m_bPoof(false)
{
}

CPlayerBullet::~CPlayerBullet(void)
{
	Release();
	
}
void CPlayerBullet::Initialize(void)
{
	m_eBulletType = BUL_PLAYER;
	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 64.f;

	m_strKey = "PlayerTear";
	
	m_fAngle		= 0;
	m_iHP			= 2;		//공격력
	m_fSpeedX		= 10;		//속도
	m_fSpeedY		= 10;
	m_fMaxSpeedX	= 7.f;		//최대속도
	m_fMaxSpeedY	= 7.f;
	m_tFrame		= FRAME(0, 15, 0, 25);
	m_fOriX			= m_tInfo.fX;
	m_fOriY			= m_tInfo.fY;

	m_fTime			= 0;
	m_tShadowInfo	= m_tInfo;

	m_iReach		= 300;

	//CDevice::GetInstance()->LoadWave(L"../Sound/Bullet.wav");  //4
	//CDevice::GetInstance()->LoadWave(L"../Sound/BulletEnd.wav");  //5
	CDevice::GetInstance()->SoundPlay(BULLET, 0);
	

}
int CPlayerBullet::Progress(void)
{
	if(((CIsaac*)m_pPlayer)->GetArrowType() == ARROW_BLOOD)
		m_strKey = "EnemyTear";

	if(m_tFrame.iStart == m_tFrame.iLast)		
		return 1;
		
	if(!m_bPoof)
	{
		if(DistanceMake() >= m_iReach - 100)		/*포물선운동*/
		{
			if(DistanceMake() >= m_iReach/*사거리*/)		/*사거리*/
			{
				m_bPoof = true;
				return 0;
			}
			m_tInfo.fX += m_fSpeedX * cosf(m_fAngle - 90 * 3.14f / 180 );				//눈물이동
			m_tInfo.fY += m_fSpeedY * sinf(m_fAngle - 90 * 3.14f / 180 ) + 3.f;
			m_tShadowInfo.fX += m_fSpeedX * cosf(m_fAngle - 90 * 3.14f / 180 );			//그림자이동
			m_tShadowInfo.fY += m_fSpeedY * sinf(m_fAngle - 90 * 3.14f / 180 ) + 1.f;
			return 0;
		}

		m_tInfo.fX += m_fSpeedX * cosf(m_fAngle - 90 * 3.14f / 180 );
		m_tInfo.fY += m_fSpeedY * sinf(m_fAngle - 90 * 3.14f / 180 );	
		m_tShadowInfo.fX = m_tInfo.fX;
		m_tShadowInfo.fY = m_tInfo.fY;
	}
	return 0;

}
void CPlayerBullet::Render(HDC hdc)
{
	if(m_bPoof)
	{
		if(m_tFrame.iStart == 0)
			CDevice::GetInstance()->SoundPlay(BULLETEND, 0);
		if(m_dwBulletTime + m_tFrame.dwTime < GetTickCount())
		{

			m_dwBulletTime = GetTickCount();
			++m_tFrame.iStart;
		}
	}

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
		(*m_pBitMap)[m_strKey]->GetMemDC(),
		int(m_tInfo.fCX * m_tFrame.iStart),	
		int(m_tInfo.fCY * m_tFrame.iScene),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		RGB(61, 16, 123));


}
void CPlayerBullet::Release(void)
{
}

float CPlayerBullet::DistanceMake()
{
	float fDistance = sqrtf(pow(m_tInfo.fX - m_fOriX, 2) + pow(m_tInfo.fY - m_fOriY, 2));

	return fDistance; 
}

RECT CPlayerBullet::GetCOlRect()
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
