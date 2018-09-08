/*!
 * \file Stage.h
 * \date 2015/07/02 12:29
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

#ifndef Stage_h__
#define Stage_h__

#include "Scene.h"

namespace Engine
{
	class CResourcesMgr;
	class CGameObject;
}

class CCollisionMgr;

class CStage
	: public Engine::CScene
{
//public:
//	enum LAYERID {LAYER_ENVIRONMENT, LAYER_GAMELOGIC, LAYER_UI};

private:
	explicit CStage(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CStage(void);

public:
	virtual HRESULT InitScene(void);
	virtual void Update(void);
	virtual void Render(void);

public:
	static CStage* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Add_Pause_Layer(void);
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_Camera_Layer(void);
	HRESULT Add_Choice_Layer(void);
	HRESULT Add_UI_Layer(void);
	HRESULT Add_Rain_Layer(void);

	void SetUp(void);
	void Release(void);

private:
	Engine::CResourcesMgr*		m_pResourceMgr;
	CCollisionMgr*				m_pCollisionMgr;

	Engine::CGameObject*		m_GoMainButton;
	Engine::CGameObject*		m_RestartButton;
	Engine::CGameObject*		m_StartButton;

public:
	bool	bGameStart;

	Engine::CLayer*			pLayer;
	Engine::CGameObject*	pGameObject;

	Engine::CLayer*			m_pUILayer;
	bool					m_bInputUI;

	bool					m_bbb;
};

#endif // Stage_h__