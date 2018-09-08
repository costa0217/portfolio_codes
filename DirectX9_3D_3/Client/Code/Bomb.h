#ifndef Bomb_h__
#define Bomb_h__

#include "Bullet.h"

namespace Engine
{
	class CResourcesMgr;
	class CTimeMgr;
	class CInfoSubject;
	class CManagement;

	class CTexture;
	class CVIBuffer;
	class CTransform;

	class CCameraObserver;
}

class CCameraObserver;
class CCollisionMgr;

class CBomb
	: public CBullet
{
private:
	explicit CBomb(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CBomb(void);
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
public:
	static CBomb* Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	HRESULT AddComponent(void);
	void Move(void);
private:
	bool* m_bFragmentCreate;
public:
	void SetbFragmentCreate(bool* bFragmentCreate){m_bFragmentCreate = bFragmentCreate;}

};

#endif // Bomb_h__
