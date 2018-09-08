#include "stdafx.h"
#include "StaticObject_Destroy.h"
#include "StaticObject_Default.h"
#include "CullingMgr.h"
#include "CollisionMgr.h"
#include "Player.h"
#include "Layer.h"
#include "Monster.h"

#include "SoundMgr.h"

CStaticObject_Destroy::CStaticObject_Destroy(LPDIRECT3DDEVICE9 pGraphicDev)
: CStaticObject(pGraphicDev)
{

}

CStaticObject_Destroy::~CStaticObject_Destroy(void)
{

}

_int CStaticObject_Destroy::Update(const _float& fTimeDelta)
{
	if(CCullingMgr::GetInstance()->CullingCamera(&m_vWorldCenter, m_fWorldRadius))
	{
		PlayerAttackBox_And_MyBodyBox(fTimeDelta);

		if(m_fObjectHp < 0)
		{
			Create_DestroyBrick();
			if(!lstrcmp(m_pObjectName, L"wood_fence01") || !lstrcmp(m_pObjectName, L"wood_fence02"))
			{
				CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::octopus_drill_wood_01 + rand()%3), _vec3(0.f, 0.f, 0.f), 3.f);
			}
			else if(!lstrcmp(m_pObjectName, L"tombstone04"))
			{
				CSoundMgr::GetInstance()->PlayEffect(SOUND::material_rock_break, _vec3(0.f, 0.f, 0.f), 3.f);
			}
			else if(!lstrcmp(m_pObjectName, L"iron_gate02"))
			{
				CSoundMgr::GetInstance()->PlayEffect(SOUND::Hit_Metal_StrongSlash, _vec3(0.f, 0.f, 0.f), 3.f);
			}
			return -1;
		}

		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
		Engine::CGameObject::Update(fTimeDelta);
	}

	return 0;
}

void CStaticObject_Destroy::Render(void)
{
	if(NULL == m_pEffect)
		return;

	Set_ConstantTable();

	m_pEffect->Begin(NULL, 0);

	m_pEffect->BeginPass(m_iPassIndex);

	m_pMeshCom->Render_MeshForShader(m_pEffect, true);

	m_pEffect->EndPass();

	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}

CStaticObject_Destroy* CStaticObject_Destroy::Create(LPDIRECT3DDEVICE9 pGraphicDev, TCHAR* pObjectName)
{
	CStaticObject_Destroy* pGameObject = new CStaticObject_Destroy(pGraphicDev);
	pGameObject->m_pObjectName = new TCHAR[128];
	lstrcpy(pGameObject->m_pObjectName, pObjectName);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CStaticObject_Destroy Create Faild");
		Engine::Safe_Release(pGameObject);
	}

	//파괴되는 오브젝트는 추후 삭제히 충돌 매니저에 있는 리스트를 지워 준다.
	CCollisionMgr::GetInstance()->Get_StaticObjectList()->push_back(pGameObject);
	return pGameObject;
}

HRESULT CStaticObject_Destroy::Initialize(void)
{
	if(FAILED(CStaticObject::Initialize()))
		return E_FAIL;

	m_pEffect = Engine::Get_EffectHandle(L"Shader_Default");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	if(m_pObjectName == L"wood_fence01")
		m_fObjectHp = 1400.f;
	else
		m_fObjectHp = 500.f;

	return S_OK;
}

HRESULT CStaticObject_Destroy::Add_Component(void)
{
	return S_OK;
}

void CStaticObject_Destroy::Set_ConstantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);
}

