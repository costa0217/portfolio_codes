#ifndef IceUp_h__
#define IceUp_h__


#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CFrame;
}

class CIceUp	: public Engine::CGameObject
{
private:
	explicit CIceUp(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CIceUp(void);

public: //Get
	_bool	Get_Switch(void){ return m_bSwitch; } 
public: //Set
	void	Set_Switch(_bool bSwitch) { m_bSwitch = bSwitch; }


public:
	virtual HRESULT	Initialize(_vec3 vPos);
	virtual HRESULT Add_Component(void);
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);
public:
	static CIceUp* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos);
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

private:
	void	Set_ContantTable(void);
	void	Set_SettingData(_vec3 vPos);
public:
	virtual _ulong Release(void);
};

#endif // IceUp_h__
