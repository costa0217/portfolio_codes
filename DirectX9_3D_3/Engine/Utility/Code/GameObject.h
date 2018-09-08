/*!
 * \file GameObject.h
 * \date 2015/07/06 13:19
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

#ifndef GameObject_h__
#define GameObject_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CComponent;

class ENGINE_DLL CGameObject
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pDevice);

public:
	virtual ~CGameObject(void);

public:
	const CComponent* GetComponent(const wstring& wstrComponentKey);
	virtual const VTXTEX* GetTerrainVtx(void) {return NULL;}

public:
	virtual HRESULT Initialize(void);
	virtual int Update(void);
	virtual void Render(void) {};

private:
	void Release(void);

protected:
	LPDIRECT3DDEVICE9		m_pDevice;

protected:
	typedef map<wstring, CComponent*>		MAPCOMPONENT;
	MAPCOMPONENT		m_mapComponent;
	
};


END

#endif // GameObject_h__