#include "StdAfx.h"
#include "UI.h"

CUI::CUI(void)
{
}

CUI::~CUI(void)
{
	//Release();
}
void CUI::SetPlayer(CObj* _pPlayer)
{
	m_pPlayer = _pPlayer;
}
//
//void CUI::Initialize(void)
//{
//	m_tInfo		= INFO(620.f , 40.f , 50.f, 50.f);
//	m_tFrame	= FRAME(0, 2, 5, 80);
//}
//int CUI::Progress(void)
//{
//	return 0;
//}
//void CUI::Render(HDC hdc)
//{
//	///*Rectangle(hdc,
//	//	int(m_tInfo.fX - m_tInfo.fCX / 2.f),
//	//	int(m_tInfo.fY - m_tInfo.fCY / 2.f),
//	//	int(m_tInfo.fX + m_tInfo.fCX / 2.f),
//	//	int(m_tInfo.fY + m_tInfo.fCY / 2.f));*/
//
//	//TransparentBlt(hdc, 
//	//	int(m_tInfo.fX - m_tInfo.fCX / 2.f),
//	//	int(m_tInfo.fY - m_tInfo.fCY / 2.f),
//	//	150, 50,
//	//	(*m_pBitMap)["hart"]->GetMemDC(),
//	//	50 * m_tFrame.iStart,
//	//	50 * m_tFrame.iScene,
//	//	int(m_tInfo.fCX) * 3, int(m_tInfo.fCY), RGB(28, 24, 25));
//}
//void CUI::Release(void)
//{
//}


RECT CUI::GetRect()
{
	RECT rc = {
			0,
			0,
			0,
			0};
	return rc;
}