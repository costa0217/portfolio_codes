#ifndef Bunker_h__
#define Bunker_h__

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
	class CPlayerObserver;
}

class CPlayerObserver;
class CCameraObserver;
class CCollisionMgr;
class CMouseCol;

class CTerrainCol;

class CBunker
	: public Engine::CGameObject
{
private:
	explicit CBunker(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CBunker(void);

public:
	void SetPos(const D3DXVECTOR3& rPos);
	void SetHP(float rHP);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CBunker* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT AddComponent(void);
	void TransformMatrix(void);
	void Release(void);

private:
	void	SetParts(void);
	void	CombinePart(void);

	void	SetPawRotation(void);

private:
	typedef list<CGameObject*>			OBJECTLIST;
	typedef map<wstring, OBJECTLIST>	MAPOBJLIST;
	MAPOBJLIST*							m_pMapObjlist;

public:
	void	SetPlayer(Engine::CGameObject* _pPlayer){ m_pPlayer = _pPlayer; }

	//총알 추가용
	void	SetMapObjlist(MAPOBJLIST* pMapObjlist){m_pMapObjlist = pMapObjlist;}
	void	BulletCreate(D3DXVECTOR3 _vDir);

	//아이템 추가용
	void	ItemCreate();

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
	CPlayerObserver*		m_pPlayerObserver;

private:
	Engine::VTXCOL*			m_pVertex;
	Engine::VTXCOL*			m_pConvertVertex;
	const Engine::VTXTEX*	m_pTerrainVertex;

private:
	Engine::CGameObject*	m_pPlayer;


	bool			m_bLockOn;

	DWORD			m_dwVtxCnt;
	float			m_fShotTime;

	bool			m_bOnceItem;

	bool m_bDie;
};


#endif // Bunker_h__
