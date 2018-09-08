#include "stdafx.h"
#include "Shield.h"
#include "Export_Function.h"

CShield::CShield(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pEffect(NULL)
, m_pParentMatrix(NULL)
, m_pParentWorldMatrix(NULL)
, m_pTransCom(NULL)
, m_pMeshCom(NULL)
{
}

CShield::~CShield(void)
{
}

HRESULT CShield::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Meshes");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vPosition = _vec3(0.0f, 0.f, 0.0f);
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Z] = D3DXToRadian(90.0f);
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian(180.0f);
	//m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_X] = D3DXToRadian(90.0f);

	return S_OK;
}

HRESULT CShield::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Shield");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));


	return S_OK;
}

_int CShield::Update(const _float& fTimeDelta)
{
	if(NULL == m_pParentMatrix)
	{	
		Engine::CDynamicMesh* pPlayerMeshCom = (Engine::CDynamicMesh*)Engine::Get_Component(L"Com_Mesh", L"GameLogic", L"Player");
		m_pParentMatrix = pPlayerMeshCom->Get_FrameMatrix("BN_Weapon_L");

		Engine::CTransform* pPlayerTransCom =  (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Player");
		m_pParentWorldMatrix = &pPlayerTransCom->m_matWorld;		
	}

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
	Engine::CGameObject::Update(fTimeDelta);

	m_pTransCom->m_matWorld = m_pTransCom->m_matWorld * *m_pParentMatrix * *m_pParentWorldMatrix;

	

	return 0;
}

void CShield::Render(void)
{
	if(NULL == m_pEffect)
		return;

	

	Set_ContantTable();

	// ������ɷ����������������� ��������ʰ�, ���̴��� �׸��ڽ��ϴ�.
	m_pEffect->Begin(NULL, 0);

	m_pEffect->BeginPass(0);

	m_pMeshCom->Render_MeshForShader(m_pEffect, true);

	m_pEffect->EndPass();

	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
	m_pEffect->SetTexture("g_NormalTexture", NULL);
	m_pEffect->SetTexture("g_SpecularTexture", NULL);

}

CShield* CShield::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShield*		pGameObject = new CShield(pGraphicDev);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CTombStone Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CShield::Set_ContantTable(void)
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

_ulong CShield::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{	
		delete this;
	}

	return dwRefCnt;
}
