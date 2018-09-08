#pragma once
#include "tower.h"

class CObj;
class CEmptyTower :
	public CTower
{
private:
	int		m_iStageNum;
public:
	void	SetStageNum(int _iStageNum){ m_iStageNum = _iStageNum; }
private:
	void KeyInput();
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CEmptyTower(void);
	~CEmptyTower(void);
};
