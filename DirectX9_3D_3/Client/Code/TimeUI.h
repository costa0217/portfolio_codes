/*!
 * \file TimeUI.h
 * \date 2015/07/31 2:55
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

#ifndef TimeUI_h__
#define TimeUI_h__

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
class CCollisionMgr;

class CTimeUI
	: public Engine::CGameObject
{
private:
	explicit CTimeUI(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CTimeUI(void);

public:
	void SetPos(const D3DXVECTOR3& rPos);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CTimeUI* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT AddComponent(void);
	void TransformMatrix(void);
	void Release(void);
	void CntTime(void);

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

private:
	Engine::VTXTEX*				m_pVertex;
	Engine::VTXTEX*				m_pConvertVertex;
	const Engine::VTXTEX*		m_pTerrainVertex;

private:
	TCHAR	m_szTime[128];
	float	m_Min;
	float	m_Sec;

private:
	DWORD			m_dwVtxCnt;
};

#endif // TimeUI_h__