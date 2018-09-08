#include "StdAfx.h"
#include "Meat.h"

#include "ObjMgr.h"
#include "ObjFactory.h"

#include "Blood.h"
#include "MonBullet.h"

#include "Device.h"

CMeat::CMeat(void)
:m_dwMeatTime(GetTickCount())
,m_dwDIRTime(GetTickCount())
,m_iPatternCircle(0)
{
}

CMeat::~CMeat(void)
{
}
void CMeat::Initialize(void)
{
	m_eMonsterType	= MON_MEAT;
	m_fSpeedX		= 0;
	m_fSpeedY		= 0;
	m_iHP			= 5;

	m_tInfo.fCX = 64.f;
	m_tInfo.fCY = 64.f;

	m_tColInfo.fCX = m_tInfo.fCX * 0.8f;
	m_tColInfo.fCY = m_tInfo.fCY * 0.8f ;

	m_tFrame	= FRAME(0, 4, 0, 150);
	m_tMonDead	= FRAME(0, 15, 0, 70);			//죽는모션
	m_tPoofFrame= FRAME(0, 15, 0, 70);			//등장모션

	m_dwPoofTime= GetTickCount();
	m_appear	= false;

	m_iDirLR	= rand()%2;

	if(m_iDirLR == 0)		/*LEFT*/
		m_tFrame.iScene = 0;
	else					/*RIGHT*/
		m_tFrame.iScene = 1;

}
int CMeat::Progress(void)
{
	if(m_iHP <= 0)
	{	
		if(m_dwMeatTime + m_tMonDead.dwTime < GetTickCount())
		{
			m_dwMeatTime = GetTickCount();
			++m_tMonDead.iStart;
			if(m_tMonDead.iStart == 2)
			{
				CDevice::GetInstance()->SoundPlay(MONSTERDEAD, 0);
				CObj* pBlood = CObjFactory<CBlood>::CreateObj(m_tInfo.fX, m_tInfo.fY);
				m_pBloodlist->push_back(pBlood);	
			}
			if(m_tMonDead.iStart >= m_tMonDead.iLast /*fly모션 마지막장면*/)
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
			//if(m_dwDIRTime + 1000 < GetTickCount())
			//{
			//	m_dwDIRTime = GetTickCount();
			//	m_iDirLR = rand()%2;
			//	m_iDirUD = rand()%2;

			//}
			m_tInfo.fX += m_fSpeedX;
			m_tInfo.fY += m_fSpeedY;


			if(m_dwMeatTime + m_tFrame.dwTime < GetTickCount())
			{
				m_dwMeatTime = GetTickCount();
				++m_tFrame.iStart;
				if(m_tFrame.iStart >= m_tFrame.iLast )
				{
					m_tFrame.iStart = 0;
					++m_iPatternCircle;
					
				}
				if(m_iPatternCircle == 2)		// 방향전환
				{
					CDevice::GetInstance()->SoundPlay(MEATGRUNT, 0);
					CObj* pPlayerBullet = CObjFactory<CMonBullet>::CreateObj(m_tInfo.fX, m_tInfo.fY , 0.f);
					m_pBulletlist->push_back(pPlayerBullet);
					pPlayerBullet = CObjFactory<CMonBullet>::CreateObj(m_tInfo.fX, m_tInfo.fY , 3.14f);
					m_pBulletlist->push_back(pPlayerBullet);
					pPlayerBullet = CObjFactory<CMonBullet>::CreateObj(m_tInfo.fX, m_tInfo.fY , 4.71f);
					m_pBulletlist->push_back(pPlayerBullet);
					pPlayerBullet = CObjFactory<CMonBullet>::CreateObj(m_tInfo.fX, m_tInfo.fY , 1.57f);
					m_pBulletlist->push_back(pPlayerBullet);

					m_iDirLR = rand()%3;
					m_iDirUD = rand()%3;
					if(m_iDirLR == 0)
						m_fSpeedX = 2;
					else if(m_iDirLR == 1)
						m_fSpeedX = -2;
					else
						m_fSpeedX = 0;

					if(m_iDirUD == 0)
						m_fSpeedY = -2;
					else if(m_iDirUD == 1)
						m_fSpeedY = 2;
					else
						m_fSpeedY = 0;

					m_iPatternCircle = 0;
				}
			}

			if(m_iDirLR == 0)		/*LEFT*/
				m_tFrame.iScene = 0;
			else					/*RIGHT*/
				m_tFrame.iScene = 1;
		}
	}


	return 0;
}
void CMeat::Render(HDC hdc)
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
			int(m_tInfo.fX - m_tInfo.fCX/2.4f ),
			int(m_tInfo.fY + m_tInfo.fCY/5.f),
			52, 15,
			(*m_pBitMap)["shadow"]->GetMemDC(),
			0, 0,
			120, 49,
			RGB(61, 16, 123));

		TransparentBlt(hdc,						
			int(m_tInfo.fX - m_tInfo.fCX * 0.8f),
			int(m_tInfo.fY - m_tInfo.fCY * 0.8f - 10),
			int(m_tInfo.fCX * 1.6), int(m_tInfo.fCY * 1.6),
			(*m_pBitMap)["meat"]->GetMemDC(),
			int(m_tInfo.fCX * m_tFrame.iStart),	
			int(m_tInfo.fCY * m_tFrame.iScene),
			int(m_tInfo.fCX), int(m_tInfo.fCY),
			RGB(61, 16, 123));
		if(!m_appear)
		{
			TransparentBlt(hdc,						
				int(m_tInfo.fX - m_tInfo.fCX + 5),
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
void CMeat::Release(void)
{
}
void CMeat::SetBulletlist(list<CObj*>* _pBulletlist)
{
	m_pBulletlist = _pBulletlist;
}
void CMeat::SetBloodlist(list<CObj*>* _pBloodlist)
{
	m_pBloodlist = _pBloodlist;
}