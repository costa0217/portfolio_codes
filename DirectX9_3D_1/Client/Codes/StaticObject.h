/*!
 * \file StaticObject.h
 * \date 2015/11/25 15:37
 *
 * \author Administrator
 * Contact: user@company.com
 *
 * \brief 
 *
 * TODO: ��� StaticObject�� �ֻ��� Ŭ����
		����� �޶����°� ���̴� ���� �ۿ� ������ ���߿��� ���̴����� �ٸ��� ������ ������
		�̷� ������ ���� ����Ѵ�.
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
	Engine::CLayer*			m_pLayer; // �ı� ������Ʈ ���̾ �߰��� �� �ʿ��ϴ�.

	LPD3DXEFFECT			m_pEffect;

	TCHAR*					m_pObjectName;	//������Ʈ�� �̸�. �迭���ٴ� ������ �� �����Ҽ� �ִ�. global static data�� �Ҵ����� �ش� �����͸� ���� �ִ�.

	_int					m_iPassIndex; // Shader PassIndex; 0 = default //1 = AlphaTest //2 = AlphaBlend

	_vec3					m_vLocalMin, m_vLocalMax;	// ���� ��ǥ�迡���� Min, Max
	_vec3					m_vWorldCenter;				//��ü�� �߽ɰ�
	_float					m_fWorldRadius;				//��ü�� ������
	_vec3					m_vWorldMin, m_vWorldMax;	//���� ��ǥ�迡���� Min, Max

	NxActor*				m_pBoxActor;
	D3DXMATRIXA16			m_BoxWMat;
	_bool					m_bActor; // �ν����� ������Ʈ�� ������ �� true�� �޴´�.
	_float					m_fDisappearTime; // �ν����� ������Ʈ�� ������� �ð�.
	_float					m_fMultiplyForce; // AddForce�� ������ ��.

	_matrix					m_matRotationDefault; //ȸ�� ���

protected:
	virtual HRESULT Initialize(void);

private:
	virtual HRESULT Add_Component(void);
	virtual void Set_ConstantTable(void) { }

public:
	virtual _ulong Release(void);
};
#endif // StaticObject_h__