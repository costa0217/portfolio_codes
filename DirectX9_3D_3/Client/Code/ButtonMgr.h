/*!
 * \file ButtonMgr.h
 * \date 2015/08/09 2:46
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/


#ifndef ButtonMgr_h__
#define ButtonMgr_h__

#include "Engine_Include.h"

namespace Engine
{
	class CGameObject;
}

class CButtonMgr
{

	DECLARE_SINGLETON(CButtonMgr)

private:
	CButtonMgr(void);

public:
	~CButtonMgr(void);


private:
	void Release(void);

private:
	typedef map<wstring, Engine::CGameObject*>		MAPOBJLIST;
	MAPOBJLIST		m_pMapObjlist;

public:
	void AddButton(const wstring& wstrObjKey, Engine::CGameObject* pGameObject);
	void CompareClick();
	MAPOBJLIST* getlist(){ return &m_pMapObjlist;}
};

#endif // ButtonMgr_h__