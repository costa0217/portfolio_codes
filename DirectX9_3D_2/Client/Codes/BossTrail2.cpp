#include "stdafx.h"
#include "BossTrail2.h"

#include "Boss.h"
#include "Export_Function.h"

CBossTrail2::CBossTrail2(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pvEndPos(NULL)
, m_pvStartPos(NULL)
, m_pParentMatrix(NULL)
, m_pTransCom(NULL)
, m_pTextureCom(NULL)
, m_pFrame(NULL)
, m_pBuffer(NULL)
, m_pEffect(NULL)
, m_vViewPos(0.f, 0.f, 0.f)
, m_fSetTime(0.f)
{
	ZeroMemory(m_vEPos, sizeof(_vec3) * 20);
	ZeroMemory(m_vSPos, sizeof(_vec3) * 20);
}

CBossTrail2::~CBossTrail2(void)
{

}

HRESULT CBossTrail2::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_SpriteBlack");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pFrame->Initialize(1, 1, 0.f);

	m_pVTXInfo = new Engine::VTXTEX[40];
	m_pBuffer->Get_VtxInfo(m_pVTXInfo);

	return S_OK;
}

HRESULT CBossTrail2::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBuffer = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STAGE, L"Buffer_Trail3");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Trail");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	// For.Frame Component
	pComponent = m_pFrame = Engine::CFrame::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Frame", pComponent));


	return S_OK;
}

_int CBossTrail2::Update(const _float& fTimeDelta)
{
	if(NULL == m_pParentMatrix)
	{	
		Engine::CTransform* pBossTransCom = (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Boss");
		m_pParentMatrix = &pBossTransCom->m_matWorld;
	}

	Create_trail(fTimeDelta);
	Set_TrailVB();



	Engine::CGameObject::Update(fTimeDelta);
	m_pTransCom->m_matWorld = *m_pParentMatrix;


	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXVec3TransformCoord(&m_vViewPos, &m_pTransCom->m_vPosition, &matView); 

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this, m_vViewPos.z);

	

	return 0;
}

void CBossTrail2::Render(void)
{
	if(NULL == m_pEffect)
		return;

		if(((CBoss*)m_pBoss)->Get_State() == CBoss::Boss_ATK_01
		||((CBoss*)m_pBoss)->Get_State() == CBoss::Boss_ATK_02
		||((CBoss*)m_pBoss)->Get_State() == CBoss::Boss_SK_Firing_01
		||((CBoss*)m_pBoss)->Get_State() == CBoss::Boss_SK_Firing_02
		||((CBoss*)m_pBoss)->Get_State() == CBoss::Boss_SK_Firing_03
		||((CBoss*)m_pBoss)->Get_State() == CBoss::Boss_SK_Firing_04
		||((CBoss*)m_pBoss)->Get_State() == CBoss::Boss_SK_Firing_05)
	{
		Set_ContantTable();

		// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
		m_pEffect->Begin(NULL, 0);
		m_pEffect->BeginPass(0);


		m_pBuffer->Render(NULL);

		m_pEffect->EndPass();
		m_pEffect->End();	

		m_pEffect->SetTexture("g_BaseTexture", NULL);
		m_pEffect->SetTexture("g_PlusTexture", NULL);
	}	
}

CBossTrail2* CBossTrail2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossTrail2*		pTrail = new CBossTrail2(pGraphicDev);

	if(FAILED(pTrail->Initialize()))
	{
		MSG_BOX("CBossTrail Create Faild");
		::Safe_Release(pTrail);
	}
	return pTrail;
}

void CBossTrail2::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	m_pEffect->SetInt("g_ICntX", m_pFrame->m_iCntX);
	m_pEffect->SetInt("g_ICntY", m_pFrame->m_iCntY);
	m_pEffect->SetInt("g_iFrameCnt", int(m_pFrame->m_fFrameX));
	m_pEffect->SetInt("g_iFrameHeight", m_pFrame->m_iFrameHeight);

	m_pEffect->SetVector("g_vColor", &_vec4(1.f, 1.f, 1.f, 1.f));

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", 17);	
}

void CBossTrail2::Create_trail(const _float& fTimeDelta)
{
	for(int i = 20 - 2; i > -1; --i)
	{
		m_vSPos[i+1] = m_vSPos[i];
		m_vEPos[i+1] = m_vEPos[i];
	}

	m_vSPos[0] = *((CBoss*)m_pBoss)->Get_StartPos2();
	m_vEPos[0] = *((CBoss*)m_pBoss)->Get_EndPos2();

	/*m_pVTXInfo[0].vPos = *((CBoss*)m_pBoss)->Get_EndPos2();
	m_pVTXInfo[20].vPos = *((CBoss*)m_pBoss)->Get_StartPos2();


	for(int i = 19; i > 0; --i)
	{
		m_pVTXInfo[i].vPos			=	m_pVTXInfo[i - 1].vPos + (m_pVTXInfo[i].vPos - m_pVTXInfo[i - 1].vPos) * fTimeDelta * 10.f;
		m_pVTXInfo[i + 20].vPos		=	m_pVTXInfo[i + 20 - 1].vPos + (m_pVTXInfo[i + 20].vPos - m_pVTXInfo[i + 20 - 1].vPos) * fTimeDelta * 50.f;
	}*/
}

void CBossTrail2::Set_TrailVB(void)
{
	for(int i = 0; i < 40; ++i)
	{
		if(i < 20)
		{
			m_pVTXInfo[i].vPos = m_vEPos[i];
		}
		else
		{
			m_pVTXInfo[i].vPos = m_vSPos[i - 20];
		}
	}
	m_pBuffer->Set_VtxInfo(m_pVTXInfo);
}

void CBossTrail2::Set_StartPos(_vec3* pvEndPos, _vec3* pvStartPos)
{
	m_pvStartPos = pvStartPos;
	m_pvEndPos = pvEndPos;
}

_ulong CBossTrail2::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		Engine::Safe_Delete_Array(m_pVTXInfo);
		delete this;
	}

	return dwRefCnt;
}

