#include "StdAfx.h"
#include "Hopper.h"
#include "ObjMgr.h"
#include "ObjFactory.h"

#include "Blood.h"

#include "Device.h"

CHopper::CHopper(void)
:m_dwHopperTime(GetTickCount())
,m_iPatternCircle(0)
{
}

CHopper::~CHopper(void)
{
}
void CHopper::Initialize(void)
{
	m_eMonsterType	= MON_HOPPER;
	m_fSpeedX		= 0;
	m_fSpeedY		= 0;
	m_iHP			= 5;

	m_tInfo.fCX = 32.f;
	m_tInfo.fCY = 32.f;

	m_tColInfo.fCX = 48.f;
	m_tColInfo.fCY = 48.f;

	m_tFrame	= FRAME(0, 5, 2, 100);
	m_tMonDead	= FRAME(0, 15, 0, 70);			//죽는모션
	m_tPoofFrame= FRAME(0, 15, 0, 70);			//등장모션

	m_dwPoofTime= GetTickCount();
	m_appear	= false;

}
int CHopper::Progress(void)
{
	if(m_iHP <= 0)
	{	
		if(m_dwHopperTime + m_tMonDead.dwTime < GetTickCount())
		{
			m_dwHopperTime = GetTickCount();
			++m_tMonDead.iStart;
			if(m_tMonDead.iStart == 2)
			{
				CDevice::GetInstance()->SoundPlay(MONSTERDEAD, 0);
				CObj* pBlood = CObjFactory<CBlood>::CreateObj(m_tInfo.fX, m_tInfo.fY);
				m_pBloodlist->push_back(pBlood);	
			}
			if(m_tMonDead.iStart >= m_tMonDead.iLast )
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

			if(m_dwHopperTime + m_tFrame.dwTime < GetTickCount())
			{
				m_dwHopperTime = GetTickCount();
				++m_tFrame.iStart;
				
				if(m_tFrame.iScene == 2 &&m_tFrame.iStart > m_tFrame.iLast + 1)
				{
					CDevice::GetInstance()->SoundPlay(CUTEGRUNT, 0);
					if(m_tInfo.fX < 173.f)
						m_tInfo.fX += rand()%26 * 4;
					else if(m_tInfo.fX > 622.f)
						m_tInfo.fX -= rand()%26 * 4;
					else if(m_tInfo.fX >= 173.f && m_tInfo.fX <= 622.f)
						m_tInfo.fX += (rand()%51 - 25)*4;
					else
						m_tInfo.fX = float(rand()%1296 - 648);

					if(m_tInfo.fY < 265.f)
						m_tInfo.fY += rand()%26 * 4;
					else if(m_tInfo.fY > 425.f)
						m_tInfo.fY -= rand()%26 * 4;
					else if(m_tInfo.fY >= 265.f && m_tInfo.fY <= 425.f)
						m_tInfo.fY += (rand()%51 - 25) * 4;
					else
						m_tInfo.fY = float(rand()%712 - 356);

					++m_tFrame.iScene;
					m_tFrame.iStart = 0;
				}
				else if(m_tFrame.iScene == 3 &&m_tFrame.iStart >= m_tFrame.iLast)
				{
					--m_tFrame.iScene;
					m_tFrame.iStart = 0;
				}
			}
		}
	}
	return 0;
}
void CHopper::Render(HDC hdc)
{
	if(m_iHP <= 0)
	{
		TransparentBlt(hdc,						
			int(m_tInfo.fX - m_tInfo.fCX - 14),
			int(m_tInfo.fY - m_tInfo.fCY - 11),
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
			int(m_tInfo.fX - m_tColInfo.fCX / 2.f),
			int(m_tInfo.fY - m_tColInfo.fCY / 2.f),
			int(m_tInfo.fX + m_tColInfo.fCX / 2.f),
			int(m_tInfo.fY + m_tColInfo.fCY / 2.f));*/
		TransparentBlt(hdc,										//그림자
			int(m_tInfo.fX - m_tInfo.fCX/2.1f ),
			int(m_tInfo.fY + m_tInfo.fCY/2.f),
			31, 8,
			(*m_pBitMap)["shadow"]->GetMemDC(),
			0, 0,
			120, 49,
			RGB(61, 16, 123));

		TransparentBlt(hdc,						
			int(m_tInfo.fX - m_tInfo.fCX / 2.f) - 8,
			int(m_tInfo.fY - m_tInfo.fCY / 2.f) - 8,
			48, 48,
			(*m_pBitMap)["hopperleaper"]->GetMemDC(),
			int(m_tInfo.fCX * m_tFrame.iStart),	
			int(m_tInfo.fCY * m_tFrame.iScene),
			int(m_tInfo.fCX), int(m_tInfo.fCY),
			RGB(61, 16, 123));
		if(!m_appear)
		{
			TransparentBlt(hdc,						
				int(m_tInfo.fX - m_tInfo.fCX * 1.6f),
				int(m_tInfo.fY - m_tInfo.fCY * 2.f),
				110, 110,
				(*m_pBitMap)["poof2"]->GetMemDC(),
				int(95 * m_tPoofFrame.iStart),	
				int(95 * m_tPoofFrame.iScene),
				95, 95,
				RGB(61, 16, 123));
		}
	}
}
void CHopper::Release(void)
{
}
void CHopper::SetBloodlist(list<CObj*>* _pBloodlist)
{
	m_pBloodlist = _pBloodlist;
}
