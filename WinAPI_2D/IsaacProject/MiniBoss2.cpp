#include "StdAfx.h"
#include "MiniBoss2.h"
#include "ObjMgr.h"
#include "ObjFactory.h"

#include "MiniBoss4.h"
#include "Blood.h"
#include "Device.h"

CMiniBoss2::CMiniBoss2(void)
:m_dwMiniBoss2Time(GetTickCount())
{
}

CMiniBoss2::~CMiniBoss2(void)
{
}

void CMiniBoss2::Initialize(void)
{
	m_eMonsterType	= MON_MINIBOSS2;

	m_fSpeedX		= 4;
	m_fSpeedY		= 4;
	m_iHP			= 5;

	m_tInfo.fCX = 50.f;
	m_tInfo.fCY = 50.f;

	m_tColInfo.fCX = m_tInfo.fCX;
	m_tColInfo.fCY = m_tInfo.fCY;

	m_tFrame	= FRAME(rand()%4, 4, 0, 200);
	m_tMonDead	= FRAME(0, 15, 0, 70);			//죽는모션
	m_tPoofFrame= FRAME(0, 15, 0, 70);			//등장모션

	m_dwPoofTime= GetTickCount();
	m_appear	= false;

	
	if(m_iDrawID == 0)
		m_eMiniDIR	= DIR_LU;
	else if(m_iDrawID == 1)
		m_eMiniDIR	= DIR_LD;
	else if(m_iDrawID == 2)
		m_eMiniDIR	= DIR_RU;
	else if(m_iDrawID == 3)
		m_eMiniDIR	= DIR_RD;


}
int CMiniBoss2::Progress(void)
{
	if(m_iHP <= 0)
	{	
		if(m_dwMiniBoss2Time + m_tMonDead.dwTime < GetTickCount())
		{
			if(m_tMonDead.iStart == 1)
			{
				CObj* pMonster = CObjFactory<CMiniBoss4>::CreateObj(m_tInfo.fX, m_tInfo.fY, 2);
				m_pMonsterlist->push_back(pMonster);
				((CMiniBoss4*)pMonster)->SetBloodlist(m_pBloodlist);
				pMonster = CObjFactory<CMiniBoss4>::CreateObj(m_tInfo.fX, m_tInfo.fY, 3);
				m_pMonsterlist->push_back(pMonster);
				((CMiniBoss4*)pMonster)->SetBloodlist(m_pBloodlist);

				CDevice::GetInstance()->SoundPlay(MONSTERDEAD, 0);
				CObj* pBlood = CObjFactory<CBlood>::CreateObj(m_tInfo.fX, m_tInfo.fY);
				m_pBloodlist->push_back(pBlood);

			}
			m_dwMiniBoss2Time = GetTickCount();
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

			if(m_dwMiniBoss2Time + m_tFrame.dwTime < GetTickCount())
			{
				m_dwMiniBoss2Time = GetTickCount();
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
void CMiniBoss2::Render(HDC hdc)
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
			44, 15,
			(*m_pBitMap)["shadow"]->GetMemDC(),
			0, 0,
			120, 49,
			RGB(61, 16, 123));

		TransparentBlt(hdc,						
			int(m_tInfo.fX - m_tInfo.fCX / 2.f),
			int(m_tInfo.fY - m_tInfo.fCY / 2.f),
			int(m_tInfo.fCX) + 10, int(m_tInfo.fCY) + 10,
			(*m_pBitMap)["miniboss"]->GetMemDC(),
			int(64 * m_tFrame.iStart),	
			int(64 * m_tFrame.iScene),
			64, 64,
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
void CMiniBoss2::Release(void)
{
}
void CMiniBoss2::SetMonsterlist(list<CObj*>* _pMonsterlist)
{
	m_pMonsterlist = _pMonsterlist;
}
void CMiniBoss2::SetBloodlist(list<CObj*>* _pBloodlist)
{
	m_pBloodlist = _pBloodlist;
}