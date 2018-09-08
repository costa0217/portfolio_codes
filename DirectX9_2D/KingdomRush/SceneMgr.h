#pragma once

#include "Include.h"

class CScene;
class CSceneMgr
{
	DECLARE_SINGLETON(CSceneMgr)

private:
	CScene*			m_pScene;


public:
	void Initialize(SCENEID _eScene);
	void Progress(void);
	void Render(void);
	void Release(void);


private:
	CSceneMgr(void);
	~CSceneMgr(void);
};
