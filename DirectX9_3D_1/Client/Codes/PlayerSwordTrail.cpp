#include "stdafx.h"
#include "PlayerSwordTrail.h"

#include "Export_Function.h"
#include "EffectMgr.h"
#include "Player.h"
#include "RenderTargetMgr.h"

CPlayerSwordTrail::CPlayerSwordTrail(LPDIRECT3DDEVICE9 pGraphicDev)
: CGameObject(pGraphicDev)
, m_pmatSword(NULL)
, m_vSwordStartPoint(0.f, 0.f, 0.f)
, m_vSwordEndPoint(0.f, 0.f, 0.f)
, m_iImageNum(0)
, m_vColor(0.f, 0.f, 0.f, 0.f)
, m_fTimeDelta(0.f)
, m_fSkillTime(0.f)
, m_pVTXInfo(NULL)
, m_pOriVTXInfo(NULL)
{
	ZeroMemory(m_bTrailCheck, sizeof(_bool) * 2);
}

CPlayerSwordTrail::~CPlayerSwordTrail(void)
{

}

CPlayerSwordTrail* CPlayerSwordTrail::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayerSwordTrail*		pPlayerSwordTrail = new CPlayerSwordTrail(pGraphicDev);

	if(FAILED(pPlayerSwordTrail->Initialize()))
	{
		MSG_BOX("PlayerSwordTrail Create Faild");
		::Safe_Release(pPlayerSwordTrail);
	}
	return pPlayerSwordTrail;
}

HRESULT CPlayerSwordTrail::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_PlayerSwordTrail");
	if(NULL == m_pEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	m_pVTXInfo		= new Engine::VTXTEX[80];
	m_pOriVTXInfo 	= new Engine::VTXTEX[80];
	m_pBufferCom->Get_VtxInfo(m_pVTXInfo);

	return S_OK;
}

HRESULT CPlayerSwordTrail::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Transform", pComponent));

	// For.VIBuffer Component
	pComponent = m_pBufferCom = (Engine::CVIBuffer*)Engine::Clone_Resource(RESOURCE_STATIC, L"Buffer_PlayerTrailTex");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Buffer", pComponent));

	// For.Texture Component
	pComponent = m_pTextureCom = (Engine::CTexture*)Engine::Clone_Resource(RESOURCE_STATIC, L"Texture_PlayerTrail");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Texture", pComponent));

	return S_OK;
}

