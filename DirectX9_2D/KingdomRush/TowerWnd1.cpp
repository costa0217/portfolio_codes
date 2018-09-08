#include "StdAfx.h"
#include "TowerWnd1.h"

#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"
#include "KeyMgr.h"
#include "ObjMgr.h"

#include "ObjFactory.h"

#include "Tower.h"
#include "KnightTower.h"
#include "ArchorTower.h"
#include "MagicTower.h"
#include "BombTower.h"

#include "Obj.h"
#include "Mouse.h"

#include "Player.h"
#include "SDevice.h"

#define	dfButtSize 30
enum TowerNum{ NOCHECK, KNIGHT, ARCHOR, MAGIC, BOMB };
CTowerWnd1::CTowerWnd1(void)
{
}

CTowerWnd1::~CTowerWnd1(void)
{
	Release();
}
HRESULT CTowerWnd1::Initialize(void)
{
	m_fScale		= 0.8f;
	m_iRenderNum	= 0;
	m_fRange		= 0;		// 타워 공격범위
	m_byStyle		= 0;		// 범위스타일
	m_wstrObjKey	= L"";
	m_wstrStateKey	= L"";
	
	m_vBtNum1	= D3DXVECTOR3( m_tInfo.vPos.x - 35.f, m_tInfo.vPos.y - 42.f, 0.f);		//아처
	m_vBtNum2	= D3DXVECTOR3( m_tInfo.vPos.x + 34.5f, m_tInfo.vPos.y - 42.f, 0.f);		//병사
	m_vBtNum3	= D3DXVECTOR3( m_tInfo.vPos.x - 35.f, m_tInfo.vPos.y + 26.5f, 0.f);		//마법
	m_vBtNum4	= D3DXVECTOR3( m_tInfo.vPos.x + 34.5f, m_tInfo.vPos.y + 26.5f, 0.f);	//폭탄

	m_pPlayerList	= CObjMgr::GetInstance()->GetObjList(OBJ_PLAYER);

	return S_OK;
}
int CTowerWnd1::Progress(void)		
{
	MouseInput();
	if(m_fScale == 1 && KeyInput())	// 다커졌을때만 클릭가능
	{
		CSDevice::GetInstance()->SoundPlay(SD_TOWERBUILD, 0);
		// 타워누른거 없애기 작업
		list<CObj*>::iterator Toweriter = m_pTowerList->begin();	
		list<CObj*>::iterator TowerEnditer = m_pTowerList->end();
		for(; Toweriter != TowerEnditer; ++Toweriter )
		{
			if( D3DXVec3Length( & ((*Toweriter)->GetInfo().vPos - m_tInfo.vPos)) < 30
				&& ((CTower*)(*Toweriter))->GetTowerID() == TWR_EMPTY )
			{
				((CTower*)(*Toweriter))->SetDestroyTower(true);
			}
		}
		return 1;
	}
	m_fScale < 1.f ? m_fScale += CTimeMgr::GetInstance()->GetTime() : m_fScale = 1.f ;
		
	return 0;
}
void CTowerWnd1::Render(void)		
{
	D3DXMATRIX	matScale, matRangeScale, matTrans;
	if(m_iRenderNum != NOCHECK)
	{
		///* 타워 범위 */
		const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Tower", L"TowerUI", m_byStyle);

		float	fX			= pTexture->tImgInfo.Width  / 2.f;
		float	fY			= pTexture->tImgInfo.Height / 2.f;
		float	fRangeScale = m_fRange / pTexture->tImgInfo.Width;
		D3DXMatrixScaling(&matRangeScale, fRangeScale, fRangeScale/1.3f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&(matRangeScale * matTrans));
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
			&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

		/* 투명 타워 이미지 */
		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey, 0);
		D3DXMatrixScaling(&matScale, m_fScale, m_fScale, 0.f);
		
		m_tInfo.matWorld = matScale * matTrans;

		fX = pTexture->tImgInfo.Width  / 2.f;
		fY = pTexture->tImgInfo.Height / 1.5f;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
			&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(170,255,255,255));
	}
	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"TowerUp");
	D3DXMatrixScaling(&matScale, m_fScale, m_fScale, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	
	m_tInfo.matWorld = matScale * matTrans;

	float	fX = pTexture->tImgInfo.Width  / 2.f;
	float	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));


}
void CTowerWnd1::Release(void)		
{
}
void CTowerWnd1::MouseInput()
{
	D3DXVECTOR3 vMousePos = CMouse::GetPos();
	
	if( D3DXVec3Length(& ( m_vBtNum1 - vMousePos)) < dfButtSize)
	{
		m_iRenderNum = KNIGHT;
	}
	else if( D3DXVec3Length(& ( m_vBtNum2 - vMousePos)) < dfButtSize)
	{
		m_iRenderNum = ARCHOR;
	}
	else if( D3DXVec3Length(& ( m_vBtNum3 - vMousePos)) < dfButtSize)
	{
		m_iRenderNum = MAGIC;
	}
	else if( D3DXVec3Length(& ( m_vBtNum4 - vMousePos)) < dfButtSize)
	{
		m_iRenderNum = BOMB;
	}
	else
		m_iRenderNum = NOCHECK;

	switch(m_iRenderNum)
	{
	case 1:
		m_wstrObjKey	= L"Archer1T";
		m_wstrStateKey	= L"Tower";
		m_fRange		= ARCHOR_RANGE;
		m_byStyle		= 13;
		break;
	case 2:
		m_wstrObjKey	= L"Knight1T";
		m_wstrStateKey	= L"Tower";
		m_fRange		= KNIGHT_RANGE;
		m_byStyle		= 11;
		break;
	case 3:
		m_wstrObjKey	= L"Magic1T";
		m_wstrStateKey	= L"Tower";
		m_fRange		= MAGIC_RANGE;
		m_byStyle		= 13;
		break;
	case 4:
		m_wstrObjKey	= L"Bomb1T";
		m_wstrStateKey	= L"Tower";
		m_fRange		= BOMB_RANGE;
		m_byStyle		= 13;
		break;			

	}
}
bool CTowerWnd1::KeyInput()
{
	D3DXVECTOR3 vMousePos = CMouse::GetPos();
	if(CKeyMgr::GetInstance()->KeyPressed(KEY_LBUTTON, KEYNUM_2))
	{
		if( D3DXVec3Length(& ( m_vBtNum1 - vMousePos)) <= dfButtSize)		//archor
		{
			CObj* pArchorTower = new CArchorTower;
			pArchorTower->SetPos(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f));
			((CArchorTower*)pArchorTower)->SetSoldierList(m_pSoldierList);
			pArchorTower->Initialize();

			m_pTowerList->push_back(pArchorTower);

			((CPlayer*)m_pPlayerList->front())->SetGold(((CPlayer*)m_pPlayerList->front())->GetGold() - 70 );


			return true;
		}
		else if( D3DXVec3Length(& ( m_vBtNum2 - vMousePos)) <= dfButtSize)	//knight
		{
			CObj* pKnightTower = new CKnightTower;
			pKnightTower->SetPos(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f));
			((CKnightTower*)pKnightTower)->SetRellyPoint(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y + 60.f, 0.f));
			((CKnightTower*)pKnightTower)->SetSoldierList(m_pSoldierList);
			pKnightTower->Initialize();

			m_pTowerList->push_back(pKnightTower);

			((CPlayer*)m_pPlayerList->front())->SetGold(((CPlayer*)m_pPlayerList->front())->GetGold() - 70 );
			return true;
		}
		else if( D3DXVec3Length(& ( m_vBtNum3 - vMousePos)) <= dfButtSize)	//magic
		{
			//CObj* pMagicTower = CObjFactory<CMagicTower>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f));
			CObj* pMagicTower = new CMagicTower;
			pMagicTower->SetPos(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f));
			((CMagicTower*)pMagicTower)->SetSoldierList(m_pSoldierList);
			pMagicTower->Initialize();

			m_pTowerList->push_back(pMagicTower);

			((CPlayer*)m_pPlayerList->front())->SetGold(((CPlayer*)m_pPlayerList->front())->GetGold() - 100 );
			return true;
		}
		else if( D3DXVec3Length(& ( m_vBtNum4 - vMousePos)) <= dfButtSize)	//bomb
		{
			CObj* pBombTower = CObjFactory<CBombTower>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f));
			m_pTowerList->push_back(pBombTower);

			((CPlayer*)m_pPlayerList->front())->SetGold(((CPlayer*)m_pPlayerList->front())->GetGold() - 125 );
			return true;
		}
	}
	return false;
}
void CTowerWnd1::TowerCreate()
{
}