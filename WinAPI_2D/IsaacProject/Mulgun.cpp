#include "StdAfx.h"
#include "Mulgun.h"

#include "ObjMgr.h"
#include "ObjFactory.h"

#include "Blood.h"
#include "Fly.h"

#include "Device.h"

CMulgun::CMulgun(void)
{
}

CMulgun::~CMulgun(void)
{
}
void CMulgun::Initialize(void)
{
	m_eMonsterType	= MON_MULGUN;
	m_fSpeedX		= 0.5f;
	m_fSpeedY		= 0.5f;
	m_iHP			= 5;

	m_tInfo.fCX = 48.f;
	m_tInfo.fCY = 56.f;
	m_tColInfo.fCX = m_tInfo.fCX;
	m_tColInfo.fCY = m_tInfo.fCY;

	m_tFrame	= FRAME(0, 5, 0, 170);
	m_tMonDead	= FRAME(0, 15, 0, 70);			//죽는모션
	m_tPoofFrame= FRAME(0, 15, 0, 70);			//등장모션

	m_dwPoofTime		= GetTickCount();
	m_dwMulgunTime		= GetTickCount();
	m_iPatternCircle	= 0;
	m_appear	= false;
	
	PX = 0.f;
	PY = 0.f;
	thetaAngle = 0.f;
	m_iCircle = rand()%3 + 2;
}
int CMulgun::Progress(void)
{
	if(m_iHP <= 0)
	{	
		if(m_dwMulgunTime + m_tMonDead.dwTime < GetTickCount())
		{
			m_dwMulgunTime = GetTickCount();
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
					CDevice::GetInstance()->SoundPlay(MULGUN, 0);
					m_appear = true;
				}
			}
		}
		else
		{
			if(m_iPatternCircle == m_iCircle)
			{
				m_tFrame.iScene = 2;
				if(m_dwMulgunTime + m_tFrame.dwTime < GetTickCount())
				{
					m_dwMulgunTime = GetTickCount();
					++m_tFrame.iStart;
					if(m_tFrame.iStart > 2 )
					{
						CDevice::GetInstance()->SoundPlay(MULGUN, 0);
						CObj* pMonster = CObjFactory<CFly>::CreateObj(m_tInfo.fX, m_tInfo.fY);
						m_pMonsterlist->push_back(pMonster);
						((CFly*)pMonster)->SetPlayer(m_pPlayer);

						m_iPatternCircle = 0;	
						m_tFrame.iStart = 0;
						m_tFrame.iScene = 0;
						m_iCircle = rand()%3 + 2;
					}					
				}
			}
			else
			{	
				PX	=	m_pPlayer->GetInfo().fX;
				PY	=	m_pPlayer->GetInfo().fY;

				if(PX > m_tInfo.fX)
					m_tFrame.iScene = 0;
				else
					m_tFrame.iScene = 1;

				float b = sqrtf(pow(PX - m_tInfo.fX, 2) + pow(PY - m_tInfo.fY, 2));

				thetaAngle = acos((PX - m_tInfo.fX) / b);

				if(PY - m_tInfo.fY > 0)
				{
					m_tInfo.fX -= m_fSpeedX * cosf(thetaAngle);
					m_tInfo.fY -= m_fSpeedY * sinf(thetaAngle);	
				}
				else
				{
					m_tInfo.fX += m_fSpeedX * cosf(float(3.141592) - thetaAngle);
					m_tInfo.fY += m_fSpeedY * sinf(float(3.141592) - thetaAngle);
				}
			}
			if(m_iPatternCircle < m_iCircle && m_dwMulgunTime + m_tFrame.dwTime < GetTickCount())
			{
				m_dwMulgunTime = GetTickCount();
				++m_tFrame.iStart;
				if(m_tFrame.iStart >= m_tFrame.iLast )
				{
					m_tFrame.iStart = 0;
					++m_iPatternCircle;
				}
			}	
		}
	}

	return 0;
}
void CMulgun::Render(HDC hdc)
{
	if(m_iHP <= 0)
	{
		TransparentBlt(hdc,						
			int(m_tInfo.fX - m_tInfo.fCX + 5),
			int(m_tInfo.fY - m_tInfo.fCY),
			100, 100,
			(*m_pBitMap)["MonsterDead"]->GetMemDC(),
			int(64 * m_tMonDead.iStart),	
			int(64 * m_tMonDead.iScene),
			64, 64,
			RGB(61, 16, 123));
	}

	else
	{
		/*Rectangle(hdc,
			int(m_tInfo.fX - m_tInfo.fCX * 0.8f / 2.f),
			int(m_tInfo.fY - m_tInfo.fCY * 0.8f / 2.f),
			int(m_tInfo.fX + m_tInfo.fCX * 0.8f / 2.f),
			int(m_tInfo.fY + m_tInfo.fCY * 0.8f / 2.f));*/
		TransparentBlt(hdc,										//그림자
			int(m_tInfo.fX - m_tInfo.fCX/3.f ),
			int(m_tInfo.fY + m_tInfo.fCY/5.f),
			42, 15,
			(*m_pBitMap)["shadow"]->GetMemDC(),
			0, 0,
			120, 49,
			RGB(61, 16, 123));

		TransparentBlt(hdc,						
			int(m_tInfo.fX - m_tInfo.fCX / 2.f),
			int(m_tInfo.fY - m_tInfo.fCY / 2.f),
			int(m_tInfo.fCX) + 10, int(m_tInfo.fCY) + 10,
			(*m_pBitMap)["mulligan"]->GetMemDC(),
			int(m_tInfo.fCX * m_tFrame.iStart),	
			int(m_tInfo.fCY * m_tFrame.iScene),
			int(m_tInfo.fCX), int(m_tInfo.fCY),
			RGB(61, 16, 123));
		if(!m_appear)
		{
			TransparentBlt(hdc,						
				int(m_tInfo.fX - m_tInfo.fCX - 7),
				int(m_tInfo.fY - m_tInfo.fCY),
				110, 110,
				(*m_pBitMap)["poof2"]->GetMemDC(),
				int(95 * m_tPoofFrame.iStart),	
				int(95 * m_tPoofFrame.iScene),
				95, 95,
				RGB(61, 16, 123));
		}
	}
}
void CMulgun::Release(void)
{
}
void CMulgun::SetMonsterlist(list<CObj*>* _pMonsterlist)
{
	m_pMonsterlist = _pMonsterlist;
}
void CMulgun::SetPlayer(CObj*	_pPlayer)
{
	m_pPlayer = _pPlayer;
}
void CMulgun::SetBloodlist(list<CObj*>* _pBloodlist)
{
	m_pBloodlist = _pBloodlist;
}