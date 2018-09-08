/*!
 * \file CargoUI.h
 * \date 2015/08/12 17:20
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef CargoUI_h__
#define CargoUI_h__

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

class CCargoUI
	: public Engine::CGameObject
{
private:
	explicit CCargoUI(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CCargoUI(void);

public:
	void SetPos(const D3DXVECTOR3& rPos);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CCargoUI* Create(LPDIRECT3DDEVICE9 pDevice);

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
	TCHAR	m_szMission[128];
	int		m_Mission;
	int		m_Clear;

private:
	DWORD			m_dwVtxCnt;
};

#endif // CargoUI_h__