#ifndef SkyDome_h__
#define SkyDome_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;
}

class CSkyDome	: public Engine::CGameObject
{
private:
	explicit CSkyDome(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSkyDome(void);

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CSkyDome* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CTransform*			m_pTransCom;	
	Engine::CStaticMesh*		m_pMeshCom;

private:
	LPD3DXEFFECT				m_pEffect;

private:
	void Set_ContantTable(void);

public:
	virtual _ulong Release(void);


};

#endif // SkyDome_h__
