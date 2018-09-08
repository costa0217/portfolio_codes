#include "stdafx.h"
#include "IceThone.h"
#include "Export_Function.h"
#include "Mesh.h"
#include "Player.h"

#include "SDevice.h"

CIceThone::CIceThone(LPDIRECT3DDEVICE9 pGraphicDev, int iIdx)
: Engine::CGameObject(pGraphicDev)
, m_iIndex(iIdx)
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
, m_fAccTime(0.f)
, m_fPlayerSpeed(0.f)

, m_vViewPos(0.f, 0.f, 0.f)
, m_fAlpha(0.7f)
, m_bSound(false)
{
	D3DXMatrixIdentity(&m_matColli);
}

CIceThone::~CIceThone(void)
{

}



HRESULT CIceThone::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_MeshesAlpha");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vPosition = _vec3(0.0f, 0.f, 0.0f);

	return S_OK;
}

HRESULT CIceThone::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)	return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TransCom", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Crystal");
	if(NULL == pComponent)	return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	return S_OK;
}

_int CIceThone::Update(const _float& fTimeDelta)
{
	if(m_pPlayerTransCom == NULL)
	{
		Set_SettingData();	
	}
	if(!m_bSwitch)
	{
		((CPlayer*)m_pPlayer)->Set_Speed(5);
		return 0;
	}
	m_fAccTime += fTimeDelta;
	if(m_fAccTime < 1.f + ((m_iIndex - 1)  * 0.1f))	return 0;

	//Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
	Engine::CGameObject::Update(fTimeDelta);

	Check_Collision();

	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXVec3TransformCoord(&m_vViewPos, &m_pTransCom->m_vPosition, &matView); 


	if(m_fAccTime > 9.f)
	{
		m_fAlpha -= fTimeDelta;
		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this, m_vViewPos.z);
	}
	else
	{
		
		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
	}

	if(m_fAccTime > 10.f)
	{
		CSDevice::GetInstance()->SoundPlay(SD_BROKENICE, 0);
		m_bSwitch = false;
	}

	return 0;
}

void CIceThone::Render(void)
{
	if(NULL == m_pEffect)
		return;

	if(!m_bSwitch)	return;
	if(m_fAccTime < 1.f + ((m_iIndex - 1)  * 0.1f))	return ;

	Set_ContantTable();

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pMeshCom->Render_MeshForShader(m_pEffect, true);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
	m_pEffect->SetTexture("g_NormalTexture", NULL);
	m_pEffect->SetTexture("g_SpecularTexture", NULL);

	
}

CIceThone* CIceThone::Create(LPDIRECT3DDEVICE9 pGraphicDev, int iIdx)
{
	CIceThone*		pGameObject = new CIceThone(pGraphicDev, iIdx);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CFireBall Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CIceThone::Check_Collision(void)
{
	if(Engine::Collision_SphereSphere(m_pTransCom->m_vPosition, m_fMySphereRad, m_pPlayerTransCom->m_vPosition, m_fPlayerSphereRad))
	{
		if(Engine::Collision_OBB(*m_pMyOBBInfo, m_matColli * m_pTransCom->m_matWorld, *m_pPlayerOBBInfo, m_pPlayerTransCom->m_matWorld))
		{
			((CPlayer*)m_pPlayer)->Set_StateSlow(true);
		}
	} 
}

void CIceThone::Set_ContantTable(void)
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

	m_pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));
	m_pEffect->SetVector("g_vLightDiffuse", &_vec4((_float*)&pLightInfo->Diffuse));
	m_pEffect->SetVector("g_vLightAmbient", &_vec4(1.f, 1.f, 1.f, 1.f));	

	m_pEffect->SetFloat("g_fAlpha", m_fAlpha);
}


void CIceThone::Set_SettingData(void)
{	
	m_pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Player");
	m_pBossTransCom = (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Boss");
	m_pTransCom->m_vScale = m_pBossTransCom->m_vScale * 0.7f;
	//m_pTransCom->m_vScale = _vec3(m_pTransCom->m_vScale.x * 2.f * m_iIndex, m_pTransCom->m_vScale.y, m_pTransCom->m_vScale.z * 2.f * m_iIndex);

	Engine::CDynamicMesh* pPlayerMeshCom = (Engine::CDynamicMesh*)Engine::Get_Component(L"Com_Mesh", L"GameLogic", L"Player");		
	//m_pMatrix = pPlayerMeshCom->Get_FrameMatrix("BN_Hair_B_2");

	Engine::CMesh*	 pPlayerMeshcom2 = (Engine::CMesh*)Engine::Get_Component(L"Com_Mesh", L"GameLogic", L"Player");
	m_pPlayerOBBInfo = pPlayerMeshcom2->Get_OBBInfo();
	m_pMyOBBInfo = m_pMeshCom->Get_OBBInfo();

	m_pMeshCom->Get_pMinMax(m_pvMyMin, m_pvMyMax);
	pPlayerMeshcom2->Get_pMinMax(m_pvPlayerMin, m_pvPlayerMax);

	D3DXMatrixScaling(&m_matColli, 0.8f, 0.8f, 0.8f);

	m_fMySphereRad = D3DXVec3Length(&(*m_pvMyMax - *m_pvMyMin)) * m_pTransCom->m_vScale.x;
}

void CIceThone::Set_Pos(_vec3 vPos)
{
	m_pTransCom->m_vPosition = vPos;
}

void CIceThone::Set_DataInit(void)
{
	m_bSwitch		= false;	

	int iRand = rand()%361;
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian(iRand);
	m_fAlpha = 1.f;

	m_bSound = false;


	m_fAccTime = 0.f;
}
_ulong CIceThone::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{	
		delete this;
	}
	return dwRefCnt;
}

