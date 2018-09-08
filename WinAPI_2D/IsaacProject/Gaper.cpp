#include "StdAfx.h"
#include "Gaper.h"
#include "ObjMgr.h"
#include "ObjFactory.h"

#include "Blood.h"
#include "MonBullet.h"

#include "Device.h"

CGaper::CGaper(void)
:m_dwGaperTime(GetTickCount())
{
}

CGaper::~CGaper(void)
{
}

void CGaper::Initialize(void)
{
	m_eMonsterType = MON_GAPER;
	m_iHP		= 6;

	m_tInfo.fCX = 32.f;
	m_tInfo.fCY = 32.f;

	m_tColInfo.fCX = m_tInfo.fCX / 2.f;
	m_tColInfo.fCY = m_tInfo.fCY / 2.f;
	m_tFrame	= FRAME(0, 2, 1, 1000);
	m_tMonDead	= FRAME(0, 15, 0, 70);			//죽는모션
	m_tPoofFrame= FRAME(0, 15, 0, 70);			//등장모션

	m_fVibrateX = m_tInfo.fX;
	m_fVibrateY = m_tInfo.fY;
	thetaAngle	= 0.f;
	m_bLockOn	= false;	

	m_dwPoofTime= GetTickCount();
	m_dwShotTime= GetTickCount();

	m_appear	= false;
}

int CGaper::Progress(void)
{
	if(m_iHP <= 0)
	{	
		if(m_dwGaperTime + m_tMonDead.dwTime < GetTickCount())
		{
			m_dwGaperTime = GetTickCount();
			++m_tMonDead.iStart;
			if(m_tMonDead.iStart == 2)
			{

				CDevice::GetInstance()->SoundPlay(MONSTERDEAD, 0);
				CObj* pBlood = CObjFactory<CBlood>::CreateObj(m_tInfo.fX, m_tInfo.fY);
				m_pBloodlist->push_back(pBlood);	
			}
			if(m_tMonDead.iStart >= m_tMonDead.iLast)
			{
				return 1;
			}
		}
	}
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
					m_appear = true;
				}
			}
		}
		/* 진동 운동 */
		if(!m_bLockOn)
		{
			if(m_fVibrateX == m_tInfo.fX)
				m_tInfo.fX += (rand()%5 - 2);
			else if(m_fVibrateX > m_tInfo.fX)
				m_tInfo.fX += (rand()%3);
			else if(m_fVibrateX < m_tInfo.fX)
				m_tInfo.fX -= (rand()%3);

			if(m_fVibrateY == m_tInfo.fY)
				m_tInfo.fY += (rand()%5 - 2);
			else if(m_fVibrateY > m_tInfo.fY)
				m_tInfo.fY += (rand()%3);
			else if(m_fVibrateY < m_tInfo.fY)
				m_tInfo.fY -= (rand()%3);
		}

		/* 플레이어와 거리 계산*/
		float fDistance = sqrtf(pow(m_pPlayer->GetInfo().fX - m_tInfo.fX, 2)
			+ pow(m_pPlayer->GetInfo().fY - m_tInfo.fY, 2));

		thetaAngle = acos(  (m_pPlayer->GetInfo().fX - m_tInfo.fX) / fDistance );
		if (fDistance < 280)/*거리 안에 들어오면 락온*/
		{
			m_bLockOn		= true;
			//m_tFrame.iScene = 1;
			if(m_dwShotTime + m_tFrame.dwTime < GetTickCount())
			{
				m_dwShotTime	= GetTickCount();
				if(m_tFrame.iStart == 0)
					CDevice::GetInstance()->SoundPlay(MONSTER, 0);
				++m_tFrame.iStart;
				if(m_tFrame.iStart >= m_tFrame.iLast)
					m_tFrame.iStart = 0;

				else
				{
					if( m_pPlayer->GetInfo().fY - m_tInfo.fY > 0)
					{
						CObj* pMonsterBullet = CObjFactory<CMonBullet>::CreateObj(m_tInfo.fX, m_tInfo.fY, thetaAngle, 1);
						m_pBulletlist->push_back(pMonsterBullet);
					}
					else
					{
						CObj* pMonsterBullet = CObjFactory<CMonBullet>::CreateObj(m_tInfo.fX, m_tInfo.fY, float(3.141592) - thetaAngle, -1);
						m_pBulletlist->push_back(pMonsterBullet);
					}
				}
			}		
		}
		else
		{
			m_bLockOn		= false;
			m_tFrame.iStart = 0;
		}
	}
	return 0;
}
void CGaper::Render(HDC hdc)
{
	/*Rectangle(hdc,
	int(m_tInfo.fX - 25),
	int(m_tInfo.fY - 25),
	int(m_tInfo.fX + 25),
	int(m_tInfo.fY + 25));*/
	if(m_iHP <= 0)
	{
		TransparentBlt(hdc,						
			int(m_tInfo.fX - m_tInfo.fCX * 1.5f),
			int(m_tInfo.fY - m_tInfo.fCY * 1.3f),
			100, 100,
			(*m_pBitMap)["MonsterDead"]->GetMemDC(),
			int(64 * m_tMonDead.iStart),	
			int(64 * m_tMonDead.iScene),
			64, 64,
			RGB(61, 16, 123));
	}

	else
	{
		TransparentBlt(hdc,										//그림자
			int(m_tInfo.fX - m_tInfo.fCX/1.6f ),
			int(m_tInfo.fY + m_tInfo.fCY/5.f),
			41, 20,
			(*m_pBitMap)["shadow"]->GetMemDC(),
			0, 0,
			120, 49,
			RGB(61, 16, 123));

		TransparentBlt(hdc,						
			int(m_tInfo.fX - 25),
			int(m_tInfo.fY - 25),
			50, 50,
			(*m_pBitMap)["gaper"]->GetMemDC(),
			int(m_tInfo.fCX * m_tFrame.iStart),	
			int(m_tInfo.fCY * m_tFrame.iScene),
			32, 32,
			RGB(61, 16, 123));
		if(!m_appear)
		{
			TransparentBlt(hdc,						
				int(m_tInfo.fX - m_tInfo.fCX * 1.9f),
				int(m_tInfo.fY - m_tInfo.fCY * 1.9f),
				110, 110,
				(*m_pBitMap)["poof2"]->GetMemDC(),
				int(95 * m_tPoofFrame.iStart),	
				int(95 * m_tPoofFrame.iScene),
				95, 95,
				RGB(61, 16, 123));
		}
	}
}
void CGaper::Release(void)
{
}
void CGaper::SetPlayer(CObj*	_pPlayer)
{
	m_pPlayer = _pPlayer;
}
void CGaper::SetBulletlist(list<CObj*>* _pBulletlist)
{
	m_pBulletlist = _pBulletlist;
}
void CGaper::SetBloodlist(list<CObj*>* _pBloodlist)
{
	m_pBloodlist = _pBloodlist;
}