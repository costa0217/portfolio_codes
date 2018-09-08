#include "StdAfx.h"
#include "Field.h"

#include "Device.h"
#include "Back.h"
#include "ObjFactory.h"
#include "Obj.h"
#include "ObjMgr.h"
#include "CollisionMgr.h"
#include "TextureMgr.h"
#include "SceneMgr.h"

#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Mouse.h"

#include "SDevice.h"

CField::CField(void)
{
}

CField::~CField(void)
{
	Release();
}
HRESULT CField::Initialize(void)
{
	m_vStage1	= D3DXVECTOR3(200.f, 400.f, 0.f);
	m_vStage2	= D3DXVECTOR3(250.f, 100.f, 0.f);

	m_fScaleStage1	= 1.f;
	m_fScaleStage2	= 1.f;

	m_bStage1Wnd	= false;
	m_bStage2Wnd	= false;

	CSDevice::GetInstance()->SoundPlay(SD_FIELD, 1);		// ÇÊµåºñÁö¿¥
	
	return S_OK;
}
int CField::Progress(void)		
{
	KeyInput();
	return 0;
}
void CField::Render(void)		
{
	D3DXMATRIX		matTrans, matScale;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Field");
	
	D3DXMatrixTranslation(&matTrans, WINCX / 2.f, WINCY / 2.f, 0.f);

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

	pTexture = CTextureMgr::GetInstance()->GetTexture(L"StageFlag");
	
	D3DXMatrixScaling(&matScale, m_fScaleStage1, m_fScaleStage1, 0.f);
	D3DXMatrixTranslation(&matTrans, m_vStage1.x, m_vStage1.y, 0.f);

	fX = pTexture->tImgInfo.Width  / 2.f;
	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&(matScale *matTrans));
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));


	pTexture = CTextureMgr::GetInstance()->GetTexture(L"StageFlag");
	
	D3DXMatrixScaling(&matScale, m_fScaleStage2, m_fScaleStage2, 0.f);
	D3DXMatrixTranslation(&matTrans, m_vStage2.x, m_vStage2.y, 0.f);

	fX = pTexture->tImgInfo.Width  / 2.f;
	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&(matScale *matTrans));
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

	if(m_bStage1Wnd)
	{
		const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Stage1Wnd");
	
		D3DXMatrixTranslation(&matTrans, WINCX * 0.75f, WINCY / 2.f, 0.f);

		float fX = pTexture->tImgInfo.Width  / 2.f;
		float fY = pTexture->tImgInfo.Height / 2.f;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
	}
	else if(m_bStage2Wnd)
	{
		const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Stage2Wnd");
	
		D3DXMatrixTranslation(&matTrans, WINCX * 0.75f, WINCY / 2.f, 0.f);

		float fX = pTexture->tImgInfo.Width  / 2.f;
		float fY = pTexture->tImgInfo.Height / 2.f;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
	}

}
void CField::Release(void)		
{
	CSDevice::GetInstance()->SoundStop(SD_FIELD);		// ÇÊµå ºñÁö¿¥
	CSDevice::GetInstance()->SoundStop(SD_BUTTOVER);
}
void CField::KeyInput(void)	
{
	if(D3DXVec3Length(& (CMouse::GetPos() - m_vStage1)) < 20)
	{
		m_fScaleStage1	= 1.4f;
		m_bStage1Wnd	= true;

		if( !CSDevice::GetInstance()->SoundPlaying(SD_BUTTOVER) )
		{
			CSDevice::GetInstance()->SoundPlay(SD_BUTTOVER, 0);
		}
	}
	else
	{
		m_fScaleStage1	= 1.f;
		m_bStage1Wnd	= false;
	}
	if(D3DXVec3Length(& (CMouse::GetPos() - m_vStage2)) < 20)
	{
		m_fScaleStage2	= 1.4f;
		m_bStage2Wnd	= true;
		if( !CSDevice::GetInstance()->SoundPlaying(SD_BUTTOVER) )
		{
			CSDevice::GetInstance()->SoundPlay(SD_BUTTOVER, 0);
		}
	}
	else
	{
		m_fScaleStage2  = 1.f;
		m_bStage2Wnd	= false;
	}
	if(CKeyMgr::GetInstance()->KeyPressed(KEY_LBUTTON, KEYNUM_8))
	{
		if(D3DXVec3Length(& (CMouse::GetPos() - m_vStage1)) < 20)
		{
			if(!CSDevice::GetInstance()->SoundPlaying(SD_BUUTCLICK))
			{
				CSDevice::GetInstance()->SoundStop(SD_BUTTOVER);
				CSDevice::GetInstance()->SoundPlay(SD_BUUTCLICK, 0);
			}
			CSceneMgr::GetInstance()->Initialize(SC_STAGE1);
		}
		else if(D3DXVec3Length(& (CMouse::GetPos() - m_vStage2)) < 20)
		{
			if(!CSDevice::GetInstance()->SoundPlaying(SD_BUUTCLICK))
			{
				CSDevice::GetInstance()->SoundStop(SD_BUTTOVER);
				CSDevice::GetInstance()->SoundPlay(SD_BUUTCLICK, 0);
			}
			CSceneMgr::GetInstance()->Initialize(SC_STAGE2);
		}
	}
}
