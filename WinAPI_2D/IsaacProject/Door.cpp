#include "StdAfx.h"
#include "Door.h"

CDoor::CDoor(void)
:m_bOpenDoor(false)
,m_bKeyOpenDoor(false)
{
}

CDoor::~CDoor(void)
{
}
void CDoor::Initialize(void)
{
	m_iStageNum		= 0;
	m_tFrame		= FRAME(1, 2, 0, 200);
	m_BossDoor		= INFO(m_tInfo.fX, m_tInfo.fY, 80.f, 60.f);
	
	if(m_iIndex == 7)
	{
		m_tFrame.iScene = 0;
		m_tInfo.fCX		= 70;
		m_tInfo.fCY		= 50;
	}
	else if(m_iIndex == 60)
	{
		m_tFrame.iScene = 2;
		m_tInfo.fCX		= 50;
		m_tInfo.fCY		= 70;
	}
	else if(m_iIndex == 74)
	{
		m_tFrame.iScene = 3;
		m_tInfo.fCX		= 50;
		m_tInfo.fCY		= 70;
	}
	else if(m_iIndex == 127)
	{
		m_tFrame.iScene = 1;
		m_tInfo.fCX		= 70;
		m_tInfo.fCY		= 50;
	}
	m_tCOlInfo		= INFO(m_tInfo.fX, m_tInfo.fY, m_tInfo.fCX / 2.f, m_tInfo.fCY / 2.f);

	
	m_bKeyInsert = false;


}
int CDoor::Progress(void)
{
	if(!m_bKeyInsert)
	{
		if(m_iStageNum == 2 && m_iIndex == 127)
			m_bKey = false;
		else
			m_bKey = true;

		if(!m_bKey)
			m_bOpenDoor = false;
	}



	if(m_bOpenDoor)
		m_tFrame.iStart = 1;
	else
		m_tFrame.iStart = 0;

	return 0;
}
void CDoor::Render(HDC hdc)
{
	

	if((m_iStageNum == 5 && m_iIndex == 7))
	{
		TransparentBlt(hdc,						
		int(m_BossDoor.fX - m_BossDoor.fCX / 2.f),
		int(m_BossDoor.fY - m_BossDoor.fCY / 2.f),
		int(m_BossDoor.fCX), int(m_BossDoor.fCY),
		(*m_pBitMap)["bossdoor"]->GetMemDC(),
		int(55 * m_tFrame.iStart),	
		int(42 * m_tFrame.iScene),
		55,42,
		RGB(61, 16, 123));

	}
	else if((m_iStageNum == 5 && m_iIndex == 127))
	{
		TransparentBlt(hdc,						
		int(m_BossDoor.fX - m_BossDoor.fCX / 2.f),
		int(m_BossDoor.fY - m_BossDoor.fCY / 2.f),
		int(m_BossDoor.fCX), int(m_BossDoor.fCY),
		(*m_pBitMap)["treasuredoor"]->GetMemDC(),
		int(55 * m_tFrame.iStart),	
		int(42 * m_tFrame.iScene),
		55,42,
		RGB(61, 16, 123));

	}
	else
	{
		if(m_iStageNum == 2 && m_iIndex == 127)
		{

		TransparentBlt(hdc,						
			int(m_tInfo.fX - m_tInfo.fCX / 2.f),
			int(m_tInfo.fY - m_tInfo.fCY / 2.f),
			int(m_tInfo.fCX), int(m_tInfo.fCY),
			(*m_pBitMap)["door"]->GetMemDC(),
			int(50 * m_tFrame.iStart),	
			int(50 * 4),
			50,50,
			RGB(61, 16, 123));
		}
		else
		{
			TransparentBlt(hdc,						
			int(m_tInfo.fX - m_tInfo.fCX / 2.f),
			int(m_tInfo.fY - m_tInfo.fCY / 2.f),
			int(m_tInfo.fCX), int(m_tInfo.fCY),
			(*m_pBitMap)["door"]->GetMemDC(),
			int(50 * m_tFrame.iStart),	
			int(50 * m_tFrame.iScene),
			50,50,
			RGB(61, 16, 123));
		}

	}

}
void CDoor::Release(void)
{
}

void CDoor::SetOpenDoor(bool _bOpenDoor)
{
	m_bOpenDoor = _bOpenDoor;
}
void CDoor::SetKeyOpenDoor(bool _bKeyOpenDoor)
{
	m_bKeyOpenDoor = _bKeyOpenDoor;
}