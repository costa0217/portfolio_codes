#include "StdAfx.h"
#include "Boss.h"

#include "ObjMgr.h"
#include "ObjFactory.h"

#include "MonBullet.h"
#include "BossSkill.h"

#include "Device.h"

CBoss::CBoss(void)
:m_dwBossTime(GetTickCount())
{
}

CBoss::~CBoss(void)
{
}
void CBoss::Initialize(void)
{
	m_eMonsterType	= MON_BOSS;
	m_strKey		= "BossLeft";
	m_fSpeedX		= 0;
	m_fSpeedY		= 0;
	m_iHP			= 50;
	m_iMaxHP		= m_iHP;

	m_iSecPatt	= 0.0f;

	m_tInfo.fCX = 110.f;
	m_tInfo.fCY = 142.f;

	m_tColInfo.fCX = m_tInfo.fCX / 1.2f;
	m_tColInfo.fCY = m_tInfo.fCY / 2.5f;

	m_tFrame	= FRAME(0, 3, 0, 300);
	m_tMonDead	= FRAME(0, 15, 0, 70);			//죽는모션
	m_tPoofFrame= FRAME(0, 15, 0, 70);			//등장모션

	m_dwPoofTime= GetTickCount();

	m_dwPattBasicTime	= GetTickCount();

	m_dwPattFstTime		= GetTickCount();
	m_dwPattSecTime		= GetTickCount();
	m_dwPattSec2Time	= GetTickCount();

	m_dwPattTrdTime		= GetTickCount();
	m_dwPattFndTime		= GetTickCount();


	m_appear		= false;
	m_bNotAttack	= false;

	m_iPatternCnt = 0;
	m_iPatternNum = 0;

	m_iPattFndTime	= 0;

	m_tHealthbar	= INFO(325.f, 135.f, 150.f, 64.f);

}
int CBoss::Progress(void)
{
	if(m_bNotAttack)
	{
		m_tColInfo.fCX = 0;
		m_tColInfo.fCY = 0;
	}
	else
	{
		m_tColInfo.fCX = m_tInfo.fCX / 1.2f;
		m_tColInfo.fCY = m_tInfo.fCY / 2.5f;
	}

	if(m_iHP <= 0)
	{	
		m_bNotAttack = true;
		if(m_dwBossTime + m_tMonDead.dwTime < GetTickCount())
		{
			m_dwBossTime = GetTickCount();
			if(m_tMonDead.iStart == 1)
				CDevice::GetInstance()->SoundPlay(BOSSDEAD, 0);
			++m_tMonDead.iStart;
			if(m_tMonDead.iStart >= m_tMonDead.iLast)
				return 1;
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
		else
		{
			m_tInfo.fX += m_fSpeedX;
			m_tInfo.fY += m_fSpeedY;

			if(m_dwPattBasicTime + 3000 < GetTickCount()) /* 베이직 패턴 */
			{
				m_dwPattBasicTime = GetTickCount();
				if(!m_bNotAttack)
					Basicpattern();
			}
			if(m_tFrame.iScene == 0 && m_dwBossTime + m_tFrame.dwTime < GetTickCount())
			{
				m_tFrame.iScene = 0;
				m_dwBossTime = GetTickCount();
				++m_tFrame.iStart;
				if(m_tFrame.iStart > m_tFrame.iLast)
				{
					m_tFrame.iStart = 0;
					Movepattern();	/* 이동패턴 */
					++m_iPatternCnt;
				}
			}	

			if( m_iPatternCnt > 2 )
			{
				CDevice::GetInstance()->SoundPlay(BOSSROAR, 0);
				m_iPatternNum	= rand()%4 + 1;	//패턴 선택
				m_iPatternCnt	= 0;
				m_tFrame.iStart = 0;

				m_fSpeedX	= 0;
				m_fSpeedY	= 0;
			}

			if(m_iPatternNum == 1)				// 패턴 1
			{
				m_fSpeedX	= 0;
				m_fSpeedY	= 0;
				m_tFrame.iScene = 1;
				if(m_dwPattFstTime + m_tFrame.dwTime < GetTickCount())
				{
					m_dwPattFstTime = GetTickCount();
					++m_tFrame.iStart;
					if(m_tFrame.iStart > m_tFrame.iLast)
					{
						for(int i = 0; i < 20; ++i)
						{
							CObj* pPlayerBullet = CObjFactory<CMonBullet>::CreateObj(m_tInfo.fX, m_tInfo.fY , 0.3306f * i);
							m_pBulletlist->push_back(pPlayerBullet);
						}
						m_tFrame.iScene = 0;
						m_iPatternNum	= 0;
					}
				}
			}
			else if(m_iPatternNum == 2)			// 패턴  2
			{
				m_fSpeedX	= 0;
				m_fSpeedY	= 0;
				m_tFrame.iScene = 1;
				if(m_dwPattSecTime + m_tFrame.dwTime - 250 < GetTickCount())
				{
					m_dwPattSecTime = GetTickCount();
					++m_tFrame.iStart;
					if(m_tFrame.iStart > m_tFrame.iLast)
					{
						m_tFrame.iStart -= 2;

						m_iSecPatt	+= 0.1f;
						for(int i = 0; i < 4; ++i)
						{
							CObj* pPlayerBullet = CObjFactory<CMonBullet>::CreateObj(m_tInfo.fX, m_tInfo.fY , 1.57f * i + m_iSecPatt);
							m_pBulletlist->push_back(pPlayerBullet);
						}
						if(m_iSecPatt >= 3)
						{
							m_tFrame.iScene = 0;
							m_iPatternNum	= 0;
							m_iSecPatt		= 0;
						}
					}
				}
			}
			else if(m_iPatternNum == 3)			// 패턴  3
			{
				m_fSpeedX	= 0;
				m_fSpeedY	= 0;
				m_tFrame.iScene = 1;
				if(m_dwPattSecTime + m_tFrame.dwTime < GetTickCount())
				{
					m_dwPattSecTime = GetTickCount();
					++m_tFrame.iStart;
					if(m_tFrame.iStart > m_tFrame.iLast)
					{
						CDevice::GetInstance()->SoundPlay(BOSSKILLHILL, 0);
						CObj* pPlayerBullet = CObjFactory<CBossSkill>::CreateObj(float(rand()%449 + 173), float(rand()%160 + 265));
						m_pBulletlist->push_back(pPlayerBullet);

						m_tFrame.iScene = 0;
						m_iPatternNum	= 0;
					}
				}				
			}
			else if(m_iPatternNum == 4)
			{
				
				m_tFrame.iScene = 2;
				if(m_dwPattFndTime + m_tFrame.dwTime < GetTickCount())
				{
					m_dwPattFndTime = GetTickCount();
					
					if(m_tFrame.iStart == 0)
					{
						m_tFrame.iStart = 1;
					}

					else if(m_tFrame.iStart == 1)
					{
						CDevice::GetInstance()->SoundPlay(BOSSWING, 0);
						m_bNotAttack	= true;
						PX = m_pPlayer->GetInfo().fX;
						PY = m_pPlayer->GetInfo().fY;
						//m_tInfo.fX = PX;
						m_fSpeedY	-= 70.f;
						m_tInfo.fY	+= m_fSpeedY;

						
						if(m_iPattFndTime >= 3)
						{
							m_fSpeedY	= 0;
							m_iPattFndTime	= 0;
							m_tFrame.iStart = 2;
						}
						++m_iPattFndTime;
					}
					else if(m_tFrame.iStart == 2)
					{
						
						m_tInfo.fX = PX;
						m_tInfo.fY = PY;
						
						++m_iSecPatt;
						if(m_iSecPatt > 2)
						{
							m_iSecPatt = 0;
							m_tFrame.iStart = 3;
							m_bNotAttack	= false;

						}

					}
					else if(m_tFrame.iStart == 3)
					{
						
						CDevice::GetInstance()->SoundPlay(BOSS_GROUNDPOUND, 0);
						m_tFrame.iStart	= 0;
						m_tFrame.iScene = 0;
						m_iPatternNum	= 0;
					}
				}
			}
		}
	}
	return 0;
}
void CBoss::Render(HDC hdc)
{
	//m_tHealthbar	= INFO(325.f, 135.f, 150.f, 64.f);

	TransparentBlt(hdc,								//체력
		int(250.f + (m_iMaxHP - m_iHP) / 1.5f),
		int(135.f - 32.f),
		int(m_iHP * 6.f), int(64.f),
		(*m_pBitMap)["bosshealthbar"]->GetMemDC(),
		int(150 * 0),	
		int(32 * 0),
		150, 32,
		RGB(127, 0, 163));

	TransparentBlt(hdc,								//체력바
		int(325.f - 75.f),
		int(135.f - 32.f),
		int(150.f * 2.f), int(64.f),
		(*m_pBitMap)["bosshealthbar"]->GetMemDC(),
		int(150 * 0),	
		int(32 * 1),
		150, 32,
		RGB(127, 0, 163));



	if(m_iHP <= 0)
	{
		TransparentBlt(hdc,						
			int(m_tInfo.fX - m_tInfo.fCX + 5),
			int(m_tInfo.fY - m_tInfo.fCY + 10),
			200, 200,
			(*m_pBitMap)["MonsterDead"]->GetMemDC(),
			int(64 * m_tMonDead.iStart),	
			int(64 * m_tMonDead.iScene),
			64, 64,
			RGB(61, 16, 123));
	}

	else
	{
		/*Rectangle(hdc,
			int(m_tInfo.fX - m_tInfo.fCX / 2.4f),
			int(m_tInfo.fY - m_tInfo.fCY / 5.f),
			int(m_tInfo.fX + m_tInfo.fCX / 2.4f),
			int(m_tInfo.fY + m_tInfo.fCY / 5.f));*/


		//if(!m_bNotAttack)
		{
			TransparentBlt(hdc,										//그림자
				int(m_tInfo.fX - m_tInfo.fCX / 2.4f),
				int(m_tInfo.fY + m_tInfo.fCY / 8.f),
				90, 20,
				(*m_pBitMap)["shadow"]->GetMemDC(),
				0, 0,
				120, 49,
				RGB(61, 16, 123));
		}

		TransparentBlt(hdc,								//보스
			int(m_tInfo.fX - m_tInfo.fCX / 2.f),
			int(m_tInfo.fY - m_tInfo.fCY / 1.3f),
			110, 142,
			(*m_pBitMap)[m_strKey]->GetMemDC(),
			int(80 * m_tFrame.iStart),	
			int(112 * m_tFrame.iScene),
			80, 112,
			RGB(61, 16, 123));




		if(!m_appear)
		{
			TransparentBlt(hdc,						
				int(m_tInfo.fX - m_tInfo.fCX - 7),
				int(m_tInfo.fY - m_tInfo.fCY),
				250, 200,
				(*m_pBitMap)["poof2"]->GetMemDC(),
				int(95 * m_tPoofFrame.iStart),	
				int(95 * m_tPoofFrame.iScene),
				95, 95,
				RGB(61, 16, 123));
		}
	}
}
void CBoss::Release(void)
{
}
void CBoss::SetPlayer(CObj*	_pPlayer)
{
	m_pPlayer = _pPlayer;
}
void CBoss::SetBulletlist(list<CObj*>* _pBulletlist)
{
	m_pBulletlist = _pBulletlist;
}
void CBoss::Basicpattern()
{

	float fDistance = sqrtf(pow(m_pPlayer->GetInfo().fX - m_tInfo.fX, 2)
		+ pow(m_pPlayer->GetInfo().fY - m_tInfo.fY, 2));
	float thetaAngle = acos(  (m_pPlayer->GetInfo().fX - m_tInfo.fX) / fDistance );

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
void CBoss::Movepattern()
{

	/*이동패턴*/
	m_iDirLR = rand()%6;
	m_iDirUD = rand()%6;

	if(m_tInfo.fX < 173.f)
	{
		m_strKey	= "BossRight";
		m_fSpeedX	= 3.5f;
	}
	else if(m_tInfo.fX > 622.f)
	{
		m_strKey	= "BossLeft";
		m_fSpeedX	= -3.5f;
	}
	else
	{
		if(m_iDirLR == 0)
		{
			m_strKey	= "BossRight";
			m_fSpeedX	= 4;
		}
		else if(m_iDirLR == 1)
		{
			m_strKey	= "BossRight";
			m_fSpeedX	= 3;
		}
		else if(m_iDirLR == 2)
		{
			m_strKey	= "BossLeft";
			m_fSpeedX	= -3;
		}
		else if(m_iDirLR == 3)
		{
			m_strKey	= "BossLeft";
			m_fSpeedX	= -4;
		}
		else
			m_fSpeedX = 0;
	}

	if(m_tInfo.fY < 265.f)
		m_fSpeedY = 3.5f;
	else if(m_tInfo.fY > 425.f)
		m_fSpeedY = -3.5f;
	else
	{
		if(m_iDirUD == 0)
			m_fSpeedY = -4;
		else if(m_iDirUD == 1)
			m_fSpeedY = -2;
		else if(m_iDirUD == 2)
			m_fSpeedY = 2;
		else if(m_iDirUD == 3)
			m_fSpeedY = 4;
		else
			m_fSpeedY = 0;
	}
}