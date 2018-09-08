#ifndef GatlingGun_h__
#define GatlingGun_h__

#include "Barrel.h"

class CGatlingGun
	: public CBarrel
{
private:
	explicit CGatlingGun(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CGatlingGun(void);
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
public:
	static CGatlingGun* Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	HRESULT AddComponent(void);
	void Move(void);
};

#endif // GatlingGun_h__
