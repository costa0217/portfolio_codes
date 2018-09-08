#ifndef MultiBullet_h__
#define MultiBullet_h__

#include "Bullet.h"



class CMultiBullet
	: public CBullet
{
private:
	explicit CMultiBullet(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CMultiBullet(void);
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
public:
	static CMultiBullet* Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	HRESULT AddComponent(void);
	void Move(void);
};

#endif // MultiBullet_h__
