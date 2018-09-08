#include "StdAfx.h"
#include "Crator.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"

CCrator::CCrator(void)
{
}

CCrator::~CCrator(void)
{
}
HRESULT CCrator::Initialize(void)
{
	m_eProID		= PRO_CRATOR;

	m_iCnt			= 0;
	m_bOneCircle	= false;

	m_fRange		= 20.f;
	m_iDamage		= 2000;

	m_wstrObjKey	= L"Meteo";
	SetFrame(L"Dot");

	return S_OK;
}
int CCrator::Progress(void)		
{
	m_bOneCircle = FrameMove(2.f);
	if(m_bOneCircle)
		++m_iCnt;

	if(m_iCnt > 10)
		return 1;

	return 0;
}
void CCrator::Render(void)		
{
	D3DXMATRIX		matScale, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey, (int)m_tFrame.fFrame);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
}
void CCrator::Release(void)		
{
}
