/*!
 * \file TerrainTex.h
 * \date 2015/07/09 18:13
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
#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex
	: public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CTerrainTex(void);

public:
	virtual CResources* CloneResource(void);

public:
	virtual HRESULT CreateBuffer(const wstring& wstrPath/*const WORD& wCntX, const WORD& wCntZ, const WORD& wItv*/);	

public:
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 pDevice, const wstring& wstrPath
		/*, const WORD& wCntX, const WORD& wCntZ, const WORD& wItv*/);

public:
	void GetVtxCnt(DWORD* pdwCntX, DWORD* pdwCntZ); //0805

private:
	DWORD	m_dwVtxCntX; //0805
	DWORD	m_dwVtxCntZ; //0805

};


END

#endif // TerrainTex_h__