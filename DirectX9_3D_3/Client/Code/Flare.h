#pragma once
#include "Effect.h"



class CFlare :
	public CEffect
{

private:
	explicit CFlare(LPDIRECT3DDEVICE9 pDevice);
public:
	~CFlare(void);
public:
	static CFlare* Create(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Move(void);
	virtual HRESULT AddComponent(void);
};
