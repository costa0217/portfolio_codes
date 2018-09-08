#ifndef BulletCol_h__
#define BulletCol_h__

#include "Collision.h"

namespace Engine
{
	class CGameObject;
}

class CBulletCol
	: public Engine::CCollision
{
private:
	CBulletCol(void);
public:
	virtual ~CBulletCol(void);

public:
	virtual Engine::CCollision* Clone(void);

public:
	virtual void Update(void);

public:
	static CBulletCol* Create(void);

private:
	void	Release(void);

private:	
	typedef list<Engine::CGameObject*>	OBJECTLIST;
	D3DXVECTOR3*		m_pBulletPos;	//미사일pos값
	float				m_fDamage;
	
	OBJECTLIST*			m_pMonsterList;

	bool	m_bCollision;

	
	float	m_fMonsterSize;
	
public:
	void	SetColInfo(D3DXVECTOR3* pBulletPos, float Damage, OBJECTLIST* pMonsterList);

	bool	GetbCollision(){return m_bCollision;}
	void	SetbCollision(bool bCollision){m_bCollision = bCollision;}

};

#endif // BulletCol_h__
