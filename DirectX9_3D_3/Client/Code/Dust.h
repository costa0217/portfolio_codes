#pragma once
#include "Effect.h"



class CDust :
	public CEffect
{

private:
	explicit CDust(LPDIRECT3DDEVICE9 pDevice);
public:
	~CDust(void);
public:
	static CDust* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	D3DXVECTOR3		m_vDir;
	D3DXVECTOR3		m_vScale;
public:
	void DustRotationY();
	
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Move(void);
	virtual HRESULT AddComponent(void);
};
