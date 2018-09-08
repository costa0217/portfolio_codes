#include "CollisioneMgr.h"
#include "GameObject.h"
#include "Collide.h"

IMPLEMENT_SINGLETON(CCollisioneMgr)

CCollisioneMgr::CCollisioneMgr()
{
}
CCollisioneMgr::~CCollisioneMgr()
{
}
bool CCollisioneMgr::CheckCollision(MATRIX mMy, MyVector vDeltaMove, CGameObject* pMy)
{
	//bool bResult = true;

	//auto Myiter = find(pvecObjects->begin(), pvecObjects->end(), pMy);
	//MyVector*	pMin = &MyVector(0.f, 0.f, 0.f);
	//MyVector*	pMax = &MyVector(0.f, 0.f, 0.f);

	//mMy._41 += vDeltaMove.x;
	//mMy._42 += vDeltaMove.y;
	//mMy._43 += vDeltaMove.z;

	//MyTransformCoord(pMin, &(*Myiter)->m_pCollide->m_vMin, &mMy);
	//MyTransformCoord(pMax, &(*Myiter)->m_pCollide->m_vMax, &mMy);

	//vector<CGameObject*>::iterator iter = pvecObjects->begin();
	//for (; iter != pvecObjects->end(); ++iter)
	//{
	//	if (iter == Myiter) continue;

	//	MyVector*	pOtherMin = &MyVector(0.f, 0.f, 0.f);
	//	MyVector*	pOtherMax = &MyVector(0.f, 0.f, 0.f);

	//	MyTransformCoord(pOtherMin, &(*iter)->m_pCollide->m_vMin, &((*iter)->m_matWorld));
	//	MyTransformCoord(pOtherMax, &(*iter)->m_pCollide->m_vMax, &((*iter)->m_matWorld));

	//	if (pMax->x < pOtherMin->x || pMin->x > pOtherMax->x) continue;
	//	if (pMax->y < pOtherMin->y || pMin->y > pOtherMax->y) continue;
	//	if (pMax->z < pOtherMin->z || pMin->z > pOtherMax->z) continue;

	//	return true;
	//}
	return false;
}

bool CCollisioneMgr::CheckCollisToCamera(MATRIX mMy, MyVector vDeltaMove, CCollide* myColl)
{
	//auto Myiter = find(pvecObjects->begin(), pvecObjects->end(), pMy);
	//MyVector*	pMin = &MyVector(0.f, 0.f, 0.f);
	//MyVector*	pMax = &MyVector(0.f, 0.f, 0.f);

	//mMy._41 += vDeltaMove.x;
	//mMy._42 += vDeltaMove.y;
	//mMy._43 += vDeltaMove.z;

	//MyTransformCoord(pMin, &myColl->m_vMin, &mMy);
	//MyTransformCoord(pMax, &myColl->m_vMax, &mMy);

	//vector<CGameObject*>::iterator iter = pvecObjects->begin();
	//for (; iter != pvecObjects->end(); ++iter)
	//{
	//	if ((*iter)->m_eType != OBJ_MAP) continue;

	//	MyVector*	pOtherMin = &MyVector(0.f, 0.f, 0.f);
	//	MyVector*	pOtherMax = &MyVector(0.f, 0.f, 0.f);

	//	MyTransformCoord(pOtherMin, &(*iter)->m_pCollide->m_vMin, &((*iter)->m_matWorld));
	//	MyTransformCoord(pOtherMax, &(*iter)->m_pCollide->m_vMax, &((*iter)->m_matWorld));

	//	if (pMax->x < pOtherMin->x || pMin->x > pOtherMax->x) continue;
	//	if (pMax->y < pOtherMin->y || pMin->y > pOtherMax->y) continue;
	//	if (pMax->z < pOtherMin->z || pMin->z > pOtherMax->z) continue;

	//	return true;
	//}
	return false;
}

