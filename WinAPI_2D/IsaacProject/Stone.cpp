#include "StdAfx.h"
#include "Stone.h"
#include "ObjMgr.h"
#include "Obj.h"

#include "Device.h"

CStone::CStone(void)
:m_bStoneCrush(false)
,m_bStoneFade(false)
{
}

CStone::~CStone(void)
{
}
void CStone::Initialize(void)
{
	m_tInfo.fCX		= 50;
	m_tInfo.fCY		= 50;
	m_tFrame		= FRAME(0, 3, 0, 500);
	m_tCOlInfo		= INFO(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX, m_tInfo.fCY);
}
int CStone::Progress(void)
{
	if(m_bStoneCrush)
	{
		++m_tFrame.iStart;

		if(m_tFrame.iStart == m_tFrame.iLast -1)
		{
			CDevice::GetInstance()->SoundPlay(ROCKCRUSH, 0);
			m_bStoneFade = true;
		}
		if(m_tFrame.iStart >= m_tFrame.iLast)
		{
			m_tFrame.iStart = m_tFrame.iLast;
			
		}
		m_bStoneCrush = false;
	}
	return 0;
}
void CStone::Render(HDC hdc)
{
	TransparentBlt(hdc,						
		int(m_tInfo.fX - m_tInfo.fCX / 2.f),
		int(m_tInfo.fY - m_tInfo.fCY / 2.f),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		(*m_pBitMap)["Stone"]->GetMemDC(),
		int(32 * m_tFrame.iStart),	
		int(32 * m_tFrame.iScene),
		32,32,
		RGB(61, 16, 123));
}
void CStone::Release(void)
{
}
