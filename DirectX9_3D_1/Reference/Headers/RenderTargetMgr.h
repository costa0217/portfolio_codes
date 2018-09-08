#ifndef RenderTargetMgr_h__
#define RenderTargetMgr_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CRenderTarget;
class ENGINE_DLL CRenderTargetMgr
{
	DECLARE_SINGLETON(CRenderTargetMgr)
private:
	explicit CRenderTargetMgr(void);
	~CRenderTargetMgr(void);
public:
	LPDIRECT3DTEXTURE9	Get_TargetTexture(const TCHAR* pTargetTag);
	CRenderTarget*		Find_Target(const TCHAR* pTargetTag);

public:
	void	Set_TargetOnGraphicDev(const _uint& iIndex, const TCHAR* pTargetTag, _bool bShadow = false);
	void	Reset_TargetOnGraphicDev(const _uint& iIndex, const TCHAR* pTargetTag, _bool bShadow = false);
	void	Set_MRT(const TCHAR* pMRTTag);
	void	Reset_MRT(const TCHAR* pMRTTag);

public:
	HRESULT Ready_RenderTarget(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pTargetTag
		, const _uint& iSizeX, const _uint& iSizeY, D3DFORMAT Format
		, _vec4 vColor = _vec4(0.f, 0.f, 0.f, 0.f)
		, _bool bShadow = false);
	HRESULT Ready_MRT(const TCHAR* pMRTTag, const TCHAR* pTargetTag);
	HRESULT Ready_DebugBuffer(const TCHAR* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
	void	Render_DebugBuffer(const TCHAR* pTargetTag);

private:
	boost::unordered_map<const TCHAR*, CRenderTarget*>				m_mapTargets;
	typedef boost::unordered_map<const TCHAR*, CRenderTarget*>		MAPTARGETS;

	boost::unordered_map<const TCHAR*, list<CRenderTarget*>>			m_mapMRT;
	typedef boost::unordered_map<const TCHAR*, list<CRenderTarget*>>	MAPMRT;

private:
	list<CRenderTarget*>* Find_MRT(const TCHAR* pMRTTag);
public:
	void Release(void);
};

END
#endif // RenderTargetMgr_h__