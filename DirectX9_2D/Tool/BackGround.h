#pragma once
#include "obj.h"
#include "Include.h"

class CToolView;
class CBackGround :
	public CObj
{
public:
	vector<TILE*>	m_VecTile;
	CToolView*		m_MainView;

public:
	int		GetTileIndex(const D3DXVECTOR3& vPos);
	void	TileChange(const D3DXVECTOR3& vPos, const BYTE& byDrawID);
	bool	Picking(const D3DXVECTOR3& vPos, const int& iIndex);
public:
	virtual HRESULT Initialize(void);
	virtual void Progress(void);
	virtual void Render(void);
	virtual void Release(void);

public:
	void SetMainView(CToolView*	_MainView) { m_MainView = _MainView; }
public:
	CBackGround(void);
	virtual ~CBackGround(void);
};
