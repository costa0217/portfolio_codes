#pragma once
#include "scene.h"

class CMyMenu :
	public CScene
{
private:
	FRAME	m_tMenuFrame;
	FRAME	m_tStartButton;
	DWORD	m_dwMenuTime;
public:
	virtual void Initialize(void);
	virtual void Progress(void);
	virtual void Render(HDC hdc);
	virtual void Release(void);
public:
	CMyMenu(void);
	~CMyMenu(void);
};
