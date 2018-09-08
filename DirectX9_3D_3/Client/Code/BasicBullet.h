#ifndef BasicBullet_h__
#define BasicBullet_h__

#include "Bullet.h"

class CBasicBullet
	: public CBullet
{
private:
	explicit CBasicBullet(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CBasicBullet(void);
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
public:
	static CBasicBullet* Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	HRESULT AddComponent(void);
	void Move(void);

private:
	float	m_fDeleteTime;
};

#endif // BasicBullet_h__
