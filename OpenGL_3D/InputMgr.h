#pragma once
#include "IncludeHeader.h"


class CInputMgr
{
	DECLARE_SINGLETON(CInputMgr);

public:
	float	m_fDeltaX;
	float	m_fDeltaY;
	float	m_fMousePosX;
	float	m_fMousePosY;
	bool	m_bCheck{ false };

public:
	void	UpdateInput(float mPosx, float mPosy);
public:
	CInputMgr();
	~CInputMgr();
};

