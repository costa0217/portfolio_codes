#include "StdAfx.h"
#include "Wizard.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"

#include "Tower.h"

CWizard::CWizard(void)
{
}

CWizard::~CWizard(void)
{
	Release();
}
HRESULT CWizard::Initialize(void)	
{
	m_wstrObjKey	= L"Magic1T";
	m_wstrStateKey	= L"Stand_D";

	m_eSoldierID	= SOL_WIZARD;
	m_bDie			= false;

	m_tInfo.vDir	= D3DXVECTOR3(1.f, 0.f, 0.f);

	m_iLevel		= 0;
	m_bLevelup		= false;
	for(int i = 0; i < LVL_END; ++i)
		m_bLvl[i] = false;

	return S_OK;
}
int		CWizard::Progress(void)		
{
	if(m_bDie)
		return 1;

	if(m_bLevelup)
		Leveling();

	return 0;
}
void	CWizard::Render(void)		
{
	D3DXMATRIX		matScale, matTrans;

	if( !((CTower*)m_pTower)->GetFreezing())
	{
		const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey, 0);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		float fX = pTexture->tImgInfo.Width  / 2.f;
		float fY = pTexture->tImgInfo.Height / 2.f;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
			&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
	}
}
void	CWizard::Release(void)		
{
}
void	CWizard::Leveling()
{
	if(m_bLvl[LVL_1])
	{
		m_wstrObjKey	= L"Magic1T";
		m_iLevel		= 0;
	}
	else if(m_bLvl[LVL_2])
	{
		m_wstrObjKey	= L"Magic2T";
		m_iLevel		= 1;
	}
	else if(m_bLvl[LVL_3])
	{
		m_wstrObjKey	= L"Magic3T";
		m_iLevel		= 2;
	}
	else if(m_bLvl[LVL_41])
	{
		m_wstrObjKey	= L"Magic4-2T";
		m_tInfo.vPos.x	+= 1;
		m_iLevel		= 3;
	}
	else if(m_bLvl[LVL_42])
	{
		m_wstrObjKey	= L"Magic4-1T";
		m_tInfo.vPos.x	+= 3;
		m_iLevel		= 4;
	}

	for(int i = 0; i < LVL_END; ++i)
		m_bLvl[i] = false;

	m_bLevelup = false;
}