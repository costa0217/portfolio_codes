/*!
 * \file Renderer.cpp
 * \date 2015/07/01 12:31
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
#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CScene;
class ENGINE_DLL CRenderer
{
private:
	explicit CRenderer(LPDIRECT3DDEVICE9 pDevice);

public:
	~CRenderer(void);

public:
	void SetSceneChange(CScene* pScene);

public:
	HRESULT InitScene(void);
	void Render(float fTime);

public:
	static CRenderer* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	CScene*		m_pScene;

private:
	LPDIRECT3DDEVICE9	m_pDevice;

private:
	ID3DXFont*		m_pD3DXFont;
	float			m_fTime;
	int				m_iFrameCnt;
	TCHAR			m_szFps[128];
	TCHAR			m_szCnt[128];
public:
	ID3DXFont* GetFont(){return m_pD3DXFont;}
};

END

#endif // Renderer_h__