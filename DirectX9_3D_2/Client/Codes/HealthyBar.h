#ifndef HealthyBar_h__
#define HealthyBar_h__


#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
}

class CHealthyBar : public Engine::CGameObject
{
private:
	explicit CHealthyBar(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CHealthyBar(void);
public:
	virtual HRESULT	Initialize(void);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);	
public:
	static CHealthyBar* Create(LPDIRECT3DDEVICE9 pGraphicDev);
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
	void Set_ContantTable(void);
public:
	virtual _ulong Release(void);
};

#endif // HealthyBar_h__
