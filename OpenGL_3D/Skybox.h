#pragma once
#include "GameObject.h"
class CSkybox :
	public CGameObject
{
public:
	GLubyte*	pBytes[6];
	BITMAPINFO*	info;
	MyVector	vRenderMax;
	MyVector	vRenderMin;
public:
	CSkybox();
	~CSkybox();

public:
	virtual void	Initialize();
	virtual int		Update(float fTime);
	virtual void	Render();
	virtual void	Release();

public:
	GLubyte * LoadDIBitmap(const char *filename, BITMAPINFO **info);


};

