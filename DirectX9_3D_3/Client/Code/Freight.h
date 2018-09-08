#ifndef Freight_h__
#define Freight_h__

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

class CHook;
class CCameraObserver;
class CFreight :
	public Engine::CGameObject
{
protected:
	explicit CFreight(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CFreight(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual HRESULT AddComponent(void);

public:
	static CFreight* Create(LPDIRECT3DDEVICE9 pDevice);
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
	//юс╫ц
	TCHAR			m_szFps[128];
	//

	CHook* m_pHook;
	bool m_bArrive;
private:
	DWORD			m_dwVtxCnt;
	Engine::CTransform*		m_pParentInfo;
public:
	void SetParentInfo(Engine::CTransform* pParentInfo){m_pParentInfo = pParentInfo;}
	void SetpHook(CHook* pHook){m_pHook = pHook;}
protected:
	Engine::VTXCUBE*		m_pVertex;
	Engine::VTXCUBE*		m_pConvertVertex;
	const Engine::VTXTEX*		m_pTerrainVertex;

};


#endif // Freight_h__
