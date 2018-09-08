#include "stdafx.h"
#include "LogoScene.h"

#include "Export_Function.h"
#include "SceneSelector.h"
#include "Loading.h"
#include "LogoBack.h"

#include "SoundMgr.h"
#include "FadeInOut.h"

CLogoScene::CLogoScene(LPDIRECT3DDEVICE9 pGrpahicDev)
: CScene(pGrpahicDev)
{
	Gausianpow	= 10.0f;	//수치가 높아지면 뿌여진다. 0 ~ 1
	fExposure	= 10.0f; //수치가 높아지면 밝아진다. 0 ~ 1
}

CLogoScene::~CLogoScene(void)
{

}

HRESULT CLogoScene::Init_Scene(void)
{
	/*CSoundMgr::GetInstance()->StopBGM();
	CSoundMgr::GetInstance()->EnvironmentStopEFF();*/

	Engine::CSceneNumMgr::GetInstance()->Set_CurrentScene(1);	// 로고
	CFadeInOut::GetInstance()->SetFadeInOut(false, 1.5f);

	if(FAILED(Ready_Lighting()))
	{
		MSG_BOX("Ready_Lighting Failed");
		return E_FAIL;
	}

	if(FAILED(Ready_Resources()))
	{
		MSG_BOX("Ready_Resources Failed");
		return E_FAIL;
	}

	if(FAILED(Ready_Environment()))
	{
		MSG_BOX("Ready_Environment Failed");
		return E_FAIL;
	}

	return S_OK;
}

_int CLogoScene::Update(const _float& fTimeDelta)
{
	CScene::Update(fTimeDelta);	

	CFadeInOut::GetInstance()->Update(fTimeDelta);//가장 마지막에 업데이트해야한다.

	if(true == m_pLoading->GetComplete())
	{
		if(GetAsyncKeyState(VK_RETURN) & 0x8000)		
		{
			if(CLoading::m_eLoadingType == CLoading::LOADING_DYE)
			{
				if(FAILED(Engine::Change_Scene(CSceneSelector(CSceneSelector::SCENE_DYE))))
				{
					MSG_BOX("Change_Scene(SCENE_DYE) Failed");
					return E_FAIL;
				}
			}
			else if(CLoading::m_eLoadingType == CLoading::LOADING_MIDDLE_STAGE)
			{
				if(FAILED(Engine::Change_Scene(CSceneSelector(CSceneSelector::SCENE_MIDDLESCENE))))
				{
					MSG_BOX("Change_Scene(SCENE_MIDDLESCENE) Failed");
					return E_FAIL;
				}
			}
			else if(CLoading::m_eLoadingType == CLoading::LOADING_STAGE)
			{
				if(FAILED(Engine::Change_Scene(CSceneSelector(CSceneSelector::SCENE_STAGE))))
				{
					MSG_BOX("Change_Scene(SCENE_STAGE) Failed");
					return E_FAIL;
				}
			}

			return 0;
		}
	}

	return  0;
}

void CLogoScene::Render(void)
{
	Engine::Render_Font(L"Font_Default", _vec3(800.f, 860.f, 0.f), m_pLoading->GetLoadingMessage()
		, D3DXCOLOR(1.f, 0.f, 0.f, 1.f));
}

HRESULT CLogoScene::Ready_Lighting(void)
{
	D3DLIGHT9		LightInfo;
	ZeroMemory(&LightInfo, sizeof(D3DLIGHT9));

	LightInfo.Type = D3DLIGHT_DIRECTIONAL;
	LightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	LightInfo.Direction = _vec3(0.f, 0.f, 1.f);

	if(FAILED(Engine::Ready_Light(m_pGraphicDev, &LightInfo, 0)))
		return E_FAIL;	

	return S_OK;
}

HRESULT CLogoScene::Ready_Resources(void)
{
	//로딩창에서 사용할 리소스를 미리 로드한다.
	m_pLoading = CLoading::Create(CLoading::m_eLoadingType);
	if(NULL == m_pLoading)
	{
		MSG_BOX("CLoading Create Failed");
		return E_FAIL;
	}

	// For.Buffers ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_LOGO, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_RcTexFront")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return E_FAIL;
	}


	if(FAILED(Engine::Ready_Buffer(m_pGraphicDev, RESOURCE_LOGO, Engine::CVIBuffer::TYPE_RCTEX, L"Buffer_RcTexBack")))
	{
		MSG_BOX("Ready_Buffer Failed");
		return E_FAIL;
	}
	// For.Texture ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
	if(CLoading::m_eLoadingType == CLoading::LOADING_DYE)
	{
		if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, Engine::CTexture::TYPE_NORMAL, L"Texture_Logo", L"../Bin/Resources/Texture/LogoScene/LogoThree%d.jpg", 2)))
		{
			MSG_BOX("Ready_Buffer Failed");
			return E_FAIL;
		}
	}
	else if(CLoading::m_eLoadingType == CLoading::LOADING_STAGE)
	{
		if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, Engine::CTexture::TYPE_NORMAL, L"Texture_Logo", L"../Bin/Resources/Texture/LogoScene/LogoTwo%d.jpg", 2)))
		{
			MSG_BOX("Ready_Buffer Failed");
			return E_FAIL;
		}
	}
	else if(CLoading::m_eLoadingType == CLoading::LOADING_MIDDLE_STAGE)
	{
		if(FAILED(Engine::Ready_Texture(m_pGraphicDev, RESOURCE_LOGO, Engine::CTexture::TYPE_NORMAL, L"Texture_Logo", L"../Bin/Resources/Texture/LogoScene/LogoOne%d.jpg", 2)))
		{
			MSG_BOX("Ready_Buffer Failed");
			return E_FAIL;
		}
	}

	return S_OK;
}

HRESULT CLogoScene::Ready_Environment(void)
{
	Engine::CLayer*			pLayer = Engine::CLayer::Create(m_pGraphicDev);

	Engine::CGameObject*		pGameObject = NULL;

	// For.CLogoBack Instance
	pGameObject = CLogoBack::Create(m_pGraphicDev);
	if(NULL == pGameObject)
		return E_FAIL;
	pLayer->Ready_Object(L"LogoBack", pGameObject);

	m_mapLayer.insert(MAPLAYER::value_type(L"Layer_Environment", pLayer));

	return S_OK;
}

CLogoScene* CLogoScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogoScene*	pScene = new CLogoScene(pGraphicDev);

	if(FAILED(pScene->Init_Scene()))
	{
		MSG_BOX("pScene Initialized Failed");
		::Safe_Release(pScene);
	}

	return pScene;
}

_ulong CLogoScene::Release(void)
{
	_ulong dwRefCnt = Engine::CScene::Release();

	if(0 == dwRefCnt)
	{
		printf("CLogoScene Delete\n");
		::Safe_Release(m_pLoading);
		Engine::Release_Light();
		Engine::Reset_Resource(RESOURCE_LOGO);
		delete this;
	}

	return dwRefCnt;
}