void CStaticObject_Destroy::PlayerAttackBox_And_MyBodyBox(const _float& fTimeDelta)
{
	//플레이어와의 충돌 처리
	if(Engine::CCollision::GetInstance()->Collision_Sphere_Sphere(CPlayer::GetInstance()->Get_TransCom()->m_vPosition, PlayerCollisionDistance, m_pTransCom->m_vPosition, m_fWorldRadius))
	{
		if(CPlayer::GetInstance()->Get_ClientBoxInfo() != NULL)	//플레이어의 공격 박스가 존재 한다면
		{
			if( CPlayer::GetInstance()->Get_Check_EndPeriodMulTime_Start() == false )
				return;

			if( CPlayer::GetInstance()->Get_Check_EndPeriodMulTime_End() == true )
				return;

			//플레이어의 박스와 몬스터의 구가 충돌 했으면 true
			if(Engine::CCollision::GetInstance()->Collision_AABB_Sphere(_vec3(0.f, -7.f, -5.f), _vec3(100.f, 7.f, 5.f), (*CPlayer::GetInstance()->GetpAttachment_RH_Matrix() * CPlayer::GetInstance()->Get_TransCom()->m_matWorld), m_vWorldCenter, m_fWorldRadius))
			{
				if(Engine::CCollision::GetInstance()->Collision_OBB_OBB(_vec3(0.f, -7.f, -5.f), _vec3(100.f, 7.f, 5.f), (*CPlayer::GetInstance()->GetpAttachment_RH_Matrix() * CPlayer::GetInstance()->Get_TransCom()->m_matWorld), m_vLocalMin, m_vLocalMax, m_pTransCom->m_matWorld))
				{
					m_fObjectHp -= 10000.f * fTimeDelta;	//플레이어의 공격력을 할까 하다 이게 좀더 자연스러운거 같아서 임시로함
					return; //사실상 플레이어와 몬스터가 동시에 공격 중일때는 플레이어꺼만 처리해도 될거 같다.
				}
			}
		}
	}

	//몬스터와의 충돌 처리
	list<CMonster*>::iterator Monsteriter		= CCollisionMgr::GetInstance()->Get_MonsterList()->begin();
	list<CMonster*>::iterator MonsteriterEnd	= CCollisionMgr::GetInstance()->Get_MonsterList()->end();

	for( ; Monsteriter != MonsteriterEnd; ++Monsteriter )
	{
		if(Engine::CCollision::GetInstance()->Collision_Sphere_Sphere((*Monsteriter)->Get_TransCom()->m_vPosition, PlayerCollisionDistance, m_pTransCom->m_vPosition, m_fWorldRadius))
		{
			//추후 몬스터의 타입을 조사해야 타입이 공격중일때만 처리 하도록 한다.
			boost::unordered_map<int, vector<CLIENTBOXINFO*>>::iterator MapiterEnd	= (*Monsteriter)->Get_MonsterMapAttackBox()->end();
			boost::unordered_map<int, vector<CLIENTBOXINFO*>>::iterator MapiterFind	= (*Monsteriter)->Get_MonsterMapAttackBox()->find((*Monsteriter)->Get_MonsterAnimationIndex());

			if(MapiterFind != MapiterEnd)
			{
				vector<CLIENTBOXINFO*>::iterator iter		= MapiterFind->second.begin();
				vector<CLIENTBOXINFO*>::iterator iterEnd	= MapiterFind->second.end();

				for(; iter != iterEnd; ++iter)
				{
					if((*Monsteriter)->Get_MeshCom()->Check_EndPeriodMulTime((*iter)->fStartTime) == false)
						return;

					if((*Monsteriter)->Get_MeshCom()->
						Check_EndPeriodMulTime((*iter)->fEndTime) == true)
						return;

					//몬스터의 어택 박스와 플레이어의 구가 충돌하면
					const _matrix pMonsterAttackBoxMatrix = *(*Monsteriter)->Get_MeshCom()->Get_FrameMatrix((*iter)->pFrameName) * (*Monsteriter)->Get_TransCom()->m_matWorld;

					if(Engine::CCollision::GetInstance()->Collision_AABB_Sphere((*iter)->vMin, (*iter)->vMax,
						pMonsterAttackBoxMatrix
						, m_pTransCom->m_vPosition, m_fWorldRadius))
					{						
						if(Engine::CCollision::GetInstance()->Collision_OBB_OBB((*iter)->vMin, (*iter)->vMax,
							pMonsterAttackBoxMatrix, m_vLocalMin, m_vLocalMax, m_pTransCom->m_matWorld))
						{
							m_fObjectHp -= 10000.f * fTimeDelta;	//추후 몬스터의 공격력으로 하던가 생각좀해야함
							return; //사실상 한번만 처리해도 왜만하면 부셔지니 리턴하여 다음에 다시 검사하게 한다.
						}						
					}
				}
			}
		}

	}
}

