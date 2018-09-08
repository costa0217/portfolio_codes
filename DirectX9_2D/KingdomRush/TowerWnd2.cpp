#include "StdAfx.h"
#include "TowerWnd2.h"

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
CTowerWnd2::CTowerWnd2(void)
{
}

CTowerWnd2::~CTowerWnd2(void)
{
	Release();
}
HRESULT CTowerWnd2::Initialize(void)
{

	m_fScale		= 0.8f;
	fRangeScale		= 0.f;
	m_iRenderNum	= 0;
	m_wstrObjKey	= L"";
	m_wstrStateKey	= L"";
	m_szMouseKey	= L"RellyPoint";

	m_vMousePos	= D3DXVECTOR3( m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);	

	//upgrade
	m_vBtNum1	= D3DXVECTOR3( m_tInfo.vPos.x - 35.f, m_tInfo.vPos.y - 42.f, 0.f);	
	//sell
	m_vBtNum2	= D3DXVECTOR3( m_tInfo.vPos.x, m_tInfo.vPos.y + 45.f, 0.f);	
	//relly
	m_vBtNum3	= D3DXVECTOR3( m_tInfo.vPos.x + 45.f, m_tInfo.vPos.y + 30.f, 0.f);	

	m_bRelly	= false;

	m_pPlayerList	= CObjMgr::GetInstance()->GetObjList(OBJ_PLAYER);

	return S_OK;
}
int CTowerWnd2::Progress(void)		
{
	m_vMousePos = CMouse::GetPos();

	MouseInput();
	if(KeyInput())
	{
		if(m_bRelly)
		{
			CSDevice::GetInstance()->SoundPlay(SD_RALLY, 0);

		}
		return 1;
	}
	m_fScale < 1.f ? m_fScale += CTimeMgr::GetInstance()->GetTime() : m_fScale = 1.f ;
	return 0;
}
void CTowerWnd2::Render(void)		
{
	D3DXMATRIX	matScale, matRangeScale, matTrans, matPriceTrans;


	/* 유아이창 */
	const TEXINFO*	pTexture = CTextureMgr::GetInstance()->GetTexture(L"Upgrade(Move)");
	D3DXMatrixScaling(&matScale, m_fScale, m_fScale, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

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
	if(m_bRelly)
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
void CTowerWnd2::Release(void)		
{
}
void CTowerWnd2::MouseInput()
{
	if(m_bRelly)
	{
		int	iThisIndex	= GetTileIndex(m_vMousePos);

		if(iThisIndex == -1)
			return;

		/* 타원의 방정식 */
		float fMouseX	= m_vMousePos.x - m_tInfo.vPos.x;
		float fMouseY	= m_vMousePos.y - m_tInfo.vPos.y;

		float fRange	= ((CTower*)m_pTower)->GetTowerStats().iRange / 2.f;
	
		if( (pow(fMouseX, 2) / pow(fRange, 2)) + (pow(fMouseY, 2) / pow(fRange/ 1.3f , 2)) < 1.f )
			m_szMouseKey = L"RellyPoint";
		else
			m_szMouseKey = L"Dontmove";

		if( (*m_pVecTile)[iThisIndex]->byDrawID != 1 )
			m_szMouseKey = L"Dontmove";

	}
}
bool CTowerWnd2::KeyInput()
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
		else if( D3DXVec3Length(& ( m_vBtNum2 - m_vMousePos)) <= dfButtSize2)
		{
			((CTower*)m_pTower)->SetDestroyTower(true);

			CObj* pEmptyTower = CObjFactory<CEmptyTower>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f));
			m_pTowerList->push_back(pEmptyTower);

			CSDevice::GetInstance()->SoundPlay(SD_TOWERSELL, 0);
			return true;
		}
		else if( D3DXVec3Length(& ( m_vBtNum3 - m_vMousePos)) <= dfButtSize2)
		{
			m_bRelly	= true;
			return false;
		}

		
	}
	return false;
}
 
bool CTowerWnd2::Picking(const D3DXVECTOR3& vPos, const int& iIndex)
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
int CTowerWnd2::GetTileIndex(const D3DXVECTOR3& vPos)
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