#pragma once
#include "effect.h"

class CEgg :
	public CEffect
{
private:
	CObj*			m_pSpiderMother;

	vector<TILE*>*	m_pVecTile;
	vector<int>*	m_pVecMotherRoute;
	vector<int>		m_VecBabyRoute;

	list<CObj*>*	m_pMonsterList;
public:
	void	SetMother(CObj* _pSpiderMother){ m_pSpiderMother = _pSpiderMother; }
	void	SetMotherRoute(vector<int>* _pVecMotherRoute) { m_pVecMotherRoute = _pVecMotherRoute;}

	void	SetVecTile(vector<TILE*>* _pVecTile){ m_pVecTile = _pVecTile; }
public:
	virtual HRESULT Initialize(void)	;
	virtual int		Progress(void)		;
	virtual void	Render(void)		;
	virtual void	Release(void)		;
public:
	CEgg(void);
	~CEgg(void);
};
