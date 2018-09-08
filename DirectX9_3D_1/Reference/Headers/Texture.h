/*!
 * \file Texture.h
 * \date 2015/11/07 0:54
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: CTexture class.
 *
 * \note
*/

#ifndef Texture_h__
#define Texture_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CTexture
	: public CResources
{
public:
	enum TEXTURETYPE {TYPE_NORMAL, TYPE_CUBE, TYPE_END};

private:
	explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTexture(void);

public:
	void Set_ConstantTable(LPD3DXEFFECT pEffect, const char* pConstantTag, const _ulong& dwIndex = 0);

public:
	virtual CResources* Clone_Resource(void);
	HRESULT Load_Texture(TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt);
	void Render(const DWORD& dwStage, const DWORD& iIndex);

public:
	static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev, TEXTURETYPE eTextureType, const TCHAR* pFilePath, const WORD& wCnt);

private:
	vector<IDirect3DBaseTexture9*>			m_vecTexture;
	typedef vector<IDirect3DBaseTexture9*>	VECTEXTURE;

private:
	_ulong		m_dwContainerSize;

public:
	virtual _ulong Release(void);
};

END

#endif //Texture_h__