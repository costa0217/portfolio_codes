#pragma once
#include "UIObject.h"
class CLoadingBack :
	public CUIObject
{
public:
	CLoadingBack();
	~CLoadingBack();

public:
	static CLoadingBack* Create(XMVECTOR xyPos, XMVECTOR xySize, string strTexName);

	void	SetInfo(XMVECTOR xyPos, XMVECTOR xySize, string strTexName);

public:
	virtual HRESULT Initialize() ;
	virtual int		Update(float fTimeElapsed) ;
	virtual void	Render() ;
	virtual void	Release() ;

private:
	int		m_iType{ 0 };

private:
	void	SetParameter();
};

