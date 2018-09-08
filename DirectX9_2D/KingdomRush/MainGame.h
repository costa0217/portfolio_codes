#pragma once

#include "Include.h"
#include "Device.h"

class CMainGame
{
private:
	CDevice*		m_pDevice;
	float			m_fTime;
	int				m_iCount;
	TCHAR			m_szFPS[MIN_STR];
	D3DXMATRIX		m_matTrans;
private:
	void			ImageRoad();
	void			SoundRoad();
public:
	HRESULT			Initialize(void);
	void			Progress(void);
	void			Render(void);
	void			Release(void);
public:
	CMainGame(void);
	~CMainGame(void);
};
