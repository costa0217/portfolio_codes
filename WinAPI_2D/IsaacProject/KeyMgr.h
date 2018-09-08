#pragma once

#include "Include.h"
class CKeyMgr
{
private:
	DWORD		m_dwKey;

public:
	void KeyCheck(void);
	DWORD GetKey(void);

private:
	static CKeyMgr*			m_pInst;

public:
	static CKeyMgr* GetInst(void)
	{
		if(m_pInst == NULL)
			m_pInst = new CKeyMgr;

		return m_pInst;
	}

	void DestroyInst(void)
	{
		delete m_pInst;
		m_pInst = NULL;
	}

private:
	CKeyMgr(void);
	~CKeyMgr(void);
};
