#pragma once
#include "scene.h"

class CField :
	public CScene
{
private:
	D3DXVECTOR3		m_vStage1;
	D3DXVECTOR3		m_vStage2;

	float			m_fScaleStage1;
	float			m_fScaleStage2;
	bool			m_bStage1Wnd;
	bool			m_bStage2Wnd;

public:
	void	KeyInput();
public:
	virtual HRESULT Initialize(void);
	virtual int Progress(void)		;
	virtual void Render(void)		;
	virtual void Release(void)		;
public:
	CField(void);
	~CField(void);
};
