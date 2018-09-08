#include "StdAfx.h"
#include "BitBmp.h"


CBitBmp::CBitBmp(void)
{
}

CBitBmp::~CBitBmp(void)
{
	Release();
}

CBitBmp* CBitBmp::LoadBmp(TCHAR* pFileName)
{
	m_hdc = GetDC(g_hWnd);

	m_MemDC = CreateCompatibleDC(m_hdc);

	ReleaseDC(g_hWnd, m_hdc);

	m_bitmap = (HBITMAP)LoadImage(NULL, pFileName, IMAGE_BITMAP, 
		0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	m_oldbmp = (HBITMAP)SelectObject(m_MemDC, m_bitmap);

	if(m_bitmap == NULL)
		MessageBox(g_hWnd, L"Error", L"Image Load Error", MB_OK);

	return this;
}

void CBitBmp::Release(void)
{
	SelectObject(m_MemDC, m_oldbmp);
	DeleteObject(m_bitmap);
	DeleteDC(m_MemDC);
}
