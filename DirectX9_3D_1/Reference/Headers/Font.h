/*!
 * \file Font.h
 * \date 2015/11/06 22:01
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: CFontMgr와 연계하여 폰트에 대한 정보를 초기화하여 사용하는 class.
 *
 * \note
*/

#ifndef Font_h__
#define Font_h__

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFont : public Engine::CBase
{
private:
	explicit CFont(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CFont(void);

public:
	HRESULT Init_Font(const UINT& iWidth, const UINT& iHeight, const UINT& iWeight, const TCHAR* pFaceName);
	void Render(const TCHAR* pString, D3DXCOLOR Color, const D3DXVECTOR3& vPos);

public:
	static CFont* Create(LPDIRECT3DDEVICE9 pGraphicDev
		, const UINT& iWidth, const UINT& iHeight, const UINT& iWeight, const TCHAR* pFaceName);

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	LPD3DXFONT			m_pFont;
	UINT				m_iHeight;
	UINT				m_iWidth;

public:
	virtual _ulong Release(void);
};

END

#endif // Font_h__