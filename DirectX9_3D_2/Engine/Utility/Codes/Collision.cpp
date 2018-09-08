#include "Collision.h"

USING(Engine)

IMPLEMENT_SINGLETON(CCollision)

CCollision::CCollision(void)
{

}

CCollision::~CCollision(void)
{

}

_bool CCollision::Collision_AABB(const _vec3& vDestMin, const _vec3& vDestMax, const _matrix& mDestWorld
					 , const _vec3& vSourMin, const _vec3& vSourMax, const _matrix& mSourWorld)
{
	_vec3			vWorldDestMin, vWorldDestMax;
	_vec3			vWorldSourMin, vWorldSourMax;


	// 월드영역으로 변화나낟.
	D3DXVec3TransformCoord(&vWorldDestMin, &vDestMin, &mDestWorld);
	D3DXVec3TransformCoord(&vWorldDestMax, &vDestMax, &mDestWorld);

	D3DXVec3TransformCoord(&vWorldSourMin, &vSourMin, &mSourWorld);
	D3DXVec3TransformCoord(&vWorldSourMax, &vSourMax, &mSourWorld);

	// 충돌하자.
	_float				fMin, fMax;

	// x축에서 바라봤을때 충돌
	fMin = max(vWorldDestMin.x, vWorldSourMin.x);
	fMax = min(vWorldDestMax.x, vWorldSourMax.x);

	if(fMin > fMax)
		return false;

	// y축에서 바라봤을때 충돌
	fMin = max(vWorldDestMin.y, vWorldSourMin.y);
	fMax = min(vWorldDestMax.y, vWorldSourMax.y);

	if(fMin > fMax)
		return false;

	// z축에서 바라봤을때 충돌
	fMin = max(vWorldDestMin.z, vWorldSourMin.z);
	fMax = min(vWorldDestMax.z, vWorldSourMax.z);

	if(fMin > fMax)
		return false;

	return true;	
}

_bool Engine::CCollision::Collision_OBB(const OBBINFO& DestInfo, const _matrix& mDestWorld , const OBBINFO& SourInfo, const _matrix& mSourWorld)
{
	_vec3			vWDest_CenterPos, vWSour_CenterPos;
	_vec3			vWDest_AxisDir[3], vWSour_AxisDir[3];

	// 월드영역으로 변화시킴.
	D3DXVec3TransformCoord(&vWDest_CenterPos, &DestInfo.vCenterPos, &mDestWorld);
	D3DXVec3TransformCoord(&vWSour_CenterPos, &SourInfo.vCenterPos, &mSourWorld);

	for(int i = 0; i < 3; ++i)
	{
		D3DXVec3TransformNormal(&vWDest_AxisDir[i], &DestInfo.vAxisDir[i], &mDestWorld);
		D3DXVec3TransformNormal(&vWSour_AxisDir[i], &SourInfo.vAxisDir[i], &mSourWorld);
	}

	_vec3 vDestDist, vSourDist;
	_vec3 vCToCDist;
	
	float fCToCProjDist;
	_vec3 vCompareAxis;
	

	vCToCDist = vWDest_CenterPos - vWSour_CenterPos;

	for( int i = 0; i < 3; ++i)
	{
		float fDestProjDist = 0.f;
		float fSourProjDist = 0.f;

		D3DXVec3Normalize(&vCompareAxis, &vWDest_AxisDir[i]);
		fCToCProjDist = abs(D3DXVec3Dot(&vCToCDist, &vCompareAxis));

		for( int j = 0; j < 3; ++j)
		{
			fDestProjDist += abs(D3DXVec3Dot(&vWDest_AxisDir[j], &vCompareAxis));
			fSourProjDist += abs(D3DXVec3Dot(&vWSour_AxisDir[j], &vCompareAxis));
		}

		if( fCToCProjDist > fDestProjDist + fSourProjDist)
			return false;
	}
	for( int i = 0; i < 3; ++i)
	{
		float fDestProjDist = 0.f;
		float fSourProjDist = 0.f;

		D3DXVec3Normalize(&vCompareAxis, &vWSour_AxisDir[i]);
		fCToCProjDist = abs(D3DXVec3Dot(&vCToCDist, &vCompareAxis));
		
		for( int j = 0; j < 3; ++j)
		{
			fDestProjDist += abs(D3DXVec3Dot(&vWDest_AxisDir[j], &vCompareAxis));
			fSourProjDist += abs(D3DXVec3Dot(&vWSour_AxisDir[j], &vCompareAxis));
		}

		if( fCToCProjDist > fDestProjDist + fSourProjDist)
			return false;
	}
	return true;
}



_bool Engine::CCollision::Collision_SphereSphere(const _vec3& vDestPos, const _float& fDestRadius, const _vec3& vSourPos, const _float& fSourRadius)
{

	float fDToSDist = D3DXVec3Length(&(vDestPos - vSourPos));

	if(fDToSDist < fDestRadius + fSourRadius)
	{
		return true;
	}
	return false;
}

_bool Engine::CCollision::Collision_SphereOBB(const _vec3& vDestMin, const _vec3& vDestMax, const _matrix& mDestWorld, const _vec3& vSourPos, const _float& fSourRadius)
{

	_vec3			vWorldDestMin, vWorldDestMax;

	// 월드영역으로 변화한다.
	D3DXVec3TransformCoord(&vWorldDestMin, &vDestMin, &mDestWorld);
	D3DXVec3TransformCoord(&vWorldDestMax, &vDestMax, &mDestWorld);

	if (vSourPos.x < vWorldDestMin.x && vWorldDestMin.x - vSourPos.x > fSourRadius)
		return false;

	if (vSourPos.x > vWorldDestMax.x && vSourPos.x  - vWorldDestMax.x > fSourRadius)
		return false;

	if (vSourPos.y < vWorldDestMin.y && vWorldDestMin.y - vSourPos.y > fSourRadius)
		return false;

	if (vSourPos.y > vWorldDestMax.y && vSourPos.y  - vWorldDestMax.y > fSourRadius)
		return false;

	if (vSourPos.z < vWorldDestMin.z && vWorldDestMin.z - vSourPos.z > fSourRadius)
		return false;

	if (vSourPos.z > vWorldDestMax.z && vSourPos.z  - vWorldDestMax.z > fSourRadius)
		return false;

	return true;	
}

_ulong Engine::CCollision::Release(void)
{
	delete this;

	return 0;
}
