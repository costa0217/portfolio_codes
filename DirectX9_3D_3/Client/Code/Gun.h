#ifndef Gun_h__
#define Gun_h__

#include "Barrel.h"

class CGun
	: public CBarrel
{
private:
	explicit CGun(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CGun(void);
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
public:
	static CGun* Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	HRESULT AddComponent(void);
	void Move(void);
};

#endif // Gun_h__
