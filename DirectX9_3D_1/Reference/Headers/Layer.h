/*!
 * \file Layer.h
 * \date 2015/11/07 0:58
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: CScene클래스와 연동하여 Map STL을 통해서 오브젝트들을 하나의 레이어로 묶어관리 하기 위한 class.
 *
 * \note
*/

#ifndef Layer_h__
#define Layer_h__

#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CComponent;
class ENGINE_DLL CLayer : public Engine::CBase
{
private:
	explicit CLayer(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CLayer(void);

public:
	Engine::CComponent*	Get_Component(const TCHAR* pComponentTag, const TCHAR* pObjectTag, const _uint& iCnt);
	
	_int	Get_ObjectListSize(const TCHAR* pObjectTag);
	list<CGameObject*>* GetpObjectList(const TCHAR* pObjectTag);
	void	Delete_ObjectListIndex(const TCHAR* pObjectTag, _int iSelectNumber);

public:
	HRESULT Ready_Object(const TCHAR* pObjectTag, CGameObject* pGameObject);
	HRESULT Ready_Object2(const TCHAR* pObjectTag, CGameObject* pGameObject);

	_int Update(const _float& fTimeDelta);
	void Render(void);

public:
	static CLayer* Create(LPDIRECT3DDEVICE9 pDevice);

private:
	list<CGameObject*>* Find_ObjectList(const TCHAR* pObjectTag);
	list<CGameObject*>* Find_ObjectList2(const TCHAR* pObjectTag);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;

	typedef list<CGameObject*>				OBJECTLIST;
	boost::unordered_map<const TCHAR*, OBJECTLIST>			m_mapObjlist;
	boost::unordered_map<const TCHAR*, OBJECTLIST>			m_mapLastObjlist;
	typedef boost::unordered_map<const TCHAR*, OBJECTLIST>	MAPOBJLIST;

public:
	virtual _ulong Release(void);
};

END

#endif // Layer_h__