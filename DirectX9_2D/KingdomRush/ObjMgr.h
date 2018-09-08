#pragma once

#include "Include.h"

class CObj;
class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr)

private:
	list<CObj*>			m_ObjList[OBJ_END];
	bool				m_bSetScene;
public:
	void	SetScene(bool _bSetScene){ m_bSetScene = _bSetScene; }
public:
	void	AddObject(OBJID _eObjID, CObj* pObj);

	list<CObj*>*	GetObjList(OBJID _eID){ return &m_ObjList[_eID]; }
public:
	HRESULT Initialize(void);
	int Progress(void);
	void Render(void);
	void Release(void);
private:
	CObjMgr(void);
	~CObjMgr(void);
};
