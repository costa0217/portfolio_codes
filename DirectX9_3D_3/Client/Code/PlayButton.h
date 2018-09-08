/*!
 * \file PlayButton.h
 * \date 2015/08/04 5:17
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

#ifndef PlayButton_h__
#define PlayButton_h__

#include "GameObject.h"

namespace Engine
{
	class CResourcesMgr;
	class CTimeMgr;
	class CInfoSubject;
	class CKeyMgr;

	class CTexture;
	class CVIBuffer;
	class CTransform;
}

class CCameraObserver;
class CCollisionMgr;

class CPlayButton
	: public Engine::CGameObject
{
private:
	explicit CPlayButton(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CPlayButton(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CPlayButton* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT AddComponent(void);
	void TransformMatrix(void);
	void MouseOver(void);
	void Release(void);

public:
	bool GetBool(void);

private:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CTimeMgr*		m_pTimeMgr;
	Engine::CInfoSubject*	m_pInfoSubject;
	Engine::CKeyMgr*		m_pKeyMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CTexture*		m_pTexture;
	Engine::CVIBuffer*		m_pBuffer;

private:
	CCameraObserver*		m_pCameraObserver;
	CCollisionMgr*			m_pCollisionMgr;

private:
	Engine::VTXTEX*				m_pVertex;
	Engine::VTXTEX*				m_pConvertVertex;

private:
	DWORD			m_dwVtxCnt;
	int				m_iButton;
	bool			m_bPlay;

};

#endif // PlayButton_h__