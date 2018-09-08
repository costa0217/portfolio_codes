#include "stdafx.h"
#include "BossSword.h"
#include "Export_Function.h"
#include "Mesh.h"
#include "Player.h"

#include "SDevice.h"

CBossSword::CBossSword(LPDIRECT3DDEVICE9 pGraphicDev, TCHAR* pwstrMeshName, _int iIdx)
: Engine::CGameObject(pGraphicDev)
, m_pEffect(NULL)
, m_pTransCom(NULL)
, m_pMeshCom(NULL)
, m_bSwitch(false)
, m_pBossTransCom(NULL)
, m_pPlayerTransCom(NULL)
, m_fStartTime(0.f)
, m_vAtkDir(0.f, 0.f, 0.f)
, m_bOnce(false)
, m_fAccelSpeed(30.f)
, m_iIndex(iIdx)
, m_bRender(false)
, m_fGoTime(0.f)

, m_pPlayerOBBInfo(NULL)
, m_pMyOBBInfo(NULL)
, m_pvPlayerMin(NULL), m_pvPlayerMax(NULL)
, m_pvMyMin(NULL), m_pvMyMax(NULL)
, m_fPlayerSphereRad(0.f), m_fMySphereRad(0.f)

, m_bSound(false)
{
	ZeroMemory(m_wstrMeshName, sizeof(TCHAR) * 128);
	lstrcpy(m_wstrMeshName,pwstrMeshName);
}

CBossSword::~CBossSword(void)
{
}

HRESULT CBossSword::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Meshes");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vPosition = _vec3(0.0f, 0.f, 0.0f);

	return S_OK;
}

HRESULT CBossSword::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CQuaternion::Create();
	if(NULL == pComponent)	return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Quaternion", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, m_wstrMeshName);
		if(NULL == pComponent)	return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	return S_OK;
}

_int CBossSword::Update(const _float& fTimeDelta)
{
	if(m_pBossTransCom == NULL)
	{
		Set_SettingData();	
	}
	if(!m_bSwitch)	return 0;

	m_fMySphereRad = D3DXVec3Length(&(*m_pvMyMax - *m_pvMyMin)) * m_pTransCom->m_vScale.x;
	m_fPlayerSphereRad = D3DXVec3Length(&(*m_pvPlayerMax - *m_pvPlayerMin)) * m_pPlayerTransCom->m_vScale.x;

	if(m_fStartTime > m_fGoTime * 0.5f)
		m_bRender = true;

	m_fStartTime += fTimeDelta;
	if(m_fStartTime > 4.f - (m_fGoTime * 1.5f))
	{
		if(!m_bSound)
		{
			CSDevice::GetInstance()->SoundPlay(SD_BOSSSWORDSHOT, 0);
			m_bSound = true;
		}
		m_bOnce = true;
	}
	else
	{
		_vec3 vPlayerHeadPos;
		memcpy(&vPlayerHeadPos, &m_pMatrix->m[3][0], sizeof(_vec3));
		D3DXVec3TransformCoord(&vPlayerHeadPos, &vPlayerHeadPos, &m_pPlayerTransCom->m_matWorld);			
		m_vAtkDir = vPlayerHeadPos - m_pTransCom->m_vPosition;
	}
	
	D3DXVec3Normalize(&m_vAtkDir, &m_vAtkDir);
	D3DXVec3Normalize(&m_pTransCom->m_vDir, &m_pTransCom->m_vDir);
	
	m_pTransCom->m_fQuatAngle = acosf(D3DXVec3Dot(&m_vAtkDir, &_vec3(0.f, 1.f, 0.f)));

	D3DXVec3Cross(&m_pTransCom->m_vCross, &_vec3(0.f, 1.f, 0.f), &m_vAtkDir);
	D3DXVec3Normalize(&m_pTransCom->m_vCross, &m_pTransCom->m_vCross);


	m_pTransCom->m_vDir = m_vAtkDir;
	if(m_bOnce)
	{
		m_fAccelSpeed += fTimeDelta * 70.f;
		m_pTransCom->m_vPosition += m_pTransCom->m_vDir * fTimeDelta * m_fAccelSpeed;
	}

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
	Engine::CGameObject::Update(fTimeDelta);

	Check_Collision();

	if(m_fStartTime > 10.f)
	{
		Set_DataInit();
	}

	return 0;
}
 
void CBossSword::Render(void)
{
	if(NULL == m_pEffect)
		return;

	if(!m_bSwitch)	return;
	if(!m_bRender)	return;

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
void CBossSword::Check_Collision(void)
{
	if(Engine::Collision_SphereSphere(m_pTransCom->m_vPosition, m_fMySphereRad, m_pPlayerTransCom->m_vPosition, m_fPlayerSphereRad))
	{
		if(Engine::Collision_OBB(*m_pMyOBBInfo, m_pTransCom->m_matWorld, *m_pPlayerOBBInfo, m_pPlayerTransCom->m_matWorld))
		{
			((CPlayer*)m_pPlayer)->Set_Damaged(true);
		}
	}
}

CBossSword* CBossSword::Create(LPDIRECT3DDEVICE9 pGraphicDev, TCHAR* pwstrMeshName, _int iIdx)
{
	CBossSword*		pGameObject = new CBossSword(pGraphicDev, pwstrMeshName, iIdx);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CBossSword Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CBossSword::Set_ContantTable(void)
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
	m_pEffect->SetVector("g_vLightAmbient", &_vec4((_float*)&pLightInfo->Ambient));	
}

_ulong CBossSword::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{	
		delete this;
	}
	return dwRefCnt;
}

void CBossSword::Set_Pos(_vec3 vPos)
{
	m_pTransCom->m_vPosition = vPos;
}

void CBossSword::Set_DataInit(void)
{
	m_bSound		= false;
	m_bRender		= false;
	m_bSwitch		= false;
	m_bOnce			= false;
	m_fStartTime	= 0.f;
	m_fAccelSpeed	= 30.f;
	m_fGoTime		= rand()%10 * 0.1f;
}

void CBossSword::Set_SettingData(void)
{
	m_pBossTransCom = (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Boss");
	m_pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Player");
	m_pTransCom->m_vScale = m_pBossTransCom->m_vScale * 2.f;

	Engine::CDynamicMesh* pPlayerMeshCom = (Engine::CDynamicMesh*)Engine::Get_Component(L"Com_Mesh", L"GameLogic", L"Player");		
	m_pMatrix = pPlayerMeshCom->Get_FrameMatrix("BN_Hair_B_2");

	Engine::CMesh*	 pPlayerMeshcom2 = (Engine::CMesh*)Engine::Get_Component(L"Com_Mesh", L"GameLogic", L"Player");
	m_pPlayerOBBInfo = pPlayerMeshcom2->Get_OBBInfo();
	m_pMyOBBInfo = m_pMeshCom->Get_OBBInfo();

	m_pMeshCom->Get_pMinMax(m_pvMyMin, m_pvMyMax);
	pPlayerMeshcom2->Get_pMinMax(m_pvPlayerMin, m_pvPlayerMax);
}

