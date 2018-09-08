/*!
 * \file CollisionMgr.h
 * \date 2015/12/14 1:21
 *
 * \author USER
 * Contact: user@company.com
 *
 * \brief 충돌과 관련된 변수들을 관리하기 위한 매니저
 *
 * TODO: long description
 *
 * \note
*/

#ifndef CollisionMgr_h__
#define CollisionMgr_h__

#include "Defines.h"

#include "StaticObject.h"
#include "Monster.h"
#include "Effect.h"

class CCollisionMgr
{
	DECLARE_SINGLETON(CCollisionMgr)

private:
	explicit CCollisionMgr(void);
	virtual ~CCollisionMgr(void);

public:
	_int Update(const _float& fTimeDelta);
	void Render(void);

public:
	list<CStaticObject*>*	Get_StaticObjectList(void) { return &m_pStaticObjectList; }
	list<CStaticObject*>*	Get_StaticObjectDefaultList(void) { return &m_pStaticObjectDefaultList; }
	list<CMonster*>*		Get_MonsterList(void) { return &m_pMonsterList; }
	list<CEffect*>*			Get_EffectList(void) { return &m_pEffectList; }

private:
	list<CStaticObject*>	m_pStaticObjectList;
	list<CStaticObject*>	m_pStaticObjectDefaultList;
	list<CMonster*>			m_pMonsterList;
	list<CEffect*>			m_pEffectList;

public:
	void Release(void);
};

#endif // CollisionMgr_h__