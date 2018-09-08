#include "StdAfx.h"
#include "Scream.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"

CScream::CScream(void)
{
}

CScream::~CScream(void)
{
	Release();
}
HRESULT CScream::Initialize(void)
{
	m_eEffectID		= EFC_SCREAM;
	m_wstrObjKey	= L"UI";
	m_wstrStateKey	= L"Scream";

	m_fTheta		= D3DXToRadian( rand()%90 + 325 );

	m_fScreaTime	= 0.f;

	m_iRed			= 255;
	m_iGreen		= 255;
	m_iBlue			= 255;

	m_iRandom		= rand()%10;

	return S_OK;
}
int		CScream::Progress(void)	
{
	if(m_eScream != SR_CRIT && m_iRandom < 5)
		return 1;

	m_fScreaTime += GET_TIME;
	if(m_fScreaTime > 1.f)
		return 1;

	return 0;
}
void	CScream::Render(void)	
{
	D3DXMATRIX		matScale, matTrans, matRotZ;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey, m_eScream);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, m_fTheta);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255, m_iRed, m_iGreen, m_iBlue));
}
void	CScream::Release(void)	
{
}


void	CScream::SetRGB(int _R, int _G, int _B)
{
	m_iRed		= _R;
	m_iGreen	= _G;
	m_iBlue		= _B;
}