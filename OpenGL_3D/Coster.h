#pragma once
#include "GameObject.h"

class CCamera;
class CCoster :
	public CGameObject
{
public:
	CCamera*				m_pCamera;
	vector<CGameObject*>*	m_pVecRailPoint;
	vector<CGameObject*>*	m_pVecEffect;
	int						m_iRailPointNum{ 0 };
	int						m_iRailNum{ 0 };

	int				m_iCosterNum{ 0 };
	float			m_fStartTime;
	float			m_fAccSpeed{ 0.f };
	bool			m_bStart{ false };

public:
	CGameObject**	m_pPlayers;
public:
	CCoster();
	~CCoster();

public:
	virtual void	Initialize();
	virtual int		Update(float fTime);
	virtual void	Render();
	virtual void	Release();

private:
	void	KeyInput(float fTime);
};