bool CCollisioneMgr::CheckBulletRayToAtkObj(MyVector* pvOut, MyVector* pvOutDir, int* nType, MyVector vRayStartPos, MyVector vRayDir)
{
	//MyNormalize(&vRayDir, &vRayDir);
	//map<float, CGameObject*> mapColl;
	//map<float, MyVector> mapDir;

	//MyVector vtx[4];
	//vtx[0] = MyVector(TerrainSize, 0.f, -TerrainSize * 0.5f);
	//vtx[1] = MyVector(TerrainSize, 0.f, TerrainSize* 0.5f);
	//vtx[2] = MyVector(-TerrainSize, 0.f, TerrainSize* 0.5f);
	//vtx[3] = MyVector(-TerrainSize, 0.f, -TerrainSize* 0.5f);
	//MyVector	vPoint;
	//float		fU, fV;
	//float		fDist = 1000.f;
	//if (IntersectTriangle(vRayStartPos, vRayDir, vtx[3], vtx[2], vtx[0], &fDist, &fU, &fV))
	//{
	//	vPoint = Vec3Cross(vtx[2] - vtx[3], vtx[0] - vtx[3]);
	//	MyNormalize(&vPoint, &vPoint);
	//	if (fDist > 10.f)
	//	{
	//		mapColl.insert(make_pair(fDist, nullptr));
	//		mapDir.insert(make_pair(fDist, vPoint));
	//	}

	//}

	//if (IntersectTriangle(vRayStartPos, vRayDir, vtx[0], vtx[2], vtx[1], &fDist, &fU, &fV))
	//{
	//	vPoint = Vec3Cross(vtx[2] - vtx[0], vtx[1] - vtx[0]);
	//	MyNormalize(&vPoint, &vPoint);
	//	mapColl.insert(make_pair(fDist, nullptr));
	//	mapDir.insert(make_pair(fDist, vPoint));
	//}


	//for (int i = 0; i < OBJ_END; ++i)
	//{
	//	if (i == OBJ_MAP || i == OBJ_CHAR_ENEMY)
	//	{

	//		vector<CGameObject*>::iterator iter = pvecObjects[i].begin();
	//		vector<CGameObject*>::iterator iter_end = pvecObjects[i].end();
	//		for (; iter != iter_end; ++iter)
	//		{
	//			MyVector vPt[8];
	//			MyPlane plane[6];

	//			vPt[0] = MyVector((*iter)->m_pCollide->m_vMin.x, (*iter)->m_pCollide->m_vMax.y, (*iter)->m_pCollide->m_vMin.z);
	//			vPt[1] = MyVector((*iter)->m_pCollide->m_vMin.x, (*iter)->m_pCollide->m_vMax.y, (*iter)->m_pCollide->m_vMax.z);
	//			vPt[2] = MyVector((*iter)->m_pCollide->m_vMax.x, (*iter)->m_pCollide->m_vMax.y, (*iter)->m_pCollide->m_vMax.z);
	//			vPt[3] = MyVector((*iter)->m_pCollide->m_vMax.x, (*iter)->m_pCollide->m_vMax.y, (*iter)->m_pCollide->m_vMin.z);
	//			vPt[4] = MyVector((*iter)->m_pCollide->m_vMin.x, (*iter)->m_pCollide->m_vMin.y, (*iter)->m_pCollide->m_vMin.z);
	//			vPt[5] = MyVector((*iter)->m_pCollide->m_vMin.x, (*iter)->m_pCollide->m_vMin.y, (*iter)->m_pCollide->m_vMax.z);
	//			vPt[6] = MyVector((*iter)->m_pCollide->m_vMax.x, (*iter)->m_pCollide->m_vMin.y, (*iter)->m_pCollide->m_vMax.z);
	//			vPt[7] = MyVector((*iter)->m_pCollide->m_vMax.x, (*iter)->m_pCollide->m_vMin.y, (*iter)->m_pCollide->m_vMin.z);
	//			for (int j = 0; j < 8; ++j) {
	//				MyTransformCoord(&vPt[j], &vPt[j], &(*iter)->m_matWorld);
	//			}
	//			plane[0] = MyPlane(vPt[0], vPt[3], vPt[4], vPt[7]);		//앞
	//			plane[1] = MyPlane(vPt[3], vPt[2], vPt[7], vPt[6]);		//우
	//			plane[2] = MyPlane(vPt[2], vPt[1], vPt[6], vPt[5]);		//뒤
	//			plane[3] = MyPlane(vPt[1], vPt[0], vPt[5], vPt[4]);		//좌
	//			plane[4] = MyPlane(vPt[1], vPt[2], vPt[0], vPt[3]);		//상
	//			plane[5] = MyPlane(vPt[4], vPt[7], vPt[5], vPt[6]);		//하

	//			for (int j = 0; j < 6; ++j)
	//			{
	//				MyVector	vPoint;
	//				float		fU, fV;
	//				float		fDist = 1000.f;
	//				if (IntersectTriangle(vRayStartPos, vRayDir, plane[j].v0, plane[j].v1, plane[j].v2, &fDist, &fU, &fV))
	//				{
	//					vPoint = Vec3Cross(plane[j].v1 - plane[j].v0, plane[j].v2 - plane[j].v0);
	//					MyNormalize(&vPoint, &vPoint);
	//					if (fDist > 10.f)
	//					{
	//						mapColl.insert(make_pair(fDist, (*iter)));
	//						mapDir.insert(make_pair(fDist, vPoint));
	//					}
	//				}

	//				if (IntersectTriangle(vRayStartPos, vRayDir, plane[j].v1, plane[j].v3, plane[j].v2, &fDist, &fU, &fV))
	//				{
	//					vPoint = Vec3Cross(plane[j].v3 - plane[j].v1, plane[j].v2 - plane[j].v1);
	//					MyNormalize(&vPoint, &vPoint);
	//					if (fDist > 10.f)
	//					{
	//						mapColl.insert(make_pair(fDist, (*iter)));
	//						mapDir.insert(make_pair(fDist, vPoint));
	//					}
	//				}
	//			}
	//		}
	//	}
	//}

	//if (true != mapColl.empty())
	//{
	//	if (mapColl.begin()->second != nullptr)
	//		*nType = mapColl.begin()->second->m_eType == OBJ_MAP ? 0 : 1;
	//	else
	//		*nType = 0;

	//	*pvOut = vRayStartPos + vRayDir * mapColl.begin()->first;
	//	*pvOutDir = mapDir.begin()->second;
	//	return true;
	//}

	return false;
}

