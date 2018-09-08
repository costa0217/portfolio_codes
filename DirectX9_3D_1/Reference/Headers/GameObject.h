/*!
 * \file GameObject.h
 * \date 2015/11/07 1:02
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: Engine::CBase를 상속받고 클라이언트 오브젝트의 최상위 부모 class로 오브젝트에 필요한 멤버 변수와 함수를 선언한다.
 *
 * \note
*/

#ifndef GameObject_h__
#define GameObject_h__

#include "Base.h"

BEGIN(Engine)

class CComponent;
class ENGINE_DLL CGameObject : public Engine::CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGameObject(void);

public:
	Engine::CComponent*	Get_Component(const TCHAR* pComponentTag);

public:
	virtual	_int Update(const _float& fTimeDelta);
	virtual	void Render(void){}
	virtual	void Render_Shadow(void){};
	virtual	void Render_Velocity(void){};

protected:
	virtual	HRESULT Initialize(void) {return S_OK;}
	virtual	HRESULT Add_Component(void) {return S_OK;}

private:
	Engine::CComponent*	Find_Component(const TCHAR* pComTag);

protected:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	boost::unordered_map<const TCHAR*, CComponent*>			m_mapComponent;
	typedef boost::unordered_map<const TCHAR*, CComponent*>	MAPCOMPONENT;




public:
	virtual	_ulong Release(void);
};

END

#endif // GameObject_h__