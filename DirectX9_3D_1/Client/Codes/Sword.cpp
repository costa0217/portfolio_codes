#include "stdafx.h"
#include "Sword.h"

#include "Export_Function.h"
#include "EffectMgr.h"

#include "Player.h"

SWORDINFO	g_tSwordInfo;

CSword::CSword(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pEffect(NULL)
, m_pParentMatrix(NULL)
, m_pParentWorldMatrix(NULL)
{
	D3DXMatrixIdentity(&m_matPreView);
}

CSword::~CSword(void)
{

}

HRESULT CSword::Initialize(_matrix mat, TCHAR* pMeshName)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Weapon");
	if(NULL == m_pEffect)		return E_FAIL;	
	m_pShadowEffect = Engine::Get_EffectHandle(L"Shader_Shadow");
	if(NULL == m_pShadowEffect)		return E_FAIL;	
	m_pVelocityEffect = Engine::Get_EffectHandle(L"Shader_Velocity");
	if(NULL == m_pVelocityEffect)		return E_FAIL;	

	if(FAILED(Add_Component(mat, pMeshName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSword::Add_Component(_matrix mat, TCHAR* pMeshName)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Mesh Component
	TCHAR	szName[128];
	lstrcpy(szName, pMeshName);
	pComponent = m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STATIC, szName);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	return S_OK;
}

_int CSword::Update(const _float& fTimeDelta)
{
	if(NULL == m_pParentMatrix)
	{	
		Engine::CPlayerDynamicMesh* pPlayerMeshCom = CPlayer::GetInstance()->Get_MeshCom();
		m_pParentMatrix = pPlayerMeshCom->Get_FrameMatrix("ValveBiped_Anim_Attachment_RH");

		Engine::CTransform* pPlayerTransCom =   CPlayer::GetInstance()->Get_TransCom();
		m_pParentWorldMatrix = &pPlayerTransCom->m_matWorld;		
	}

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_SHADOW, this);
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_VELOCITY, this);

	Engine::CGameObject::Update(fTimeDelta);

	m_pTransCom->m_matWorld = m_pTransCom->m_matWorld * *m_pParentMatrix * *m_pParentWorldMatrix;

	return 0;
}

void CSword::Render(void)
{
	if(NULL == m_pEffect)
		return;

	//m_pTransCom->m_matWorld = m_pTransCom->m_matWorld * *m_pParentMatrix * *m_pParentWorldMatrix;

	Set_ContantTable();

	// ¸Þ½¬ ·»´õ
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pMeshCom->Render_MeshForShader(m_pEffect, true, &(g_tSwordInfo.matColor));

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
}
void CSword::Render_Shadow(void)
{
	if(NULL == m_pShadowEffect)		return;

	Set_ContantTable();

	//±¤¿ø
	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if(NULL == pLightInfo)	return;

	_matrix			matLightView, matLightProj;
	D3DXMatrixIdentity(&matLightView);
	D3DXMatrixIdentity(&matLightProj);
	_vec3	vAt, vUp, vEye;
	_vec3	vLightDir =  _vec3(pLightInfo->Direction);
	D3DXVec3Normalize(&vLightDir,  &vLightDir);

	_vec3	vParentPos(0.f, 0.f, 0.f);
	memcpy(&vParentPos, &m_pParentWorldMatrix->m[3][0], sizeof(_vec3));

	vEye = vParentPos + ( vLightDir * -100.f);
	vAt = vEye + vLightDir;
	vUp = _vec3(0.f, 1.f, 0.f);


	D3DXMatrixLookAtLH(&matLightView, &vEye, &vAt, &vUp);
	D3DXMatrixPerspectiveFovLH(&matLightProj, D3DX_PI * 0.4f, float(WINSIZEX) / WINSIZEY,  0.1f, 1000.f);

	m_pShadowEffect->SetMatrix("g_matLightWV", &(m_pTransCom->m_matWorld * matLightView));
	m_pShadowEffect->SetMatrix("g_matLightProj", &matLightProj);

	m_pShadowEffect->Begin(NULL, 0);
	m_pShadowEffect->BeginPass(0);

	m_pMeshCom->Render_MeshForShader(m_pShadowEffect, true, &(g_tSwordInfo.matColor));

	m_pShadowEffect->EndPass();
	m_pShadowEffect->End();	
}

void CSword::Render_Velocity(void)
{
	m_pVelocityEffect->SetTexture("g_BaseTexture", Engine::CRenderTargetMgr::GetInstance()->Get_TargetTexture(L"Target_Blend"));

	_matrix	matView, matProj, matWV, matPWV;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	matWV		= m_pTransCom->m_matWorld * matView;
	matPWV		= m_pTransCom->m_matWorld * m_matPreView;

	m_pVelocityEffect->SetMatrix("g_matWV",	&matWV);
	m_pVelocityEffect->SetMatrix("g_matPWV",	&matPWV);
	m_pVelocityEffect->SetMatrix("g_matProj",	&matProj);

	m_pVelocityEffect->Begin(NULL, 0);
	m_pVelocityEffect->BeginPass(0);

	m_pMeshCom->Render_MeshForShaderVelocity(m_pVelocityEffect, true);

	m_pVelocityEffect->EndPass();
	m_pVelocityEffect->End();	

	m_pVelocityEffect->SetTexture("g_BaseTexture", NULL);
	m_matPreView = matView;
}



CSword* CSword::Create(LPDIRECT3DDEVICE9 pGraphicDev, _matrix mat, TCHAR* pMeshName)
{
	CSword*		pGameObject = new CSword(pGraphicDev);

	if(FAILED(pGameObject->Initialize(mat,  pMeshName)))
	{
		MSG_BOX("CSword Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CSword::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj, matInv;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	_vec3		vCamera;
	D3DXMatrixInverse(&matInv, NULL, &matView);
	memcpy(&vCamera, &matInv.m[3][0], sizeof(_vec3));

	m_pEffect->SetVector("g_WorldCamPos", &_vec4(vCamera, 1.f));
	m_pEffect->SetMatrix("g_matProj", &matProj);
}

_ulong CSword::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{	
		delete this;
	}

	return dwRefCnt;
}

