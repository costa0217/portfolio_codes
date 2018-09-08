#include "StdAfx.h"
#include "Golem.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"

#include "ObjMgr.h"
#include "ObjFactory.h"

#include "Monster.h"

CGolem::CGolem(void)
:m_bStand(false)
,m_bMove(false)
,m_bAttack(false)
,m_pMonster(NULL)
{
}

CGolem::~CGolem(void)
{
	Release();
}
HRESULT CGolem::Initialize(void)
{
	m_eSoldierID	= SOL_GOLEM;
	m_wstrObjKey	= L"Magic4-1T";
	m_wstrStateKey	= L"S_Birth";

	m_tStats		= UNITSTATS(L"Magic4-1T", 300, 10, 20, 100.f);
	m_fMaxHP		= (float)m_tStats.iHP;

	SetFrame(m_wstrStateKey);

	m_tInfo.vPos	= m_vRellyPoint;

	m_bBirth		= false;
	m_bDie			= false;
	m_bDeadImg		= false;

	m_bBattleMode	= false;

	m_bDamage		= false;

	m_tInfo.vDir	= D3DXVECTOR3(1.f, 0.f, 0.f);
	m_tInfo.vLook	= D3DXVECTOR3(1.f, 0.f, 0.f);

	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_EFFECT);

	return S_OK;
}
int		CGolem::Progress(void)	
{
	bool bFrame = FrameMove();	// 프레임속도 조정
	if(!m_bBirth && bFrame)
	{
		m_bBirth = true;

		m_wstrStateKey	= L"S_Stand";
		SetFrame(m_wstrStateKey);
	}
	if(!m_bBirth)
		return 0;

	if(!m_bBattleMode)
		m_pMonster = NULL;

	if(m_bBattleMode && bFrame)
		m_bDamage = false;

	if(m_bDie)
	{
		if(!m_bDeadImg)
		{
			m_wstrStateKey	= L"S_Die";
			SetFrame(m_wstrStateKey);
			m_bDeadImg = true;
		}
		if(int(m_tFrame.fFrame) == 5)
			return 1;
		else
			return 0;
	}

	if( m_pMonster	!= NULL && ((CMonster*)m_pMonster)->GetTelpo() )
	{
		m_pMonster		= NULL;
		m_bBattleMode	= false;
	}

	if(m_pMonster != NULL || m_bBattleMode)
	{
		D3DXVECTOR3 vMonster = D3DXVECTOR3(m_pMonster->GetInfo().vPos.x	- (((CMonster*)m_pMonster)->GetSize())
										, m_pMonster->GetInfo().vPos.y - 2.f, 0.f);
		m_tInfo.vDir = vMonster - m_tInfo.vPos;
		D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
		if( D3DXVec3Length(& ( vMonster - m_tInfo.vPos )) < 4 )
		{
			if(!m_bAttack)
			{
				m_wstrStateKey	= L"S_Attack";
				SetFrame(m_wstrStateKey);
				m_bAttack	= true;
			}
			if( int(m_tFrame.fFrame) == 7 && !m_bDamage)
			{
				((CMonster*)m_pMonster)->SetMonsterHP(((CMonster*)m_pMonster)->GetMonsterHP() - (m_tStats.iMaxDam));
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
			if(m_pMonster != NULL)
			{
				D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
				m_tInfo.vPos += m_tInfo.vDir * m_tStats.fSpeed * GET_TIME;	
				if(!m_bMove)
				{
					m_wstrStateKey	= L"S_Move";
					SetFrame(m_wstrStateKey);			
					m_bMove		= true;
					m_bStand	= false;
				}
			}
		}
	}
	else
	{
		if(!m_bBattleMode)
		{
			m_tInfo.vDir = m_vRellyPoint - m_tInfo.vPos;
			D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

			if( D3DXVec3Length(& ( m_vRellyPoint - m_tInfo.vPos )) < 4 )
			{
				if(!m_bStand)
				{
					SetFrame(L"S_Stand");

					m_bStand	= true;
					m_bMove		= false;
					m_bAttack	= false;
				}
			}
			else
			{
				D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
				m_tInfo.vPos += m_tInfo.vDir * m_tStats.fSpeed * GET_TIME;
				if(!m_bMove)
				{
					m_wstrStateKey	= L"S_Move";
					SetFrame(L"S_Move");

					m_bMove		= true;
					m_bStand	= false;
					m_bAttack	= false;
				}
			}
		}
	}

	return 0;
}
void	CGolem::Render(void)	
{
	D3DXMATRIX		matScale, matTrans;

	
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
	float fY = pTexture->tImgInfo.Height / 1.3f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

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
void	CGolem::Release(void)	
{
}
