#ifndef Collision_h__
#define Collision_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CCollision
{
	DECLARE_SINGLETON(CCollision)
private:
	explicit CCollision(void);
	~CCollision(void);
public:
	_bool Collision_AABB(const _vec3& vDestMin, const _vec3& vDestMax, const _matrix& mDestWorld
		, const _vec3& vSourMin, const _vec3& vSourMax, const _matrix& mSourWorld);

	_bool Collision_OBB(const OBBINFO& DestInfo, const _matrix& mDestWorld
		, const OBBINFO& SourInfo, const _matrix& mSourWorld);

	_bool Collision_SphereSphere(const _vec3& vDestPos, const _float& fDestRadius,
		const _vec3& vSourPos, const _float& fSourRadius);

	_bool Collision_SphereOBB(const _vec3& vDestMin, const _vec3& vDestMax, const _matrix& mDestWorld,
		const _vec3& vSourPos, const _float& fSourRadius);
public:
	_ulong Release(void);
};

END

#endif // Collision_h__
