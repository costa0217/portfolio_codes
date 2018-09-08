/*!
 * \file Monster.h
 * \date 2015/07/15 13:48
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

#ifndef Monster_h__
#define Monster_h__


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
}

class CCameraObserver;
class CCollisionMgr;
class CMouseCol;

class CTerrainCol;

class CMonster
	: public Engine::CGameObject
{
private:
	explicit CMonster(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CMonster(void);

public:
	void SetPos(const D3DXVECTOR3& rPos);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pDevice);

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
	CTerrainCol*			m_pTerrainCol;

private:
	CCameraObserver*		m_pCameraObserver;

private:
	Engine::VTXTEX*		m_pVertex;
	Engine::VTXTEX*		m_pConvertVertex;
	const Engine::VTXTEX*		m_pTerrainVertex;

private:
	DWORD			m_dwVtxCnt;
};

#endif // Monster_h__