#include "StdAfx.h"
#include "RenderMgr.h"
#include "Obj.h"

CRenderMgr* CRenderMgr::m_pInst = NULL;

CRenderMgr::CRenderMgr(void)
{
}

CRenderMgr::~CRenderMgr(void)
{
	Release();
}

void CRenderMgr::AddObj(CObj* pObj)
{
	m_MapRenderObj.insert(multimap<float, CObj*>::value_type(float(pObj->GetRect().bottom), pObj));
}

void CRenderMgr::ObjRender(HDC hdc)
{
	for(multimap<float, CObj*>::iterator iter = m_MapRenderObj.begin();
		iter != m_MapRenderObj.end(); ++iter)
	{
		iter->second->Render(hdc);
	}
}

void CRenderMgr::Release(void)
{
	m_MapRenderObj.clear();
}
