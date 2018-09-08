#pragma once
#include "chapter.h"

class CObj;
class CStage4 :
	public CChapter
{
private:
	CObj*			m_pBack;
	list<CObj*>		m_ObjList[OBJ_END];

	bool			m_bMiniBossResurrection;
public:
	virtual void Initialize(void);
	virtual void Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CStage4(void);
	~CStage4(void);
};
