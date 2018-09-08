#ifndef BossTrail2_h__
#define BossTrail2_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CTexture;
	class CVIBuffer;

	class CFrame;
}

class CBossTrail2	: public Engine::CGameObject
{
private:
	explicit CBossTrail2(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossTrail2(void);
public:
	const _vec3* Get_ViewPos(void) {return &m_vViewPos;}
	void Set_Boss(Engine::CGameObject* pBoss){ m_pBoss = pBoss; }

public:
	virtual		HRESULT	Initialize(void);
	virtual		HRESULT Add_Component(void);
	virtual		_int Update(const _float& fTimeDelta);
	virtual		void Render(void);

public:
	static CBossTrail2* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CTransform*			m_pTransCom;
	Engine::CTexture*			m_pTextureCom;
	Engine::CVIBuffer*			m_pBuffer;
	Engine::CFrame*				m_pFrame;

	Engine::VTXTEX*				m_pVTXInfo;

private:
	Engine::CGameObject*		m_pBoss;

private:
	LPD3DXEFFECT				m_pEffect;
	_vec3						m_vViewPos;

private:
	const _matrix*				m_pParentMatrix;

	_vec3*						m_pvEndPos;
	_vec3*						m_pvStartPos;

	_vec3						m_vEPos[20];
	_vec3						m_vSPos[20];

	_float						m_fSetTime;

private:
	void		Set_ContantTable(void);

	void		Create_trail(const _float& fTimeDelta);
	void		Set_TrailVB(void);
public:
	void		Set_StartPos(_vec3* pvEndPos, _vec3* pvStartPos);
	
public:
	virtual _ulong Release(void);
};


#endif // BossTrail2_h__
