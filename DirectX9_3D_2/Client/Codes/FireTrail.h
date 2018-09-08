#ifndef FireTrail_h__
#define FireTrail_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CQuaternion;
	class CStaticMesh;

	class CFrame;
}

class CFireTrail	: public Engine::CGameObject
{
private:
	explicit CFireTrail(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFireTrail(void);

public:
	virtual HRESULT	Initialize(_vec3 vPos, _vec3 vDir);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CFireTrail* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vDir);

private:	
	Engine::CFrame*				m_pFrame;

	Engine::CQuaternion*		m_pTransCom;
	Engine::CStaticMesh*		m_pMeshCom;
private:
	LPD3DXEFFECT				m_pEffect;

private:
	_bool						m_bSwitch;

	_vec3						m_vViewPos;


private:
	_float						m_fAccTime;
	_float						m_fAlpha;

	_bool						m_bSound;


private:
	void	Set_ContantTable(void);

public:
	virtual _ulong Release(void);

};
#endif // FireTrail_h__
