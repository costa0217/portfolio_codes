#include "StdAfx.h"
#include "Alpha.h"

CAlpha::CAlpha(void)
{
}

CAlpha::~CAlpha(void)
{
}
void CAlpha::Initialize(void)
{
	m_iNum = 255;
	bf.AlphaFormat	= AC_SRC_OVER;
	bf.BlendFlags	= 0;
	bf.BlendOp		= 0;
	bf.SourceConstantAlpha = m_iNum;
}
int CAlpha::Progress(void)
{
	if(m_iNum > 0)
		m_iNum -= 3
		;

	if(m_iNum == 0)
		return 1;

	bf.SourceConstantAlpha = m_iNum;
	return 0;
}
void CAlpha::Render(HDC hdc)
{

	//Rectangle(hdc,
	//	m_iNum, 0, 400, 400);

	AlphaBlend(hdc, 0, 0, WINCX, WINCY,
		(*m_pBitMap)["alpha"]->GetMemDC(),
		0, 0, WINCX, WINCY, bf);

	//BitBlt(hdc, 0, 0, WINCX, WINCY, (*m_pBitMap)["BackGround"]->GetMemDC(), 0, 0, SRCCOPY);
}
void CAlpha::Release(void)
{
}
