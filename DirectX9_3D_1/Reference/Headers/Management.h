/*!
 * \file Management.h
 * \date 2015/11/07 1:07
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: m_pRenderer(CRenderer)을 통하여 씬을 업데이트 하고 랜더하는 class.
 *
 * \note
*/


#ifndef Management_h__
#define Management_h__

#include "Engine_Defines.h"
#include "Renderer.h"

BEGIN(Engine)

class SCene;
class CComponent;
class ENGINE_DLL CManagement
{
	DECLARE_SINGLETON(CManagement)
private:
	explicit CManagement(void);
	~CManagement(void);

public:
	Engine::CComponent*	Get_Component(const TCHAR* pComponentTag, const TCHAR* pLayerTag, const TCHAR* pObjectTag, const _uint& iCnt = 0);
	bool	Get_RendererShadowState(){ return m_pRenderer->Get_ShadowState(); }

public:
	void Add_RenderGroup(Engine::CRenderer::RENDERTYPE eType, Engine::CGameObject* pGameObject, float fViewZ);

	HRESULT Ready_Management(LPDIRECT3DDEVICE9 pGraphicDev);
	_int Update(const _float& fTimeDelta);
	void Render(const _float& fTimeDelta);

public:
	template <typename T>
	HRESULT SceneChange(T& Functor);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;

	CScene*				m_pScene;
	CRenderer*			m_pRenderer;

public:
	void Release(void);

};

template <typename T>
HRESULT Engine::CManagement::SceneChange(T& Functor)
{
	if(m_pScene != NULL)
		Engine::Safe_Release(m_pScene);

	FAILED_CHECK_RETURN(Functor(&m_pScene, m_pGraphicDev), E_FAIL);

	m_pRenderer->SetCurrentScene(m_pScene);

	m_pRenderer->Clear_RenderGroup();

	return S_OK;
}

END

#endif // Management_h__