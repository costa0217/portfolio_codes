#pragma once

#include "Include.h"

class CObj;
class CRenderMgr
{
	DECLARE_SINGLETON(CRenderMgr)
private:
	multimap<float, CObj*>			m_MapRenderObj;

public:
	void AddObj(CObj* pObj);
	void ObjRender();
	void Release(void);

private:
	CRenderMgr(void);
	~CRenderMgr(void);
};
