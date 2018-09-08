#pragma once
#include "chapter.h"

class CObj;
class CStage2 :
	public CChapter
{
private:
	CObj*			m_pBack;
	list<CObj*>		m_ObjList[OBJ_END];

public:
	virtual void Initialize(void);
	virtual void Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CStage2(void);
	~CStage2(void);
};
