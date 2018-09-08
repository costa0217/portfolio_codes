#include "stdafx.h"
#include "Wang.h"
#include "Export_Function.h"
#include "Mesh.h"
#include "SDevice.h"

CWang::CWang(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pEffect(NULL)
, m_pTransCom(NULL)
, m_pMeshCom(NULL)
, m_bSwitch(false)

, m_fAccTime(0.f)
, m_fAlpha(0.f)
, m_bSound(false)
{

}

CWang::~CWang(void)
{

}

HRESULT CWang::Initialize(_vec3 vPos)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_SpriteBlackAlpha");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vPosition = vPos + _vec3(0.f, 2.f, 0.f);
	m_pTransCom->m_vScale = _vec3(0.7f, 0.7f, 0.7f);
	//m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Z] = D3DXToRadian(90.f);

	m_pFrame->Initialize(1, 1, 0.f);

	//m_vColor = _vec4(0.7f, 0.4f, 0.f, 1.f);

	return S_OK;
}

HRESULT CWang::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)	return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_TransCom", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CStaticMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Wand");
	if(NULL == pComponent)	return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	// For.Frame Component
	pComponent = m_pFrame = Engine::CFrame::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Frame", pComponent));

	return S_OK;
}

_int CWang::Update(const _float& fTimeDelta)
{
	//if(!m_bSwitch){	return 0; }

	m_fAccTime += fTimeDelta;

	if(m_fAccTime < 3.5f)
	{
		m_pTransCom->m_vScale -= _vec3(fTimeDelta, 0.f, fTimeDelta) * 0.1f;
		
		m_fAlpha += fTimeDelta * 0.2f;
	}
	else
	{
		if(!m_bSound)
		{
			CSDevice::GetInstance()->SoundPlay(SD_Wang, 0);	
			m_bSound = true;
		}
		

		m_pTransCom->m_vScale += _vec3(m_fAccTime, 0.f, m_fAccTime) * 0.1f;

		m_fAlpha -= fTimeDelta * 0.25f;
	}

	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] += m_fAccTime * 0.1f;

	if(m_pTransCom->m_vScale.x > 25.f)
	{
		return -1;
	}
	//m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = (D3DX_PI * 2.f)* m_fAccTime * 2.f;

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this/*, m_vViewPos.z*/);
	Engine::CGameObject::Update(fTimeDelta);

	return 0;
}

void CWang::Render(void)
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

CWang* CWang::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CWang*		pGameObject = new CWang(pGraphicDev);

	if(FAILED(pGameObject->Initialize(vPos)))
	{
		MSG_BOX("CWang Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}


void CWang::Set_ContantTable(void)
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


_ulong CWang::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{	
		delete this;
	}
	return dwRefCnt;
}

