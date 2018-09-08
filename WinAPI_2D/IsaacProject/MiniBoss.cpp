#include "StdAfx.h"
#include "MiniBoss.h"
#include "ObjMgr.h"
#include "ObjFactory.h"

#include "Blood.h"
#include "MiniBoss2.h"
#include "Device.h"


CMiniBoss::CMiniBoss(void)
:m_dwMiniBossTime(GetTickCount())
{
}

CMiniBoss::~CMiniBoss(void)
{
}
void CMiniBoss::Initialize(void)
{
	m_eMonsterType	= MON_MINIBOSS;
	m_fSpeedX		= 3;
	m_fSpeedY		= 3;
	m_iHP			= 5;

	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 64.f;

	m_tColInfo.fCX = m_tInfo.fCX;


	m_tColInfo.fCY = m_tInfo.fCY;

	m_tFrame	= FRAME(0, 4, 0, 200);
	m_tMonDead	= FRAME(0, 15, 0, 70);			//죽는모션
	m_tPoofFrame= FRAME(0, 15, 0, 70);			//등장모션

	m_dwPoofTime= GetTickCount();
	m_appear	= false;

	m_eMiniDIR	= DIR_RD;

}
int CMiniBoss::Progress(void)
{
	if(m_iHP <= 0)
	{	
		if(m_dwMiniBossTime + m_tMonDead.dwTime < GetTickCount())
		{
			if(m_tMonDead.iStart == 1)
			{
				
				CObj* pMonster = CObjFactory<CMiniBoss2>::CreateObj(m_tInfo.fX, m_tInfo.fY, 0);
				m_pMonsterlist->push_back(pMonster);
				((CMiniBoss2*)pMonster)->SetBloodlist(m_pBloodlist);
				pMonster = CObjFactory<CMiniBoss2>::CreateObj(m_tInfo.fX, m_tInfo.fY, 3);
				m_pMonsterlist->push_back(pMonster);
				((CMiniBoss2*)pMonster)->SetBloodlist(m_pBloodlist);

				CDevice::GetInstance()->SoundPlay(MONSTERDEAD, 0);
				CObj* pBlood = CObjFactory<CBlood>::CreateObj(m_tInfo.fX, m_tInfo.fY);
				m_pBloodlist->push_back(pBlood);

				for(list<CObj*>::iterator iter = m_pMonsterlist->begin();
					iter != m_pMonsterlist->end(); ++iter)
				{
					if( ((CMonster*)(*iter))->GetMonsterID() == MON_MINIBOSS2)
						((CMiniBoss2*)(*iter))->SetMonsterlist(m_pMonsterlist);
				}
				
			}
			m_dwMiniBossTime = GetTickCount();
			++m_tMonDead.iStart;
			
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
		else
		{
			if(m_eMiniDIR == DIR_RD)
			{
				m_tInfo.fX += m_fSpeedX;
				m_tInfo.fY += m_fSpeedY;
			}
			if(m_eMiniDIR == DIR_RU)
			{
				m_tInfo.fX += m_fSpeedX;
				m_tInfo.fY -= m_fSpeedY;
			}
			if(m_eMiniDIR == DIR_LD)
			{
				m_tInfo.fX -= m_fSpeedX;
				m_tInfo.fY += m_fSpeedY;
			}
			if(m_eMiniDIR == DIR_LU)
			{
				m_tInfo.fX -= m_fSpeedX;
				m_tInfo.fY -= m_fSpeedY;
			}

			if(m_dwMiniBossTime + m_tFrame.dwTime < GetTickCount())
			{
				m_dwMiniBossTime = GetTickCount();
				++m_tFrame.iStart;
				if(m_tFrame.iStart >= m_tFrame.iLast)
				{
					CDevice::GetInstance()->SoundPlay(MONSTERGRUNT1, 0);
					m_tFrame.iStart = 0;
				}
			}
		}
	}

	return 0;
}
void CMiniBoss::Render(HDC hdc)
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
			int(m_tInfo.fY + m_tInfo.fCY/3.f),
			52, 20,
			(*m_pBitMap)["shadow"]->GetMemDC(),
			0, 0,
			120, 49,
			RGB(61, 16, 123));

		TransparentBlt(hdc,						
			int(m_tInfo.fX - m_tInfo.fCX / 2.f),
			int(m_tInfo.fY - m_tInfo.fCY / 2.f),
			int(m_tInfo.fCX) + 10, int(m_tInfo.fCY) + 10,
			(*m_pBitMap)["miniboss"]->GetMemDC(),
			int(m_tInfo.fCX * m_tFrame.iStart),	
			int(m_tInfo.fCY * m_tFrame.iScene),
			64, 64,
			RGB(61, 16, 123));
		if(!m_appear)
		{
			TransparentBlt(hdc,						
				int(m_tInfo.fX - m_tInfo.fCX - 7),
				int(m_tInfo.fY - m_tInfo.fCY),
				160, 110,
				(*m_pBitMap)["poof2"]->GetMemDC(),
				int(95 * m_tPoofFrame.iStart),	
				int(95 * m_tPoofFrame.iScene),
				95, 95,
				RGB(61, 16, 123));
		}
	}
}
void CMiniBoss::Release(void)
{
}
void CMiniBoss::SetMonsterlist(list<CObj*>* _pMonsterlist)
{
	m_pMonsterlist = _pMonsterlist;
}
void CMiniBoss::SetBloodlist(list<CObj*>* _pBloodlist)
{
	m_pBloodlist = _pBloodlist;
}