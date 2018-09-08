#include "StdAfx.h"
#include "Egg.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"

#include "ObjMgr.h"
#include "Player.h"

#include "Monster.h"
#include "Spider.h"
#include "BabySpider.h"

#include "Monster.h"


CEgg::CEgg(void)
:m_pSpiderMother(NULL)
{

}

CEgg::~CEgg(void)
{
}
HRESULT CEgg::Initialize(void)
{
	m_eEffectID		= EFC_EGG;
	m_wstrObjKey	= L"BabySpider";
	m_wstrStateKey	= L"Egg";
	SetFrame(m_wstrStateKey);

	m_VecBabyRoute	= (*m_pVecMotherRoute);

	m_pMonsterList	= CObjMgr::GetInstance()->GetObjList(OBJ_MONSTER);

	return S_OK;
}
int		CEgg::Progress(void)	
{
	if( ((CMonster*)m_pSpiderMother)->GetMonsterDie())
		return 1;
	if(FrameMove(0.2f))
	{
		//CObj*	pMonster = CMonFactory<CSpider>::CreateMonster(&m_VecTile, &m_mapRoute[3]);

		CObj* pBabySpider = new CBabySpider;
		((CBabySpider*)pBabySpider)->SetVecTile(m_pVecTile);
		((CBabySpider*)pBabySpider)->SetVecRoute(&m_VecBabyRoute);
		pBabySpider->SetPos(m_tInfo.vPos);
		pBabySpider->Initialize();

		m_pMonsterList->push_back(pBabySpider);


		return 1;
	}

	return 0;
}
void	CEgg::Render(void)	
{
	D3DXMATRIX		matScale, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey, (int)m_tFrame.fFrame);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 0.f);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matTrans;

	float fX = pTexture->tImgInfo.Width  / 2.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
}
void	CEgg::Release(void)	
{
}
