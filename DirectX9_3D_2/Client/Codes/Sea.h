#ifndef Sea_h__
#define Sea_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CMaterial;
	class CVIBuffer;
	class CTexture;
}

class CSea	: public Engine::CGameObject
{
private:
	explicit CSea(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSea(void);

public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

public:
	static CSea* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	Engine::CTransform*			m_pTransCom;
	Engine::CMaterial*			m_pMtrlCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;

private:
	LPD3DXEFFECT				m_pEffect;

private:
	_float						m_fTimeDelta;
	_float						m_fAccTime;

private:
	void Set_ContantTable(void);

public:
	virtual _ulong Release(void);

};

#endif // Sea_h__
