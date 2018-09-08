#include "stdafx.h"
#include "StaticObject.h"

#include "StaticObject_Default.h"
#include "StaticObject_Destroy.h"
#include "Player.h"
CStaticObject::CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev)
: CGameObject(pGraphicDev)
, m_pMeshCom(NULL)
, m_pTransCom(NULL)
, m_pEffect(NULL)
, m_pObjectName(NULL)
{
	m_pBoxActor = NULL;
	D3DXMatrixIdentity( &m_BoxWMat );
}

CStaticObject::~CStaticObject(void)
{

}

CStaticObject* CStaticObject::Create(LPDIRECT3DDEVICE9 pGraphicDev, TCHAR* pObjectName, StaticObjectType eSOT)
{
	CStaticObject* pGameObject = NULL;


	if(eSOT == SOT_Default)
	{
		pGameObject = CStaticObject_Default::Create(pGraphicDev, pObjectName);
	}
	else if(eSOT == SOT_Destroy)
	{
		pGameObject = CStaticObject_Destroy::Create(pGraphicDev, pObjectName);
	}

	if(NULL == pGameObject)
	{
		MSG_BOX("CstaticObject is NULL");
	}

	return pGameObject;
}

_ulong CStaticObject::Release(void)
{
	_ulong dwRefCnt = CGameObject::Release();

	if(0 == dwRefCnt)
	{	

	}

	return dwRefCnt;
}

HRESULT CStaticObject::Initialize(void)
{
	if(FAILED(CStaticObject::Add_Component()))
		return E_FAIL;

	m_iPassIndex = Shader_Default;

	m_bActor = false;

	m_fMultiplyForce = 0.f;

	return S_OK;
}

HRESULT CStaticObject::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	//Mesh Component
	TCHAR szTag[128] = L"Mesh_";
	lstrcat(szTag, m_pObjectName);
	pComponent = m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STATIC, szTag);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	//Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	return S_OK;
}

void CStaticObject::InitializeInfo(void)
{
	Engine::CGameObject::Update(0.f);

	m_pMeshCom->Get_MinMax(&m_vLocalMin, &m_vLocalMax);

	D3DXVec3TransformCoord(&m_vWorldMin, &m_vLocalMin, &m_pTransCom->m_matWorld);
	D3DXVec3TransformCoord(&m_vWorldMax, &m_vLocalMax, &m_pTransCom->m_matWorld);

	m_vWorldCenter = (m_vWorldMin + m_vWorldMax) / 2.f;
	m_fWorldRadius = D3DXVec3Length(&(m_vWorldMax - m_vWorldCenter));
}

