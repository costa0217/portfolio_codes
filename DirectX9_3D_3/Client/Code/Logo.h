/*!
 * \file Logo.h
 * \date 2015/07/01 13:54
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

#ifndef Logo_h__
#define Logo_h__

#include "Scene.h"
#include "VIBuffer.h"

namespace Engine
{
	class CResourcesMgr;
	class CGameObject;
	class CInfoSubject;
}


class CCollisionMgr;

class CLogo
	: public Engine::CScene
{
//private:
//	enum LAYERID {LAYER_GAMELOGIC, LAYER_RAIN, LAYER_UI, LAYER_ENVIRONMENT};

private:
	explicit CLogo(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CLogo(void);

public:
	virtual HRESULT InitScene(void);
	virtual void Update(void);
	virtual void Render(void);

public:
	static CLogo* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	HRESULT Add_GameLogic_Layer(void);
	HRESULT Add_Camera_Layer(void);
	void Release(void);

private:
	Engine::CResourcesMgr*		m_pResourceMgr;
	Engine::CGameObject*		m_playbutton;

};

#endif // Logo_h__