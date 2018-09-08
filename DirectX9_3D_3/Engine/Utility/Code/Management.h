/*!
 * \file Management.h
 * \date 2015/07/01 13:04
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

#ifndef Management_h__
#define Management_h__

#include "Engine_Include.h"
#include "Renderer.h"

BEGIN(Engine)

class CScene;

class ENGINE_DLL CManagement
{
public:
	DECLARE_SINGLETON(CManagement)

private:
	CManagement(void);
	~CManagement(void);

public:
	const VTXTEX* GetTerrainVtx(const WORD& LayerID, const wstring& wstrObjKey);

public:
	HRESULT InitManagement(LPDIRECT3DDEVICE9 pDevice);
	void Update(void);
	void Render(float fTime);

public:
	template <typename T>
	HRESULT SceneChange(T& Scene);

private:
	void Release(void);

private:
	CScene*			m_pScene;
	CRenderer*		m_pRenderer;

private:
	LPDIRECT3DDEVICE9		m_pDevice;
public:
	CRenderer* GetRenderer(){return m_pRenderer;}
};

template <typename T>
HRESULT Engine::CManagement::SceneChange(T& SceneSelector)
{
	if(m_pScene != NULL)
		Engine::Safe_Delete(m_pScene);

	FAILED_CHECK(SceneSelector(&m_pScene, m_pDevice));

	m_pRenderer->SetSceneChange(m_pScene);
	return S_OK;
}

END

#endif // Management_h__