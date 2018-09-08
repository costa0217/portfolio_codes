#pragma once
#include "chapter.h"

class CObj;
class CStage1 :
	public CChapter
{
private:

	list<CObj*>		m_ObjList[OBJ_END];
	CObj*			m_pBack;
	CObj*			m_pAlpha;


public:
	virtual void Initialize(void);
	virtual void Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);

public:
	CStage1(void);
	~CStage1(void);
};
