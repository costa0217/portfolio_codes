#ifndef Trail_h__
#define Trail_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CTexture;
	class CVIBuffer;
}

class CTrail	: public Engine::CGameObject
{
private:
	explicit CTrail(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTrail(void);
public:
	const _vec3* Get_ViewPos(void) {return &m_vViewPos;}
	void Set_Player(Engine::CGameObject* pPlayer){ m_pPlayer = pPlayer; }

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CTrail* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CTransform*			m_pTransCom;
	Engine::CTexture*			m_pTextureCom;
	Engine::CVIBuffer*			m_pBuffer;

	Engine::VTXTEX*				m_pVTXInfo;

private:
	Engine::CGameObject*		m_pPlayer;

private:
	LPD3DXEFFECT				m_pEffect;
	_vec3						m_vViewPos;

private:
	const _matrix*				m_pParentMatrix;

	_vec3*						m_pvEndPos;
	_vec3*						m_pvStartPos;

	_vec3						m_vSwordEPos[20];
	_vec3						m_vSwordSPos[20];

	_float						m_fSetTime;

private:
	void Set_ContantTable(void);

	void Create_trail(const _float& fTimeDelta);
	void Set_TrailVB(void);
public:
	void Set_WeaponPos(_vec3* pvEndPos, _vec3* pvStartPos);


public:
	virtual _ulong Release(void);


};


#endif // Trail_h__

