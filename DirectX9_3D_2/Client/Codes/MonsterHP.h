#ifndef MonsterHP_h__
#define MonsterHP_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;

	class CHPoint;
}

class CMonsterHP : public Engine::CGameObject
{
private:
	explicit CMonsterHP(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterHP(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);	
public:
	static CMonsterHP* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void	SetBoss(Engine::CGameObject* pBoss) {m_pBoss = pBoss;}
private:
	Engine::CGameObject*		m_pBoss;

private:
	LPD3DXEFFECT				m_pEffect;

private:
	Engine::CHPoint*			m_pHpPtCom;

private:
	_matrix						m_matOrtho;
	_float						m_fX, m_fY;
	_float						m_fSizeX, m_fSizeY;

	_float						m_fMaxHP;

	_bool						m_bRender;
private:
	Engine::CTransform*			m_pTransCom;
	Engine::CVIBuffer*			m_pHpBufferCom;
	Engine::CTexture*			m_pTextureCom;
private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);
};

#endif // MonsterHP_h__
