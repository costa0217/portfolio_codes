#include "StdAfx.h"
#include "Win.h"

#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"

#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Mouse.h"
#include "KeyMgr.h"

#include "UI.h"
#include "StopButt.h"

#include "SDevice.h"


CWin::CWin(void)
{
}
CWin::~CWin(void)
{
	Release();
}
HRESULT CWin::Initialize(void)
{
	m_tInfo.vPos	= D3DXVECTOR3(WINCX /2.f , WINCY / 2.f, 0.f);
	m_eWndType		= WND_WIN;

	if(CSDevice::GetInstance()->SoundPlaying(SD_STOBATTLE))
		CSDevice::GetInstance()->SoundStop(SD_STOBATTLE);

	CSDevice::GetInstance()->SoundPlay(SD_WIN, 0);

	m_fNamTime		= 0.f;

	return S_OK;
}
int		CWin::Progress(void)	
{
	m_fNamTime	+= GET_TIME;
	if(m_fNamTime > 5.f)
	{
		m_fNamTime = 0.f;
		return 2;
	}
	return 0;
}
void	CWin::Render(void)	
{
	D3DXMATRIX		matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"MiniStop");

	D3DXMatrixTranslation(&matTrans, WINCX/2, WINCY/2, 0.f);

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(100,255,255,255));

	
	/* â */
	pTexture = CTextureMgr::GetInstance()->GetTexture(L"Win");

	D3DXMatrixTranslation(&matTrans, WINCX/2, WINCY/2, 0.f);

	fX = pTexture->tImgInfo.Width  / 2.f;
	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
}
void	CWin::Release(void)	
{
}

