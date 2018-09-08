/*!
 * \file Terrain.h
 * \date 2015/07/09 12:32
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

#ifndef Terrain_h__
#define Terrain_h__

#include "GameObject.h"
#include "Include.h" //0805

namespace Engine
{
	class CResourcesMgr;
	class CInfoSubject;

	class CTexture;
	class CVIBuffer;
	class CTransform;
}

class CCameraObserver;
class CTreeRect;

class CTerrain
	: public Engine::CGameObject
{
private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CTerrain(void);

public:
	virtual const Engine::VTXTEX* GetTerrainVtx(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pDevice, TERRAINTYPE eTerrainType);

private:
	HRESULT AddComponent(void);
	void TransformMatrix(void);
	void SetTerrainType(TERRAINTYPE _eTerrainType);
	void Release(void);

private:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CInfoSubject*	m_pInfoSubject;

private:
	Engine::CTexture*		m_pTexture;
	Engine::CVIBuffer*		m_pBuffer;
	Engine::CTransform*		m_pInfo;

private:
	CCameraObserver*		m_pCameraObserver;

private:
	Engine::VTXTEX*		m_pVertex;
	Engine::VTXTEX*		m_pConvertVertex;

private:
	DWORD		m_dwVtxCnt;
	DWORD		m_dwVtxCntX; // 0805
	DWORD		m_dwVtxCntZ; // 0805

	TERRAINTYPE	m_eTerrainType;

	vector<CTreeRect*>			m_vecTree;
	
};

#endif // Terrain_h__