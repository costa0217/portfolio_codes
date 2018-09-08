#ifndef Barrel_h__
#define Barrel_h__

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

class CBarrel
	: public Engine::CGameObject
{
protected:
	explicit CBarrel(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CBarrel(void);
public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CBarrel* Create(LPDIRECT3DDEVICE9 pDevice);

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
	CPlayerObserver*		m_pPlayerObserver;

protected:
	Engine::VTXCOL*		m_pVertex;
	Engine::VTXCOL*		m_pConvertVertex;
	const Engine::VTXTEX*		m_pTerrainVertex;
protected:
	float			m_fSpeed;
	DWORD			m_dwVtxCnt;
	float			m_fAngleZ;
	float			m_fAngleX;
	float			m_fAngleY;

	float			m_fDestAngleZ;
	float			m_fDestAngleX;
	float			m_fDestAngleY;

	D3DXMATRIX*		m_pParentMatrix;
	D3DXMATRIX*		m_pNoRotParMatrix;		// 회전을 안먹은 부모행렬

	bool			m_bDie;

public:
	//타겟 포인터
	Engine::CGameObject*	m_pTarget;

public:
	void SetfDestAngleX(float fAngleX){m_fDestAngleX = fAngleX;}
	void SetfDestAngleZ(float fAngleZ){m_fDestAngleZ = fAngleZ;}
	void SetfDestAngleY(float fAngleY){m_fDestAngleY = fAngleY;}

	void SetParentMatrix(D3DXMATRIX* pParentMatrix){m_pParentMatrix = pParentMatrix;}
	void SetNoRotParMatrix(D3DXMATRIX* pNoRotParMatrix){m_pNoRotParMatrix = pNoRotParMatrix;}
	Engine::VTXCOL* GetpVertex(){return m_pVertex;}

	void SetTarget(Engine::CGameObject* _pTarget){m_pTarget = _pTarget;}

	void SetPos(D3DXVECTOR3 vPos);
	void SetbDie(bool bDie){m_bDie = bDie;}
};

#endif // Barrel_h__
