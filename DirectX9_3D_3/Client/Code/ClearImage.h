/*!
 * \file ClearImage.h
 * \date 2015/08/12 15:49
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

#ifndef ClearImage_h__
#define ClearImage_h__

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

class CClearImage
	: public Engine::CGameObject
{
private:
	explicit CClearImage(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CClearImage(void);

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void);

public:
	static CClearImage* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT AddComponent(void);
	void TransformMatrix(void);
	void Release(void);

public:
	void SetImage(int iii);

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
	Engine::VTXTEX*				m_pVertex;
	Engine::VTXTEX*				m_pConvertVertex;

private:
	DWORD			m_dwVtxCnt;
	int				m_iButton;
};

#endif // ClearImage_h__