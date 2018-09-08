/*!
 * \file FontMgr.h
 * \date 2015/11/06 22:02
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: Map STL을 통해서 CFont class를 관리하는 Singleton Manager clsss.
 *
 * \note
*/

#ifndef FontMgr_h__
#define FontMgr_h__

#include "Engine_Defines.h"

BEGIN(Engine)

class CFont;
class ENGINE_DLL CFontMgr
{
public:
	DECLARE_SINGLETON(CFontMgr)

private:
	explicit CFontMgr(void);
	~CFontMgr(void);

public:
	HRESULT Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const TCHAR* pFontKey, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
	void Render_Font(const TCHAR* pFontKey, const D3DXVECTOR3& vPos, const TCHAR* pString, D3DXCOLOR Color);

private:
	boost::unordered_map<const TCHAR*, CFont*>			m_mapFont;
	typedef boost::unordered_map<const TCHAR*, CFont*>	MAPFONT;

public:
	void Release(void);
};

END

#endif // FontMgr_h__