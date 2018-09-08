#pragma once
#include "GameObject.h"
class CMyTree :
	public CGameObject
{
public:
	CMyTree();
	~CMyTree();

public:
	virtual void	Initialize();
	virtual int		Update(float fTime);
	virtual void	Render();
	virtual void	Release();

};

