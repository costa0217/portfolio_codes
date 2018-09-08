#include "StdAfx.h"
#include "BackGround.h"
#include "TextureMgr.h"
#include "ToolView.h"
#include "Device.h"

CBackGround::CBackGround(void)
{
}

CBackGround::~CBackGround(void)
{
	Release();
}

HRESULT CBackGround::Initialize()
{
	for(int i = 0; i < TILEY; ++i)
	{
		for(int j = 0; j < TILEX; ++j)
		{
			TILE*		pTile = new TILE;

			float		fX = j * TILECX + (i % 2) * (TILECX / 2.f);
			float		fY = i * (TILECY / 2.f);

			pTile->vPos  = D3DXVECTOR3(fX, fY, 0.f);
			pTile->vSize = D3DXVECTOR3(float(TILECX), float(TILECY), 0.f);
			pTile->byDrawID = 0;
			pTile->byOption = 0;

			m_VecTile.push_back(pTile);

		}
	}
	return 0;
}

void CBackGround::Progress()
{

}

void CBackGround::Render()
{
	D3DXMATRIX		matScale, matTrans;
	TCHAR			szBuf[MIN_STR] = L"";

	for(int i = 0; i < TILEY; ++i)
	{
		for(int j = 0; j < TILEX; ++j)
		{
			int iIndex = i * TILEX + j;

			const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"BACK", L"Tile", m_VecTile[iIndex]->byDrawID);

			D3DXMatrixTranslation(&matTrans, 
				m_VecTile[iIndex]->vPos.x - m_MainView->GetScrollPos(0),
				m_VecTile[iIndex]->vPos.y - m_MainView->GetScrollPos(1),
				0.f);
			
			CDevice::GetInstance()->GetSprite()->SetTransform(&(matTrans/**matScale*/));
			CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
				&D3DXVECTOR3(10.f, 10.f, 0.f), NULL, D3DCOLOR_ARGB(100,255,255,255));

		
		}
	}
}

void CBackGround::Release()
{
	for_each(m_VecTile.begin(), m_VecTile.end(), DeleteOBJ());
	m_VecTile.clear();
}
int CBackGround::GetTileIndex(const D3DXVECTOR3& vPos)
{
	//vector<TILE*>::size_type
	for(size_t i = 0; i < m_VecTile.size(); ++i)
	{
		if(Picking(vPos, i))
		{
			return i;
		}
	}


	return -1;
}

void CBackGround::TileChange(const D3DXVECTOR3& vPos, const BYTE& byDrawID)
{
		int iIndex = GetTileIndex(vPos);

		if(iIndex == -1)
			return;

		m_VecTile[iIndex]->byDrawID = byDrawID;
		m_VecTile[iIndex]->byOption = 0;
}

bool CBackGround::Picking(const D3DXVECTOR3& vPos, const int& iIndex)
{
	// 내적을 이용한 PICKING 처리

	D3DXVECTOR3		vPoint[4] = 
	{
		D3DXVECTOR3(m_VecTile[iIndex]->vPos.x, m_VecTile[iIndex]->vPos.y - TILECY / 2.f, 0.f),
		D3DXVECTOR3(m_VecTile[iIndex]->vPos.x + TILECX / 2.f, m_VecTile[iIndex]->vPos.y, 0.f),
		D3DXVECTOR3(m_VecTile[iIndex]->vPos.x, m_VecTile[iIndex]->vPos.y + TILECY / 2.f, 0.f),
		D3DXVECTOR3(m_VecTile[iIndex]->vPos.x - TILECX / 2.f, m_VecTile[iIndex]->vPos.y, 0.f)
	};
	
	D3DXVECTOR3		vDirection[4] =
	{
		vPoint[1] - vPoint[0],
		vPoint[2] - vPoint[1],
		vPoint[3] - vPoint[2],
		vPoint[0] - vPoint[3]

	};

	// 법선 벡터 정의 ex) 만약 (1,0,0)이란 벡터의 법선 벡터를 만든다면? 
	// x와 y의 값을 교환하여 수직인 (0, -1, 0)의 벡터를 만들 수 있다.
	// y값에 -가 붙는 이유는 윈도우 좌표계이기 때문
	
	D3DXVECTOR3		vNormal[4] = 
	{
		D3DXVECTOR3(vDirection[0].y, -vDirection[0].x, 0.f),
		D3DXVECTOR3(vDirection[1].y, -vDirection[1].x, 0.f),
		D3DXVECTOR3(vDirection[2].y, -vDirection[2].x, 0.f),
		D3DXVECTOR3(vDirection[3].y, -vDirection[3].x, 0.f)
	};

	for(int i = 0; i < 4; ++i)
		D3DXVec3Normalize(&vNormal[i], &vNormal[i]);

	for(int i = 0; i < 4; ++i)
	{
		D3DXVECTOR3			vDestDir = vPos - vPoint[i];

		float fDotResult = D3DXVec3Dot(&vDestDir, &vNormal[i]);

		if(fDotResult > 0.f)
			return false;
	}

	return true;
}