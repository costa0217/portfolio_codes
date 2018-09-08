#ifndef Dodge_h__
#define Dodge_h__


#include "GameObject.h"

namespace Engine
{
	class CResourcesMgr;
	class CTimeMgr;
	class CInfoSubject;
	class CManagement;

	class CTexture;
	class CVIBuffer;
	class CTransform;

	class CCameraObserver;
}

class CCameraObserver;
class CCollisionMgr;
class CMouseCol;

class CTerrainCol;

class CDodge
	: public Engine::CGameObject
{
private:
	explicit CDodge(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CDodge(void);

public:
	void SetPos(const D3DXVECTOR3& rPos);
	void SetHP(float rHP);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CDodge* Create(LPDIRECT3DDEVICE9 pDevice);
private:
	typedef list<CGameObject*>			OBJECTLIST;
	typedef map<wstring, OBJECTLIST>	MAPOBJLIST;
	MAPOBJLIST*							m_pMapObjlist;

private:
	HRESULT AddComponent(void);
	void TransformMatrix(void);
	void Release(void);

private:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;
	Engine::CInfoSubject*	m_pInfoSubject;
	Engine::CManagement*	m_pManagement;
	CCollisionMgr*			m_pCollisionMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CTexture*		m_pTexture;
	Engine::CVIBuffer*		m_pBuffer;
	CTerrainCol*			m_pTerrainCol;

private:
	CCameraObserver*		m_pCameraObserver;

private:
	Engine::VTXCOL*			m_pVertex;
	Engine::VTXCOL*			m_pConvertVertex;
	const Engine::VTXTEX*	m_pTerrainVertex;

private:
	DWORD			m_dwVtxCnt;

	bool m_bDie;
public:
	//ÃÑ¾Ë Ãß°¡¿ë
	void	SetMapObjlist(MAPOBJLIST* pMapObjlist){m_pMapObjlist = pMapObjlist;}
};


#endif // Dodge_h__

