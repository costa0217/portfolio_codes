#ifndef MonsterBar_h__
#define MonsterBar_h__


#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CMonsterBar : public Engine::CGameObject
{
private:
	explicit CMonsterBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CMonsterBar(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);	
public:
	static CMonsterBar* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	void	SetBoss(Engine::CGameObject* pBoss) {m_pBoss = pBoss;}
private:
	Engine::CGameObject*		m_pBoss;

private:
	LPD3DXEFFECT				m_pEffect;

private:
	_matrix						m_matOrtho;
	_float						m_fX, m_fY;
	_float						m_fSizeX, m_fSizeY;
private:
	Engine::CTransform*			m_pTransCom;
	//Engine::CVIBuffer*			m_pHpBufferCom;
	Engine::CVIBuffer*			m_pHpBarBufferCom;
	Engine::CTexture*			m_pTextureCom;
private:
	_bool						m_bRender;

private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);
};
#endif // MonsterBar_h__