_int CPlayerSwordTrail::Update(const _float& fTimeDelta)
{ 
	m_fTimeDelta = fTimeDelta;
	if(NULL == m_pmatSword)
	{
		m_pmatSword = &((Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"PlayerSword"))->m_matWorld;
	}

	Engine::CGameObject::Update(fTimeDelta);

	_vec3 vMin(0.f, -7.f, -5.f);
	_vec3 vMax(100.f, 7.f, 5.f);

	
	m_vSwordStartPoint		= vMin;
	m_vSwordEndPoint		= vMax;

	D3DXVec3TransformCoord(&m_vSwordEndPoint, &m_vSwordEndPoint, m_pmatSword);
	D3DXVec3TransformCoord(&m_vSwordStartPoint, &m_vSwordStartPoint, m_pmatSword);

	memcpy(m_pOriVTXInfo, m_pVTXInfo, sizeof(Engine::VTXTEX) * 80);

	Move(fTimeDelta);
	Set_Point(m_vSwordStartPoint, m_vSwordEndPoint);


	if(!Engine::CRenderer::m_bMotionBlurRender) //모션 블러가 없으면 리턴
		return 0;

	if(CPlayer::GetInstance()->Get_PlayerState() == CPlayer::PS_BreakAttack
		|| CPlayer::GetInstance()->Get_PlayerState() == CPlayer::PS_Run) //봉돌 중일때는 리턴
		return 0;

	////트레일의 색을 결정한다.
	//_float fCombo = (float)CPlayer::GetInstance()->GetPlayerAttackCombo();
	//if(fCombo <= 20.f)
	//{
	//	m_vColor = _vec4( 0.f, -0.2f + (fCombo / 25.f), -0.2f + (fCombo / 25.f), 0.0f);
	//}
	//else
	//{
	//	m_vColor = _vec4(0.f, -1.f, -1.f, 0.0f);
	//}

	//트레일의 텍스쳐를 결정한다.
	if(CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_01_a)
	{
		m_iImageNum = 3;
	}
	else if(CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_01_b)
	{
		m_iImageNum = 3;
	}
	else if(CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_02)
	{
		m_iImageNum = 3;
	}
	else if(CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_03)
	{
		m_iImageNum = 3;
	}
	else if(CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_04)
	{
		m_iImageNum = 3;
	}
	else if(CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_strong_01)
	{
		m_iImageNum = 2;
	}
	else if(CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_strong_02_a)
	{
		m_iImageNum = 5;
	}
	else if(CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_strong_03)
	{
		m_iImageNum = 1;
	}
	else if(CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_strong_04_a)
	{
		m_iImageNum = 0;
	}
	else if(CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() == Player::battle_attack_strong_04_b)
	{
		m_iImageNum = 0;
	}
	else if(CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() == Player::SP_attack_charge_slash_up_l)
	{
		m_iImageNum = 3;
	}
	else if(CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() == Player::SP_attack_charge_slash_up_r)
	{
		m_iImageNum = 3;
	}
	else if(CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() == Player::SP_attack_execution)
	{
		m_iImageNum = 2;
	}
	else if(CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() == Player::SP_attack_hell)
	{
		m_iImageNum = 1;
	}
	else if(CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() == Player::SP_attack_judgement_l)
	{
		m_iImageNum = 4;
	}
	else if(CPlayer::GetInstance()->Get_MeshCom()->Get_CurrentAniIdx() == Player::SP_attack_judgement_r)
	{
		m_iImageNum = 4;
	}
	else
		m_iImageNum = 3;

	_matrix			matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXVec3TransformCoord(&m_vViewPos, &m_vSwordEndPoint, &(*m_pmatSword * matView));

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ALPHA, this, m_vViewPos.z);

	//Engine::Add_RenderGroup(Engine::CRenderer::RENDER_DISTORTION, this, m_vViewPos.z);
	return 0;
}

void CPlayerSwordTrail::Render(void)
{
	if(NULL == m_pEffect)		return;

	m_pBufferCom->Set_VtxInfo(m_pOriVTXInfo);

	Set_ContantTable();
	LPDIRECT3DTEXTURE9 pDepthTexture = Engine::CRenderTargetMgr::GetInstance()->Get_TargetTexture(L"Target_Depth");
	m_pEffect->SetTexture("g_DepthTexture", pDepthTexture);

	// 고정기능렌더링파이프라인을 사용하지않고, 셰이더로 그리겠습니다.
	m_pEffect->Begin(NULL, 0);
	m_pEffect->BeginPass(0);

	m_pBufferCom->Render(NULL);

	m_pEffect->EndPass();
	m_pEffect->End();	

	m_pEffect->SetTexture("g_BaseTexture", NULL);
	m_pEffect->SetTexture("g_DepthTexture", NULL);

	m_pBufferCom->Set_VtxInfo(m_pVTXInfo);
}

void CPlayerSwordTrail::Set_ContantTable(void)
{
	m_pEffect->SetMatrix("g_matWorld", &m_pTransCom->m_matWorld);

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	m_pEffect->SetMatrix("g_matView", &matView);
	m_pEffect->SetMatrix("g_matProj", &matProj);

	m_pEffect->SetVector("g_vColor", &m_vColor);

	m_pTextureCom->Set_ConstantTable(m_pEffect, "g_BaseTexture", m_iImageNum);	
}

