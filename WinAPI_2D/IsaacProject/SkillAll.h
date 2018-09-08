#pragma once
#include "bullet.h"

class CSkillAll :
	public CBullet
{
private:
	DWORD	m_dwSkillAllTime;
public:
	RECT	GetCOlRect();
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CSkillAll(void);
	~CSkillAll(void);
};
