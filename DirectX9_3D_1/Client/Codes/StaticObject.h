/*!
 * \file StaticObject.h
 * \date 2015/11/25 15:37
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: 모든 StaticObject의 최상위 클래스
		현재는 달라지는게 셰이더 파일 밖에 없지만 나중에는 셰이더말고도 다른게 있을수 있으니
		이런 구조로 만든어서 사용한다.
 *
 * \note
*/

#ifndef StaticObject_h__
#define StaticObject_h__

#include "Defines.h"
#include "GameObject.h"
#include "Export_Function.h"

namespace Engine
{
	class CStaticMesh;
	class CTransform;
	class CLayer;
}

class CStaticObject : public Engine::CGameObject
{
public:
	enum StaticObjectType { SOT_Default, SOT_Destroy, SOT_END };
	enum ActorType { Actor_Box, Actor_Sphere, Actor_Plane };
	enum ShaderType { Shader_Default, Shader_AlphaTest, Shader_AlphaBlend };
protected:
	explicit CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticObject(void);

public:
	virtual _int Update(const _float& fTimeDelta) { return 0; }
	virtual void Render(void) { }
	void InitializeInfo(void);

public://getter setter
	void SetObjectPos(_vec3 vPos) { m_pTransCom->m_vPosition = vPos; }
	void SetObjectScale(_vec3 vScale) { m_pTransCom->m_vScale = vScale; }
	void SetObjectAngle(_vec3 vAngle)
	{
		m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_X] = (vAngle.x);
		m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = (vAngle.y);
		m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Z] = (vAngle.z);
	}
	_float GetObjectRadianY(void) { return m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]; }

	void SetObjectPassIndex(_int iPassIndex){m_iPassIndex = iPassIndex;}

	void Set_Layer(Engine::CLayer* pLayer){m_pLayer = pLayer;}
	
	void Set_Actor(ActorType Type, _float fDisappearTime);
	void Set_MultiplyForce(_float fMultiplyForce){m_fMultiplyForce = fMultiplyForce;}

	_vec3 Get_LocalMin(void) { return m_vLocalMin; }
	_vec3 Get_LocalMax(void) { return m_vLocalMax; }
	_vec3 Get_WorldCenter(void) { return m_vWorldCenter; }
	_float Get_WorldRadius(void) { return m_fWorldRadius; }
	_vec3 Get_WorldMin(void) { return m_vWorldMin; }
	_vec3 Get_WorldMax(void) { return m_vWorldMax; }

	Engine::CTransform* Get_TransCom(void) { return m_pTransCom; }

	Engine::CStaticMesh* Get_MeshCom(void) { return m_pMeshCom; }

public:
	static CStaticObject* Create(LPDIRECT3DDEVICE9 pGraphicDev, TCHAR* pObjectName, StaticObjectType eSOT);

protected:
	Engine::CStaticMesh*	m_pMeshCom;
	Engine::CTransform*		m_pTransCom;
	Engine::CLayer*			m_pLayer; // 파괴 오브젝트 레이어에 추가할 때 필요하다.

	LPD3DXEFFECT			m_pEffect;

	TCHAR*					m_pObjectName;	//오브젝트의 이름. 배열보다는 공간을 더 절약할수 있다. global static data를 할당한후 해당 포인터를 갖고 있다.

	_int					m_iPassIndex; // Shader PassIndex; 0 = default //1 = AlphaTest //2 = AlphaBlend

	_vec3					m_vLocalMin, m_vLocalMax;	// 로컬 좌표계에서의 Min, Max
	_vec3					m_vWorldCenter;				//객체의 중심값
	_float					m_fWorldRadius;				//객체의 반지름
	_vec3					m_vWorldMin, m_vWorldMax;	//월드 좌표계에서의 Min, Max

	NxActor*				m_pBoxActor;
	D3DXMATRIXA16			m_BoxWMat;
	_bool					m_bActor; // 부숴지는 오브젝트가 생성될 때 true로 받는다.
	_float					m_fDisappearTime; // 부숴지는 오브젝트가 사라지는 시간.
	_float					m_fMultiplyForce; // AddForce에 곱해줄 값.

	_matrix					m_matRotationDefault; //회전 행렬

protected:
	virtual HRESULT Initialize(void);

private:
	virtual HRESULT Add_Component(void);
	virtual void Set_ConstantTable(void) { }

public:
	virtual _ulong Release(void);
};
#endif // StaticObject_h__