/*!
 * \file SceneSelector.h
 * \date 2015/11/07 23:41
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 새로운 Scene을 생성하는 기능을 수행한다.
 *
 * TODO: long description
 *
 * \note
*/

#ifndef SceneSelector_h__
#define SceneSelector_h__

#include "StageScene.h"
#include "LogoScene.h"
#include "DyeScene.h"
#include "MiddleScene.h"
#include "Loading.h"



class CSceneSelector
{
public:
	enum SCENE {SCENE_LOGO, SCENE_DYE, SCENE_STAGE, SCENE_MIDDLESCENE, SCENE_END};
public:
	explicit CSceneSelector(SCENE eSceneID) : m_eSceneID(eSceneID) {}
	~CSceneSelector(void) {}

public:
	HRESULT operator () (Engine::CScene** ppScene, LPDIRECT3DDEVICE9 pDevice) 
	{
		switch(m_eSceneID)
		{
		case SCENE_LOGO:
			*ppScene = CLogoScene::Create(pDevice);
			break;

		case SCENE_DYE:
			*ppScene = CDyeScene::Create(pDevice);
			break;

		case SCENE_STAGE:
			*ppScene = CStageScene::Create(pDevice);
			break;

		case SCENE_MIDDLESCENE:
			*ppScene = CMiddleScene::Create(pDevice);
			break;
		}

		if(NULL == *ppScene)
			return E_FAIL;

		return S_OK;
	}
private:
	SCENE			m_eSceneID;
};


#endif // SceneSelector_h__