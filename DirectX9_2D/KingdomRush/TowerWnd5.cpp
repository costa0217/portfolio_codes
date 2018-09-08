#include "StdAfx.h"
#include "TowerWnd5.h"

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
CTowerWnd5::CTowerWnd5(void)
{
}
CTowerWnd5::~CTowerWnd5(void)
{
	Release();
}
HRESULT CTowerWnd5::Initialize(void)
{
	m_fScale		= 0.8f;
	fRangeScale		= 0.f;
	m_iRenderNum	= -1;
	m_fRange		= 0;		// 타워 공격범위
	m_iSkillCnt		= 0;		// 스킬갯수

	m_iTowerSkillOn[0] = 0;		// 타워의 스킬이 찍혔는가.
	m_iTowerSkillOn[1] = 0;

	switch(((CTower*)m_pTower)->GetTowerID())
	{
	case TWR_ARCHOR1:
		((CTower*)m_pTower)->GetLevel() == 3 ? m_wstrObjKey = L"Archor4-1" : m_wstrObjKey = L"Archor4-2";
		m_iSkillCnt = 2;
		break;
	case TWR_KNIGHT1:
		((CTower*)m_pTower)->GetLevel() == 3 ? m_wstrObjKey = L"Knight4-1" : m_wstrObjKey = L"Knight4-2";
		m_iSkillCnt = 2;
		break;
	case TWR_MAGIC1:
		((CTower*)m_pTower)->GetLevel() == 3 ? m_wstrObjKey = L"Magic4-1" : m_wstrObjKey = L"Magic4-2";
		((CTower*)m_pTower)->GetLevel() == 3 ? m_iSkillCnt = 2 : m_iSkillCnt = 1 ;
		break;
	case TWR_BOMB1:
		((CTower*)m_pTower)->GetLevel() == 3 ? m_wstrObjKey = L"Bomb4-1" : m_wstrObjKey = L"Bomb4-2";
		m_iSkillCnt = 1;
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

	m_vSkillOK[0] = D3DXVECTOR3(0.f, 0.f, 0.f);
	m_vSkillOK[1] = D3DXVECTOR3(0.f, 0.f, 0.f);

	
	//m_vRellyPoint	= D3DXVECTOR3( m_tInfo.vPos.x, m_tInfo.vPos.y + 80.f, 0.f);


	//skill Ok
	for( int i = 0; i < m_iSkillCnt; ++i )
	{
		m_vSkillOK[i] = D3DXVECTOR3( m_tInfo.vPos.x - 36.f + (70.f * i), m_tInfo.vPos.y - 58.f, 0.f);
	}

	m_pPlayerList	= CObjMgr::GetInstance()->GetObjList(OBJ_PLAYER);

	return S_OK;
}
int CTowerWnd5::Progress(void)		
{
	m_vMousePos = CMouse::GetPos();

	for( int i = 0; i < m_iSkillCnt; ++i )
	{
		if(((CTower*)m_pTower)->GetSkillOnOff(i))
			m_iTowerSkillOn[i] = 1;
		
		else
			m_iTowerSkillOn[i] = 0;			
	}

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
void CTowerWnd5::Render(void)		
{
	D3DXMATRIX	matScale, matRangeScale, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey);
	D3DXMatrixScaling(&matScale, m_fScale, m_fScale, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);
	
	m_tInfo.matWorld = matScale * matTrans;

	float	fX = pTexture->tImgInfo.Width  / 2.f;
	float	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

	if((m_wstrObjKey == L"Knight4-1" || m_wstrObjKey == L"Knight4-2" || m_wstrObjKey == L"Magic4-2")
		&& m_bRelly)
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
	if(m_fScale == 1.f)
	{
		for( int i = 0; i < m_iSkillCnt; ++i )
		{
			//m_vSkillOK[i] = D3DXVECTOR3( m_tInfo.vPos.x - 36.f + (70.f * i), m_tInfo.vPos.y - 58.f, 0.f);
			const TEXINFO*	pTexture = CTextureMgr::GetInstance()->GetTexture(L"Tower", L"TowerButton", m_iTowerSkillOn[i]);
			D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
			D3DXMatrixTranslation(&matTrans, m_vSkillOK[i].x, m_vSkillOK[i].y, 0.f);

			m_tInfo.matWorld = matScale * matTrans;

			float fX = pTexture->tImgInfo.Width  / 2.f;
			float fY = pTexture->tImgInfo.Height / 2.f;

			CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
			CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
				&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

		}
	}

}
void CTowerWnd5::Release(void)		
{
}
void CTowerWnd5::MouseInput()
{
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
bool CTowerWnd5::KeyInput()
{
	if(CKeyMgr::GetInstance()->KeyPressed(KEY_LBUTTON, KEYNUM_4))
	{
		if(m_bRelly && m_szMouseKey == L"RellyPoint")
		{
			m_vRellyPoint = m_vMousePos;

			if(m_wstrObjKey == L"Knight4-1" || m_wstrObjKey == L"Knight4-2")
				((CKnightTower*)m_pTower)->SetRellyPoint(m_vRellyPoint);
			else if(m_wstrObjKey == L"Magic4-2")
				((CMagicTower*)m_pTower)->SetRellyPoint(m_vRellyPoint);
			return true;
		}

		//1.button upgrade
		if( D3DXVec3Length(& ( m_vBtNum1 - m_vMousePos)) <= dfButtSize)		
		{
			if( ((CTower*)m_pTower)->GetSkillOnOff(0))
				return false;

			((CTower*)m_pTower)->SetSkillOnOff(0, true);
			((CPlayer*)m_pPlayerList->front())->SetGold(((CPlayer*)m_pPlayerList->front())->GetGold() - 300 );
			return false;
		}
		//2.button upgrade
		else if( D3DXVec3Length(& ( m_vBtNum2 - m_vMousePos)) <= dfButtSize)		
		{
			if(m_iSkillCnt == 1)
				return false;

			if( ((CTower*)m_pTower)->GetSkillOnOff(1))
				return false;

			((CTower*)m_pTower)->SetSkillOnOff(1, true);
			((CPlayer*)m_pPlayerList->front())->SetGold(((CPlayer*)m_pPlayerList->front())->GetGold() - 300 );
			return false;
		}

		//3. Sell button
		if( D3DXVec3Length(& ( m_vBtNum3 - m_vMousePos)) <= dfButtSize2)
		{
			((CTower*)m_pTower)->SetDestroyTower(true);
			
			CObj* pEmptyTower = CObjFactory<CEmptyTower>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f));
			m_pTowerList->push_back(pEmptyTower);

			CSDevice::GetInstance()->SoundPlay(SD_TOWERSELL, 0);
			return true;
		}
		else if( (m_wstrObjKey == L"Knight4-1" || m_wstrObjKey == L"Knight4-2" || m_wstrObjKey == L"Magic4-2")
			&& D3DXVec3Length(& ( m_vBtNum4 - m_vMousePos)) <= dfButtSize2)
		{
			m_bRelly	= true;
			((CTower*)m_pTower)->SetTowerClick(false);
			return false;
		}
	}
	return false;
}
bool CTowerWnd5::Picking(const D3DXVECTOR3& vPos, const int& iIndex)
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
int CTowerWnd5::GetTileIndex(const D3DXVECTOR3& vPos)
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