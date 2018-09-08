#ifndef AirGun_h__
#define AirGun_h__

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

class CCameraObserver;
class CPlayerObserver;
class CCollisionMgr;
class CMouseCol;

class CTerrainCol;

enum AIRPART_TYPE { APART_BODY, APART_HEAD, APART_PAW, APART_END };

class CAirGun
	: public Engine::CGameObject
{
private:
	explicit CAirGun(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CAirGun(void);

public:
	void SetPos(const D3DXVECTOR3& rPos);
	void SetHP(float rHP);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CAirGun* Create(LPDIRECT3DDEVICE9 pDevice);

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

	//ÃÑ¾Ë Ãß°¡¿ë
	void	SetMapObjlist(MAPOBJLIST* pMapObjlist){m_pMapObjlist = pMapObjlist;}
	void	BulletCreate(D3DXVECTOR3 _vDir);


private:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;
	Engine::CInfoSubject*	m_pInfoSubject;
	Engine::CManagement*	m_pManagement;
	CCollisionMgr*			m_pCollisionMgr;

private:
	Engine::CTransform*		m_pInfo[APART_END];
	Engine::CVIBuffer*		m_pBuffer[APART_END];
	CTerrainCol*			m_pTerrainCol;

private:
	CCameraObserver*		m_pCameraObserver;
	CPlayerObserver*		m_pPlayerObserver;

private:
	Engine::VTXCOL*			m_pVertex[APART_END];
	Engine::VTXCOL*			m_pConvertVertex[APART_END];
	const Engine::VTXTEX*	m_pTerrainVertex;

private:
	Engine::CGameObject*	m_pPlayer;

	bool			m_bLockOn;
	float			m_fShotTime;
	bool m_bDie;

private:
	DWORD			m_dwVtxCnt[APART_END];
	float			m_fAngle[Engine::ANGLE_END];
};


#endif // AirGun_h__
