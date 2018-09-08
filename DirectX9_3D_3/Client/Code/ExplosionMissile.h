#ifndef ExplosionMissile_h__
#define ExplosionMissile_h__


#include "Effect.h"

class CExplosionMissile :
	public CEffect
{

private:
	explicit CExplosionMissile(LPDIRECT3DDEVICE9 pDevice);
public:
	~CExplosionMissile(void);
public:
	static CExplosionMissile* Create(LPDIRECT3DDEVICE9 pDevice);
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

#endif // ExplosionMissile_h__
