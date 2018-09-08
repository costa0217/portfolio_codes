#include "stdafx.h"
#include "FireTrail.h"
#include "Export_Function.h"
#include "Mesh.h"
#include "SDevice.h"


CFireTrail::CFireTrail(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pEffect(NULL)
, m_pTransCom(NULL)
, m_pMeshCom(NULL)
, m_bSwitch(false)

, m_fAccTime(0.f)
, m_vViewPos(0.f, 0.f, 0.f)
, m_fAlpha(1.f)
, m_bSound(false)
{

}

CFireTrail::~CFireTrail(void)
{

}


HRESULT CFireTrail::Initialize(_vec3 vPos, _vec3 vDir)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_SpriteBlackAlpha");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vPosition = vPos + _vec3(0.f, 1.f, 0.f);
	m_pTransCom->m_vDir	= vDir;

	m_pFrame->Initialize(8, 4, 30.f);

	m_pTransCom->m_vScale =_vec3(0.07f, 0.07f, 0.07f);

	

	return S_OK;
}

HRESULT CFireTrail::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CQuaternion::Create();
	if(NULL == pComponent)	return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TransCom", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_FireTrail");
	if(NULL == pComponent)	return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	// For.Frame Component
	pComponent = m_pFrame = Engine::CFrame::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Frame", pComponent));

	return S_OK;
}

_int CFireTrail::Update(const _float& fTimeDelta)
{
	m_fAccTime += fTimeDelta;

	//if(!m_bSound)
	{
		/*if(CSDevice::GetInstance()->SoundPlaying(SD_FireTrail))
			CSDevice::GetInstance()->SoundPlay(SD_FireTrail, 0);*/

		m_bSound = true;
	}

	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXVec3TransformCoord(&m_vViewPos, &m_pTransCom->m_vPosition, &matView); 

	m_pTransCom->m_fQuatAngle = acosf(D3DXVec3Dot(&m_pTransCom->m_vDir, &_vec3(0.f, 1.f, 0.f)));

	D3DXVec3Cross(&m_pTransCom->m_vCross, &_vec3(0.f, 1.f, 0.f), &m_pTransCom->m_vDir);
	D3DXVec3Normalize(&m_pTransCom->m_vCross, &m_pTransCom->m_vCross);

	Engine::CGameObject::Update(fTimeDelta);
	if(m_pFrame->m_bEnd)
	{
		return  -1;
	}

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this, m_vViewPos.z);
	



	return 0;
}

void CFireTrail::Render(void)
{
	if(NULL == m_pEffect)	return;
	//if(!m_bSwitch)	return;

	if(!Engine::Get_Frustum()->SphereInFrustum(&m_pTransCom->m_vPosition, 3.f))
		return;

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

CFireTrail* CFireTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _vec3 vDir)
{
	CFireTrail*		pEffect = new CFireTrail(pGraphicDev);

	if(FAILED(pEffect->Initialize(vPos, vDir)))
	{
		MSG_BOX("CSmokeSphere Create Failed");
		::Safe_Release(pEffect);
	}
	return pEffect;
}

void CFireTrail::Set_ContantTable(void)
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

	m_pEffect->SetFloat("g_fAlpha", m_fAlpha);
	m_pEffect->SetVector("g_vColor", &_vec4(1.f,1.f,1.f,1.f));
}

_ulong CFireTrail::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{	
		delete this;
	}
	return dwRefCnt;
}

