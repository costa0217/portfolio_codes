/*!
 * \file Layer.h
 * \date 2015/07/06 13:27
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

#ifndef Layer_h__
#define Layer_h__

#include "Engine_Include.h"

BEGIN(Engine)

class CGameObject;
class CComponent;

class ENGINE_DLL CLayer
{
private:
	explicit CLayer(LPDIRECT3DDEVICE9 pDevice);

public:
	~CLayer(void);

public:
	const CComponent* GetComponent(const wstring& wstrObjKey
		, const wstring& wstrComponentKey);
	const VTXTEX* GetTerrainVertex(const wstring& wstrObjKey);

public:
	HRESULT AddObject(const wstring& wstrObjKey, CGameObject* pGameObject);

public:
	void Update(void);
	void Render(void);

public:
	static CLayer* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	void Release(void);

private:
	LPDIRECT3DDEVICE9	m_pDevice;

private:
	typedef list<CGameObject*>				OBJECTLIST;
	typedef map<wstring, OBJECTLIST>		MAPOBJLIST;
	MAPOBJLIST		m_mapObjlist;
public:
	MAPOBJLIST* GetMapObjlist(){return &m_mapObjlist;}
};

END

#endif // Layer_h__