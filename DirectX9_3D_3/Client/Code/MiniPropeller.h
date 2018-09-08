#ifndef MiniPropeller_h__
#define MiniPropeller_h__

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
}

class CCameraObserver;

class CMiniPropeller :
	public Engine::CGameObject
{
protected:
	explicit CMiniPropeller(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CMiniPropeller(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Move(void);
	virtual HRESULT AddComponent(void);

public:
	static CMiniPropeller* Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	void TransformMatrix(void);
	void Release(void);
protected:
	Engine::CTransform*		m_pInfo;
	Engine::CTexture*		m_pTexture;
	Engine::CVIBuffer*		m_pBuffer;
protected:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CInfoSubject*	m_pInfoSubject;
	Engine::CTimeMgr*		m_pTimeMgr;
private:
	CCameraObserver*		m_pCameraObserver;
private:
	DWORD			m_dwVtxCnt;
	D3DXMATRIX*		m_pParentMatrix;
	float*			m_fHeliPosY;
	bool			m_bHeliDie;
	float			m_fTime;
	D3DXVECTOR3		m_vMoveDir;
public:
	void SetParentMatrix(D3DXMATRIX* pParentMatrix){m_pParentMatrix = pParentMatrix;}
	void SetHeliPosY(float* fHeliPosY){m_fHeliPosY = fHeliPosY;}
	void SetbHeliDie(bool bHeliDie){m_bHeliDie = bHeliDie;}

	void SetPos(D3DXVECTOR3 vPos);
protected:
	Engine::VTXCOL*		m_pVertex;
	Engine::VTXCOL*		m_pConvertVertex;
	const Engine::VTXTEX*		m_pTerrainVertex;

	float			m_fAngle;
};


#endif // MiniPropeller_h__