void CStaticObject_Destroy::Create_DestroyBrick()
{
	_int iBrickNum = 0;
	ActorType eType = Actor_Box;
	_float fDisappearTime = 0.f;
	_float fMultiplyForce = 0.f;
	
	 if(!lstrcmp(m_pObjectName, L"wood_fence02"))
	{
		iBrickNum = 8;
		eType = Actor_Sphere;
		fDisappearTime = 0.5f;
		fMultiplyForce = 0.6f;
	}
	else if(!lstrcmp(m_pObjectName, L"wood_fence01"))
	 {
		 iBrickNum = 6;
		 eType = Actor_Sphere;
		 fDisappearTime = 0.5f;
		 fMultiplyForce = 1.f;
	 }
	else if(!lstrcmp(m_pObjectName, L"tombstone04"))
	{
		iBrickNum = 8;
		eType = Actor_Sphere;
		fDisappearTime = 5.f;
		fMultiplyForce = 1.f;
	}
	else if(!lstrcmp(m_pObjectName, L"iron_gate02"))
	{
		iBrickNum = 1;
		eType = Actor_Sphere;
		fDisappearTime = 5.f;
		fMultiplyForce = 150.f;
	}

	char* pNumber = new char[128];
	for(_int i = 0; i < iBrickNum; ++i)
	{
		Engine::CGameObject* pDestroyBrick = NULL;
		TCHAR szName[128] = L"";
		TCHAR szNumber[128] = L"";

		lstrcpy(szName, m_pObjectName);
		lstrcat(szName, L"_");
		itoa(i, pNumber, 10);
		MultiByteToWideChar(CP_ACP, 0, pNumber, -1, szNumber, 128*sizeof(TCHAR));
		lstrcat(szName, szNumber);
		pDestroyBrick = CStaticObject_Default::Create(m_pGraphicDev, szName);
		((CStaticObject*)pDestroyBrick)->SetObjectPos(m_pTransCom->m_vPosition);
		((CStaticObject*)pDestroyBrick)->SetObjectScale(m_pTransCom->m_vScale);
		((CStaticObject*)pDestroyBrick)->SetObjectAngle(_vec3(m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_X], m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y], m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Z]));
		((CStaticObject*)pDestroyBrick)->InitializeInfo();
		((CStaticObject*)pDestroyBrick)->Set_MultiplyForce(fMultiplyForce);
		((CStaticObject*)pDestroyBrick)->Set_Actor(eType, fDisappearTime);
		m_pLayer->Ready_Object(szName, pDestroyBrick);
		
	}
	::Safe_Delete(pNumber);

}

_ulong CStaticObject_Destroy::Release(void)
{
	_ulong dwRefCnt = CStaticObject::Release();

	if(0 == dwRefCnt)
	{
		list<CStaticObject*>::iterator iter = CCollisionMgr::GetInstance()->Get_StaticObjectList()->begin();
		list<CStaticObject*>::iterator iterEnd = CCollisionMgr::GetInstance()->Get_StaticObjectList()->end();

		for(; iter != iterEnd; )
		{
			if((*iter) == this)
				iter = CCollisionMgr::GetInstance()->Get_StaticObjectList()->erase(iter);
			else
				++iter;
		}
		::Safe_Delete(m_pObjectName);
		delete this;
	}

	return dwRefCnt;
}

