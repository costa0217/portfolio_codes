/*!
 * \file Mousecol.h
 * \date 2015/07/15 12:36
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

#ifndef Mousecol_h__
#define Mousecol_h__

#include "Collision.h"

namespace Engine
{
	class CInfoSubject;
}

class CCameraObserver;

class CMouseCol
	: public Engine::CCollision
{
private:
	CMouseCol(void);

public:
	virtual ~CMouseCol(void);

public:
	virtual Engine::CCollision* Clone(void);
	static POINT GetMousePos(void);

public:
	void PickTerrain(D3DXVECTOR3* pOut, const Engine::VTXTEX* pTerrainVtx);
	void PickObject(D3DXVECTOR3* pOut
		, const Engine::VTXTEX* pVertex
		, const D3DXMATRIX* pmatWorld);

public:
	static CMouseCol* Create(void);

private:
	HRESULT Initialize(void);
	void Translation_ViewSpace(void);
	void Translation_Local(const D3DXMATRIX* pWorld);
	void Release(void);

private:
	Engine::CInfoSubject*		m_pInfoSubject;

private:
	CCameraObserver*		m_pCameraObserver;

private:
	D3DXVECTOR3		m_vPivotPos;
	D3DXVECTOR3		m_vRayDir;
};

#endif // Mousecol_h__