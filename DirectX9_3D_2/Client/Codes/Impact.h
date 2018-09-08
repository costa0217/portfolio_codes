#ifndef Impact_h__
#define Impact_h__

#include "Defines.h"
#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CVIBuffer;
	class CTexture;
	class CFrame;
}

class CImpact : public Engine::CGameObject
{
private:
	explicit CImpact(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CImpact(void);

public:
	void		Set_Switch(_bool bSwitch){ m_bSwitch = bSwitch; }
	_bool		Get_Switch(void){ return m_bSwitch; } 
public:
	virtual		HRESULT	Initialize( _int iIdx);
	virtual		HRESULT Add_Component(void);
	virtual		_int Update(const _float& fTimeDelta);
	virtual		void Render(void);	
public:
	static CImpact* Create(LPDIRECT3DDEVICE9 pGraphicDev, _int iIdx);
private:
	LPD3DXEFFECT				m_pEffect;

private:
	_bool						m_bSwitch;
	_float						m_fUIDepth;
private:
	_matrix						m_matOrtho;
	_float						m_fX, m_fY;
	_float						m_fSizeX, m_fSizeY;

private:
	Engine::CTransform*			m_pTransCom;

	Engine::CVIBuffer*			m_pBufferCom;
	Engine::CTexture*			m_pTextureCom;
	Engine::CFrame*				m_pFrame;

	_int						m_iIndex;
	_float						m_fAccTime;
	_vec4						m_vColor;

	_float						m_fAlpha;
private:
	void	Set_ContantTable(void);
	void	Set_SettingData(_vec3 vPos);
public:
	virtual _ulong Release(void);
};

#endif // Impact_h__
