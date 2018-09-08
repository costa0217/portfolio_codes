#include "StdAfx.h"
#include "FootMan.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"

#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Heal.h"

#include "Monster.h"

#include "Scream.h"
#include "SDevice.h"

CFootMan::CFootMan(void)
:m_bStand(false)
,m_bMove(false)
,m_bAttack(false)
,m_pMonster(NULL)
,m_pTower(NULL)
,m_bEat(false)
{
}

CFootMan::~CFootMan(void)
{
	Release();
}
HRESULT CFootMan::Initialize(void)	
{
	m_wstrObjKey	= L"Knight1T";
	m_wstrStateKey	= L"Stand";
	m_tStats		= UNITSTATS(L"Knight1T", 50, 1, 3, 100.f);
	m_fMaxHP		= (float)m_tStats.iHP;

	m_eSoldierID	= SOL_FOOTMAN;
	m_bDie			= false;
	m_bDeadImg		= false;

	m_bBattleMode	= false;

	m_bDamage		= false;

	m_bHeal			= false;
	m_fHealTime		= 0.f;

	m_tInfo.vDir	= D3DXVECTOR3(1.f, 0.f, 0.f);
	m_tInfo.vLook	= D3DXVECTOR3(1.f, 0.f, 0.f);


	/* 레벨 시스템 */
	m_iLevel		= 0;
	m_bLevelup		= false;
	for(int i = 0; i < LVL_END; ++i)
		m_bLvl[i] = false;

	m_fAtkSpeed		= 2.f;
	m_iPlusDamage	= 0;
	m_iHPup			= false;

	/* 병사 간격조정 */
	switch(m_iIndex)
	{
	case 1:
		m_vMyRelly	= D3DXVECTOR3(m_vRellyPoint.x, m_vRellyPoint.y - 17.f, 0.f);
		break;
	case 2:
		m_vMyRelly	= D3DXVECTOR3(m_vRellyPoint.x - 17.f, m_vRellyPoint.y + 7.f, 0.f);
		break;
	case 3:
		m_vMyRelly	= D3DXVECTOR3(m_vRellyPoint.x + 17.f, m_vRellyPoint.y + 7.f, 0.f);
		break;
	case 4:
		m_vMyRelly	= D3DXVECTOR3(m_vRellyPoint.x + 10.f, m_vRellyPoint.y, 0.f);
		break;
	case 5:
		m_vMyRelly	= D3DXVECTOR3(m_vRellyPoint.x - 10.f, m_vRellyPoint.y, 0.f);
		break;
	}

	m_fDirTime	= 0;
	SetFrame(L"Stand");		//기본 모션

	for(int i = 0; i < 2; ++i)
		m_bSkill[i] = false;

	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_EFFECT);

	m_fLifeTime	= 0.f;
	m_fNoAttackTime	= 0.f;

	return S_OK;
}
int		CFootMan::Progress(void)		
{
	FrameMove(m_fAtkSpeed);	// 프레임속도 조정

	if(!m_bAttack)
	{
		m_fNoAttackTime += GET_TIME;
		if(m_fNoAttackTime > 1.f)
		{
			CSDevice::GetInstance()->SoundStop(SD_FOOTMANBATTLE);
			m_fNoAttackTime = 0;
		}
	}
	else
		m_fNoAttackTime = 0;
	if(m_bBattleMode && m_tFrame.fFrame == 0)
		m_bDamage = false;
	if(m_wstrStateKey == L"Skill2" && m_tFrame.fFrame == 0)
	{
		m_wstrStateKey = L"Stand";
		SetFrame(m_wstrStateKey);
	}

	
	if(m_bDie)
	{
		if(!m_bDeadImg)
		{
			m_wstrStateKey	= L"Die";
			SetFrame(m_wstrStateKey);
			m_bDeadImg = true;

			CSDevice::GetInstance()->SoundPlay(SD_FOOTMANDIE, 0);
		}
		if(int(m_tFrame.fFrame) == 5)
			return 1;

		else
			return 0;
	}

	if( m_iIndex == 4 || m_iIndex == 5)
	{
		m_fLifeTime += GET_TIME;
		if( m_fLifeTime > 6.f )
		{
			m_bDie = true;
			m_fLifeTime	= 0.f;
		}
	}
	if( m_pMonster	!= NULL && ((CMonster*)m_pMonster)->GetTelpo() )
	{
		m_pMonster		= NULL;
		m_bBattleMode	= false;
	}

	if(m_bLevelup)
		Leveling();

	if(m_bHeal)
	{
		m_fHealTime += GET_TIME;
		if(m_fHealTime > 2.f)
		{
			if(m_fMaxHP > (float)m_tStats.iHP + 10)
			{
				m_tStats.iHP += 10;
				CObj*	pHeal = CObjFactory<CHeal>::CreateObj(m_tInfo.vPos);
				((CHeal*)pHeal)->SetFootMan(this);

				m_pEffectList->push_back(pHeal);
			}
			m_fHealTime = 0.f;
		}
	}


	if(m_bBattleMode)	// 전투 상태
	{
		if(!CSDevice::GetInstance()->SoundPlaying(SD_FOOTMANBATTLE))
		{
			CSDevice::GetInstance()->SoundPlay(SD_FOOTMANBATTLE, 0);
		}
		D3DXVECTOR3 vMonster = D3DXVECTOR3(m_pMonster->GetInfo().vPos.x	- ((CMonster*)m_pMonster)->GetSize()
										, m_pMonster->GetInfo().vPos.y - 2.f, 0.f);
		m_tInfo.vDir = vMonster - m_tInfo.vPos;
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
		
		if( D3DXVec3Length(& ( vMonster - m_tInfo.vPos )) < 2 )
		{
			if(!m_bAttack)
			{
				m_wstrObjKey == L"Knight4-1T" ? m_wstrStateKey	= L"Attack1" : m_wstrStateKey	= L"Attack";
				
				SetFrame(m_wstrStateKey);
				m_bAttack	= true;
			}
			if( int(m_tFrame.fFrame) == 3 && !m_bDamage)
			{
				CObj* pPow = CObjFactory<CScream>::CreateObj(D3DXVECTOR3(m_tInfo.vPos.x, m_tInfo.vPos.y - 20.f, 0.f));
				((CScream*)pPow)->SetScream(SR_POW);
				m_pEffectList->push_back(pPow);

				((CMonster*)m_pMonster)->SetMonsterHP(((CMonster*)m_pMonster)->GetMonsterHP() - (m_tStats.iMaxDam + m_iPlusDamage));
				m_bDamage = true;				
			}
			if( ((CMonster*)m_pMonster)->GetMonsterHP() < 0 )
			{
				((CMonster*)m_pMonster)->SetMonsterDie(true);
				m_pMonster		= NULL;
				m_bBattleMode	= false;
				m_bAttack		= false;
				m_bDamage		= false;
			}
			m_bStand	= false;
			m_bMove		= false;
		}
		else
		{
			/* 몬스터방향으로 이동 */
			m_tInfo.vPos += m_tInfo.vDir * m_tStats.fSpeed * GET_TIME;	
			if(!m_bMove)
			{
				m_wstrStateKey	= L"Move";
				SetFrame(m_wstrStateKey);			
				m_bMove		= true;
				m_bStand	= false;
			}
		}
	}
	else			//비전투 상태
	{
		//CSDevice::GetInstance()->SoundStop(SD_FOOTMANBATTLE);
		/* 병사 간격조정 */
		switch(m_iIndex)
		{
		case 1:
			m_vMyRelly	= D3DXVECTOR3(m_vRellyPoint.x, m_vRellyPoint.y - 17.f, 0.f);
			break;
		case 2:
			m_vMyRelly	= D3DXVECTOR3(m_vRellyPoint.x - 18.f, m_vRellyPoint.y + 7.f, 0.f);
			break;
		case 3:
			m_vMyRelly	= D3DXVECTOR3(m_vRellyPoint.x + 18.f, m_vRellyPoint.y + 7.f, 0.f);
			break;
		case 4:
			m_vMyRelly	= D3DXVECTOR3(m_vRellyPoint.x + 10.f, m_vRellyPoint.y, 0.f);
			break;
		case 5:
			m_vMyRelly	= D3DXVECTOR3(m_vRellyPoint.x - 10.f, m_vRellyPoint.y, 0.f);
			break;
		}

		m_tInfo.vDir = m_vMyRelly - m_tInfo.vPos;
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

		if( D3DXVec3Length(& ( m_vMyRelly - m_tInfo.vPos )) < 2 )
		{
			if(!m_bStand)
			{
				m_wstrStateKey	= L"Stand";
				SetFrame(m_wstrStateKey);

				m_bStand	= true;
				m_bMove		= false;
				m_bAttack	= false;
			}
		}
		else
		{
			m_tInfo.vPos += m_tInfo.vDir * m_tStats.fSpeed * GET_TIME;
			if(!m_bMove)
			{
				m_wstrStateKey	= L"Move";
				SetFrame(m_wstrStateKey);

				m_bMove		= true;
				m_bStand	= false;
				m_bAttack	= false;
			}
		}
	}

	/* 1번스킬 활성화 */
	if(m_bSkill[0])
	{
		if(m_wstrObjKey == L"Knight4-1T")
		{
			m_bHeal = true;
		}
		else if(m_wstrObjKey == L"Knight4-2T")
		{
			m_iPlusDamage = 100;
		}
	}
	/* 2번스킬 활성화 */
	if(m_bSkill[1])
	{
		if(m_wstrObjKey == L"Knight4-1T")
		{
			if(!m_iHPup)
			{
				//m_fMaxHP		= (float)m_tStats.iHP;
				m_fMaxHP		+= 50;
				m_tStats.iHP	= (int)m_fMaxHP;			
				m_iHPup			= true;
				m_bAttack		= false;
				m_wstrStateKey	= L"Skill2";
				SetFrame(m_wstrStateKey);		//기본 모션
			}
		}
		else if(m_wstrObjKey == L"Knight4-2T")
		{
			m_fAtkSpeed		= 2.f;
		}
	}


	return 0;
}
void	CFootMan::Render(void)	
{
	D3DXMATRIX		matScale, matTrans;

	//이동시 방향별로 이미지 반전
	if(!m_bEat)
	{

	float		fScale = 1.f;
	m_tInfo.vDir.x < 0 ? fScale *= -1 : fScale *= 1;

	//공격모드일경우 몬스터 방향 바라보기
	if(m_bAttack && m_pMonster != NULL)
	{
		m_tInfo.vPos.x < m_pMonster->GetInfo().vPos.x ? fScale = 1 : fScale = -1;
	}

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey, (int)m_tFrame.fFrame);
	D3DXMatrixScaling(&matScale, fScale, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

	/* 체력바 작업 */
	if(!m_bDie)
	{
		pTexture = CTextureMgr::GetInstance()->GetTexture(L"HpEmpty");

		fX = pTexture->tImgInfo.Width  / 2.f;
		fY = (float)pTexture->tImgInfo.Height;

		D3DXMatrixScaling(&matScale, 15 / (fX * 2) , 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
			&D3DXVECTOR3(fX, 15.f, 0.f), NULL, D3DCOLOR_ARGB(200,255,255,255));

		/* 체력 */
		pTexture = CTextureMgr::GetInstance()->GetTexture(L"HpBar");

		fX = pTexture->tImgInfo.Width  / 2.f;
		fY = (float)pTexture->tImgInfo.Height;

		RECT rt = { 0, 0, LONG(pTexture->tImgInfo.Width * (m_tStats.iHP / m_fMaxHP)), 4};
		D3DXMatrixScaling(&matScale, 15 / (fX * 2) , 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, &rt, 
			&D3DXVECTOR3(fX, 15.f, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
	}
	}
}
void	CFootMan::Release(void)		
{
	CSDevice::GetInstance()->SoundStop(SD_FOOTMANBATTLE);
}
void	CFootMan::Leveling()
{
	if(m_bLvl[LVL_1])
	{
		m_wstrObjKey	= L"Knight1T";
		m_iLevel		= 0;
		m_tStats		= UNITSTATS(L"Knight1T", 50, 1, 3, 100.f);
		m_fMaxHP		= (float)m_tStats.iHP;
	}
	else if(m_bLvl[LVL_2])
	{
		m_wstrObjKey	= L"Knight2T";
		m_iLevel		= 1;
		m_tStats		= UNITSTATS(L"Knight2T", 100, 3, 4, 100.f);
		m_fMaxHP		= (float)m_tStats.iHP;
	}
	else if(m_bLvl[LVL_3])
	{
		m_wstrObjKey	= L"Knight3T";
		m_iLevel		= 2;
		m_tStats		= UNITSTATS(L"Knight3T", 150, 6, 10, 100.f);
		m_fMaxHP		= (float)m_tStats.iHP;
	}
	else if(m_bLvl[LVL_41])
	{
		m_wstrObjKey	= L"Knight4-1T";
		m_iLevel		= 3;
		m_tStats		= UNITSTATS(L"Knight4-1T", 200, 12, 18, 100.f);
		m_fMaxHP		= (float)m_tStats.iHP;
	}
	else if(m_bLvl[LVL_42])
	{
		m_wstrObjKey	= L"Knight4-2T";
		m_iLevel		= 4;
		m_tStats		= UNITSTATS(L"Knight4-2T", 150, 16, 24, 100.f);
		m_fMaxHP		= (float)m_tStats.iHP;
		m_fAtkSpeed		= 1.f;
	}

	for(int i = 0; i < LVL_END; ++i)
		m_bLvl[i] = false;

	m_bLevelup	= false;

	m_bStand	= false;
	m_bMove		= false;
	m_bAttack	= false;
}