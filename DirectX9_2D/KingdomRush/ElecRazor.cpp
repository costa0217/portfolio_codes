#include "StdAfx.h"
#include "ElecRazor.h"

#include "TimeMgr.h"
#include "Device.h"
#include "TextureMgr.h"
#include "ObjMgr.h"

#include "ObjFactory.h"
#include "Monster.h"
#include "ElecShock.h"

#include "Scream.h"

#include "SDevice.h"

#define RazorWidth 81
#define RazorHeight 27
CElecRazor::CElecRazor(void)
:m_pMonster(NULL)
{
}

CElecRazor::~CElecRazor(void)
{
}
HRESULT CElecRazor::Initialize(void)
{
	m_eProID		= PRO_ERAZOR;

	m_wstrObjKey	= L"Bomb4-1T";
	m_wstrStateKey	= L"Bomb";
	m_fRazorLength	= 0.f;
	m_fTheta		= 0.f;
	m_bChain		= false;

	m_pMonsterList	= CObjMgr::GetInstance()->GetObjList(OBJ_MONSTER);
	m_pBulletList	= CObjMgr::GetInstance()->GetObjList(OBJ_PROJECTILE);
	m_pEffectList	= CObjMgr::GetInstance()->GetObjList(OBJ_EFFECT);

	SetFrame(m_wstrStateKey);
	return S_OK;
}
int CElecRazor::Progress(void)		
{
	if(m_iChainNum < 0)
		return 1;

	if(((CMonster*)m_pMonster)->GetMonsterDie())
		return 1;

	if( m_pMonster != NULL && ((CMonster*)m_pMonster)->GetTelpo() )
		return 1;
	
	if(FrameMove(2.f))
	{
		for(list<CObj*>::iterator iter = m_pMonsterList->begin();
			iter != m_pMonsterList->end(); ++iter)
		{
			if((*iter) != m_pMonster && ((CMonster*)(*iter))->GetMonType() != MTYPE_FLY)
			{
				if( D3DXVec3Length(& (m_pMonster->GetInfo().vPos - (*iter)->GetInfo().vPos)) < 80.f) 
				{
					if(((CMonster*)(*iter))->GetMonsterDie())
						return 1;

					CObj* pRazor = new CElecRazor;
					((CElecRazor*)pRazor)->SetMonTarget(*iter);
					pRazor->Initialize();
					pRazor->SetPos(D3DXVECTOR3(m_pMonster->GetInfo().vPos.x, m_pMonster->GetInfo().vPos.y, 0.f));
					((CElecRazor*)pRazor)->SetChain(true);
					((CElecRazor*)pRazor)->SetChainNum(m_iChainNum - 1);
					((CElecRazor*)pRazor)->SetBombDamage(m_iDamage - 10);

					m_pBulletList->push_back(pRazor);
					break;
				}
			}	
		}
		CObj* pWhaam = CObjFactory<CScream>::CreateObj(D3DXVECTOR3(m_pMonster->GetInfo().vPos.x, m_pMonster->GetInfo().vPos.y - 20.f, 0.f));
		((CScream*)pWhaam)->SetScream(SR_WHAAM);
		m_pEffectList->push_back(pWhaam);

		CObj* pElecshock = CObjFactory<CElecShock>::CreateObj(D3DXVECTOR3(m_pMonster->GetInfo().vPos.x, m_pMonster->GetInfo().vPos.y, 0.f));
		((CElecShock*)pElecshock)->SetMonster(m_pMonster);
		m_pEffectList->push_back(pElecshock);

		((CMonster*)m_pMonster)->SetMonsterHP(((CMonster*)m_pMonster)->GetMonsterHP() - m_iDamage);

		CSDevice::GetInstance()->SoundPlay(SD_ELECRAZOR, 0);

		return 1;
	}

	m_tInfo.vDir = m_pMonster->GetInfo().vPos - m_tInfo.vPos;
	D3DXVec3Normalize(&m_tInfo.vDir, &m_tInfo.vDir);

	m_fTheta = acos(D3DXVec3Dot(&m_tInfo.vDir, &m_tInfo.vLook));
	if(m_tInfo.vDir.y < m_tInfo.vLook.y)
		m_fTheta = (D3DX_PI * 2) - m_fTheta;


	m_fRazorLength = D3DXVec3Length( &( m_pMonster->GetInfo().vPos - m_tInfo.vPos)) / RazorWidth;


	return 0;
}
void CElecRazor::Render(void)		
{
	D3DXMATRIX		matScale, matRotZ, matTrans;

	const TEXINFO*		pTexture = CTextureMgr::GetInstance()->GetTexture(m_wstrObjKey, m_wstrStateKey, (int)m_tFrame.fFrame);
	D3DXMatrixScaling(&matScale, m_fRazorLength, 1.f, 0.f);
	D3DXMatrixRotationZ(&matRotZ, m_fTheta);
	D3DXMatrixTranslation(&matTrans, m_tInfo.vPos.x, m_tInfo.vPos.y, 0.f);

	m_tInfo.matWorld = matScale * matRotZ * matTrans;

	float fX = /*pTexture->tImgInfo.Width  / 2.f*/0.f;
	float fY = pTexture->tImgInfo.Height / 2.f;

	CDevice::GetInstance()->GetSprite()->SetTransform(&m_tInfo.matWorld);
	CDevice::GetInstance()->GetSprite()->Draw(pTexture->pTexture, NULL, 
		&D3DXVECTOR3(fX, fY, 0.f), NULL, D3DCOLOR_ARGB(255,255,255,255));
}
void CElecRazor::Release(void)		
{
}