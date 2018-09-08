#pragma once

#include "IncludeHeader.h"
class CGameObject;
class CCollide;
class CInputMgr;
class CCamera
{
public:
	MATRIX		m_matCam;
	CInputMgr*	m_pInputMgr;
	CGameObject**	m_pPlayer;
public:
	MyVector	m_vLook;

	float		m_fSpeed{ 100.f };
	float		m_fHeight;
	float		m_fMouseMoveSpd;

	float		m_fAngleX{ 0.f };
	float		m_fAngleY{ 0.f };

	WORD		m_wKey;
	WORD		m_wMouseInput;
	bool		m_bMouseCenter{ false };

	bool		m_bFPStype{ true };
	bool		m_bCosterEye{ false };

public:
	MATRIX		m_world;
	MyVector	m_vPos;
	MyVector	m_vPreDir;
	MyVector	m_vDir;

	float		m_fLerpTime{ 1.f };

public:
	bool		m_bJump{ false };
	float		m_fAccTime{ 0.f };

public:
	bool		m_bZooIn{ false };
	void		ZoomInOut();


public:
	void InputData(float fTime);
	void UpdateCamera();

private:
	void KeyInput(float fTime, MyVector& rMove);
public:
	CCamera();
	~CCamera();

public:

};

