#include "stdafx.h"
#include "SelectObj.h"

bool CSelectObj::Begin()
{
	CGameObject::Begin();

	return false;
}

void CSelectObj::Animate(float fTimeElapsed)
{
	if (m_bPresentReady != m_bReadyWeapon)
	{
		if (m_bReadyWeapon == true)
		{
			m_nAnimNum = 1;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
		}
		else
		{
			m_nAnimNum = 3;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
		}
	}
	if (m_pAnimater) m_pAnimater->Update(TIMEMGR->GetTimeElapsed());

	if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
		if(m_nAnimNum == 1)			m_nAnimNum = 2;
		else if (m_nAnimNum == 3)	m_nAnimNum = 0;

		m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
	}

	m_bPresentReady = m_bReadyWeapon;
}

void CSelectObj::RegistToContainer()
{
	CGameObject::RegistToContainer();
}

CSelectObj::CSelectObj(string name, tag t)
	: CGameObject(name, t)
{
}

CSelectObj::~CSelectObj()
{
}
