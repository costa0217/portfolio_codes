#include "StdAfx.h"
#include "SkillButt.h"
#include "Device.h"
#include "TextureMgr.h"

#include "Mouse.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

#include "Meteo.h"
#include "ObjMgr.h"
#include "ObjFactory.h"

#include "FootMan.h"
#include "Soldier.h"

#include "SDevice.h"


CSkillButt::CSkillButt(void)
{
}

CSkillButt::~CSkillButt(void)
{
}
HRESULT CSkillButt::Initialize(void)
{
	m_eUIType		= UI_SKILLBUTT;

	m_tCloseFrame	= FRAME(0.f, 14.f, 14.f);
	m_tActiveFrame	= FRAME(0.f, 16.f, 16.f);
	m_tNormalFrame	= FRAME(0.f, 1.f, 1.f);

	//m_wstrStateKey	= ;

	m_iIndex == 1 ? m_wstrSkillImg = L"Met" : m_wstrSkillImg = L"Mill";
	m_wstrObjKey = L"Skill";
	m_iIndex == 1 ? SetFrame(L"Meteo_cursor") : SetFrame(L"Militia_cursor");
	m_bClick		= false;
	m_bMeteo		= false;

	m_bChoice		= false;

	m_vPoint		= D3DXVECTOR3(0.f, 0.f, 0.f);
	m_iMeteoNum		= 3;

	m_pProjecList	= CObjMgr::GetInstance()->GetObjList(OBJ_PROJECTILE);
	m_pSoldierList	= CObjMgr::GetInstance()->GetObjList(OBJ_SOLDIER);


	m_bMeteoCool		= false;
	m_bMillCool			= false;
	m_fMeteoCoolTime	= 3.f;
	m_fMillCoolTime		= 2.f;


	return S_OK;
}
int CSkillButt::Progress(void)		
{
	if(m_bMeteoCool)
	{
		m_fMeteoCoolTime -= GET_TIME;
		m_bClick		= false;

		if(m_fMeteoCoolTime < 0.f)
		{
			m_bMeteoCool		= false;
			m_fMeteoCoolTime	= 3.f;
		}
		else
			return 0;
	}
	if(m_bMillCool)
	{
		m_fMillCoolTime	-= GET_TIME;
		m_bClick		= false;

		if(m_fMillCoolTime < 0.f)
		{
			m_bMillCool		= false;
			m_fMillCoolTime	= 2.f;
		}
		else
			return 0;
	}
		

	FrameMove();
	KeyInput();

	return 0;
}
void CSkillButt::Render(void)		
{
	D3DXMATRIX		matTrans;

	if(m_bClick)
		m_bChoice = true;

	int iImgNum = 0;
	if(m_bClick)
		iImgNum = 1;

	//스킬 이미지
	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrSkillImg);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 1.6f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

	if(m_bMeteoCool || m_bMillCool)
	{
		float fCoolTime = 0.f;
		float fMaxTime	= 0.f;
		if(m_bMeteoCool)
		{
			fCoolTime	= m_fMeteoCoolTime;
			fMaxTime	= 3.f;
		}
		else
		{
			fCoolTime	= m_fMillCoolTime;
			fMaxTime	= 2.f;
		}

		const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrSkillImg);
		D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

		float fX = pTexture->tImgInfo.Width  / 2.f;
		float fY = pTexture->tImgInfo.Height / 1.6f;

		RECT rc = {0, 0, LONG(pTexture->tImgInfo.Width), LONG(pTexture->tImgInfo.Height * (fCoolTime / fMaxTime))};

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, &rc, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(150,0,0,0));
	}
	

	//스킬 윈도우
	pTexture = CTextureMgr::GetInstance()->GetTexture(L"SkillWind", L"Normal", iImgNum);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	fX = pTexture->tImgInfo.Width  / 2.f;
	fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

	if(m_bClick)
	{
		const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey, (int)m_tFrame.fFrame);
		D3DXMatrixTranslation(&matTrans, CMouse::GetPos().x, CMouse::GetPos().y, 0.f);

		float fX = pTexture->tImgInfo.Width  / 2.f;
		float fY = pTexture->tImgInfo.Height / 2.f;

		CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
		CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
	}
}
void CSkillButt::Release(void)		
{
}
void CSkillButt::KeyInput()
{
	if(m_bChoice)
	{
		if(CKeyMgr::GetInstance()->KeyPressed(KEY_LBUTTON, KEYNUM_6))
		{
			m_vPoint	= CMouse::GetPos();
			m_bChoice	= false;
			m_bMeteo	= true;
		}	
	}
	if(D3DXVec3Length(& m_vPoint) != 0)
	{
		if(m_iIndex == 1)
		{
			m_bMeteoCool = true;
			if(m_bMeteoLV[1])
				m_iMeteoNum = 6;
			for( int i = 0; i < m_iMeteoNum; ++i)
			{
				CObj*	pMeteo = CObjFactory<CMeteo>::CreateObj(D3DXVECTOR3( (m_vPoint.x - 52.5f) + rand()%105, (m_vPoint.y - 32.5f) + rand()%65 - 700.f, 0.f ));
				
				if(m_bMeteoLV[2])
					((CMeteo*)pMeteo)->SetCrator(true);
				m_pProjecList->push_back(pMeteo);
			}

			if(m_bMeteoLV[3])
			{
				int iRanNum = rand()%((*m_pmapRoute).size()) + 1;

				unsigned int iVecSize = (*m_pmapRoute)[iRanNum].size();
				for(unsigned int i = 0; i < iVecSize; ++i)
				{
					if( i%2 == 0)
						continue;

					CObj*	pMeteo = CObjFactory<CMeteo>::CreateObj( D3DXVECTOR3((*m_pVecTile)[ (*m_pmapRoute)[iRanNum][i] ]->vPos.x, (*m_pVecTile)[ (*m_pmapRoute)[iRanNum][i] ]->vPos.y -700.f - rand()%50, 0.f));
					m_pProjecList->push_back(pMeteo);
				}
			}
		}
		else if(m_iIndex == 2)
		{
			m_bMillCool = true;
			int iLevel = 0;
			if(m_bMillLV[1])
				iLevel = 1;
			if(m_bMillLV[2])
				iLevel = 2;
			if(m_bMillLV[3])
				iLevel = 3;
			for( int i = 0; i < 2; ++i)
			{
				CObj* pFootMan = new CFootMan;
				((CFootMan*)pFootMan)->SetRellyPoint(m_vPoint);
				((CFootMan*)pFootMan)->SetIndex(i + 4);
				pFootMan->SetPos(m_vPoint);
				pFootMan->Initialize();
				((CFootMan*)pFootMan)->SetLevelup(iLevel);

				m_pSoldierList->push_back(pFootMan);
				if( i == 0 )
					continue;

				if(iLevel == 0)
				{
					CSDevice::GetInstance()->SoundPlay(SD_MILL_1, 0);
				}
				else if(iLevel == 1)
				{
					CSDevice::GetInstance()->SoundPlay(SD_MILL_2, 0);
				}
				else if(iLevel == 2)
				{
					CSDevice::GetInstance()->SoundPlay(SD_MILL_3, 0);
				}
				else if(iLevel == 3)
				{
					CSDevice::GetInstance()->SoundPlay(SD_MILL_4, 0);
				}
			}
		}
		m_vPoint	= D3DXVECTOR3(0.f, 0.f, 0.f);
		m_bMeteo	= false;
	}
}