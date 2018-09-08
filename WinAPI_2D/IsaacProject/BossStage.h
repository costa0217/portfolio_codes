#pragma once
#include "chapter.h"

class CObj;
class CBossStage :
	public CChapter
{
private:
	CObj*			m_pBack;
	list<CObj*>		m_ObjList[OBJ_END];
	FRAME			m_tEndingFrame;
	DWORD			m_dwEndingTime;
public:
	virtual void Initialize(void);
	virtual void Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CBossStage(void);
	~CBossStage(void);
};
