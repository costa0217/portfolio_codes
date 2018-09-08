#include "stdafx.h"
#include "Swirl.h"
#include "Export_Function.h"
#include "Mesh.h"

#include "SDevice.h"

CSwirl::CSwirl(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pEffect(NULL)
, m_pTransCom(NULL)
, m_pMeshCom(NULL)
, m_bSwitch(false)

, m_fAccTime(0.f)
, m_fAlpha(0.0f)
, m_iIdx(0)
{

}

CSwirl::~CSwirl(void)
{

}

HRESULT CSwirl::Initialize(_vec3 vPos, int _iIdx)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_SpriteBlackAlpha");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vPosition = vPos/* + _vec3(0.f, 2.f, 0.f)*/;
	m_pTransCom->m_vScale = _vec3(0.9f, 0.6f, 0.9f);
	//m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Z] = D3DXToRadian(90.f);

	m_pFrame->Initialize(1, 1, 0.f);

	//m_vColor = _vec4(0.7f, 0.4f, 0.f, 1.f);

	m_iIdx = _iIdx;
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DX_PI * m_iIdx;

	return S_OK;
}

HRESULT CSwirl::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)	return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TransCom", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Swirl");
	if(NULL == pComponent)	return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	// For.Frame Component
	pComponent = m_pFrame = Engine::CFrame::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Frame", pComponent));

	return S_OK;
}

_int CSwirl::Update(const _float& fTimeDelta)
{
	//if(!m_bSwitch){	return 0; }

	m_fAccTime += fTimeDelta;

	if(m_fAccTime < 1.f)
	{
		if(!CSDevice::GetInstance()->SoundPlaying(SD_Wind))
			CSDevice::GetInstance()->SoundPlay(SD_Wind, 0);

		m_fAlpha += fTimeDelta * 0.3f;
	}
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] += m_fAccTime * 0.05f;

	m_pTransCom->m_vScale -= _vec3(fTimeDelta, 0.f, fTimeDelta) * 0.14f;

	if(m_fAccTime > 4.f)
	{
		m_fAlpha -= fTimeDelta * 0.5f;
	}
	if(m_fAccTime > 5.f)
		return -1;

	
	//m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = (D3DX_PI * 2.f)* m_fAccTime * 2.f;

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this/*, m_vViewPos.z*/);
	Engine::CGameObject::Update(fTimeDelta);

	return 0;
}

void CSwirl::Render(void)
{
	if(NULL == m_pEffect)
		return;

	//if(!m_bSwitch) return;

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

CSwirl* CSwirl::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos, _int iIdx)
{
	CSwirl*		pGameObject = new CSwirl(pGraphicDev);

	if(FAILED(pGameObject->Initialize(vPos,iIdx )))
	{
		MSG_BOX("CSwirl Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}


void CSwirl::Set_ContantTable(void)
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

	m_pEffect->SetInt("g_ICntX", m_pFrame->m_iCntX);
	m_pEffect->SetInt("g_ICntY", m_pFrame->m_iCntY);
	m_pEffect->SetInt("g_iFrameCnt", int(m_pFrame->m_fFrameX));
	m_pEffect->SetInt("g_iFrameHeight", m_pFrame->m_iFrameHeight);

	m_pEffect->SetVector("g_vColor", &_vec4(1.f,1.f,1.f,1.f));

	m_pEffect->SetFloat("g_fAlpha", m_fAlpha);
}


_ulong CSwirl::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{	
		CSDevice::GetInstance()->SoundStop(SD_Wind);
		delete this;
	}
	return dwRefCnt;
}

