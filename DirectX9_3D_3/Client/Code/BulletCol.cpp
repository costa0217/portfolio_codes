#include "stdafx.h"
#include "BulletCol.h"

#include "Include.h"
#include "Transform.h"
#include "GameObject.h"

#include "Player.h"
#include "Retona.h"
#include "Dodge.h"
#include "Bunker.h"
#include "AirGun.h"
#include "Tank.h"

CBulletCol::CBulletCol(void)
: m_bCollision(false)
, m_fMonsterSize(20.f)
, m_fDamage(0.f)
{

}

CBulletCol::~CBulletCol(void)
{
	Release();
}

Engine::CCollision* CBulletCol::Clone(void)
{
	++(*m_pwRefCnt);

	return new CBulletCol(*this);
}


void CBulletCol::Update(void)
{
	//RETON 충돌
	OBJECTLIST::iterator iter		= m_pMonsterList->begin();
	OBJECTLIST::iterator iterEnd	= m_pMonsterList->end();
	float fLength;
	
	for(;iter != iterEnd; ++iter)
	{
		D3DXVECTOR3 vMonPos = ((const Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_vPos;
		D3DXVECTOR3 vDir =  vMonPos - (*m_pBulletPos);
		fLength = D3DXVec3Length(&vDir);

		if(fLength < m_fMonsterSize)
		{
			Engine::OBJ_NAME eName = ((const Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_eName;

			float	fHP = ((const Engine::CTransform*)(*iter)->GetComponent(L"Transform"))->m_fHP;

			switch(eName)
			{
			case Engine::OBJ_PLAYER:
				((CPlayer*)(*iter))->SetHP(fHP - m_fDamage);
				break;
			case Engine::OBJ_RETONA:
				((CRetona*)(*iter))->SetHP(fHP - m_fDamage);
				break;
			case Engine::OBJ_DODGE:
				((CDodge*)(*iter))->SetHP(fHP - m_fDamage);
				break;
			case Engine::OBJ_BUNKER:
				((CBunker*)(*iter))->SetHP(fHP - m_fDamage);
				break;
			case Engine::OBJ_AIRGUN:
				((CAirGun*)(*iter))->SetHP(fHP - m_fDamage);
				break;
			case Engine::OBJ_TANK:
				((CTank*)(*iter))->SetHP(fHP - m_fDamage);
				break;
			}
			m_bCollision = true;
			return;
		}
	}
}

CBulletCol* CBulletCol::Create(void)
{
	return new CBulletCol;
}

void CBulletCol::Release(void)
{
	if( (*m_pwRefCnt) == 0 )
		Engine::Safe_Delete(m_pwRefCnt);
	else
		--(*m_pwRefCnt);
}

void CBulletCol::SetColInfo( D3DXVECTOR3* pBulletPos, float Damage, OBJECTLIST* pMonsterList )
{
	m_pBulletPos	= pBulletPos;		// 총알위치
	m_fDamage		= Damage;
	m_pMonsterList	= pMonsterList;		// 몬스터리스트
}