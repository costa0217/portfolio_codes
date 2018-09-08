/*!
 * \file Water.h
 * \date 2015/07/27 23:09
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
#ifndef Water_h__
#define Water_h__

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

class CWater
	: public Engine::CGameObject
{
private:
	explicit CWater(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CWater(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CWater* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT AddComponent(void);
	void TransformMatrix(void);
	//void KeyCheck(void);
	void Release(void);

public:
	void SetPos(const D3DXVECTOR3& rPos);
	void SetMoveDir(bool byunsoo);

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
	Engine::VTXTEX*		m_pVertex;
	Engine::VTXTEX*		m_pConvertVertex;

private:
	DWORD		m_dwVtxCnt;
	float		m_fFrameCnt;
	float		m_fFrameMax;
	bool		m_bDir;
	int			dir;
	float		f;
	DWORD		m_fTime;

};

#endif // Water_h__

