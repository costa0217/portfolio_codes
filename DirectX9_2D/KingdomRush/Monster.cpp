#include "StdAfx.h"
#include "Monster.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"

#include "ObjMgr.h"

CMonster::CMonster(void)
:m_fPoisonTime(0)
,m_fDePoisonTime(0)
,m_fTelpoTime(0)
,m_bGoalLine(false)
,m_bBooster(false)
{
	//m_pPlayerList	= CObjMgr::GetInstance()->GetObjList(OBJ_PLAYER);
}

CMonster::~CMonster(void)
{
}
int CMonster::Progress(void)
{
	if(m_tStats.iHP <= 0)
		m_bDie = true;

	if(m_bTelpo)
	{
		m_fTelpoTime += GET_TIME;
		if(m_fTelpoTime > 0.05f )
		{
			m_bBattle	 = false;
			m_bAttack	 = false;
			m_bTelpo	 = false;
			m_pSoldier	 = NULL;
			m_fTelpoTime = 0.f;
		}
	}

	/* 독 */
	if(m_bPoison)
	{
		m_fPoisonTime += GET_TIME;
		if(m_fPoisonTime > 1.f)
		{
			m_tStats.iHP -= 10;
			m_fPoisonTime = 0.f;
		}
		m_fDePoisonTime += GET_TIME;
	}
	if(m_fDePoisonTime > 10.f)	// 해독
	{
		m_bPoison = false;
		m_fDePoisonTime	= 0.f;
	}


	if(!m_bBattle && !m_bIntengle)
	{
		if(!m_MyVecRoute.empty())
		{	
			if(D3DXVec3Length(&(m_tInfo.vPos - D3DXVECTOR3((*m_pVecTile)[m_MyVecRoute.back()]->vPos.x, (*m_pVecTile)[m_MyVecRoute.back()]->vPos.y, 0.f))) < 5)
			{
				m_MyVecRoute.pop_back();
				if(m_MyVecRoute.size() > 0)
				{
					m_tInfo.vDir	= D3DXVECTOR3((*m_pVecTile)[m_MyVecRoute.back()]->vPos.x, (*m_pVecTile)[m_MyVecRoute.back()]->vPos.y, 0.f) - m_tInfo.vPos;
					D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);
				}			
			}
			else
			{
				m_tInfo.vPos += m_tInfo.vDir * m_tStats.fSpeed * GET_TIME;
			}
		}
		else if(m_MyVecRoute.empty())
		{
			/*m_tInfo.vPos	= D3DXVECTOR3((*m_pVecTile)[m_pVecRoute->back()]->vPos.x, (*m_pVecTile)[m_pVecRoute->back()]->vPos.y, 0.f);
			m_MyVecRoute	= (*m_pVecRoute);*/
			// 도착하면 죽이고 생명깍고/ 도착해서 죽인게아니면 골드주고
			m_bGoalLine		= true;
			m_bDie			= true;
		}
		DirectionMake();
	}	
	return 0;
}
void CMonster::DirectionMake()
{
	m_fTheta = acos(D3DXVec3Dot(&m_tInfo.vDir, &m_tInfo.vLook));

	if(m_tInfo.vDir.y < m_tInfo.vLook.y)
		m_fTheta = (D3DX_PI * 2) - m_fTheta;

	if(m_fTheta > D3DXToRadian(60.f) && m_fTheta < D3DXToRadian(120.f))
	{
		if(!m_bMoveDown)
		{
			m_wstrStateKey	= L"Move_D";
			SetFrame(m_wstrStateKey);
		
			m_bMoveLR	= false;
			m_bMoveUp	= false;
			m_bMoveDown	= true;
		}
	}
	else if( m_fTheta > D3DXToRadian(240.f) && m_fTheta < D3DXToRadian(320.f))
	{
		if(!m_bMoveUp)
		{
			m_wstrStateKey	= L"Move_U";
			SetFrame(m_wstrStateKey);
			
			m_bMoveLR	= false;
			m_bMoveUp	= true;
			m_bMoveDown	= false;
		}		
	}
	else
	{
		if(!m_bMoveLR)
		{
			m_wstrStateKey	= L"Move_LR";
			SetFrame(m_wstrStateKey);
	
			m_bMoveLR	= true;
			m_bMoveUp	= false;
			m_bMoveDown	= false;
		}
	}
}
void CMonster::Render(void)		
{
	if(!m_bDie)
	{
		D3DXMATRIX		matScale, matTrans;
		/* 체력바 */
		const TEXINFO*	pTexture = CTextureMgr::GetInstance()->GetTexture(L"HpEmpty");

		float fX = pTexture->tImgInfo.Width  / 2.f;
		float fY = (float)pTexture->tImgInfo.Height;

		D3DXMatrixScaling(&matScale, m_fMonWidth / (fX * 2) , 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
			&D3DXVECTOR3(fX, m_fHPPostion, 0.f), NULL, D3DCOLOR_ARGB(200,255,255,255));

		/* 체력 */
		pTexture = CTextureMgr::GetInstance()->GetTexture(L"HpBar");

		fX = pTexture->tImgInfo.Width  / 2.f;
		fY = (float)pTexture->tImgInfo.Height;

		RECT rt = { 0, 0, LONG(pTexture->tImgInfo.Width * (m_tStats.iHP / m_fMaxHP)), 4};
		D3DXMatrixScaling(&matScale, m_fMonWidth / (fX * 2) , 1.f, 0.f);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

		m_tInfo.matWorld = matScale * matTrans;

		CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, &rt, 
			&D3DXVECTOR3(fX, m_fHPPostion, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
	}
}