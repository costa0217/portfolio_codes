#ifndef Sword_h__
#define Sword_h__

#include "Defines.h"
#include "GameObject.h"

extern SWORDINFO	g_tSwordInfo;

namespace Engine
{
	class CStaticMesh;
	class CTransform;
}

class CSword : public Engine::CGameObject
{
private:
	explicit CSword(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSword(void);

public:
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

	virtual void Render_Shadow(void);
	virtual void Render_Velocity(void);

public:
	static CSword* Create(LPDIRECT3DDEVICE9 pGraphicDev, _matrix mat, TCHAR* pMeshName);

private:	
	Engine::CStaticMesh*		m_pMeshCom;
	Engine::CTransform*			m_pTransCom;

private:
	LPD3DXEFFECT			m_pShadowEffect; //¡Ú ÀçÈÆÃß°¡ - ±×¸²ÀÚ ½¦ÀÌ´õ
	LPD3DXEFFECT			m_pEffect;
	LPD3DXEFFECT			m_pVelocityEffect;
	_matrix						m_matPreView;

	const _matrix*			m_pParentMatrix;
	const _matrix*			m_pParentWorldMatrix;

private:
	virtual HRESULT	Initialize(_matrix mat, TCHAR* pMeshName);
	virtual HRESULT Add_Component(_matrix mat, TCHAR* pMeshName);
	void Set_ContantTable(void);

public:
	virtual _ulong Release(void);


};


#endif // Sword_h__
