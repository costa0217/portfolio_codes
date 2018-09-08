/*!
 * \file Collision.h
 * \date 2015/11/07 1:19
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: 오브젝트들의 충돌여부를 매개변수로 값을 받아 bool변수로 반환하는 Singleton CCollision class.
 *
 * \note
*/

#ifndef Collision_h__
#define Collision_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CCollision
{
	DECLARE_SINGLETON(CCollision)
private:
	typedef struct tagOBB
	{
		_vec3			vPoint[8];
		_vec3			vCenter;
		_vec3			vProjAxis[3];
		_vec3			vAxis[3];		
	}OBB;

private:
	explicit CCollision(void);
	~CCollision(void);
public:
	_bool Collision_AABB_AABB(const _vec3& vDestMin, const _vec3& vDestMax, const _matrix& mDestWorld
		, const _vec3& vSourMin, const _vec3& vSourMax, const _matrix& mSourWorld); //AABB - AABB

	_bool Collision_AABB_Sphere(const _vec3& vDestMin, const _vec3& vDestMax, const _matrix& mDestWorld,
		const _vec3& vSourPos, const _float& fSourRadius); //AABB - Sphere

	_bool Collision_WorldAABB_WorldSphere(const _vec3& vDestWorldMin, const _vec3& vDestWorldMax,
		const _vec3& vSourPos, const _float& fSourRadius); //WorldAABB - Sphere

	_bool Collision_OBB_OBB(const _vec3& vDestMin, const _vec3& vDestMax, const _matrix& mDestWorld
		, const _vec3& vSourMin, const _vec3& vSourMax, const _matrix& mSourWorld); //OBB - OBB

	_bool Collision_OBB_WorldSphere(const _vec3& vDestMin, const _vec3& vDestMax, const _matrix& mDestWorld,
		const _vec3 vScale, const _vec3& vSourPos, const _float& fSourRadius);	//OBB - WorldSphere

	_bool Collision_OBB_WorldSphere_SlidingVector(const _vec3& vDestMin, const _vec3& vDestMax, const _matrix& mDestWorld,
		const _vec3 vScale, const _vec3& vSourPos, const _float& fSourRadius, const _vec3& vOriginalPos, _vec3* vAfterPos);

	_bool Collision_Sphere_Sphere(const _vec3& vDestPos, const _float& fDestRadius,
		const _vec3& vSourPos, const _float& fSourRadius);	// Sphere - Sphere

	_bool Collision_WorldSphere_Sphere(const _vec3& vDestPos, const _float& fDestRadius,  const _matrix& mDestWorld,
		const _vec3& vSourPos, const _float& fSourRadius);	// WorldSphere - Sphere

private:
	OBB						m_tOBB[2];

private:
	void Set_Points(OBB* pOBB, const _vec3& vMin, const _vec3& vMax);
	void Set_Axis(OBB* pOBB);

public:
	_ulong Release(void);
};

END

#endif // Collision_h__
