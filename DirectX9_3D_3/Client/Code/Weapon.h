#ifndef Weapon_h__
#define Weapon_h__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class CResourcesMgr;
	class CTimeMgr;
	class CInfoSubject;
	class CManagement;

	class CVIBuffer;
	class CTransform;

	class CCameraObserver;
	class CPlayerObserver;
}
class CPlayerObserver;
class CCameraObserver;
class CCollisionMgr;

class CWeapon
	: public Engine::CGameObject
{
private:
	explicit CWeapon(LPDIRECT3DDEVICE9 pDevice);

public:
	~CWeapon(void);
public:
	HRESULT		Initialize(void);
	int			Update(void);
	void		Render(void);

public:
	static CWeapon* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT		AddComponent(void);
	void		TransformMatrix(void);
	void		Move(void);
	void		Release();

private:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;
	Engine::CInfoSubject*	m_pInfoSubject;
	Engine::CManagement*	m_pManagement;
	CCollisionMgr*			m_pCollisionMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;

private:
	CCameraObserver*		m_pCameraObserver;
	CPlayerObserver*		m_pPlayerObserver;

private:
	Engine::VTXCOL*			m_pVertex;
	Engine::VTXCOL*			m_pConvertVertex;
	const Engine::VTXTEX*	m_pTerrainVertex;

	Engine::CGameObject*	m_pPlayer;

private:
	DWORD			m_dwVtxCnt;
	float			m_fAngleZ;
	float			m_fAngleX;
	float			m_fAngleY;

	D3DXMATRIX*		m_pParentMatrix;
	D3DXMATRIX*		m_pNoRotParMatrix;		// 회전을 안먹은 부모행렬

	bool			m_bDie;
	D3DXVECTOR3		m_vMyPos;
	
	WEAPON_TYPE		m_eType;		//총 타입

	float			m_fBulletTime;
	float			m_fFireSpeed;

	//총알생성 위해
	typedef list<CGameObject*>				OBJECTLIST;
	typedef map<wstring, OBJECTLIST>		MAPOBJLIST;
	MAPOBJLIST*		m_pMapObjlist;

	//버퍼 재생성 하기 위해
	wchar_t*		m_wstrBufferName;
	wchar_t*		m_CurrentBuffer;
	

public:
	//타겟 포인터
	Engine::CGameObject*	m_pTarget;
	Engine::CGameObject*	m_pBullet;


public:
	void SetParentMatrix(D3DXMATRIX* pParentMatrix){m_pParentMatrix = pParentMatrix;}
	void SetNoRotParMatrix(D3DXMATRIX* pNoRotParMatrix){m_pNoRotParMatrix = pNoRotParMatrix;}

	//void SetTarget(Engine::CGameObject* _pTarget){m_pTarget = _pTarget;}

	void SetPos(D3DXVECTOR3 vPos);
	void SetbDie(bool bDie){m_bDie = bDie;}

public:
	void SetType(WEAPON_TYPE _eType){ m_eType = _eType; }
	void SetMapObjlist(MAPOBJLIST* pMapObjlist){m_pMapObjlist = pMapObjlist;}
	void SetPlayer(Engine::CGameObject* _pPlayer){ m_pPlayer = _pPlayer; }
	void ChangeBuffer(wchar_t* BufferName);

private:
	void BasicBulletCreate(D3DXVECTOR3& vDir);		//베이직
	void MultiBulletCreate(D3DXVECTOR3& vDir);		// 멀티
	void MissileCreate(D3DXVECTOR3& vDir);			//미사일

	void FindTarget(D3DXVECTOR3& vDir);
 
};

#endif // Weapon_h__

