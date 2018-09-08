#include "StdAfx.h"
#include "TowerWnd3.h"

#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

#include "ObjFactory.h"

#include "Tower.h"
#include "KnightTower.h"
#include "ArchorTower.h"
#include "MagicTower.h"
#include "BombTower.h"
#include "EmptyTower.h"

#include "Obj.h"
#include "Mouse.h"
#include "Player.h"
#include "ObjMgr.h"
#include "SDevice.h"


#define	dfButtSize 30
#define	dfButtSize2 15
enum TowerNum{ NOCHECK, KNIGHT, ARCHOR, MAGIC, BOMB };
CTowerWnd3::CTowerWnd3(void)
{
}

CTowerWnd3::~CTowerWnd3(void)
{
	Release();
}
HRESULT CTowerWnd3::Initialize(void)
{
	m_bMouseOn		= false;
	m_fScale		= 0.8f;
	m_iRenderNum	= -1;
	m_fRange		= 0.f;
	m_wstrObjKey	= L"";
	m_wstrStateKey	= L"";

	//upgrade
	m_vBtNum1	= D3DXVECTOR3( m_tInfo.vPos.x - 35.f, m_tInfo.vPos.y - 42.f, 0.f);	
	//sell
	m_vBtNum2	= D3DXVECTOR3( m_tInfo.vPos.x, m_tInfo.vPos.y + 45.f, 0.f);	
	
	m_pPlayerList	= CObjMgr::GetInstance()->GetObjList(OBJ_PLAYER);

	return S_OK;
	
}
int CTowerWnd3::Progress(void)		
{
	MouseInput();
	if(KeyInput())
		return 1;
	m_fScale < 1.f ? m_fScale += CTimeMgr::GetInstance()->GetTime() : m_fScale = 1.f ;
	return 0;
}
void CTowerWnd3::Render(void)		
{
	D3DXMATRIX	matScale, matRangeScale, matTrans, matPriceTrans;

	/* 업글타워 사정거리 */
	
	if(m_bMouseOn)
	{
		const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Tower", L"TowerUI", 13);

		float	fX			= pTexture->tImgInfo.Width  / 2.f;
		float	fY			= pTexture->tImgInfo.Height / 2.f;
		float	fRangeScale = m_fRange / pTexture->tImgInfo.Width;
		D3DXMatrixScaling(&matRangeScale, fRangeScale, fRangeScale/1.3f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&(matRangeScale * matTrans));
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
			&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}


	/* 유아이 창 */
	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Upgrade(Basic)");
	D3DXMatrixScaling(&matScale, m_fScale, m_fScale, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	
	m_tInfo.matWorld = matScale * matTrans;

	float	fX = pTexture->tImgInfo.Width  / 2.f;
	float	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

	/* 업글 가격 */
	if(m_fScale > 0.9)
	{
		TCHAR		szBuf[MIN_STR] = L"";
		wsprintf(szBuf, L"%d", ((CTower*)m_pTower)->GetTowerStats().iNextPrice);

		D3DXMatrixTranslation(&matPriceTrans, m_tInfo.vPos.x - 44.f, m_tInfo.vPos.y - 18.5f, 0.f);
	
		CDevice::GetInstance()->GetSprite()->SetTransform(&(matPriceTrans));
		CDevice::GetInstance()->GetUIFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), 
			szBuf, lstrlen(szBuf), NULL, 0, D3DCOLOR_ARGB(255, 255, 255, 0));
	}
}
void CTowerWnd3::Release(void)		
{
}
void CTowerWnd3::MouseInput()
{
	D3DXVECTOR3 vMousePos = CMouse::GetPos();
	
	switch(((CTower*)m_pTower)->GetTowerID())
	{
	case TWR_ARCHOR1:
		m_iRenderNum = ARCHOR;
		break;
	case TWR_KNIGHT1:
		m_iRenderNum = KNIGHT;
		break;
	case TWR_MAGIC1:
		m_iRenderNum = MAGIC;
		break;
	case TWR_BOMB1:
		m_iRenderNum = BOMB;
		break;
	}
	if( D3DXVec3Length(& ( m_vBtNum1 - vMousePos)) <= dfButtSize)
	{
		if(m_iRenderNum == ARCHOR)
		{
			if( ((CTower*)m_pTower)->GetLevel() == 0 )
				m_fRange = 280.f;
			else if( ((CTower*)m_pTower)->GetLevel() == 1 )
				m_fRange = 280.f;
		}
		else if(m_iRenderNum == MAGIC)
		{
			if( ((CTower*)m_pTower)->GetLevel() == 0 )
				m_fRange = 250.f;
			else if( ((CTower*)m_pTower)->GetLevel() == 1 )
				m_fRange = 260.f;
		}
		else if(m_iRenderNum == BOMB)
		{
			if( ((CTower*)m_pTower)->GetLevel() == 0 )
				m_fRange = 290.f;
			else if( ((CTower*)m_pTower)->GetLevel() == 1 )
				m_fRange = 300.f;
		}
		m_bMouseOn = true;
	}
	else
	{
		m_iRenderNum = NOCHECK;
		m_fRange	 = 0;
		m_bMouseOn	 = false;
	}	
}
bool CTowerWnd3::KeyInput()
{
	D3DXVECTOR3 vMousePos = CMouse::GetPos();
	if(CKeyMgr::GetInstance()->KeyPressed(KEY_LBUTTON, KEYNUM_4))
	{
		//1.button
		if( D3DXVec3Length(& ( m_vBtNum1 - vMousePos)) <= dfButtSize)		
		{
			int iNextLv = ((CTower*)m_pTower)->GetLevel() + 1;

			if(iNextLv == 3)
				iNextLv = 3;

			((CPlayer*)m_pPlayerList->front())->SetGold(((CPlayer*)m_pPlayerList->front())->GetGold() - ((CTower*)m_pTower)->GetTowerStats().iNextPrice );

			((CTower*)m_pTower)->SetLevelup(iNextLv);
			((CTower*)m_pTower)->SetTowerClick(false);

			CSDevice::GetInstance()->SoundPlay(SD_TOWERUPGRADE, 0);			
			return true;
		}
		//2. Sell button
		else if( D3DXVec3Length(& ( m_vBtNum2 - vMousePos)) <= dfButtSize2)
		{
			((CTower*)m_pTower)->SetDestroyTower(true);
			
			CObj* pEmptyTower = CObjFactory<CEmptyTower>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f));
			m_pTowerList->push_back(pEmptyTower);

			CSDevice::GetInstance()->SoundPlay(SD_TOWERSELL, 0);
			return true;
		}
	}
	return false;
}