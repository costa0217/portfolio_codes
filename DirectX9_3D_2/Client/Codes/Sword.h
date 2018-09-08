#ifndef Sword_h__
#define Sword_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CStaticMesh;
}

class CSword	: public Engine::CGameObject
{
private:
	explicit CSword(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSword(void);

public:
	_vec3*	Get_SwordEndPos(){ return &m_vSwordEndPos; }

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CSword* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:	
	Engine::CTransform*			m_pTransCom;
	Engine::CStaticMesh*		m_pMeshCom;

	Engine::CTransform*			m_pPlayerTransCom;
private:
	LPD3DXEFFECT				m_pEffect;
	const _matrix*				m_pParentMatrix;
	const _matrix*				m_pParentWorldMatrix;

	_vec3						m_vSwordEndPos;

private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);


};


#endif // Sword_h__
