#include "StdAfx.h"
#include "BigGoblin.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"

#include "Soldier.h"
#include "FootMan.h"
#include "Golem.h"

#include "ObjMgr.h"
#include "Player.h"

#include "Gold.h"
#include "ObjFactory.h"
#include "SDevice.h"

CBigGoblin::CBigGoblin(void)
{
}

CBigGoblin::~CBigGoblin(void)
{
	Release();
}
HRESULT CBigGoblin::Initialize(void)
{
	m_pSoldier		= NULL;

	m_eMonsterID	= MON_BIGGOB;
	m_eMonType		= MTYPE_GROUND;

	m_tStats		= UNITSTATS(L"BigGoblin", 200, 3, 15, 30);
	m_iGold			= rand()%15 + 30;
	m_fMaxHP		= (float)m_tStats.iHP;

	m_wstrObjKey	= L"BigGoblin";
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
	m_bDamage		= false;
	m_fMonSize		= 17.f;
	m_fHPPostion	= 37.f;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_tStats.szName, m_wstrStateKey, 0);
	m_fMonWidth		= pTexture->tImgInfo.Width / 2.f;

	m_bPoison		= false;
	m_bIntengle		= false;
	m_bIntenImg		= false;

	m_bSlow			= false;
	m_bTelpo		= false;

	m_pPlayerList	= CObjMgr::GetInstance()->GetObjList(OBJ_PLAYER);
	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_EFFECT);

	return S_OK;
}
int CBigGoblin::Progress(void)
{
	FrameMove();
	if(m_bBattle && m_tFrame.fFrame == 0)
		m_bDamage = false;
	if(m_bIntengle && m_tFrame.fFrame == 0)
	{
		m_bIntengle = false;
		m_bIntenImg	= false;
	}

	if( m_bDie )
	{
		if(!m_bDeadImg)
		{
			m_wstrStateKey	= L"Die";
			SetFrame(m_wstrStateKey);
			m_bDeadImg = true;
			CSDevice::GetInstance()->SoundPlay(SD_BIG_DIE, 0);

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
		if(int(m_tFrame.fFrame) == 9)
			return 1;

		else
			return 0;
	}

	CMonster::Progress();

	if( m_bIntengle )
	{
		m_bMoveUp		= false;
		m_bMoveDown		= false;
		m_bMoveLR		= false;

		if(!m_bIntenImg)
		{
			m_wstrStateKey	= L"Intengle";
			SetFrame(m_wstrStateKey);
			m_bIntenImg	= true;

			CSDevice::GetInstance()->SoundPlay(SD_INTENGLE, 0);
		}
	}

	if(m_bBattle && !m_bIntengle)
	{
		if(!m_bAttack)
		{
			m_wstrStateKey	= L"Attack";
			SetFrame(m_wstrStateKey);
			m_bMoveUp		= false;
			m_bMoveDown		= false;
			m_bMoveLR		= false;
			m_bAttack		= true;
		}
		if( int(m_tFrame.fFrame) == 8 && !m_bDamage)
		{
			if( ((CSoldier*)m_pSoldier)->GetSoliderID() == SOL_FOOTMAN )
				((CFootMan*)m_pSoldier)->SetFootManHP(((CFootMan*)m_pSoldier)->GetFootManHP() - m_tStats.iMaxDam);
			else if( ((CSoldier*)m_pSoldier)->GetSoliderID() == SOL_GOLEM )
				((CGolem*)m_pSoldier)->SetGolemHP(((CGolem*)m_pSoldier)->GetGolemHP() - m_tStats.iMaxDam);

			m_bDamage = true;				
		}
		if( ((CSoldier*)m_pSoldier)->GetSoliderID() == SOL_FOOTMAN )
		{
			if( ((CFootMan*)m_pSoldier)->GetFootManHP() <= 0 )
			{
				((CFootMan*)m_pSoldier)->SetDie(true);
				m_pSoldier		= NULL;
				m_bBattle		= false;
				m_bAttack		= false;
				m_bDamage		= false;
			}
		}
		else if( ((CSoldier*)m_pSoldier)->GetSoliderID() == SOL_GOLEM )
		{
			if( ((CGolem*)m_pSoldier)->GetGolemHP() <= 0 )
			{
				((CGolem*)m_pSoldier)->SetDie(true);
				m_pSoldier		= NULL;
				m_bBattle		= false;
				m_bAttack		= false;
				m_bDamage		= false;
			}
		}
		m_bMoveUp		= false;
		m_bMoveDown		= false;
		m_bMoveLR		= false;
	}

	if( m_pSoldier != NULL 
		&& (((CFootMan*)m_pSoldier)->GetDie() || ((CGolem*)m_pSoldier)->GetDie()))
	{
		m_bBattle	= false;
		m_bAttack	= false;
	}

	return 0;
}
void CBigGoblin::Render(void)
{
	D3DXMATRIX		matScale, matTrans;

	float		fScale = 1.f;
	
	m_tInfo.vDir.x < 0 ? fScale *= -1 : fScale *= 1;

	//공격모드일경우 몬스터 방향 바라보기
	if(m_bAttack && m_pSoldier != NULL)
		fScale = -1;

	int iRedBlue = 255;
	if(m_bPoison)
		iRedBlue = 100;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_tStats.szName, m_wstrStateKey, (int)m_tFrame.fFrame);
	D3DXMatrixScaling(&matScale, fScale, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 1.3f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,iRedBlue,255,iRedBlue));

	CMonster::Render();
}
void CBigGoblin::Release(void)
{
}
