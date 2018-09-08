#include "StdAfx.h"
#include "StopWnd.h"

#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"

#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Mouse.h"
#include "KeyMgr.h"

#include "UI.h"
#include "StopButt.h"


CStopWnd::CStopWnd(void)
:m_pStopUI(NULL)
{
}

CStopWnd::~CStopWnd(void)
{
	Release();
}
HRESULT CStopWnd::Initialize(void)	
{
	m_tInfo.vPos	= D3DXVECTOR3(WINCX /2.f , WINCY / 2.f, 0.f);

	m_eWndType		= WND_STOP;

	m_bClick		= true;

	//for(int i = 0; i < 4; ++i)
	//{
	//	m_bMeteoLV[i]	= false;
	//	m_bMillLV[i]	= false;

	//	m_iMeteoRGB[i]	= 100;
	//	m_bMillRGB[i]	= 100;
	//}

	//m_bMeteoLV[0]	= true;
	//m_bMillLV[0]	= true;

	return S_OK;
}
int		CStopWnd::Progress(void)		
{
	if(!m_bClick)
	{
		((CUI*)m_pStopUI)->SetClick(false);
		((CUI*)m_pStopUI)->SetChoice(false);
		return 1;
	}

	KeyInput();
	for(int i = 0; i < 4; ++i)
	{
		if(m_bMeteoLV[i])
		{
			m_iMeteoRGB[i] = 255;
		}
		else
			m_iMeteoRGB[i] = 100;
		if(m_bMillLV[i])
		{
			m_bMillRGB[i] = 255;
		}
		else
			m_bMillRGB[i] = 100;
	}

	return 0;
}
void	CStopWnd::Render(void)		
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

	/* ¹öÆ° */

	//1
	pTexture = CTextureMgr::GetInstance()->GetTexture(L"UI", L"SkillButton", 19);

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - 80.f , m_tInfo.vPos.y  + 75.f, 0.f);

	fX = pTexture->tImgInfo.Width  / 2.f;
	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255 , 255, m_iMeteoRGB[0], m_iMeteoRGB[0]));

	//2
	pTexture = CTextureMgr::GetInstance()->GetTexture(L"UI", L"SkillButton", 20);

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - 80.f , m_tInfo.vPos.y , 0.f);

	fX = pTexture->tImgInfo.Width  / 2.f;
	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255,m_iMeteoRGB[1],m_iMeteoRGB[1]));

	//3
	pTexture = CTextureMgr::GetInstance()->GetTexture(L"UI", L"SkillButton", 21);

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - 80.f , m_tInfo.vPos.y  - 75.f, 0.f);

	fX = pTexture->tImgInfo.Width  / 2.f;
	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,m_iMeteoRGB[2],m_iMeteoRGB[2]));

	//4
	pTexture = CTextureMgr::GetInstance()->GetTexture(L"UI", L"SkillButton", 23);

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x - 80.f , m_tInfo.vPos.y - 150.f, 0.f);

	fX = pTexture->tImgInfo.Width  / 2.f;
	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,m_iMeteoRGB[3],m_iMeteoRGB[3]));

	//5
	pTexture = CTextureMgr::GetInstance()->GetTexture(L"UI", L"SkillButton", 25);

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 70.f , m_tInfo.vPos.y  + 75.f, 0.f);

	fX = pTexture->tImgInfo.Width  / 2.f;
	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,m_bMillRGB[0],m_bMillRGB[0]));

	//6
	pTexture = CTextureMgr::GetInstance()->GetTexture(L"UI", L"SkillButton", 26);

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 70.f , m_tInfo.vPos.y, 0.f);

	fX = pTexture->tImgInfo.Width  / 2.f;
	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,m_bMillRGB[1],m_bMillRGB[1]));

	//7
	pTexture = CTextureMgr::GetInstance()->GetTexture(L"UI", L"SkillButton", 27);

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 70.f , m_tInfo.vPos.y - 75.f, 0.f);

	fX = pTexture->tImgInfo.Width  / 2.f;
	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,m_bMillRGB[2],m_bMillRGB[2]));

	//8
	pTexture = CTextureMgr::GetInstance()->GetTexture(L"UI", L"SkillButton", 2);

	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x + 70.f , m_tInfo.vPos.y - 150.f, 0.f);

	fX = pTexture->tImgInfo.Width  / 2.f;
	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,m_bMillRGB[3],m_bMillRGB[3]));

}
void	CStopWnd::Release(void)		
{
}
void	CStopWnd::KeyInput(void)	
{
	D3DXVECTOR3	vLBtt1	= D3DXVECTOR3(m_tInfo.vPos.x - 80.f, m_tInfo.vPos.y + 75.f, 0.f);
	D3DXVECTOR3	vLBtt2	= D3DXVECTOR3(m_tInfo.vPos.x - 80.f, m_tInfo.vPos.y + 0.f, 0.f);
	D3DXVECTOR3	vLBtt3	= D3DXVECTOR3(m_tInfo.vPos.x - 80.f, m_tInfo.vPos.y - 75.f, 0.f);
	D3DXVECTOR3	vLBtt4	= D3DXVECTOR3(m_tInfo.vPos.x - 80.f, m_tInfo.vPos.y - 150.f, 0.f);

	D3DXVECTOR3	vRBtt1	= D3DXVECTOR3(m_tInfo.vPos.x + 70.f, m_tInfo.vPos.y + 75.f, 0.f);
	D3DXVECTOR3	vRBtt2	= D3DXVECTOR3(m_tInfo.vPos.x + 70.f, m_tInfo.vPos.y + 0.f,  0.f);
	D3DXVECTOR3	vRBtt3	= D3DXVECTOR3(m_tInfo.vPos.x + 70.f, m_tInfo.vPos.y - 75.f, 0.f);
	D3DXVECTOR3	vRBtt4	= D3DXVECTOR3(m_tInfo.vPos.x + 70.f, m_tInfo.vPos.y - 150.f, 0.f);

	if(CKeyMgr::GetInstance()->KeyPressed(KEY_LBUTTON, KEYNUM_7))
	{
		if( D3DXVec3Length( & ( CMouse::GetPos() - vLBtt2 ) )<= 25 
			&& m_bMeteoLV[0])
		{
			m_bMeteoLV[1] = true;
		}
		else if( D3DXVec3Length( & ( CMouse::GetPos() - vLBtt3 ) )<= 25 
			&& m_bMeteoLV[0]
			&& m_bMeteoLV[1])
		{
			m_bMeteoLV[2] = true;
		}
		else if( D3DXVec3Length( & ( CMouse::GetPos() - vLBtt4 ) )<= 25 
			&& m_bMeteoLV[0]
			&& m_bMeteoLV[1]
			&& m_bMeteoLV[2])
		{
			m_bMeteoLV[3] = true;
		}

		if( D3DXVec3Length( & ( CMouse::GetPos() - vRBtt2 ) )<= 25 
			&& m_bMillLV[0])
		{
			m_bMillLV[1] = true;
		}
		else if( D3DXVec3Length( & ( CMouse::GetPos() - vRBtt3 ) )<= 25 
			&& m_bMillLV[0]
			&& m_bMillLV[1])
		{
			m_bMillLV[2] = true;
		}
		else if( D3DXVec3Length( & ( CMouse::GetPos() - vRBtt4 ) )<= 25 
			&& m_bMillLV[0]
			&& m_bMillLV[1]
			&& m_bMillLV[2])
		{
			m_bMillLV[3] = true;
		}
	}
}
