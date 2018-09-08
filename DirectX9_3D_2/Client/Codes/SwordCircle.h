#ifndef SwordCircle_h__
#define SwordCircle_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CFrame;
}

class CSwordCircle	: public Engine::CGameObject
{
private:
	explicit CSwordCircle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSwordCircle(void);

public:
	virtual HRESULT	Initialize(_vec3 vPos);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public: 
	static CSwordCircle* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
private:
	Engine::CTransform*			m_pTransCom;
	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;
	Engine::CFrame*				m_pFrame;
private:
	LPD3DXEFFECT				m_pEffect;

private:
	_vec3						m_vViewPos;
	_bool						m_bSwitch;

	_float						m_fAccTime;

	_float						m_fAlpha;

private:
	void	Set_ContantTable(void);
public:
	virtual _ulong Release(void);
};


#endif // SwordCircle_h__
