/*!
 * \file SceneSelector.cpp
 * \date 2015/07/01 13:56
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 씬을 전환 시켜주는 함수 객체
 *
 * TODO: long description
 *
 * \note
*/
#include "Title.h"
#include "Logo.h"
#include "Stage.h"
#include "Stage2.h"


class CSceneSelector
{
public:
	explicit CSceneSelector(SCENEID eSceneID)
		: m_eSceneID(eSceneID) {};

public:
	HRESULT operator() (Engine::CScene** ppScene, LPDIRECT3DDEVICE9 pDevice)
	{
		switch(m_eSceneID)
		{
		case SCENE_TITLE:
			*ppScene = CTitle::Create(pDevice);
			break;

		case SCENE_LOGO:
			*ppScene = CLogo::Create(pDevice);
			break;

		case SCENE_STAGE:
			*ppScene = CStage::Create(pDevice);
			break;

		case SCENE_STAGE2:
			*ppScene = CStage2::Create(pDevice);
			break;
		}

		FAILED_CHECK(*ppScene);

		return S_OK;
	}

private:
	SCENEID		m_eSceneID;
};