#pragma once
#include "chapter.h"

class CObj;
class CStage6 :
	public CChapter
{
private:
	CObj*			m_pBack;
	list<CObj*>		m_ObjList[OBJ_END];
	MONSTERID		m_eMonType;

private:
	void KeyInput();
	void Picking(MONSTERID _eMonType);
public:
	virtual void Initialize(void);
	virtual void Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CStage6(void);
	~CStage6(void);
};
