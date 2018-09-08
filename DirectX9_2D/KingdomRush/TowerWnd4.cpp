#include "StdAfx.h"
#include "TowerWnd4.h"

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
CTowerWnd4::CTowerWnd4(void)
{
}

CTowerWnd4::~CTowerWnd4(void)
{
	Release();
}
HRESULT CTowerWnd4::Initialize(void)
{
	m_bMouseOn		= false;
	m_fScale		= 0.8f;
	fRangeScale		= 0.f;
	m_iRenderNum	= -1;
	m_fRange		= 0;		// 타워 공격범위

	switch(((CTower*)m_pTower)->GetTowerID())
	{
	case TWR_ARCHOR1:
		m_wstrObjKey		= L"Archor3";
		break;
	case TWR_KNIGHT1:
		m_wstrObjKey		= L"Knight3";
		break;
	case TWR_MAGIC1:
		m_wstrObjKey		= L"Magic3";
		break;
	case TWR_BOMB1:
		m_wstrObjKey		= L"Bomb3";
		break;
	}
	m_szMouseKey	= L"RellyPoint";

	m_vMousePos	= D3DXVECTOR3( m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);	

	//upgrade
	m_vBtNum1	= D3DXVECTOR3( m_tInfo.vPos.x - 35.f, m_tInfo.vPos.y - 42.f, 0.f);	
	m_vBtNum2	= D3DXVECTOR3( m_tInfo.vPos.x + 34.5f, m_tInfo.vPos.y - 42.f, 0.f);	
	//sell
	m_vBtNum3	= D3DXVECTOR3( m_tInfo.vPos.x, m_tInfo.vPos.y + 45.f, 0.f);	
	//relly
	m_vBtNum4	= D3DXVECTOR3( m_tInfo.vPos.x + 45.f, m_tInfo.vPos.y + 30.f, 0.f);	

	m_bRelly	= false;

	m_pPlayerList	= CObjMgr::GetInstance()->GetObjList(OBJ_PLAYER);

	return S_OK;
}
int CTowerWnd4::Progress(void)		
{
	m_vMousePos = CMouse::GetPos();

	MouseInput();
	if(KeyInput())
	{
		if(m_bRelly)
			CSDevice::GetInstance()->SoundPlay(SD_RALLY, 0);
		return 1;
	}
	m_fScale < 1.f ? m_fScale += CTimeMgr::GetInstance()->GetTime() : m_fScale = 1.f ;

	return 0;
}
void CTowerWnd4::Render(void)		
{
	D3DXMATRIX	matScale, matRangeScale, matTrans;

	
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
	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey);
	D3DXMatrixScaling(&matScale, m_fScale, m_fScale, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	
	m_tInfo.matWorld = matScale * matTrans;

	float	fX = pTexture->tImgInfo.Width  / 2.f;
	float	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

	if(m_wstrObjKey == L"Knight3" && m_bRelly)
	{

		const TEXINFO*	pTexture = CTextureMgr::GetInstance()->GetTexture(m_szMouseKey);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_vMousePos.x, m_vMousePos.y, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		float fX = pTexture->tImgInfo.Width  / 2.f;
		float fY = pTexture->tImgInfo.Height / 2.f;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
			&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

		///* 사정거리 */
		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Tower", L"TowerUI", 11);

		fX			= pTexture->tImgInfo.Width  / 2.f;
		fY			= pTexture->tImgInfo.Height / 2.f;

		fRangeScale = ((CTower*)m_pTower)->GetTowerStats().iRange / pTexture->tImgInfo.Width;
		D3DXMatrixScaling(&matRangeScale, fRangeScale, fRangeScale / 1.3f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

		CDevice::GetInstance()->GetSprite()->SetTransform(&(matRangeScale * matTrans));
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
			&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, 255, 255, 255));

	}
}
void CTowerWnd4::Release(void)		
{
}
void CTowerWnd4::MouseInput()
{
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
	if( D3DXVec3Length(& ( m_vBtNum1 - m_vMousePos)) <= dfButtSize)
	{
		if(m_iRenderNum == ARCHOR)
		{
			m_fRange = 300.f;
		}
		else if(m_iRenderNum == MAGIC)
		{
			m_fRange = 250.f;
		}
		else if(m_iRenderNum == BOMB)
		{
			m_fRange = 300.f;
		}
		m_bMouseOn = true;
	}
	else if( D3DXVec3Length(& ( m_vBtNum2 - m_vMousePos)) <= dfButtSize)
	{
		if(m_iRenderNum == ARCHOR)
		{
			m_fRange = 400.f;
		}
		else if(m_iRenderNum == MAGIC)
		{
			m_fRange = 270.f;
		}
		else if(m_iRenderNum == BOMB)
		{
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


	if(m_bRelly)
	{
		int	iThisIndex	= GetTileIndex(m_vMousePos);

		if(iThisIndex == -1)
			return;

		float fMouseX	= m_vMousePos.x - m_tInfo.vPos.x;
		float fMouseY	= m_vMousePos.y - m_tInfo.vPos.y;

		float fRangeTower= ((CTower*)m_pTower)->GetTowerStats().iRange / 2.f;
	
		if( (pow(fMouseX, 2) / pow(fRangeTower, 2)) + (pow(fMouseY, 2) / pow(fRangeTower/ 1.3f , 2)) < 1.f )
			m_szMouseKey = L"RellyPoint";
		else
			m_szMouseKey = L"Dontmove";

		if( (*m_pVecTile)[iThisIndex]->byDrawID != 1 )
			m_szMouseKey = L"Dontmove";
	}
}
bool CTowerWnd4::KeyInput()
{
	if(CKeyMgr::GetInstance()->KeyPressed(KEY_LBUTTON, KEYNUM_4))
	{
		if(m_bRelly && m_szMouseKey == L"RellyPoint")
		{
			m_vRellyPoint = m_vMousePos;
			((CKnightTower*)m_pTower)->SetRellyPoint(m_vRellyPoint);
			return true;
		}
		//1.button
		if( D3DXVec3Length(& ( m_vBtNum1 - m_vMousePos)) <= dfButtSize)		
		{
			if(m_iRenderNum != BOMB && m_iRenderNum != MAGIC)
			{
				((CPlayer*)m_pPlayerList->front())->SetGold(((CPlayer*)m_pPlayerList->front())->GetGold() - 230 );
			}
			else if(m_iRenderNum == MAGIC )
				((CPlayer*)m_pPlayerList->front())->SetGold(((CPlayer*)m_pPlayerList->front())->GetGold() - 300 );
			else if(m_iRenderNum == BOMB )
				((CPlayer*)m_pPlayerList->front())->SetGold(((CPlayer*)m_pPlayerList->front())->GetGold() - 400 );
			
			((CTower*)m_pTower)->SetLevelup(3);
			((CTower*)m_pTower)->SetTowerClick(false);

			CSDevice::GetInstance()->SoundPlay(SD_TOWERUPGRADE, 0);
			return true;
		}
		//2.button
		else if( D3DXVec3Length(& ( m_vBtNum2 - m_vMousePos)) <= dfButtSize)		
		{
			if(m_iRenderNum != BOMB && m_iRenderNum != MAGIC)
			{
				((CPlayer*)m_pPlayerList->front())->SetGold(((CPlayer*)m_pPlayerList->front())->GetGold() - 230 );
			}
			else if(m_iRenderNum == MAGIC )
				((CPlayer*)m_pPlayerList->front())->SetGold(((CPlayer*)m_pPlayerList->front())->GetGold() - 300 );
			else if(m_iRenderNum == BOMB )
				((CPlayer*)m_pPlayerList->front())->SetGold(((CPlayer*)m_pPlayerList->front())->GetGold() - 370 );

			((CTower*)m_pTower)->SetLevelup(4);
			((CTower*)m_pTower)->SetTowerClick(false);

			CSDevice::GetInstance()->SoundPlay(SD_TOWERUPGRADE, 0);
			return true;
		}
		//3. Sell button
		else if( D3DXVec3Length(& ( m_vBtNum3 - m_vMousePos)) <= dfButtSize2)
		{
			((CTower*)m_pTower)->SetDestroyTower(true);
			
			CObj* pEmptyTower = CObjFactory<CEmptyTower>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f));
			m_pTowerList->push_back(pEmptyTower);

			CSDevice::GetInstance()->SoundPlay(SD_TOWERSELL, 0);
			return true;
		}
		else if( m_wstrObjKey == L"Knight3"
			&& D3DXVec3Length(& ( m_vBtNum4 - m_vMousePos)) <= dfButtSize2)
		{
			m_bRelly	= true;
			return false;
		}
	}
	return false;
}

