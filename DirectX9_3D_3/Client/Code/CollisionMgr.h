/*!
 * \file CollisionMgr.h
 * \date 2015/07/14 13:35
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Engine_Include.h"

namespace Engine
{
	class CCollision;
}

class CCollisionMgr
{
public:
	DECLARE_SINGLETON(CCollisionMgr)

public:
	enum COLLSIONID {COL_TERRAIN, COL_MOUSE, };

public:
	CCollisionMgr(void);
	~CCollisionMgr(void);

public:
	Engine::CCollision* CloneColObject(COLLSIONID eCollisionID);

public:
	HRESULT AddColObject(COLLSIONID eCollisionID);

private:
	void Release(void);

private:
	typedef map<COLLSIONID, Engine::CCollision*>		MAPCOLLISION;
	MAPCOLLISION		m_mapCollision;
};

#endif // CollisionMgr_h__