#ifndef MagicCircle_h__
#define MagicCircle_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CFrame;
}

class CMagicCircle	: public Engine::CGameObject
{
private:
	explicit CMagicCircle(LPDIRECT3DDEVICE9 pGraphicDev, _int iIdx);
	virtual ~CMagicCircle(void);

public:
	virtual HRESULT	Initialize(_vec3 vPos);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public: 
	static CMagicCircle* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int iIdx);
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
	_int						m_iIndex;

	_float						m_fAccTime;
	_float						m_fMagicTime;

	_vec4						m_vColor;


private:
	void	Set_ContantTable(void);
	void	Set_SettingData(_vec3 vPos);
public:
	virtual _ulong Release(void);
};

#endif // MagicCircle_h__
