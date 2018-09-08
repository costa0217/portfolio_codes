/*!
 * \file ExitButton.h
 * \date 2015/08/02 22:50
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

#ifndef ExitButton_h__
#define ExitButton_h__

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

class CExitButton
	: public Engine::CGameObject
{
private:
	explicit CExitButton(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CExitButton(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CExitButton* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT AddComponent(void);
	void TransformMatrix(void);
	void MouseOver(void);
	void Release(void);

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

};

#endif // ExitButton_h__