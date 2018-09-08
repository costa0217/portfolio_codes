#ifndef RenderTarget_h__
#define RenderTarget_h__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTarget : public Engine::CBase
{
private:
	explicit CRenderTarget(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CRenderTarget(void);
public:
	LPDIRECT3DTEXTURE9 Get_TargetTexture(void) {
		return m_pTargetTexture;}
public:
	void			Set_TargetOnGraphicDev(const _uint& iIndex, _bool bShadow = false);
	void			Reset_TargetOnGraphicDev(const _uint& iIndex, _bool bShadow = false);
public:
	HRESULT		Ready_RenderTarget(const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, _vec4 vColor, _bool bShadow);

	HRESULT		Ready_DebugBuffer(const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
	void			Render_DebugBuffer(void);
public:
	static CRenderTarget* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format, _vec4 vColor, _bool bShadow);

private:
	LPDIRECT3DDEVICE9					m_pGraphicDev;
	LPDIRECT3DTEXTURE9				m_pTargetTexture; 
	LPDIRECT3DSURFACE9				m_pSurface;
	LPDIRECT3DSURFACE9				m_pOldSurface;

	LPDIRECT3DSURFACE9				m_pSten;
	LPDIRECT3DSURFACE9				m_pOldSten;

	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9			m_pIB; 

private:
	_vec4		m_vColor;

public:
	virtual _ulong Release(void);
};


END

#endif // RenderTarget_h__
