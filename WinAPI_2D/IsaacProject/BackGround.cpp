#include "StdAfx.h"
#include "BackGround.h"

CBackGround::CBackGround(void)
{
}

CBackGround::~CBackGround(void)
{
	Release();
}

void CBackGround::Initialize(void)
{
	m_strKey = "UI";

	m_tInfo = INFO(0, 0, 800.f, 600.f);

}

int CBackGround::Progress(void)
{
	return 0;
}

void CBackGround::Render(HDC hdc)
{
	TransparentBlt(hdc, 
		int(m_tInfo.fX),
		int(m_tInfo.fY),
		int(m_tInfo.fCX), int(m_tInfo.fCY),
		(*m_pBitMap)[m_strKey]->GetMemDC(),
		0, 0,
		int(m_tInfo.fCX), int(m_tInfo.fCY), RGB(123, 217, 121));
}

void CBackGround::Release(void)
{
	
}
