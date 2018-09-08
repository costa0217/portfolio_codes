#pragma once
#include "UIObject.h"
class CClickButton :
	public CUIObject
{
public:
	CClickButton();
	~CClickButton();

public:
	static CClickButton* Create(XMVECTOR xyPos, XMVECTOR xySize, string strTexName);
	void	SetInfo(XMVECTOR xyPos, XMVECTOR xySize, string strTexName);

private:
	int		m_nID{ 0 };
public:
	void	SetID(int nID) { m_nID = nID; }
	int		GetID() { return m_nID; }

public:
	virtual HRESULT Initialize();
	virtual int		Update(float fTimeElapsed);
	virtual void	Render();
	virtual void	Release();

private:
	void	SetParameter();
};

