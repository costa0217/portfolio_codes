#include "StdAfx.h"
#include "StartButt.h"

#include "TextureMgr.h"
#include "Device.h"
#include "ObjMgr.h"

#include "Player.h"

CStartButt::CStartButt(void)
{
}

CStartButt::~CStartButt(void)
{
}
HRESULT CStartButt::Initialize(void)
{
	m_eUIType		= UI_STARTBUTT;
	m_tInfo.vPos	= D3DXVECTOR3(655.f, 570.f, 0.f);

	m_iSwitch		= 1;

	m_bClick		= false;

	m_pPlayerList	= CObjMgr::GetInstance()->GetObjList(OBJ_PLAYER);


	return S_OK;
}
int CStartButt::Progress(void)		
{
	if(((CPlayer*)m_pPlayerList->front())->GetWave() == 0)
		m_iSwitch = 1;
	else
		(*m_pfNextTime) < 11.f ? m_iSwitch = 1 : m_iSwitch = 0 ;

	if(m_bClick)
	{
		if(m_iSwitch == 1)
		{
			int iWave = ((CPlayer*)m_pPlayerList->front())->GetWave();
			if(iWave < ((CPlayer*)m_pPlayerList->front())->GetMaxWave() )
			{
				((CPlayer*)m_pPlayerList->front())->SetWave(((CPlayer*)m_pPlayerList->front())->GetWave() + 1);
				m_pbWave[((CPlayer*)m_pPlayerList->front())->GetWave()] = true;
			}
			(*m_pbButtDown)		= true;
			(*m_pbWaveStart)	= false;

		}
		m_bClick			= false;
	}
	return 0;
}
void CStartButt::Render(void)		
{
	D3DXMATRIX		matScale, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(L"Button", L"Start", m_iSwitch);
	
	D3DXMatrixTranslation(&matTrans, float(int(m_tInfo.vPos.x)), float(int(m_tInfo.vPos.y)), 0.f);

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
}
void CStartButt::Release(void)		
{
}

