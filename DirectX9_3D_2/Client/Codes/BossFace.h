#ifndef BossFace_h__
#define BossFace_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CMaterial;
	class CVIBuffer;
	class CTexture;

	class CFrame;
}

class CBossFace	: public Engine::CGameObject
{
private:
	explicit CBossFace(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossFace(void);

public:
	void	SetBoss(Engine::CGameObject* pBoss) {m_pBoss = pBoss;}
private:
	Engine::CGameObject*		m_pBoss;



public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CBossFace* Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	Engine::CTransform*			m_pTransCom;
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;

	Engine::CFrame*				m_pFrame;

private:
	LPD3DXEFFECT				m_pEffect;

private:
	_matrix						m_matOrtho;
	_float						m_fX, m_fY;
	_float						m_fSizeX, m_fSizeY;

	_bool						m_bRender;

private:
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);

};

#endif // BossFace_h__
