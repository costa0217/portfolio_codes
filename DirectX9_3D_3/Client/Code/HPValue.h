/*!
 * \file HPValue.h
 * \date 2015/07/31 4:01
 *
 * \author ARONG
 * Contact: user@company.com
 *
 * \brief \
 *
 * TODO: long description
 *
 * \note
*/

#ifndef HPValue_h__
#define HPValue_h__

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
class CPlayerObserver;
class CCollisionMgr;

class CHPValue
	: public Engine::CGameObject
{
private:
	explicit CHPValue(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CHPValue(void);

public:
	void SetPos(const D3DXVECTOR3& rPos);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	void GetPlayerInfo(void);

public:
	static CHPValue* Create(LPDIRECT3DDEVICE9 pDevice);

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
	DWORD			m_dwVtxCnt;
	float			m_fHpvalue;
	float			pPlayerHP;
	float			m_fMax;
	float			m_fDamage;
};

#endif // HPValue_h__