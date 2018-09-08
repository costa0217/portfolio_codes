#include "StdAfx.h"
#include "OptionButt.h"

#include "TextureMgr.h"
#include "Device.h"
#include "TimeMgr.h"

#include "ObjMgr.h"
#include "ObjFactory.h"
#include "Mouse.h"

#include "OptWnd.h"

COptionButt::COptionButt(void)
{
}

COptionButt::~COptionButt(void)
{
	Release();
}
HRESULT COptionButt::Initialize(void)
{
	m_eUIType			= UI_OPTIONBUTT;
	m_tInfo.vPos		= D3DXVECTOR3(655.f, 35.f, 0.f);
	
	m_wstrObjKey		= L"Button";
	m_wstrStateKey		= L"Option";

	m_iMouseState		= 0;
	m_fButtResetTime	= 0.f;

	m_bClick			= false;
	m_bChoice			= false;
	
	m_pLastWndList	= CObjMgr::GetInstance()->GetObjList(OBJ_LASTWND);

	return S_OK;
}
int COptionButt::Progress(void)		
{
	MouseMove();

	if(m_bClick && !m_bChoice)
	{
		CObj* pOptWnd	 = new COptWnd;
		((COptWnd*)pOptWnd)->SetOptUI(this);
		pOptWnd->Initialize();


		m_pLastWndList->push_back(pOptWnd);

		m_bChoice = true;
		CTimeMgr::GetInstance()->SetTimeControl2(0.f);
	}
	if(!m_bClick)
		CTimeMgr::GetInstance()->SetTimeControl2(1.f);

	return 0;
}
void COptionButt::Render(void)		
{
	D3DXMATRIX		matScale, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey , m_iMouseState);
	
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;


	CDevice::GetInstance()->GetSprite()->SetTransform(&matTrans);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));

}
void COptionButt::Release(void)		
{
}
void COptionButt::MouseMove(void)	
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

