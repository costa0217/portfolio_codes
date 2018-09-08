#include "stdafx.h"
#include "ButtonMgr.h"

#include "Export_Function.h"
#include "Pipeline.h"

IMPLEMENT_SINGLETON(CButtonMgr)


CButtonMgr::CButtonMgr(void)
{

}

CButtonMgr::~CButtonMgr(void)
{
	Release();
}


void CButtonMgr::AddButton(const wstring& wstrObjKey, Engine::CGameObject* pGameObject)
{
	if(pGameObject)
	{
		MAPOBJLIST::iterator	iter = m_pMapObjlist.find(wstrObjKey);
		if(iter == m_pMapObjlist.end())
		{
			m_pMapObjlist.insert(MAPOBJLIST::value_type(wstrObjKey, pGameObject));
			
		}
	}
}

void CButtonMgr::CompareClick()
{

}

void CButtonMgr::Release(void)
{
	m_pMapObjlist.clear();
}
