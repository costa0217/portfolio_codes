#include "stdafx.h"
#include "FireBall.h"
#include "Export_Function.h"
#include "Mesh.h"
#include "Player.h"
#include "FireTrail.h"

#include "Layer.h"
#include "SDevice.h"


CFireBall::CFireBall(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pEffect(NULL)
, m_pTransCom(NULL)
, m_pMeshCom(NULL)
, m_bSwitch(false)
, m_pPlayerTransCom(NULL)
, m_pBossTransCom(NULL)

, m_pPlayerOBBInfo(NULL)
, m_pMyOBBInfo(NULL)
, m_pvPlayerMin(NULL), m_pvPlayerMax(NULL)
, m_pvMyMin(NULL), m_pvMyMax(NULL)
, m_fPlayerSphereRad(0.f), m_fMySphereRad(0.f)
, m_pSphereMesh(NULL)
, m_pSphereAdjancey(NULL)
, m_fAccTime(0.f)
, m_vAtkDir(0.f, 0.f, 0.f)
, m_fSphereRad(0.f)
, m_fTrailTime(0.3f)
, m_bShotSound(false)
{

}

CFireBall::~CFireBall(void)
{

}



HRESULT CFireBall::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Default");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vPosition = _vec3(0.0f, 0.f, 0.0f);

	m_pFrame->Initialize(4, 8, 30.f);


	return S_OK;
}

HRESULT CFireBall::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)	return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TransCom", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_FireBall");
	if(NULL == pComponent)	return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	// For.Frame Component
	pComponent = m_pFrame = Engine::CFrame::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Frame", pComponent));

	return S_OK;
}

_int CFireBall::Update(const _float& fTimeDelta)
{
	
	if(m_pPlayerTransCom == NULL)
	{
		Set_SettingData();	
	}
	
	if(!m_bSwitch)	return 0;
	m_fAccTime += fTimeDelta;

	//m_fMySphereRad = D3DXVec3Length(&(*m_pvMyMax - *m_pvMyMin)) * 0.34f;

	m_fPlayerSphereRad = D3DXVec3Length(&(*m_pvPlayerMax - *m_pvPlayerMin)) * m_pPlayerTransCom->m_vScale.x;

	if(m_fAccTime <= 3.f)
	{
		if(!CSDevice::GetInstance()->SoundPlaying(SD_MateoWait))
		{
			CSDevice::GetInstance()->SoundPlay(SD_MateoWait, 0);
		}
		float fMul = cosf(D3DX_PI * m_fAccTime * 3.f);
		fMul > 0 ? fMul *= 9 : fMul *= 5;
		m_pTransCom->m_vScale	+= /*m_pBossTransCom->m_vScale * 0.5f + */_vec3(fTimeDelta, fTimeDelta, fTimeDelta) * (float)fMul * 0.01f;

		m_fSphereRad = D3DXVec3Length(&(*m_pvMyMax - *m_pvMyMin)) * 0.34f;
		m_fSphereRad *= m_pTransCom->m_vScale.x;

		m_vAtkDir = m_pPlayerTransCom->m_vPosition - m_pTransCom->m_vPosition;
		D3DXVec3Normalize(&m_vAtkDir, &m_vAtkDir);
	}
	else
	{
		if(!m_bShotSound)
		{
			CSDevice::GetInstance()->SoundPlay(SD_MateoShot, 0);
			m_bShotSound = true;
		}
		

		m_pTransCom->m_vPosition += m_vAtkDir * fTimeDelta * 20.f;

		m_fTrailTime += fTimeDelta;
		if(m_fTrailTime > 0.2f)
		{
			Create_Trail();
			m_fTrailTime = 0.f;
		}
	}	
   	

	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = (D3DX_PI * 2.f) * m_fAccTime;
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_X] = (D3DX_PI) * m_fAccTime;


	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
	Engine::CGameObject::Update(fTimeDelta);

	//mat

	Check_Collision();

	if(m_fAccTime > 10.f)
	{
		Set_DataInit();
	}

	return 0;
}

void CFireBall::Render(void)
{
	if(NULL == m_pEffect)
		return;

	if(!m_bSwitch)	return;

	if(!Engine::Get_Frustum()->SphereInFrustum(&m_pTransCom->m_vPosition, 3.f))
		return;

	Set_ContantTable();

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pMeshCom->Render_MeshForShader(m_pEffect, true);

#ifdef _DEBUG
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
		m_pSphereMesh->DrawSubset(0);

		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
#endif	

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
	m_pEffect->SetTexture("g_NormalTexture", NULL);
	m_pEffect->SetTexture("g_SpecularTexture", NULL);
}