bool CCollisioneMgr::CheckOBB(CCollide * myColl, MATRIX * myWorld, MyVector vDeltaMove)
{
	//MyVector			vWDest_CenterPos;
	//MyVector			vWDest_AxisDir[3];

	//MATRIX mWorldTemp = *myWorld;
	//mWorldTemp._41 += vDeltaMove.x;
	//mWorldTemp._42 += vDeltaMove.y;
	//mWorldTemp._43 += vDeltaMove.z;

	//// 월드영역으로 변화시킴.
	//MyTransformCoord(&vWDest_CenterPos, &myColl->m_tOBBInfo.vCenterPos, &mWorldTemp);

	//for (int i = 0; i < 3; ++i)
	//{
	//	MyTransformNormal(&vWDest_AxisDir[i], &myColl->m_tOBBInfo.vAxisDir[i], &mWorldTemp);
	//}

	//vector<CGameObject*>::iterator iter = pvecObjects[OBJ_MAP].begin();
	//vector<CGameObject*>::iterator iter_end = pvecObjects[OBJ_MAP].end();
	//for (; iter != iter_end; )
	//{
	//	MyVector vDestDist, vSourDist;
	//	MyVector vCToCDist;

	//	float fCToCProjDist;
	//	MyVector vCompareAxis;

	//	MyVector			vWSour_CenterPos;
	//	MyVector			vWSour_AxisDir[3];

	//	bool bNext = false;

	//	// 월드영역으로 변화시킴.
	//	MyTransformCoord(&vWSour_CenterPos, &(*iter)->m_pCollide->m_tOBBInfo.vCenterPos, &(*iter)->m_matWorld);

	//	for (int i = 0; i < 3; ++i)
	//	{
	//		MyTransformNormal(&vWSour_AxisDir[i], &(*iter)->m_pCollide->m_tOBBInfo.vAxisDir[i], &(*iter)->m_matWorld);
	//	}

	//	vCToCDist = vWDest_CenterPos - vWSour_CenterPos;

	//	for (int i = 0; i < 3; ++i)
	//	{
	//		float fDestProjDist = 0.f;
	//		float fSourProjDist = 0.f;

	//		MyNormalize(&vCompareAxis, &vWDest_AxisDir[i]);
	//		fCToCProjDist = abs(Vec3Dot(vCToCDist, vCompareAxis));

	//		for (int j = 0; j < 3; ++j)
	//		{
	//			fDestProjDist += abs(Vec3Dot(vWDest_AxisDir[j], vCompareAxis));
	//			fSourProjDist += abs(Vec3Dot(vWSour_AxisDir[j], vCompareAxis));
	//		}

	//		if (fCToCProjDist > fDestProjDist + fSourProjDist)
	//		{
	//			bNext = true;
	//		}
	//	}
	//	if (true == bNext)
	//	{
	//		++iter;
	//		continue;
	//	}
	//	for (int i = 0; i < 3; ++i)
	//	{
	//		float fDestProjDist = 0.f;
	//		float fSourProjDist = 0.f;

	//		MyNormalize(&vCompareAxis, &vWSour_AxisDir[i]);
	//		fCToCProjDist = abs(Vec3Dot(vCToCDist, vCompareAxis));

	//		for (int j = 0; j < 3; ++j)
	//		{
	//			fDestProjDist += abs(Vec3Dot(vWDest_AxisDir[j], vCompareAxis));
	//			fSourProjDist += abs(Vec3Dot(vWSour_AxisDir[j], vCompareAxis));
	//		}

	//		if (fCToCProjDist > fDestProjDist + fSourProjDist)
	//			bNext = true;
	//	}
	//	if (true == bNext)
	//	{
	//		++iter;
	//		continue;
	//	}
	//	return true;
	//}
	return false;
}
