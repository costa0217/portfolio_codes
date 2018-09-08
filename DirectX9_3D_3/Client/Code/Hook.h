#ifndef Hook_h__
#define Hook_h__

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
}

class CCameraObserver;

class CHook:
	public Engine::CGameObject
{
protected:
	explicit CHook(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CHook(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual HRESULT AddComponent(void);

public:
	static CHook* Create(LPDIRECT3DDEVICE9 pDevice);
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
	Engine::CTransform*		m_pParentInfo;
public:
	void SetParentInfo(Engine::CTransform* pParentInfo){m_pParentInfo = pParentInfo;}
	void SetAngle(float fAngle){m_fAngle = fAngle;}
	Engine::CTransform*	 GetInfo(){return m_pInfo;}
	void SetPos(D3DXVECTOR3 vPos);
	D3DXVECTOR3 GetParentPos();
	D3DXVECTOR3	GetPos();
	void SetState(HOOK_TYPE eState){m_eState = eState;}
protected:
	Engine::VTXCOL*		m_pVertex;
	Engine::VTXCOL*		m_pConvertVertex;
	const Engine::VTXTEX*		m_pTerrainVertex;

	float			m_fAngle;
	HOOK_TYPE       m_eState;

	typedef list<CGameObject*>				OBJECTLIST;
	OBJECTLIST*		m_pFreight_HookList;

	bool m_bHang; 
public:
	bool GetbHang(){return m_bHang;}
	void SetHang(bool bHang){m_bHang = bHang;}
public:
	void SetFreight_HookList(OBJECTLIST* pFreight_HookList){m_pFreight_HookList = pFreight_HookList;}
};

#endif // Hook_h__
