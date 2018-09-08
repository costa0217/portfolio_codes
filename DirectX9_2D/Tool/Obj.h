#pragma once

class CObj
{
public:
	virtual HRESULT Initialize()	PURE;
	virtual void	Progress()		PURE;
	virtual void	Render()		PURE;
	virtual void	Release()		PURE;
	
public:
	CObj(void);
	virtual ~CObj(void);
};
