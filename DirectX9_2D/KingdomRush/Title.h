#pragma once
#include "scene.h"

class CTitle :
	public CScene
{
private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;
	TCHAR				m_szNum[MIN_STR];

	
public:
	CRITICAL_SECTION GetCrt(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CTitle(void);
	~CTitle(void);
};
