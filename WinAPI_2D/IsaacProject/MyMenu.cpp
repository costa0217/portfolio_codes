#include "StdAfx.h"
#include "MyMenu.h"
#include "SceneMgr.h"
#include "Device.h"

CMyMenu::CMyMenu(void)
{
}

CMyMenu::~CMyMenu(void)
{
	Release();
}
void CMyMenu::Initialize(void)
{
	m_tMenuFrame = FRAME(0, 0, 0, 70);
	m_tStartButton	= FRAME(0, 1, 0, 100);
	m_dwMenuTime = GetTickCount();
	m_BitMap["alpha"]		= (new CBitBmp)->LoadBmp(L"../Texture/Buff/menu/alpha.bmp");
	m_BitMap["BackGround0"]	= (new CBitBmp)->LoadBmp(L"../Texture/Buff/menu/BackGround0.bmp");
	m_BitMap["Start"]		= (new CBitBmp)->LoadBmp(L"../Texture/Buff/menu/Start.bmp");
	m_BitMap["BackGround"]	= (new CBitBmp)->LoadBmp(L"../Texture/Buff/BackGround.bmp");

	CDevice::GetInstance()->SoundPlay(0, 1);
	
}
void CMyMenu::Progress(void)
{
	
	if(m_dwMenuTime + m_tMenuFrame.dwTime < GetTickCount())
	{
		m_dwMenuTime = GetTickCount();
		++m_tMenuFrame.iScene;
		if(m_tMenuFrame.iScene > 3)
		{
			m_tMenuFrame.iScene = 0;
		}
	}

	if((::GetMouse().x > 315.f && ::GetMouse().x < 485.f) && (::GetMouse().y > 470.f && ::GetMouse().y < 545.f))
	{
		m_tStartButton.iStart = 1;
		
		if(GetAsyncKeyState(VK_LBUTTON))
		{
			
			CDevice::GetInstance()->SoundStop(START);
			CSceneMgr::GetInst()->SetScene(SC_Stage1);
		}
	}
	else
	{
		CDevice::GetInstance()->SoundPlay(CLICK, 0);
		m_tStartButton.iStart = 0;
	}
}
void CMyMenu::Render(HDC hdc)
{
	BitBlt( m_BitMap["BackGround"]->GetMemDC(), 0, 0, WINCX, WINCY, 
		m_BitMap["alpha"]->GetMemDC(), 0, 0, SRCCOPY);




	TransparentBlt(m_BitMap["BackGround"]->GetMemDC(),						
		0, 0,
		800, 600,
		m_BitMap["BackGround0"]->GetMemDC(),
		 0, int(m_tMenuFrame.iScene * 581.f), 
		783, 581,
		RGB(61, 16, 123));

	TransparentBlt(m_BitMap["BackGround"]->GetMemDC(),						
		int(WINCX / 2.f - 85.f) , int(WINCY / 1.5f + 85),
		170, 70,
		m_BitMap["Start"]->GetMemDC(),
		int(170.f * m_tStartButton.iStart),
		0, 
		170, 70,
		RGB(255, 255, 255));



	BitBlt(hdc, 0, 0, WINCX, WINCY, m_BitMap["BackGround"]->GetMemDC(), 0, 0, SRCCOPY);
}
void CMyMenu::Release(void)
{

}
