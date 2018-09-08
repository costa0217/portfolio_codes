#ifndef Shield_h__
#define Shield_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;
}

class CShield	: public Engine::CGameObject
{
private:
	explicit CShield(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShield(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CShield* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:	
	Engine::CTransform*			m_pTransCom;
	Engine::CStaticMesh*		m_pMeshCom;
private:
	LPD3DXEFFECT				m_pEffect;
	const _matrix*				m_pParentMatrix;
	const _matrix*				m_pParentWorldMatrix;

private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);


};

#endif // Shield_h__
