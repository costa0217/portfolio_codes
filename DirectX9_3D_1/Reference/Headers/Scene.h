/*!
 * \file Scene.h
 * \date 2015/11/07 0:57
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: Ŭ���̾�Ʈ���� Scene�� �ʿ��� ������ �Լ����� �����Ͽ� ������ CSceneŬ������ ��ӹް��Ͽ�
 �� Ŭ������ ����� �ϱ����� �ʿ��� class�� CLayer�� Map STL�� �̿��Ͽ� �ϳ��� ���� ���� �� �� �ְ� �Ѵ�.
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
protected: //Ŭ�������� ������ �� Ŭ������ ��� �ޱ� ���� protected�� ����
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
	typedef boost::unordered_map<const TCHAR*, CLayer*>	MAPLAYER; //���̾��� �ּҸ� ������ ����� ���ڿ��� ���� �����Ѵ�.

public:
	virtual _ulong Release(void);

};

END

#endif // Scene_h_-