void CStaticObject::Set_Actor(ActorType Type, _float fDisappearTime)
{
	m_bActor = true;
	m_fDisappearTime = fDisappearTime;

	_vec3 vBoxSize = _vec3(abs(m_vWorldMax.x - m_vWorldMin.x)/2.f, abs(m_vWorldMax.y - m_vWorldMin.y)/2.f, abs(m_vWorldMax.z - m_vWorldMin.z)/2.f);
	_vec3 vSize = _vec3(0.1f, 0.1f, 0.1f);
	
	_vec3 vPlayerSwordMin(0.f, -7.f, -5.f);
	_vec3 vPlayerSwordMax(100.f, 7.f, 5.f);

	_vec3 vPlayerSword = (vPlayerSwordMin + vPlayerSwordMax) / 2.f;
	D3DXVec3TransformCoord(&vPlayerSword, &vPlayerSword, &(*CPlayer::GetInstance()->GetpAttachment_RH_Matrix() * CPlayer::GetInstance()->Get_TransCom()->m_matWorld));
	DamageDirectionType eDirType = CMathMgr::GetInstance()->DamageDirectionDecision(vPlayerSword, m_pTransCom);
	DamageDirectionType eDirType_Sphere = CMathMgr::GetInstance()->DamageDirectionDecision(CPlayer::GetInstance()->Get_TransCom()->m_vPosition, m_pTransCom);
	
	//타입별로 Actor를 생성한다.
	if(Actor_Box == Type)
	{
		m_pBoxActor = Engine::CreateBox(&m_vWorldCenter, &(vSize));
// 		printf("BoxSize===============================\n");
// 		printf("X = %f\nY = %f \nZ = %f \n", vBoxSize.x, vBoxSize.y, vBoxSize.z);
		if(DDT_FRONT == eDirType)
		{
			_vec3 vForce(0.f, 1.f, -100.f * m_fMultiplyForce);
			D3DXMatrixRotationY(&m_matRotationDefault, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);
			D3DXVec3TransformCoord(&vForce, &vForce, &m_matRotationDefault);
			NxVec3	g_UpVec(vForce.x, vForce.y, vForce.z);
			m_pBoxActor->addForce(g_UpVec);
		}
		else if(DDT_BACK == eDirType)
		{
			_vec3 vForce(0.f, 1.f, 100.f * m_fMultiplyForce);
			D3DXMatrixRotationY(&m_matRotationDefault, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);
			D3DXVec3TransformCoord(&vForce, &vForce, &m_matRotationDefault);
			NxVec3	g_UpVec(vForce.x, vForce.y, vForce.z);
			m_pBoxActor->addForce(g_UpVec);
		}
		else if(DDT_FRONT_LEFT == eDirType)
		{
			_vec3 vForce(-50.f * m_fMultiplyForce, 1.f, -50.f * m_fMultiplyForce);
			D3DXMatrixRotationY(&m_matRotationDefault, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);
			D3DXVec3TransformCoord(&vForce, &vForce, &m_matRotationDefault);
			NxVec3	g_UpVec(vForce.x, vForce.y, vForce.z);
			m_pBoxActor->addForce(g_UpVec);
		}
		else if(DDT_BACK_LEFT == eDirType)
		{
			_vec3 vForce(-50.f * m_fMultiplyForce, 1.f, 50.f * m_fMultiplyForce);
			D3DXMatrixRotationY(&m_matRotationDefault, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);
			D3DXVec3TransformCoord(&vForce, &vForce, &m_matRotationDefault);
			NxVec3	g_UpVec(vForce.x, vForce.y, vForce.z);
			m_pBoxActor->addForce(g_UpVec);
		}
		else if(DDT_FRONT_RIGHT == eDirType)
		{
			_vec3 vForce(50.f * m_fMultiplyForce, 1.f, -50.f * m_fMultiplyForce);
			D3DXMatrixRotationY(&m_matRotationDefault, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);
			D3DXVec3TransformCoord(&vForce, &vForce, &m_matRotationDefault);
			NxVec3	g_UpVec(vForce.x, vForce.y, vForce.z);
			m_pBoxActor->addForce(g_UpVec);
		}
		else if(DDT_BACK_RIGHT == eDirType)
		{
			_vec3 vForce(50.f * m_fMultiplyForce, 1.f, 50.f * m_fMultiplyForce);
			D3DXMatrixRotationY(&m_matRotationDefault, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);
			D3DXVec3TransformCoord(&vForce, &vForce, &m_matRotationDefault);
			NxVec3	g_UpVec(vForce.x, vForce.y, vForce.z);
			m_pBoxActor->addForce(g_UpVec);
		}	
	}
	else if(Actor_Sphere == Type)
	{
		//actor크기에 따라 나무펜스의 경우 한 조각이 다른조각위로 올라타는현상발생
		_float fHeight = (m_vWorldMax.y - m_vWorldMin.y);
		m_pBoxActor = Engine::CreateSphere(&m_vWorldCenter, fHeight * 0.1f, m_fWorldRadius * 0.2f);

		if(DDT_FRONT == eDirType)
		{
			_vec3 vForce(0.f, 10.f, -5000.f * m_fMultiplyForce);
			D3DXMatrixRotationY(&m_matRotationDefault, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);
			D3DXVec3TransformCoord(&vForce, &vForce, &m_matRotationDefault);
			NxVec3	g_UpVec(vForce.x, vForce.y, vForce.z);
			m_pBoxActor->addForce(g_UpVec);
		}
		else if(DDT_BACK == eDirType)
		{
			_vec3 vForce(0.f, 10.f, 5000.f * m_fMultiplyForce);
			D3DXMatrixRotationY(&m_matRotationDefault, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);
			D3DXVec3TransformCoord(&vForce, &vForce, &m_matRotationDefault);
			NxVec3	g_UpVec(vForce.x, vForce.y, vForce.z);
			m_pBoxActor->addForce(g_UpVec);
		}
		else if(DDT_FRONT_LEFT == eDirType)
		{
			_vec3 vForce(-2500.f * m_fMultiplyForce, 10.f, -2500.f * m_fMultiplyForce);
			D3DXMatrixRotationY(&m_matRotationDefault, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);
			D3DXVec3TransformCoord(&vForce, &vForce, &m_matRotationDefault);
			NxVec3	g_UpVec(vForce.x, vForce.y, vForce.z);
			m_pBoxActor->addForce(g_UpVec);
		}
		else if(DDT_BACK_LEFT == eDirType)
		{
			_vec3 vForce(-2500.f * m_fMultiplyForce, 10.f, 2500.f * m_fMultiplyForce);
			D3DXMatrixRotationY(&m_matRotationDefault, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);
			D3DXVec3TransformCoord(&vForce, &vForce, &m_matRotationDefault);
			NxVec3	g_UpVec(vForce.x, vForce.y, vForce.z);
			m_pBoxActor->addForce(g_UpVec);
		}
		else if(DDT_FRONT_RIGHT == eDirType)
		{
			_vec3 vForce(2500.f * m_fMultiplyForce, 10.f, -2500.f * m_fMultiplyForce);
			D3DXMatrixRotationY(&m_matRotationDefault, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);
			D3DXVec3TransformCoord(&vForce, &vForce, &m_matRotationDefault);
			NxVec3	g_UpVec(vForce.x, vForce.y, vForce.z);
			m_pBoxActor->addForce(g_UpVec);
		}
		else if(DDT_BACK_RIGHT == eDirType)
		{
			_vec3 vForce(2500.f * m_fMultiplyForce, 10.f, 2500.f * m_fMultiplyForce);
			D3DXMatrixRotationY(&m_matRotationDefault, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);
			D3DXVec3TransformCoord(&vForce, &vForce, &m_matRotationDefault);
			NxVec3	g_UpVec(vForce.x, vForce.y, vForce.z);
			m_pBoxActor->addForce(g_UpVec);
		}
	}
}
