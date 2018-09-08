#ifndef Explosion_h__
#define Explosion_h__

#include "Effect.h"

class CExplosion :
	public CEffect
{

private:
	explicit CExplosion(LPDIRECT3DDEVICE9 pDevice);
public:
	~CExplosion(void);
public:
	static CExplosion* Create(LPDIRECT3DDEVICE9 pDevice);
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

#endif // Explosion_h__
