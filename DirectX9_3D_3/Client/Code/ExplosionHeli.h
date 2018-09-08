#ifndef ExplosionHeli_h__
#define ExplosionHeli_h__

#include "Effect.h"

class CExplosionHeli :
	public CEffect
{

private:
	explicit CExplosionHeli(LPDIRECT3DDEVICE9 pDevice);
public:
	~CExplosionHeli(void);
public:
	static CExplosionHeli* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	D3DXVECTOR3		m_vScale;

	float		m_fFrameCnt;
	float		m_fFrameMax;

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Move(void);
	virtual HRESULT AddComponent(void);
};

#endif // ExplosionHeli_h__
