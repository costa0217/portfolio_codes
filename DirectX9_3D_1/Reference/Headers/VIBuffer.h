/*!
 * \file VIBuffer.h
 * \date 2015/11/06 22:47
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: CResources를 상속 받고 모든 Buffer들의 부모 class
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
	enum BUFFERTYPE {TYPE_RCTEX, TYPE_RCTEX2, TYPE_CUBE, TYPE_TERRAIN, TYPE_TRAILTEX, TYPE_MONTRAILTEX, TYPE_EYETRAILTEX, TYPE_END};

protected:
	explicit CVIBuffer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVIBuffer(void);

public: //getter
	void Get_VtxInfo(void* pVtxInfo);

public:
	void Set_VtxInfo(void* pVtxInfo);

public:
	virtual HRESULT Create_Buffer(void);
	virtual CResources* Clone_Resource(void) PURE;

public:
	void Render(const D3DXMATRIX* pmatWorld);

protected:
	//버퍼 관려 변수들
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	_ulong						m_dwVtxSize;
	_ulong						m_dwVtxCnt;
	_ulong						m_dwVtxFVF;

	//인덱스 관련 변수들
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	_ulong						m_dwIdxSize;
	_ulong						m_dwTriCnt;
	D3DFORMAT					m_IdxFmt;

public:
	virtual _ulong Release(void);

};

END

#endif // VIBuffer_h__