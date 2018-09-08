#ifndef MiddleScene_h__
#define MiddleScene_h__

#include "Defines.h"
#include "Scene.h"

class CMiddleScene : public Engine::CScene
{
private:
	explicit CMiddleScene(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMiddleScene(void);

public:
	virtual HRESULT Init_Scene(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CMiddleScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	static vector<TCHAR*> vecObjName; // ¤º¤²¤·

	_bool	m_bLoad;

private:
	HRESULT Ready_Lighting(void);
	HRESULT Ready_Environment(void);
	HRESULT Ready_GameLogic_0(void);
	HRESULT Ready_GameLogic(void);
	HRESULT Ready_Effect(void);

private:
	void Load_EnvEffect();

public:
	virtual _ulong Release(void);
};
#endif // MiddleScene_h__
