#pragma once
#include "IncludeHeader.h"

class CCamera;
class CGameObject;
class CUIObject;
class CMainFrame
{
private:
	vector<CGameObject*>	vecObjects[OBJ_END];

	MyVector	m_vPos;
	MyVector	m_vScale;
	MyVector	m_vRot;

	MyVector	m_vtx[4];

	CCamera*		m_pCamera;
	CGameObject*	m_pPlayer[3];

	float			m_fAccFireWorkTime{ 0.f };
public:
	WORD		m_wKey{ 0 };
	int			m_iRailCnt{ 0 };
	int			m_iHeight{ 0 };
	bool		m_bMakeRail{ false };
	bool		m_bTernel{ false };

public:
	void		Initialize();
	int			Update(float fTime);
	void		Render();
	void		Release();

private:
	void   UIRender();
	void   KeyInput(float fTime);
	void   RenderAxis();

public:
	void   InputMousePoint(float x, float y);

private:
	void	CreateMapObject();
	void	CreateCharObject();

	void	MakePositioningRail();
	void	SettingLight();

public:
	CMainFrame();
	~CMainFrame();
};
