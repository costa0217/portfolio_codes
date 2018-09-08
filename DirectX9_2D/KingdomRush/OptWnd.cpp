#include "StdAfx.h"
#include "OptWnd.h"

#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"

#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Mouse.h"
#include "KeyMgr.h"

#include "UI.h"
#include "OptionButt.h"
#include "SceneMgr.h"


COptWnd::COptWnd(void)
:m_pOptUI(NULL)
{
}

COptWnd::~COptWnd(void)
{
	Release();
}
HRESULT COptWnd::Initialize(void)
{
	m_tInfo.vPos	= D3DXVECTOR3(WINCX /2.f , WINCY / 2.f, 0.f);

	m_eWndType		= WND_OPTION;

	m_iImgNum		= 0;
	m_bClick		= true;
	m_bGoField		= false;
	return S_OK;
}
int		COptWnd::Progress(void)	
{
	if(m_bGoField)
	{
		return 2;
	}
	if(!m_bClick)
	{
		((CUI*)m_pOptUI)->SetClick(false);
		((CUI*)m_pOptUI)->SetChoice(false);
		return 1;
	}
	KeyInput();

	return 0;
}
void	COptWnd::Render(void)	
{
	D3DXMATRIX		matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"MiniStop");

	D3DXMatrixTranslation(&matTrans, WINCX/2, WINCY/2, 0.f);

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(100,255,255,255));

	
	/* Ã¢ */
	pTexture = CTextureMgr::GetInstance()->GetTexture(L"SkillWnd");

	D3DXMatrixTranslation(&matTrans, WINCX/2, WINCY/2, 0.f);

	fX = pTexture->tImgInfo.Width  / 2.f;
	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

	//¹öÆ°
	pTexture = CTextureMgr::GetInstance()->GetTexture(L"Button", L"Quit", m_iImgNum);

	D3DXMatrixTranslation(&matTrans, WINCX/2.f, WINCY/2.f, 0.f);

	fX = pTexture->tImgInfo.Width  / 2.f;
	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

}
void	COptWnd::Release(void)	
{


}	
void	COptWnd::KeyInput(void)	
{
	if( CMouse::GetPos().y < WINCY/2.f + 27.f
		&& CMouse::GetPos().y > WINCY/2.f - 27.f
		&& CMouse::GetPos().x > WINCX/2.f - 90.f
		&& CMouse::GetPos().x < WINCX/2.f + 90.f)
	{
		m_iImgNum = 1;

		if(CKeyMgr::GetInstance()->KeyPressed(KEY_LBUTTON, KEYNUM_9))
		{
			//CObjMgr::GetInstance()->SetScene(true);
			m_bGoField = true;
		}
	}
	else
		m_iImgNum = 0;
	
}
