/*!
 * \file DyeScene.h
 * \date 2015/12/31 21:20
 *
 * \author yoon.a.y
 * Contact: yoon_ay@naver.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef DyeScene_h__
#define DyeScene_h__

#include "Defines.h"
#include "Scene.h"

class CLoading;
class CDyeScene : public Engine::CScene
{
private:
	explicit CDyeScene(LPDIRECT3DDEVICE9 pGrpahicDev);
	virtual ~CDyeScene(void);
public:
	virtual HRESULT Init_Scene(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CDyeScene* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	HRESULT Ready_Lighting(void);
	HRESULT Ready_Resources(void);
	HRESULT Ready_GameLogic(void);
	HRESULT Ready_Environment(void);
	HRESULT Ready_Effect(void);

public:
	_float			m_fAccTime;

public:
	virtual _ulong Release(void);
};



#endif // DyeScene_h__