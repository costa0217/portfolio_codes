#pragma once
#include "bullet.h"

class CSkillLemon :
	public CBullet
{
private:
	DWORD	m_dwLemonTime;
	bool	m_bPlusSize;
public:
	RECT	GetCOlRect();
	virtual RECT GetRect();
public:
	virtual void Initialize(void);
	virtual int Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CSkillLemon(void);
	~CSkillLemon(void);
};
