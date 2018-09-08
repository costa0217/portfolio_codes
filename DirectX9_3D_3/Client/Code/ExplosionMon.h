#ifndef ExplosionMon_h__
#define ExplosionMon_h__

#include "Effect.h"

class CExplosionMon :
	public CEffect
{

private:
	explicit CExplosionMon(LPDIRECT3DDEVICE9 pDevice);
public:
	~CExplosionMon(void);
public:
	static CExplosionMon* Create(LPDIRECT3DDEVICE9 pDevice);
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

#endif // ExplosionMon_h__
