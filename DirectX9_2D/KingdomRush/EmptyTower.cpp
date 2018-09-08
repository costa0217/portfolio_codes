#include "StdAfx.h"
#include "EmptyTower.h"

#include "KeyMgr.h"
#include "TextureMgr.h"
#include "Device.h"

#include "Mouse.h"

CEmptyTower::CEmptyTower(void)
{
}

CEmptyTower::~CEmptyTower(void)
{
}
HRESULT CEmptyTower::Initialize(void)
{	
	m_eTowerID		= TWR_EMPTY;
	
	m_bDestroyTower = false;
	m_wstrObjKey	= L"TowerStructuer";

	m_bFreezing		= false;
	m_bFreezImg		= false;
	

	return S_OK;
}
int CEmptyTower::Progress(void)		
{
	if(m_bDestroyTower)
		return 1;

	m_bFreezing		= false;

	if( m_iStageNum == 1)
	{
		m_wstrObjKey	= L"TowerStructuer";
	}
	else if(m_iStageNum == 2)
	{
		m_wstrObjKey	= L"TowerStructuer2";
	}
	return 0;
}
void CEmptyTower::Render(void)		
{
	D3DXMATRIX		matScale, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

}
void CEmptyTower::Release(void)		
{
}
void CEmptyTower::KeyInput()
{

}