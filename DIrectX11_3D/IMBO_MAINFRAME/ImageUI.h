#pragma once
#include "UIObject.h"
class CImageUI :
	public CUIObject
{
public:
	CImageUI();
	~CImageUI();


public:
	static CImageUI* Create(XMVECTOR xyPos, XMVECTOR xySize, string strTexName, float fRanderLayer);

	void	SetInfo(XMVECTOR xyPos, XMVECTOR xySize, string strTexName, float fRanderLayer);

public:
	virtual HRESULT Initialize();
	virtual int		Update(float fTimeElapsed);
	virtual void	Render();
	virtual void	Release();

private:
	void	SetParameter();
};

