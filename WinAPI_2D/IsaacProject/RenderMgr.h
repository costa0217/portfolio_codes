#pragma once
#include "Include.h"

class CObj;
class CRenderMgr
{
private:
	multimap<float, CObj*>			m_MapRenderObj;

public:
	void AddObj(CObj* pObj);
	void ObjRender(HDC hdc);
	void Release(void);

private:
	static CRenderMgr* m_pInst;

public:
	static CRenderMgr* GetInst(void)
	{
		if(m_pInst == NULL)
			m_pInst = new CRenderMgr;

		return m_pInst;
	}

	void DestroyInst(void)
	{
		delete m_pInst;
		m_pInst = NULL;
	}

private:
	CRenderMgr(void);
	~CRenderMgr(void);
};