CFireBall* CFireBall::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFireBall*		pGameObject = new CFireBall(pGraphicDev);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CFireBall Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CFireBall::Check_Collision(void)
{
	if(Engine::Collision_SphereSphere(m_pTransCom->m_vPosition, m_fSphereRad, m_pPlayerTransCom->m_vPosition, m_fPlayerSphereRad))
	{
		/*if(Engine::Collision_OBB(*m_pMyOBBInfo, m_pTransCom->m_matWorld, *m_pPlayerOBBInfo, m_pPlayerTransCom->m_matWorld))
		{*/
			((CPlayer*)m_pPlayer)->Set_Damaged(true);
		//}
	} 
}

void CFireBall::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);	

	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if(NULL == pLightInfo)
		return;

	m_pEffect->SetInt("g_ICntX", m_pFrame->m_iCntX);
	m_pEffect->SetInt("g_ICntY", m_pFrame->m_iCntY);
	m_pEffect->SetInt("g_iFrameCnt", int(m_pFrame->m_fFrameX));
	m_pEffect->SetInt("g_iFrameHeight", m_pFrame->m_iFrameHeight);

	m_pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	m_pEffect->SetVector("g_vLightDiffuse", &_vec4((_float*)&pLightInfo->Diffuse));
	m_pEffect->SetVector("g_vLightAmbient", &_vec4((_float*)&pLightInfo->Ambient));	
}


void CFireBall::Set_SettingData(void)
{	
	m_pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Player");
	m_pBossTransCom = (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Boss");
	m_pTransCom->m_vScale = m_pBossTransCom->m_vScale * 0.5f;

	Engine::CDynamicMesh* pPlayerMeshCom = (Engine::CDynamicMesh*)Engine::Get_Component(L"Com_Mesh", L"GameLogic", L"Player");		
	//m_pMatrix = pPlayerMeshCom->Get_FrameMatrix("BN_Hair_B_2");

	Engine::CMesh*	 pPlayerMeshcom2 = (Engine::CMesh*)Engine::Get_Component(L"Com_Mesh", L"GameLogic", L"Player");
	m_pPlayerOBBInfo = pPlayerMeshcom2->Get_OBBInfo();
	m_pMyOBBInfo = m_pMeshCom->Get_OBBInfo();

	m_pMeshCom->Get_pMinMax(m_pvMyMin, m_pvMyMax);
	pPlayerMeshcom2->Get_pMinMax(m_pvPlayerMin, m_pvPlayerMax);


	m_fSphereRad = D3DXVec3Length(&(*m_pvMyMax - *m_pvMyMin)) * 0.34f;
#ifdef _DEBUG
	D3DXCreateSphere(m_pGraphicDev, m_fSphereRad, 12, 12, &m_pSphereMesh, &m_pSphereAdjancey);
#endif
	m_fSphereRad *= m_pTransCom->m_vScale.x;
	
}

void CFireBall::Set_Pos(_vec3 vPos)
{
	m_pTransCom->m_vPosition = vPos;
}

void CFireBall::Create_Trail(void)
{
	list<Engine::CGameObject*>* m_pTrailList = m_pGameLogicLayer->Find_ObjectList(L"FireTrail");
	map<const TCHAR*, list<Engine::CGameObject*>>* m_pmapObjList = m_pGameLogicLayer->Get_pmapObjList();

	Engine::CGameObject* pGameObject = CFireTrail::Create(m_pGraphicDev, m_pTransCom->m_vPosition, m_vAtkDir);

	if(NULL == pGameObject)	return;	
	if(NULL == m_pTrailList)
	{
		list<CGameObject*>		NewObjectList;		
		NewObjectList.push_back(pGameObject);
		m_pmapObjList->insert(map<const TCHAR*, list<CGameObject*>>::value_type(L"FireTrail", NewObjectList));
	}
	else
	{
		m_pGameLogicLayer->Ready_Object(L"FireTrail", pGameObject);
	}
}

void CFireBall::Set_DataInit(void)
{
	m_bSwitch		= false;	
	m_fAccTime		= 0.f;
	m_vAtkDir		= _vec3(0.f, 0.f, 0.f);
	//m_fMySphereRad = D3DXVec3Length(&(*m_pvMyMax - *m_pvMyMin)) * 0.34f;
	m_pTransCom->m_vScale = m_pBossTransCom->m_vScale * 0.5f;
	m_fSphereRad = D3DXVec3Length(&(*m_pvMyMax - *m_pvMyMin)) * 0.34f * m_pTransCom->m_vScale.x;	

	m_bShotSound = false;
}

_ulong CFireBall::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{	
#ifdef _DEBUG
		if(Engine::Safe_Release(m_pSphereMesh))
			MSG_BOX("SphereMesh Release Failed");
#endif
		delete this;
	}
	return dwRefCnt;
}

