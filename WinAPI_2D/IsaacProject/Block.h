#pragma once
#include "obj.h"

class CBlock :
	public CObj
{
protected:
	TILE*	m_pTile;
	INFO	m_tCOlInfo;

public:
	void SetTile(TILE*);
	void SetTileSize(float _fCX, float _fCY);
	RECT GetColRect();

	int	 GetIndex(){ return m_iIndex; }
public:
	virtual void Initialize(void)PURE;
	virtual int Progress(void)PURE;
	virtual void Render(HDC hdc)PURE;
	virtual void Release(void)PURE;
public:
	CBlock(void);
	virtual ~CBlock(void);
};
