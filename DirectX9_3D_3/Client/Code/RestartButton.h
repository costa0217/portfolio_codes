/*!
 * \file RestartButton.h
 * \date 2015/08/06 12:25
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

#ifndef RestartButton_h__
#define RestartButton_h__

#include "GameObject.h"

namespace Engine
{
	class CResourcesMgr;
	class CInfoSubject;
	class CKeyMgr;

	class CTexture;
	class CVIBuffer;
	class CTransform;
}

class CCameraObserver;

class CRestartButton
	: public Engine::CGameObject
{
private:
	explicit CRestartButton(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CRestartButton(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CRestartButton* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT AddComponent(void);
	void TransformMatrix(void);
	void MouseOver(void);
	void Release(void);

public:
	bool GetBool(void);

private:
	Engine::CResourcesMgr*	m_pResourceMgr;
	Engine::CInfoSubject*	m_pInfoSubject;
	Engine::CKeyMgr*		m_pKeyMgr;

private:
	Engine::CTransform*		m_pInfo;
	Engine::CTexture*		m_pTexture;
	Engine::CVIBuffer*		m_pBuffer;

private:
	CCameraObserver*		m_pCameraObserver;

private:
	Engine::VTXTEX*			m_pVertex;
	Engine::VTXTEX*			m_pConvertVertex;

private:
	DWORD			m_dwVtxCnt;
	int				m_iButton;
	bool			m_Restart;

};

#endif // RestartButton_h__