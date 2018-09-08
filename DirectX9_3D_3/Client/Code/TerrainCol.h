/*!
 * \file TerrainCol.h
 * \date 2015/07/14 12:44
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

#ifndef TerrainCol_h__
#define TerrainCol_h__

#include "Collision.h"

class CTerrainCol
	: public Engine::CCollision
{
private:
	CTerrainCol(void);

public:
	virtual ~CTerrainCol(void);

public:
	virtual Engine::CCollision* Clone(void);

public:
	void SetColInfo(D3DXVECTOR3* pPos, const Engine::VTXTEX* pTerrainVtx, DWORD dwCntX, DWORD dwCntZ);

public:
	void SetHeliColInfo(D3DXVECTOR3* pLeftLegPos,D3DXVECTOR3* pRightLegPos,D3DXVECTOR3* pHeliPos ,const Engine::VTXTEX* pTerrainVtx, DWORD dwCntX, DWORD dwCntZ);

public:
	virtual void Update(void);

public:
	static CTerrainCol* Create(void);

private:
	void Release(void);

private:
	D3DXVECTOR3*			m_pPos;
	const Engine::VTXTEX*	m_pTerrainVtx;
	int						m_iIndex;
	bool					m_bYfix; 

	//

	D3DXVECTOR3*			m_pHeliPos;
	D3DXVECTOR3*			m_pLeftLegPos;
	D3DXVECTOR3*			m_pRightLegPos;
	bool*					m_bHeliCol;

public:
	int GetIndex(){return m_iIndex;}
	void SetbYfix(bool bYfix){m_bYfix = bYfix;}
	void SetbHeliCol(bool* bHeliCol){m_bHeliCol = bHeliCol;}
	
private:
	DWORD m_dwVtxCntX; 
	DWORD m_dwVtxCntZ; 

};

#endif // TerrainCol_h__