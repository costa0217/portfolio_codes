/*!
 * \file LimitUI.h
 * \date 2015/07/31 3:13
 *
 * \author ARONG
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef LimitUI_h__
#define LimitUI_h__

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

class CPlayerObserver;
class CCameraObserver;
class CCollisionMgr;

class CLimitUI
	: public Engine::CGameObject
{
private:
	explicit CLimitUI(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CLimitUI(void);

public:
	void SetPos(const D3DXVECTOR3& rPos);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CLimitUI* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT AddComponent(void);
	void TransformMatrix(void);
	void Release(void);

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

private:
	CCameraObserver*		m_pCameraObserver;
	CPlayerObserver*		m_pPlayerObserver;

private:
	Engine::VTXTEX*				m_pVertex;
	Engine::VTXTEX*				m_pConvertVertex;
	const Engine::VTXTEX*		m_pTerrainVertex;

private:
	TCHAR	m_szLimit[128];
	int		m_Limit;
	int		m_Ride;

private:
	DWORD			m_dwVtxCnt;
};


#endif // LimitUI_h__