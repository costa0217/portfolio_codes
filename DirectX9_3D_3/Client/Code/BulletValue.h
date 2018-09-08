/*!
 * \file BulletValue.h
 * \date 2015/07/31 3:58
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

#ifndef BulletValue_h__
#define BulletValue_h__

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

class CBulletValue
	: public Engine::CGameObject
{
private:
	explicit CBulletValue(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CBulletValue(void);

public:
	void SetPos(const D3DXVECTOR3& rPos);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CBulletValue* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT AddComponent(void);
	void TransformMatrix(void);
	void Release(void);
	void GetPlayerInfo(void);

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
	float			m_fBulletvalue;
	float			pPlayerBullet;
	float			m_fMax;
	float			m_fUse;
};

#endif // BulletValue_h__