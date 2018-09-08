#include "StdAfx.h"
#include "Freezing.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"

#include "Tower.h"
#include "ObjMgr.h"
#include "BrokeIce.h"
#include "ObjFactory.h"
#include "SDevice.h"

CFreezing::CFreezing(void)
:m_pTower(NULL)
{
}

CFreezing::~CFreezing(void)
{
	Release();
}
HRESULT CFreezing::Initialize(void)	
{
	m_eEffectID		= EFC_FREEZ;
	m_wstrObjKey	= L"IceAge";
	m_wstrStateKey	= L"Icing";
	SetFrame(m_wstrStateKey);

	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_EFFECT);

	CSDevice::GetInstance()->SoundPlay(SD_ICEAGE, 0);
	return S_OK;
}
int		CFreezing::Progress(void)		
{
	if( !((CTower*)m_pTower)->GetFreezing())
	{
		CObj*	pBrokeIce = CObjFactory<CBrokeIce>::CreateObj(m_tInfo.vPos);
		m_pEffectList->push_back(pBrokeIce);
		return 1;
	}
	if(m_tFrame.fFrame >= 9)
	{
		m_tFrame.fFrame = 9;
	}
	else
		m_tFrame.fFrame += m_tFrame.fCount * GET_TIME ;

	return 0;
}
void	CFreezing::Render(void)		
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
		&D3DXVECTOR3(fX, 45., 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
}
void	CFreezing::Release(void)		
{
}
