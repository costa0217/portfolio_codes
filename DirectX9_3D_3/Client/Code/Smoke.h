#pragma once
#include "Effect.h"



class CSmoke :
	public CEffect
{

private:
	explicit CSmoke(LPDIRECT3DDEVICE9 pDevice);
public:
	~CSmoke(void);
public:
	static CSmoke* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Move(void);
	virtual HRESULT AddComponent(void);
private:
		float			m_fAngleX;
};
