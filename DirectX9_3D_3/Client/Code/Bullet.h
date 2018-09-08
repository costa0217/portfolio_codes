#ifndef Bullet_h__
#define Bullet_h__

#include "GameObject.h"
#include "Include.h"

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
class CBulletCol;

class CBullet
	: public Engine::CGameObject
{
protected:
	explicit CBullet(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CBullet(void);
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
public:
	static CBullet* Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	HRESULT AddComponent(void);
	void TransformMatrix(void);
	void Move(void);
	void Release();
protected:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;
	Engine::CInfoSubject*	m_pInfoSubject;
	Engine::CManagement*	m_pManagement;
	CCollisionMgr*			m_pCollisionMgr;
protected:
	Engine::CTransform*		m_pInfo;
	Engine::CTexture*		m_pTexture;
	Engine::CVIBuffer*		m_pBuffer;
protected:
	CCameraObserver*		m_pCameraObserver;
protected:
	Engine::VTXCOL*		m_pVertex;
	Engine::VTXCOL*		m_pConvertVertex;
	const Engine::VTXTEX*		m_pTerrainVertex;
protected:
	float			m_fSpeed;
	float			m_fAccel;

	DWORD			m_dwVtxCnt;
	float			m_fAngleZ;
	float			m_fAngleY;
	float			m_fAngleX;
	D3DXVECTOR3		m_vDir;
	
	//충돌변수
	CBulletCol*			m_pBulletCol;

	typedef list<Engine::CGameObject*>		OBJECTLIST;
	OBJECTLIST*			m_pMonsterList;

	//이름
	BUL_TYPE		m_eBulletName;
	//데미지
	float			m_fDamage;


public:
	void SetfAngleX(float fAngleX){m_fAngleX = fAngleX;}
	void SetfAngleZ(float fAngleZ){m_fAngleZ = fAngleZ;}
	void SetDir(D3DXVECTOR3 vDir){m_vDir = vDir;}
	void SetPos(D3DXVECTOR3 vPos);
	Engine::VTXCOL* GetpVertex(){return m_pVertex;}

	void SetList(OBJECTLIST* pMonsterList){ m_pMonsterList = pMonsterList;}
};

#endif // Bullet_h__
