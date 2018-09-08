#pragma once
#include "obj.h"

class CSoldier :
	public CObj
{
protected:
	SOLDIERID		m_eSoldierID;

	int				m_iLevel;
	bool			m_bLvl[LVL_END];
	bool			m_bLevelup;

public:
	SOLDIERID		GetSoliderID(){ return m_eSoldierID; }
	int				GetLevel(){return m_iLevel;}
	void			SetLevelup(int _iLevel);
public:
	virtual HRESULT Initialize(void)	PURE;
	virtual int		Progress(void)		PURE;
	virtual void	Render(void)		PURE;
	virtual void	Release(void)		PURE;
public:
	CSoldier(void);
	virtual ~CSoldier(void);
};
