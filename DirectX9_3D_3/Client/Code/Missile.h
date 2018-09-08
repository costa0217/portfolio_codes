#ifndef Missile_h__
#define Missile_h__

#include "Bullet.h"


class CMissile
	: public CBullet
{
private:
	explicit CMissile(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CMissile(void);
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
public:
	static CMissile* Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	HRESULT AddComponent(void);
	void Move(void);

private:
	float	m_fDeleteTime;
	typedef list<CGameObject*>				OBJECTLIST;
	OBJECTLIST* m_pMistList;

	float m_fMistTime;
public:
	void SetMistList(OBJECTLIST* pMistList){m_pMistList = pMistList;}

};
#endif // Missile_h__
