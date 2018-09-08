#include "StdAfx.h"
#include "Gagoil.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"

#include "ObjMgr.h"
#include "Player.h"

#include "Gold.h"
#include "ObjFactory.h"
#include "SDevice.h"

CGagoil::CGagoil(void)
{
}

CGagoil::~CGagoil(void)
{
	Release();
}
HRESULT CGagoil::Initialize(void)
{
	m_eMonsterID	= MON_GAGOIL;
	m_eMonType		= MTYPE_FLY;

	m_tStats		= UNITSTATS(L"Gagoil", 70, 0, 0, 30);
	m_iGold			= rand()%5 + 30;
	m_fMaxHP		= (float)m_tStats.iHP;

	m_wstrObjKey	= L"Gagoil";
	m_wstrStateKey	= L"Move_LR";

	m_tInfo.vPos	= D3DXVECTOR3((*m_pVecTile)[m_pVecRoute->back()]->vPos.x, (*m_pVecTile)[m_pVecRoute->back()]->vPos.y, 0.f);
	m_tInfo.vDir	= D3DXVECTOR3(1.f, 0.f, 0.f);
	m_tInfo.vLook	= D3DXVECTOR3(1.f, 0.f, 0.f);

	SetFrame(L"Move_LR");		//기본 모션

	m_fTheta		= 0.f;
	m_bMoveUp		= false;
	m_bMoveDown		= false;
	m_bMoveLR		= false;
	m_bAttack		= false;

	m_MyVecRoute = (*m_pVecRoute);

	m_bBattle		= false;
	m_bDie			= false;
	m_bDeadImg		= false;

	m_fHPPostion	= 40.f;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_tStats.szName, m_wstrStateKey, 0);
	m_fMonWidth		= pTexture->tImgInfo.Width / 2.f;

	m_bPoison		= false;
	m_bIntengle		= false;

	m_bSlow			= false;
	m_bTelpo		= false;

	m_pPlayerList	= CObjMgr::GetInstance()->GetObjList(OBJ_PLAYER);
	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_EFFECT);

	return S_OK;
}
int CGagoil::Progress(void)
{
	FrameMove();
	if( m_bDie )
	{
		if(!m_bDeadImg)
		{
			m_wstrStateKey	= L"Die";
			SetFrame(m_wstrStateKey);
			m_bDeadImg = true;
			CSDevice::GetInstance()->SoundPlay(SD_EXPL_DIE, 0);

			if(!m_bGoalLine)
			{
				((CPlayer*)m_pPlayerList->front())->SetGold(((CPlayer*)m_pPlayerList->front())->GetGold() + m_iGold);
				CObj* pGold = CObjFactory<CGold>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y - 20.f, 0.f));
				((CGold*)pGold)->SetGold(m_iGold);
				m_pEffectList->push_back(pGold);
			}
			else
				((CPlayer*)m_pPlayerList->front())->SetLife(((CPlayer*)m_pPlayerList->front())->GetLife() - 1 );

				

		}
		if(int(m_tFrame.fFrame) == 5)
			return 1;

		else
			return 0;
	}

	CMonster::Progress();
	

	return 0;
}
void CGagoil::Render(void)
{
	D3DXMATRIX		matScale, matTrans;

	float		fScale = 1.f;
	
	m_tInfo.vDir.x < 0 ? fScale *= -1 : fScale *= 1;

	int iRedBlue = 255;
	if(m_bPoison)
		iRedBlue = 100;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_tStats.szName, m_wstrStateKey, (int)m_tFrame.fFrame);
	D3DXMatrixScaling(&matScale, fScale, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 1.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,iRedBlue,255,iRedBlue));

	CMonster::Render();
}
void CGagoil::Release(void)
{
}
