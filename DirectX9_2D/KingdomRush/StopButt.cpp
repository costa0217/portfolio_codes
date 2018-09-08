#include "StdAfx.h"
#include "StopButt.h"

#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"

#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Mouse.h"

#include "StopWnd.h"

#include "SkillButt.h"

CStopButt::CStopButt(void)
{
}

CStopButt::~CStopButt(void)
{
	Release();
}
HRESULT CStopButt::Initialize(void)
{
	m_eUIType		= UI_STOPBUTT;
	m_tInfo.vPos	= D3DXVECTOR3(600.f, 35.f, 0.f);

	m_wstrObjKey	= L"Button";
	m_wstrStateKey	= L"Stop";
	SetFrame(m_wstrStateKey);

	m_iMouseState		= 0;
	m_fButtResetTime	= 0.f;

	m_bClick			= false;
	m_bChoice			= false;
	m_bLevelPass		= false;

	m_pLastWndList	= CObjMgr::GetInstance()->GetObjList(OBJ_LASTWND);

	for(int i = 0; i < 4; ++i)
	{
		m_bMeteoLV[i]	= false;
		m_bMillLV[i]	= false;
	}
	m_bMeteoLV[0]	= true;
	m_bMillLV[0]	= true;


	return S_OK;
}
int CStopButt::Progress(void)		
{
	MouseMove();
	//if(m_bClick)
	//	//m_bChoice = true;

	if(!m_bLevelPass)
	{
		((CSkillButt*)m_pMeteoUI)->SetMeteoLV(m_bMeteoLV);
		((CSkillButt*)m_pMillUI)->SetMillLV(m_bMillLV);

		m_bLevelPass = true;
	}

	if(m_bClick && !m_bChoice)
	{
		CObj* pStopWnd	 = new CStopWnd;
		((CStopWnd*)pStopWnd)->SetStopUI(this);
		((CStopWnd*)pStopWnd)->SetMeteoLV(m_bMeteoLV);
		((CStopWnd*)pStopWnd)->SetMillLV(m_bMillLV);
		pStopWnd->Initialize();


		m_pLastWndList->push_back(pStopWnd);

		m_bChoice = true;
		CTimeMgr::GetInstance()->SetTimeControl(0.f);
	}
	if(!m_bClick)
		CTimeMgr::GetInstance()->SetTimeControl(1.f);


	return 0;
}
void CStopButt::Render(void)		
{
	D3DXMATRIX		matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey , m_iMouseState);
	
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

}
void CStopButt::Release(void)		
{
}
void CStopButt::MouseMove(void)	
{
	if(m_iMouseState != 2
		&& CMouse::GetPos().y < m_tInfo.vPos.y + 18.f
		&& CMouse::GetPos().y > m_tInfo.vPos.y - 18.f
		&& CMouse::GetPos().x > m_tInfo.vPos.x - 19.f
		&& CMouse::GetPos().x < m_tInfo.vPos.x + 19.f)
	{
		m_iMouseState = 1;
	}
	else if(m_iMouseState != 2)
		m_iMouseState = 0;
	else
	{
		m_fButtResetTime += GET_TIME;
		if(m_fButtResetTime > 0.05f)
		{
			m_iMouseState		= 0;
			m_fButtResetTime	= 0.f;
		}
	}
}

