#ifndef Mist_h__
#define Mist_h__


#include "Effect.h"


class CMist :
	public CEffect
{

private:
	explicit CMist(LPDIRECT3DDEVICE9 pDevice);
public:
	~CMist(void);
public:
	static CMist* Create(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Move(void);
	virtual HRESULT AddComponent(void);
private:
	float			m_fAngleX;

};

#endif // Mist_h__
