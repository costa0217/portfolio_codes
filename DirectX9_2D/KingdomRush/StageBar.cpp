#include "StdAfx.h"
#include "StageBar.h"

#include "Device.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "Player.h"

CStageBar::CStageBar(void)
{
}

CStageBar::~CStageBar(void)
{
}
HRESULT CStageBar::Initialize(void)
{
	m_eUIType		= UI_STAGEBAR;
	m_tInfo.vPos	= D3DXVECTOR3(100.f, 45.f, 0.f);
	m_pPlayerList	= CObjMgr::GetInstance()->GetObjList(OBJ_PLAYER);

	
	return S_OK;
}
int CStageBar::Progress(void)		
{
	if(!m_pPlayerList->empty())
	{
		m_iLife		= ((CPlayer*)m_pPlayerList->front())->GetLife();
		m_iWave		= ((CPlayer*)m_pPlayerList->front())->GetWave();
		m_iMaxWave	= ((CPlayer*)m_pPlayerList->front())->GetMaxWave();
		m_iGold		= ((CPlayer*)m_pPlayerList->front())->GetGold();
	}
	return 0;
}
void CStageBar::Render(void)		
{
	D3DXMATRIX		matScale, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Stagebar");
	D3DXMatrixScaling(&matScale, 1.1f, 1.1f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	m_tInfo.matWorld = matScale * matTrans;
	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));


	TCHAR	m_szMouse[MIN_STR];
	D3DXMatrixTranslation(&matTrans, 64.f, 26.f, 0.f);
	wsprintf(m_szMouse, L"%d", m_iLife);
	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetStageFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), m_szMouse, lstrlen(m_szMouse), 
		NULL, NULL, D3DCOLOR_ARGB(255, 200, 200, 200));

	D3DXMatrixTranslation(&matTrans, 118.f, 26.f, 0.f);
	wsprintf(m_szMouse, L"%d", m_iGold);
	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetStageFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), m_szMouse, lstrlen(m_szMouse), 
		NULL, NULL, D3DCOLOR_ARGB(255, 200, 200, 0));

	D3DXMatrixTranslation(&matTrans, 115.f, 51.f, 0.f);
	wsprintf(m_szMouse, L"%d / %d", m_iWave, m_iMaxWave);
	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetStageFont()->DrawTextW(CDevice::GetInstance()->GetSprite(), m_szMouse, lstrlen(m_szMouse), 
		NULL, NULL, D3DCOLOR_ARGB(255, 180, 150, 0));

}
void CStageBar::Release(void)		
{
}
