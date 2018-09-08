/*!
 * \file VIBuffer.h
 * \date 2015/07/02 13:34
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: long description
 *
 * \note
*/
#ifndef VIBuffer_h__
#define VIBuffer_h__

#include "Resources.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer
	: public CResources
{
public:
	enum BUFFERTYPE {BUFFER_TRICOL,  BUFFER_RCCOL, BUFFER_RCTEX, BUFFER_TERRAINTEX, BUFFER_CUBETEX, BUFFER_CUBEPLAYER, BUFFER_WIRETEX
		,BUFFER_HELICOL, BUFFER_BUNKER, BUFFER_PROPELL, BUFFER_MINIPROPELL, BUFFER_RETONA
		,BUFFER_HOOK, BUFFER_DODGE, BUFFER_TANK, BUFFER_TANKPAW, BUFFER_TANKPAWSIN
		,BUFFER_BULLET, BUFFER_MULTIBULLET, BUFFER_MISILE
		,BUFFER_GUN, BUFFER_GATLINGGUN, BUFFER_LAUNCHER
		,BUFFER_AIRGUNBODY, BUFFER_AIRGUNHEAD, BUFFER_AIRGUNPAW
		,BUFFER_BOMB, BUFFER_CHINOOK, BUFFER_FENCE, BUFFER_TENT, BUFFER_WIREFENCE
		,BUFFER_BLACKCLOUD, BUFFER_FRONTCLOUD, BUFFER_MIDCLOUD
		,BUFFER_SMALLTREE, BUFFER_TALLTREE, };

protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CVIBuffer(void);

public:
	void GetVtxInfo(void* pVertex);

public:
	void SetVtxInfo(void* pVertex);

public:
	DWORD GetVtxCnt(){return m_dwVtxCnt;}

public:
	virtual HRESULT CreateBuffer(void);

public:
	void Render(void);

public:
	void Release(void);

protected:
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	DWORD		m_dwVtxSize;
	DWORD		m_dwVtxCnt;
	DWORD		m_dwFVF;

protected:
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	DWORD		m_dwIdxSize;
	DWORD		m_dwTriCnt;
	D3DFORMAT	m_IdxFmt;
};

END

#endif // VIBuffer_h__