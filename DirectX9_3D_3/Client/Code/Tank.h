#ifndef Tank_h__
#define Tank_h__

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

	class CPlayerObserver;
	class CCameraObserver;
}

class CCameraObserver;
class CPlayerObserver;
class CCollisionMgr;
class CMouseCol;

class CTerrainCol;

enum TANKPART_TYPE { TPART_BODY, TPART_PAW, TPART_PAWSIN, TPART_END };

class CTank
	: public Engine::CGameObject
{
private:
	explicit CTank(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CTank(void);

public:
	void SetPos(const D3DXVECTOR3& rPos);
	void SetHP(float rHP);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CTank* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT AddComponent(void);
	void	TransformMatrix(void);
	void	Release(void);

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

	//ÃÑ¾Ë Ãß°¡¿ë
	void	SetMapObjlist(MAPOBJLIST* pMapObjlist){m_pMapObjlist = pMapObjlist;}
	void	BulletCreate(D3DXVECTOR3 _vDir);

public:
	void RouteMake(D3DXVECTOR3 _vPos);
	void SetDirMove(void);

	void ItemCreate(void);

private:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;
	Engine::CInfoSubject*	m_pInfoSubject;
	Engine::CManagement*	m_pManagement;
	CCollisionMgr*			m_pCollisionMgr;

private:
	Engine::CTransform*		m_pInfo[TPART_END];
	Engine::CVIBuffer*		m_pBuffer[TPART_END];
	CTerrainCol*			m_pTerrainCol;

private:
	CCameraObserver*		m_pCameraObserver;
	CPlayerObserver*		m_pPlayerObserver;

private:
	Engine::VTXCOL*			m_pVertex[TPART_END];
	Engine::VTXCOL*			m_pConvertVertex[TPART_END];
	const Engine::VTXTEX*	m_pTerrainVertex;

public:
	vector<D3DXVECTOR3>		m_vecRoute;

private:
	Engine::CGameObject*	m_pPlayer;

	bool			m_bLockOn;
	float			m_fShotTime;

private:
	DWORD					m_dwVtxCnt[TPART_END];
	float					m_fAngle[Engine::ANGLE_END];

	D3DXVECTOR3			vPoint[8];
	float				m_fSpeed;

	float				m_fMoveTime;
	int					m_iGoPoint;

	bool				m_bOnceItem;

	bool m_bDie;
};



#endif // Tank_h__
