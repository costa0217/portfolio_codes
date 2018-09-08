#ifndef Stage2_h__
#define Stage2_h__

#include "Scene.h"

namespace Engine
{
	class CResourcesMgr;
	class CGameObject;
}

class CCollisionMgr;

class CStage2
	: public Engine::CScene
{
	//public:
	//	enum LAYERID {LAYER_ENVIRONMENT, LAYER_GAMELOGIC, LAYER_UI};

private:
	explicit CStage2(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CStage2(void);

public:
	virtual HRESULT InitScene(void);
	virtual void Update(void);
	virtual void Render(void);

public:
	static CStage2* Create(LPDIRECT3DDEVICE9 pDevice);

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


#endif // Stage2_h__
