#ifndef Title_h__
#define Title_h__

#include "Scene.h"
#include "VIBuffer.h"

namespace Engine
{
	class CResourcesMgr;
	class CGameObject;
	class CInfoSubject;
}


class CCollisionMgr;

class CTitle
	: public Engine::CScene
{
private:
	enum LAYERID {LAYER_GAMELOGIC, LAYER_RAIN, LAYER_UI, LAYER_ENVIRONMENT};


private:
	explicit CTitle(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CTitle(void);

public:
	virtual HRESULT InitScene(void);
	virtual void Update(void);
	virtual void Render(void);

public:
	static CTitle* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_Camera_Layer(void);
	void	Release(void);

private:
	Engine::CResourcesMgr*		m_pResourceMgr;

private:
	HANDLE				m_hThread;
	CRITICAL_SECTION	m_Crt;
	TCHAR				m_szNum[64];

public:
	CRITICAL_SECTION GetCrt(void);

};
#endif // Title_h__
