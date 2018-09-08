#include "StdAfx.h"
#include "RenderMgr.h"
#include "Obj.h"

IMPLEMENT_SINGLETON(CRenderMgr)
CRenderMgr::CRenderMgr(void)
{
}

CRenderMgr::~CRenderMgr(void)
{
	Release();
}

void CRenderMgr::AddObj(CObj* pObj)
{
	m_MapRenderObj.insert(multimap<float, CObj*>::value_type(float(pObj->GetInfo().vPos.y), pObj));
}

void CRenderMgr::ObjRender()
{
	for(multimap<float, CObj*>::iterator iter = m_MapRenderObj.begin();
		iter != m_MapRenderObj.end(); ++iter)
	{
		iter->second->Render();
	}
}

void CRenderMgr::Release(void)
{
	m_MapRenderObj.clear();
}
