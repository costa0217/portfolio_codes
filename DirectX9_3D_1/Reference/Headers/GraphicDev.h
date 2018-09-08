/*!
 * \file GraphicDev.h
 * \date 2015/11/06 21:38
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief
 *
 * TODO: 디바이스 장치들을 초기화 하는 Singleton class.
 *
 * \note
*/

#ifndef GraphicDev_h__
#define GraphicDev_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL CGraphicDev
{
public:
	DECLARE_SINGLETON(CGraphicDev)

public:
	enum WINMODE {MODE_FULL, MODE_WIN};

private:
	explicit CGraphicDev(void);

public:
	~CGraphicDev(void);

public:
	// getter
	LPDIRECT3DDEVICE9 Get_GraphicDev(void) { return m_pDevice; }

	// setter
	void SetRenderState(_D3DRENDERSTATETYPE Type, const _ulong& dwFlag) { m_pDevice->SetRenderState(Type, dwFlag); }
	void SetSamplerState(_D3DSAMPLERSTATETYPE Type, const _ulong dwFlag) { m_pDevice->SetSamplerState(0, Type, dwFlag); }

public:
	HRESULT Ready_GraphicDev(WINMODE Mode, HWND hWnd, const _ushort& wSizeX, const _ushort& wSizeY);

private:
	void SetParameters(D3DPRESENT_PARAMETERS& d3dpp, WINMODE Mode, HWND hWnd, const _ushort& wSizeX, const _ushort& wSizeY);

private:
	LPDIRECT3D9			m_pSDK;		//IDirect3D9의 typedef된 포인트형 변수
	LPDIRECT3DDEVICE9	m_pDevice;	//IDirect3DDevice9의 typedef된 포인트형 변수로 장치를 대표하는 객체이다.

private:
	void Release(void);

};

END

#endif // GraphicDev_h__