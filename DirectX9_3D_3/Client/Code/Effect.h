/*!
 * \file Effect.h
 * \date 2015/07/21 5:25
 *
 * \author JHJ
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/

#ifndef Effect_h__
#define Effect_h__


#include "GameObject.h"

namespace Engine
{
	class CResourcesMgr;
	class CTimeMgr;
	class CInfoSubject;

	class CTexture;
	class CVIBuffer;
	class CTransform;
}

class CCameraObserver;
class CCube;

class CEffect
	: public Engine::CGameObject
{
protected:
	explicit CEffect(LPDIRECT3DDEVICE9 pDevice);
public:
	virtual ~CEffect(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);
	virtual void Move(void);
	virtual HRESULT AddComponent(void);

public:
	static CEffect* Create(LPDIRECT3DDEVICE9 pDevice);
protected:
	
	void TransformMatrix(void);
	void Release(void);
public:
	void SetPos(const D3DXVECTOR3& rPos);
	void SetPosF(D3DXVECTOR3 vPos);
	void FlameMove();
protected:
	Engine::CTransform*		m_pInfo;
	Engine::CTexture*		m_pTexture;
	Engine::CVIBuffer*		m_pBuffer;
protected:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CInfoSubject*	m_pInfoSubject;
	Engine::CTimeMgr*		m_pTimeMgr;

protected:
	CCameraObserver*		m_pCameraObserver;

protected:
	Engine::VTXTEX*		m_pVertex;
	Engine::VTXTEX*		m_pConvertVertex;

public:
	Engine::VTXTEX*	 GetpVertex(){return m_pVertex;}
	void SetpConvertVertex(Engine::VTXTEX* pConvertVertex){m_pConvertVertex = pConvertVertex;}

protected:
	float			m_fAngleZ;
	DWORD			m_dwVtxCnt;
	float			m_fSpeed;

	float			m_fFrameCnt;
	float			m_fFrameMax;

	float			m_fDieTime;
public:
	void SetAngleZ(float AngleZ){m_fAngleZ = AngleZ;}

};


#endif // Effect_h__