void CPlayerSwordTrail::Set_Point(_vec3 vStart, _vec3 vEnd)
{
	for(int i = 29; i > -1; i -= 10)
	{
		m_pVTXInfo[i+10].vPos	= m_pVTXInfo[i].vPos;
		m_pVTXInfo[i+50].vPos = m_pVTXInfo[i+40].vPos;
	}

	m_pVTXInfo[9].vPos	= m_pVTXInfo[0].vPos;
	m_pVTXInfo[49].vPos = m_pVTXInfo[40].vPos;

	m_pVTXInfo[0].vPos		= vStart;
	m_pVTXInfo[40].vPos	= vEnd;

	for(int i = 0; i < 4; ++i)
	{
		for(int j = 0; j < 9; ++j)
		{
			if( 0 == i )
			{
				if(0 == j )		continue;

				D3DXVec3CatmullRom(&m_pVTXInfo[i * 10 + j].vPos,
					&m_pVTXInfo[0].vPos,	&m_pVTXInfo[0].vPos,
					&m_pVTXInfo[9].vPos,	&m_pVTXInfo[19].vPos,
					(float)(j) * 0.125f);

				D3DXVec3CatmullRom(&m_pVTXInfo[(i * 10 + j + 40)].vPos,
					&m_pVTXInfo[40].vPos,	&m_pVTXInfo[40].vPos,
					&m_pVTXInfo[9 + 40].vPos,	&m_pVTXInfo[19 + 40].vPos,
					(float)(j) * 0.125f);
			}
			else if( 1== i )
			{
				D3DXVec3CatmullRom(&m_pVTXInfo[i * 10 + j].vPos,
					&m_pVTXInfo[0].vPos,	&m_pVTXInfo[9].vPos,
					&m_pVTXInfo[19].vPos,	&m_pVTXInfo[29].vPos,
					(float)(j) * 0.125f);

				D3DXVec3CatmullRom(&m_pVTXInfo[(i * 10 + j + 40)].vPos,
					&m_pVTXInfo[0 + 40].vPos,	&m_pVTXInfo[9 + 40].vPos,
					&m_pVTXInfo[19 + 40].vPos,	&m_pVTXInfo[29 + 40].vPos,
					(float)(j) * 0.125f);
			}	
			else if( 2 == i)
			{
				D3DXVec3CatmullRom(&m_pVTXInfo[i * 10 + j].vPos,
					&m_pVTXInfo[9].vPos,	&m_pVTXInfo[19].vPos,
					&m_pVTXInfo[29].vPos,	&m_pVTXInfo[39].vPos,
					(float)(j) * 0.125f);

				D3DXVec3CatmullRom(&m_pVTXInfo[(i * 10 + j + 40)].vPos,
					&m_pVTXInfo[9 + 40].vPos,	&m_pVTXInfo[19 + 40].vPos,
					&m_pVTXInfo[29 + 40].vPos,	&m_pVTXInfo[39 + 40].vPos,
					(float)(j) * 0.125f);
			}	
			else if( 3 == i )
			{
				D3DXVec3CatmullRom(&m_pVTXInfo[i * 10 + j].vPos,
					&m_pVTXInfo[19].vPos,	&m_pVTXInfo[29].vPos,
					&m_pVTXInfo[39].vPos,	&m_pVTXInfo[39].vPos,
					(float)(j) * 0.125f);

				D3DXVec3CatmullRom(&m_pVTXInfo[(i * 10 + j + 40)].vPos,
					&m_pVTXInfo[19 + 40].vPos,	&m_pVTXInfo[29 + 40].vPos,
					&m_pVTXInfo[39 + 40].vPos,	&m_pVTXInfo[39 + 40].vPos,
					(float)(j) * 0.125f);
			}		
		}		
	}
}

void CPlayerSwordTrail::Move(const _float& fTimeDelta)
{
	m_pBufferCom->Set_VtxInfo(m_pVTXInfo);

}

_ulong CPlayerSwordTrail::Release(void)
{
	_ulong dwRefCnt = Engine::CGameObject::Release();

	if(0 == dwRefCnt)
	{
		::Safe_Delete_Array(m_pVTXInfo);
		::Safe_Delete_Array(m_pOriVTXInfo);
		delete this;
	}
	return dwRefCnt;
}