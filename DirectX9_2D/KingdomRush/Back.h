#pragma once
#include "obj.h"

class CBack :
	public CObj
{
private:
	//wstring				m_szStateKey;

	vector<TILE*>*		m_pVecTile;
	int					m_iTileRenderX;
	int					m_iTileRenderY;

	TCHAR				m_szTileNum[MIN_STR];
	TCHAR				m_szTileID[MIN_STR];
	TCHAR				m_szMouse[MIN_STR];

	D3DXMATRIX			m_matTileTrans;		// 타일 인덱스
	D3DXMATRIX			m_matTileTrans2;	// 타일 타입
	D3DXMATRIX			m_matTileTrans3;	// 마우스 좌표
	int					m_iGoalIndex;

	D3DXVECTOR3			m_vMouse;
	
	bool				m_bTileOpen;

private:
	void		KeyInput();
	void		Scroll();

public:
	bool		Picking(const D3DXVECTOR3& vPos, const int& iIndex);
	int			GetTileIndex(const D3DXVECTOR3& vPos);
	void		SetVecTile(vector<TILE*>*	_vecTile){ m_pVecTile = _vecTile; }

	vector<TILE*>* GetVecTile(){ return m_pVecTile; }


public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void);
	virtual void Render(void);
	virtual void Release(void);
public:
	CBack(void);
	~CBack(void);
};
