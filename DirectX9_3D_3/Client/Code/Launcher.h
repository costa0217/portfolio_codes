#ifndef Launcher_h__
#define Launcher_h__

#include "Barrel.h"

class CLauncher
	: public CBarrel
{
private:
	explicit CLauncher(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CLauncher(void);
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
public:
	static CLauncher* Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	HRESULT AddComponent(void);
	void Move(void);
};


#endif // Launcher_h__