bool CTowerWnd4::Picking(const D3DXVECTOR3& vPos, const int& iIndex)
{
	// 직선의 방정식을 이용한 PICKING 처리
	
	float		fGradient = (TILECY / 2.f) / (TILECX / 2.f);		// a

	D3DXVECTOR3		vPoint[4] = 
	{
		D3DXVECTOR3((*m_pVecTile)[iIndex]->vPos.x, (*m_pVecTile)[iIndex]->vPos.y + TILECY / 2.f, 0.f),
		D3DXVECTOR3((*m_pVecTile)[iIndex]->vPos.x + TILECX / 2.f, (*m_pVecTile)[iIndex]->vPos.y, 0.f),
		D3DXVECTOR3((*m_pVecTile)[iIndex]->vPos.x, (*m_pVecTile)[iIndex]->vPos.y - TILECY / 2.f, 0.f),
		D3DXVECTOR3((*m_pVecTile)[iIndex]->vPos.x - TILECX / 2.f, (*m_pVecTile)[iIndex]->vPos.y, 0.f)
	};
	// b = y - ax;	Y절편 구하기

	float fB[4] = {

		{	-fGradient	* vPoint[0].x + vPoint[0].y		},
		{	 fGradient	* vPoint[1].x + vPoint[1].y		},
		{	-fGradient	* vPoint[2].x + vPoint[2].y		},
		{	 fGradient	* vPoint[3].x + vPoint[3].y		}
	};
	// y = ax + b   // y - ax - b = 0 직선 위   // y - ax - b < 0 직선 아래  // y - ax - b > 0 직선 위
	// (0, 0) 기준으로 지나가는 선이라 생각하자
	if(vPos.y - fGradient * vPos.x - fB[0] < 0 &&	// 위쪽 두 개의 선
		vPos.y + fGradient * vPos.x - fB[1] < 0 && 
		vPos.y - fGradient * vPos.x - fB[2] >= 0 &&     // 아래쪽 두 개의 선
		vPos.y + fGradient * vPos.x - fB[3] >= 0 )
	{		
		return true;		
	}

	return false;
}
int CTowerWnd4::GetTileIndex(const D3DXVECTOR3& vPos)
{
	//vector<TILE*>::size_type
	for(size_t i = 0; i < (*m_pVecTile).size(); ++i)
	{
		if(Picking(vPos, i))
		{
			return i;
		}
	}
	return -1;
}