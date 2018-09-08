#pragma once

#include "Include.h"

class CBitBmp;
class CScene
{
protected:
	map<string, CBitBmp*>	m_BitMap;
	string					m_strKey;

public:
	virtual void Initialize(void)PURE;
	virtual void Progress(void)PURE;
	virtual void Render(HDC hdc)PURE;
	virtual void Release(void)PURE;

public:
	CScene(void);
	virtual ~CScene(void);
};
