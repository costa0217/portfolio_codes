#ifndef Cloud_h__
#define Cloud_h__



#include "GameObject.h"

namespace Engine
{
	class CResourcesMgr;
	class CTimeMgr;
	class CInfoSubject;
	class CManagement;

	class CVIBuffer;
	class CTransform;

	class CCameraObserver;
}

class CCameraObserver;

class CCloud
	: public Engine::CGameObject
{
private:
	explicit CCloud(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCloud(void);

public:
	void SetPos(const D3DXVECTOR3& rPos);
	void SetScale(const D3DXVECTOR3& rScale);
	void SetBufferName(wchar_t*		wstrBufferName);
	void SetAngleY(float fAngle);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CCloud* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT AddComponent(void);
	void TransformMatrix(void);
	void Release(void);

private:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;
	Engine::CInfoSubject*	m_pInfoSubject;
	Engine::CManagement*	m_pManagement;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CVIBuffer*		m_pBuffer;

private:
	CCameraObserver*		m_pCameraObserver;

private:
	Engine::VTXCOL*			m_pVertex;
	Engine::VTXCOL*			m_pConvertVertex;

private:
	DWORD			m_dwVtxCnt;
	wchar_t*		m_wstrBufferName;
};

#endif // Cloud_h__
