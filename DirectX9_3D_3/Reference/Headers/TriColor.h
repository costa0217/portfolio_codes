/*!
 * \file TriColor.h
 * \date 2015/07/02 13:39
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

#ifndef TriColor_h__
#define TriColor_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTriColor
	: public CVIBuffer
{
private:
	explicit CTriColor(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CTriColor(void);

public:
	virtual CResources*	CloneResource(void);

public:
	virtual HRESULT CreateBuffer(void);

public:
	void Render(void);

public:
	static CTriColor* Create(LPDIRECT3DDEVICE9 pDevice);
};

END

#endif // TriColor_h__