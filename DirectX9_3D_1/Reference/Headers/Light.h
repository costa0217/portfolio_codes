/*!
 * \file Light.h
 * \date 2015/11/07 1:12
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: CLightMgr 와 연동하여 D3DLIGHT9구조체를 통해 빛을 관리하기 위한 class.
 *
 * \note
*/
#ifndef Light_h__
#define Light_h__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLight : public Engine::CBase
{
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLight(void);
public:
	const D3DLIGHT9* Get_LightInfo(void) {
		return &m_LightInfo;}
	void Set_LightInfo(D3DLIGHT9* pLightInfo);
public:
	HRESULT Init_Light(const D3DLIGHT9* pLightInfo, const _ulong& dwLightIdx);
	void Render(LPD3DXEFFECT pEffect);
public:
	static CLight* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const D3DLIGHT9* pLightInfo, const _ulong& dwLightIdx);

public:
	void	Compute_LightMatrix(_matrix* pViewOut, _matrix* pProjOut, const _vec3* pPos);
private:
	LPDIRECT3DDEVICE9							m_pGraphicDev;
	D3DLIGHT9										m_LightInfo;
	LPDIRECT3DVERTEXBUFFER9				m_pVB;
	LPDIRECT3DINDEXBUFFER9					m_pIB;

private:
	_float			m_fHeightPxl;
	_float			m_fWidthPxl;

	_float			m_fHeightTexel;
	_float			m_fWidthTexel;
public:
	virtual _ulong Release(void);
};

END

#endif // Light_h__

