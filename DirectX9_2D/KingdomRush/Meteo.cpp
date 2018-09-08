#include "StdAfx.h"
#include "Meteo.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ObjMgr.h"
#include "ObjFactory.h"

#include "Crator.h"
#include "SDevice.h"

CMeteo::CMeteo(void)
{
}

CMeteo::~CMeteo(void)
{
}
HRESULT CMeteo::Initialize(void)
{
	m_eProID		= PRO_METEO;

	m_vTarget		= D3DXVECTOR3( m_tInfo.vPos.x, m_tInfo.vPos.y + 700.f, 0.f);
	m_tInfo.vDir	= D3DXVECTOR3(0.f, 1.f, 0.f);

	m_fSpeed		= 300.f;
	m_fAccelTime	= 0.f;

	m_wstrObjKey	= L"Meteo";
	SetFrame(L"Fire");

	m_fImgTheta		= D3DXToRadian(90);

	//ºÕ
	{
		m_bFirstBoom	= false;
		m_bBoomImg		= false;
		m_bBoom			= false;
		m_fBoomRange	= 45.f;
		m_iBoomDamage	= 1000;	
	}
	m_bCrator			= false;
	m_pProjectileList	= CObjMgr::GetInstance()->GetObjList(OBJ_PROJECTILE);

	return S_OK;
}
int CMeteo::Progress(void)		
{
	float m_fFrameSpd = 1.f;
	m_bBoom ? m_fFrameSpd = 1.5f : m_fFrameSpd = 5.f;

	bool bOneCircle = FrameMove(m_fFrameSpd);
	if( D3DXVec3Length(& ( m_tInfo.vPos - m_vTarget)) < 2.f )
	{
		m_bBoom = true;
		if(!m_bBoomImg)
		{
			SetFrame(L"Boom");
			m_fImgTheta	= 0.f;
			m_bBoomImg	= true;
			CSDevice::GetInstance()->SoundPlay(SD_METEO, 0);

			if(m_bCrator)
			{
				CObj* pCrator = CObjFactory<CCrator>::CreateObj(m_tInfo.vPos);
				m_pProjectileList->push_back(pCrator);
			}
		}
		if(m_bBoomImg && bOneCircle)
		{
			return 1;
		}
		return 0;
	}

	m_fAccelTime += GET_TIME;
	if(m_fAccelTime > 0.05)
	{
		m_fSpeed		+= 2.f;
		m_fAccelTime	= 0.f;
	}
	m_tInfo.vPos += m_tInfo.vDir * m_fSpeed * GET_TIME;
	
	return 0;
}
void CMeteo::Render(void)		
{
	D3DXMATRIX		matScale, matRotZ, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey, (int)m_tFrame.fFrame);
	D3DXMatrixScaling(&matScale, 1.1f, 1.1f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, m_fImgTheta);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
}
void CMeteo::Release(void)		
{
}
