#include "StdAfx.h"
#include "Back.h"
#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"
#include "KeyMgr.h"

#include "Mouse.h"

CBack::CBack(void)
{
}

CBack::~CBack(void)
{
	Release();
}
HRESULT CBack::Initialize(void)
{
	m_iTileRenderX	= WINCX / TILECX + 1;
	m_iTileRenderY	= WINCY / (TILECY / 2) + 1;


	m_tInfo.vPos	= D3DXVECTOR3(0.f, 0.f, 0.f);

	m_fSpeed		= 400.f;
	//m_wstrObjKey	= L"Stage1";

	m_iGoalIndex	= 0;	// 마우스 피킹타일
	m_bTileOpen		= false;

	
	ZeroMemory(m_szTileNum,sizeof(m_szTileNum));
	ZeroMemory(m_szTileID ,sizeof(m_szTileID));

	return S_OK;
}
int CBack::Progress(void)
{
	//Scroll();
	KeyInput();
	//D3DXMatrixTranslation(&m_matTileTrans, 500.f, 80.f, 0.f);
	//D3DXMatrixTranslation(&m_matTileTrans2, 500.f, 100.f, 0.f);
	D3DXMatrixTranslation(&m_matTileTrans3, 300.f, 550.f, 0.f);
	return 0;
}
void CBack::Render(void)
{
	D3DXMATRIX		matTrans;
	D3DXMatrixIdentity(&m_tInfo.matWorld);

	/* 배경 */
	const TEXINFO*		pTexture;
	pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matTrans;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));


	/* 디버깅 용도 */
	/*wsprintf(m_szTileNum, L"TileNum : %d", m_iGoalIndex);
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matTileTrans);
	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), m_szTileNum, lstrlen(m_szTileNum), 
		NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));*/

	/*wsprintf(m_szMouse, L"X : %d, Y : %d", int(GetMouse().x - m_vScroll.x), int(GetMouse().y - m_vScroll.y));
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_matTileTrans3);
	CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), m_szMouse, lstrlen(m_szMouse), 
		NULL, NULL, D3DCOLOR_ARGB(255, 0, 255, 255));*/

	/*if(m_iGoalIndex >= 0)
	{
		wsprintf(m_szTileID, L"TileID : %d", (*m_pVecTile)[m_iGoalIndex]->byDrawID);
		CDevice::GetInstance()->GetSprite()->SetTransform(&m_matTileTrans2);
		CDevice::GetInstance()->GetFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), m_szTileID, lstrlen(m_szTileID), 
			NULL, NULL, D3DCOLOR_ARGB(255, 255, 255, 255));
	}*/
	/* */

	if(m_bTileOpen)
	{
		for(int i = 0; i < m_iTileRenderY; ++i)
		{
			for(int j = 0; j < m_iTileRenderX; ++j)
			{
				int		iCullX = int(-m_vScroll.x) / TILECX;
				int		iCullY = int(-m_vScroll.y) / (TILECY / 2);

				int iIndex = (i + iCullY) * TILEX + (j + iCullX);

				if(iIndex < 0 || iIndex >= TILEX * TILEY)
					continue;

				pTexture = CTextureMgr::GetInstance()->GetTexture(L"BACK", L"Tile", (*m_pVecTile)[iIndex]->byDrawID);

				D3DXMatrixTranslation(&matTrans, 
					(*m_pVecTile)[iIndex]->vPos.x,
					(*m_pVecTile)[iIndex]->vPos.y,
					0.f);

				m_tInfo.matWorld = matTrans;

				CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
				CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
					&D3DXVECTOR3(10.f,10.f,0.f), NULL, D3DCOLOR_ARGB(100,255,255,255));
			}
		}

		const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"UU");
		D3DXMatrixTranslation(&matTrans, 40.f, 85.f, 0.f);

		m_tInfo.matWorld = matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
			NULL, NULL, D3DCOLOR_ARGB(255,255,255,255));
	}
}
void CBack::Release(void)
{
	if( !(*m_pVecTile).empty())
	{
		for_each((*m_pVecTile).begin(), (*m_pVecTile).end(), DeleteOBJ());
		(*m_pVecTile).clear();
		vector<TILE*>().swap((*m_pVecTile));
	}
}
void CBack::KeyInput()
{
	if(CKeyMgr::GetInstance()->KeyPressed(KEY_LBUTTON, KEYNUM_1))
	{		
		D3DXVECTOR3		vMousePos = ::GetMouse();
		m_iGoalIndex	= GetTileIndex(vMousePos);				// 목적지 인덱스
	}
	if(CKeyMgr::GetInstance()->KeyDown(KEY_Q))
	{
		m_bTileOpen ? m_bTileOpen = false : m_bTileOpen = true;	
	}
	
}

bool CBack::Picking(const D3DXVECTOR3& vPos, const int& iIndex)
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
int CBack::GetTileIndex(const D3DXVECTOR3& vPos)
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