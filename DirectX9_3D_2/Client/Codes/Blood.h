#ifndef Blood_h__
#define Blood_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CFrame;
}

class CBlood	: public Engine::CGameObject
{
private:
	explicit CBlood(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBlood(void);

public: //Get
	_bool	Get_Switch(void){ return m_bSwitch; } 
public: //Set
	void	Set_Switch(_bool bSwitch) { m_bSwitch = bSwitch; }

	void	Set_Scale(_float fScale);


public:
	virtual HRESULT	Initialize(_vec3 vPos);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CBlood* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
	static CBlood* CreateScale(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _float  fScale);
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

private:
	void	Set_ContantTable(void);
	void	Set_SettingData(_vec3 vPos);
public:
	virtual _ulong Release(void);
};


#endif // Blood_h__
