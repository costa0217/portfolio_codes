/*!
 * \file Scene.h
 * \date 2015/11/07 0:57
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: 클라이언트에서 Scene에 필요한 변수와 함수들을 선언하여 엔진의 CScene클래스를 상속받게하여
 씬 클래스를 만들게 하기위해 필요한 class로 CLayer와 Map STL을 이용하여 하나의 씬을 관리 할 수 있게 한다.
 *
 * \note
*/
#ifndef Scene_h__
#define Scene_h__

#include "Base.h"
#include "Layer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public Engine::CBase
{
protected: //클래스에서 씬들이 이 클래스를 상속 받기 위해 protected로 선언
	explicit CScene(LPDIRECT3DDEVICE9 pDevice);
	virtual ~CScene(void);

public:
	Engine::CComponent*	Get_Component(const TCHAR* pComponentTag, const TCHAR* pLayerTag, const TCHAR* pObjectTag, const _uint& iCnt);

public:
	virtual HRESULT Init_Scene(void) PURE;
	virtual _int Update(const _float& fTimeDelta);
	virtual void Render(void);

private:
	CLayer* Find_Layer(const TCHAR* pLayerTag);

protected:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	
	boost::unordered_map<const TCHAR*, CLayer*>			m_mapLayer;
	typedef boost::unordered_map<const TCHAR*, CLayer*>	MAPLAYER; //레이어의 주소를 맵으로 만들어 문자열을 통해 관리한다.

public:
	virtual _ulong Release(void);

};

END

#endif // Scene_h_-