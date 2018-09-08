#include "stdafx.h"
#include "Trail.h"

#include "Player.h"
#include "Export_Function.h"

CTrail::CTrail(LPDIRECT3DDEVICE9 pGraphicDev)
: Engine::CGameObject(pGraphicDev)
, m_pvEndPos(NULL)
, m_pvStartPos(NULL)
, m_pParentMatrix(NULL)
, m_pTransCom(NULL)
, m_pTextureCom(NULL)
, m_pBuffer(NULL)
, m_pEffect(NULL)
, m_vViewPos(0.f, 0.f, 0.f)
, m_fSetTime(0.f)
{
	ZeroMemory(m_vSwordEPos, sizeof(_vec3) * 20);
	ZeroMemory(m_vSwordSPos, sizeof(_vec3) * 20);
}

CTrail::~CTrail(void)
{

}

HRESULT CTrail::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Effect");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pVTXInfo = new Engine::VTXTEX[40];
	m_pBuffer->Get_VtxInfo(m_pVTXInfo);
	

	return S_OK;
}

HRESULT CTrail::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBuffer = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STAGE, L"Buffer_Trail");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Buffer", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STAGE, L"Texture_Trail");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Texture", pComponent));

	return S_OK;
}

_int CTrail::Update(const _float& fTimeDelta)
{
	if(NULL == m_pParentMatrix)
	{	
		Engine::CTransform* pPlayerTransCom = (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Player");
		m_pParentMatrix = &pPlayerTransCom->m_matWorld;
	}

	Engine::CGameObject::Update(fTimeDelta);
	m_pTransCom->m_matWorld = *m_pParentMatrix;

	
	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);

	D3DXVec3TransformCoord(&m_vViewPos, &m_pTransCom->m_vPosition, &matView); 

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this, m_vViewPos.z);

	Create_trail(fTimeDelta);
	Set_TrailVB();
	
	return 0;
}

void CTrail::Render(void)
{
	if(NULL == m_pEffect)
		return;

	if( ((CPlayer*)m_pPlayer)->Get_State() != CPlayer::STATE_ATTACK 
		&& ((CPlayer*)m_pPlayer)->Get_State() != CPlayer::STATE_SKILL )
		return;

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

CTrail* CTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrail*		pTrail = new CTrail(pGraphicDev);

	if(FAILED(pTrail->Initialize()))
	{
		MSG_BOX("pTrail Create Faild");
		::Safe_Release(pTrail);
	}
	return pTrail;
}

void CTrail::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	if( ((CPlayer*)m_pPlayer)->Get_State() == CPlayer::STATE_SKILL)
	{
		switch ((((CPlayer*)m_pPlayer)->Get_SkillIndex()))
		{
		case 0:
			m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", 14);	
			m_pTextureCom->Set_ConstantTable(m_pEffect, "g_PlusTexture", 11);
			break;
		case 1:
			m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", 13);	
			m_pTextureCom->Set_ConstantTable(m_pEffect, "g_PlusTexture", 16);
			break;
		case 2:
			m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", 8);	
			m_pTextureCom->Set_ConstantTable(m_pEffect, "g_PlusTexture", 2);
			break;
		case 3:
			m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", 6);	
			m_pTextureCom->Set_ConstantTable(m_pEffect, "g_PlusTexture", 16);
			break;
		}
	}
	else
	{
		m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", 15);	
		m_pTextureCom->Set_ConstantTable(m_pEffect, "g_PlusTexture", 10);
	}		
}

_ulong CTrail::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		Engine::Safe_Delete_Array(m_pVTXInfo);
		delete this;
	}

	return dwRefCnt;
}

void CTrail::Set_TrailVB(void)
{
	for(int i = 0; i < 40; ++i)
	{
		if(i < 20)
		{
			m_pVTXInfo[i].vPos = m_vSwordEPos[i];
		}
		else
		{
			m_pVTXInfo[i].vPos = m_vSwordSPos[i - 20];
		}
	}
	m_pBuffer->Set_VtxInfo(m_pVTXInfo);
}

void CTrail::Create_trail(const _float& fTimeDelta)
{
	for(int i = 20 - 2; i > -1; --i)
	{
		m_vSwordSPos[i+1] = m_vSwordSPos[i];
		m_vSwordEPos[i+1] = m_vSwordEPos[i];
	}

	m_vSwordSPos[0] = *((CPlayer*)m_pPlayer)->Get_SwordStartPos();
	m_vSwordEPos[0] = *((CPlayer*)m_pPlayer)->Get_SwordEndPos();

	/*m_pVTXInfo[0].vPos = *((CPlayer*)m_pPlayer)->Get_SwordEndPos();
	m_pVTXInfo[20].vPos = *((CPlayer*)m_pPlayer)->Get_SwordStartPos();


	for(int i = 19; i > 0; --i)
	{
		m_pVTXInfo[i].vPos			=	m_pVTXInfo[i - 1].vPos + (m_pVTXInfo[i].vPos - m_pVTXInfo[i - 1].vPos) * fTimeDelta * 10.f;
		m_pVTXInfo[i + 20].vPos		=	m_pVTXInfo[i + 20 - 1].vPos + (m_pVTXInfo[i + 20].vPos - m_pVTXInfo[i + 20 - 1].vPos) * fTimeDelta * 50.f;
	}*/
}

void CTrail::Set_WeaponPos(_vec3* pvEndPos, _vec3* pvStartPos)
{
	m_pvStartPos = pvStartPos;
	m_pvEndPos = pvEndPos;
}
