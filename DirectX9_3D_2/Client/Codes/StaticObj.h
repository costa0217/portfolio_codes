#ifndef StaticObj_h__
#define StaticObj_h__

#include "GameObject.h"

namespace Engine
{
	class CStaticMesh;
	class CResourcesMgr;
	class CTransform;
	class CMaterial;
	class CNaviMesh;

	class CFrustum;
}

class CStaticObj
	: public Engine::CGameObject
{
private:
	explicit CStaticObj(LPDIRECT3DDEVICE9 pGraphicDev, TCHAR* pwstrMeshName);
public:
	virtual ~CStaticObj(void);

public:
	void	SetPos(D3DXVECTOR3 vMousePos);
	void	SetScale(D3DXVECTOR3 vScale);
	void	SetRotation(D3DXVECTOR3 vRotation);
public:
	virtual HRESULT Initialize(void);
	virtual _int	Update(const _float& fTimeDelta);
	virtual void	Render(void);

public:
	void	KeyCheck();

public:
	static CStaticObj*	Create(LPDIRECT3DDEVICE9 pGraphicDev, TCHAR* pwstrMeshName);

private:
	HRESULT		AddComponent(void);


private:	
	Engine::CTransform*			m_pTransCom;
	Engine::CStaticMesh*		m_pMeshCom;

	Engine::CFrustum*			m_pFrustum;

private:
	LPD3DXEFFECT				m_pEffect;

private:
	_vec3*						m_pvMyMin;
	_vec3*						m_pvMyMax;
	_float						m_fRadian;

private:
	void		Set_ContantTable(void);

public:
	TCHAR						m_wstrMeshName[128];

public:
	virtual _ulong Release(void);

};

#endif // StaticObj_h__
