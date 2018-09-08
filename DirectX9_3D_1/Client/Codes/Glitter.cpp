#include "stdafx.h"
#include "Glitter.h"

#include "Engine_Defines.h"
#include "Export_Function.h"

#include "Line2D.h"
#include "CullingMgr.h"

#include "Player.h"

#include "LHLightning.h"
#include "ForeHandLightning.h"
#include "AroundLightning.h"
#include "RHLightning.h"

#include "SoundMgr.h"
#include "DamageFont.h"

#include "GlitterHP.h"
#include "MoonchopperHP.h"

#include "PlayerBeShotUI.h"
CGlitter::CGlitter(LPDIRECT3DDEVICE9 pGraphicDev)
: CMonster(pGraphicDev)
, m_bRotationOnce(true)
, m_iNearPattern(0)
, m_iMidPattern(0)
, m_iFarPattern(0)
, m_fRunDistance(25.f)
, m_bRun(false)
, m_bLowHealthOnce(false)
, m_bDie(false)
, m_bEffectOnce(false)
, m_bDamage(false)
, m_bDisHP(false)
, m_iSoundStep(0)
, m_fOriginAngle(0.f)
, m_vOriginDir(0.f, 0.f, 0.f)
{

}

CGlitter::~CGlitter(void)
{

}

HRESULT CGlitter::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Monster");
	if(NULL == m_pEffect)		return E_FAIL;

	m_pShadowEffect = Engine::Get_EffectHandle(L"Shader_Shadow");
	if(NULL == m_pShadowEffect)		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	//변수들 초기화
	m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);
	m_pTransCom->m_vPosition = _vec3(180.f, 0.f, 215.f);
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian(0);

	m_iMonsterAnimationIndex = Glitter::Spawn;
	m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
	m_eMonsterState = CMonster::MS_Super;

	bInput = false;

	m_bAppear = false; // 처음에 false로 해놓고 등장 안하게 한다.

	//바운딩박스 로드
	CMonster::Load_AttackBox(L"../Bin/Resources/Data/Box/Glitter/Glitter_AttackBox.dat");
	CMonster::Load_DamageBox(L"../Bin/Resources/Data/Box/Glitter/Glitter_DamageBox.dat");
	// Boss_HP UI

	// 패턴 로드
	Load_Pattern();

	// 애니메이션 싱크
	CMonster::Load_FootAnimationSync(L"../Bin/Resources/Data/Box/Glitter/Glitter_Sync.dat");

	Initialize_Min_Max_Radius();

	m_fMaxDamageValue = 500.f;
	m_fDownValue = 0.f;
	m_fMonsterDownValue = 0.f;

	m_vMoveMin		= _vec3(-7.f, 0.f, -7.f);
	m_vMoveMax		= _vec3(7.f, 10.f, 7.f);
	m_fMoveRadius	= D3DXVec3Length(&(m_vMoveMax - m_vMoveMin)) * m_pTransCom->m_vScale.x;

	//스탯 정보
	m_tMonsterInfo.fAttack = 100.f;
	m_tMonsterInfo.fMaxHP = 70000.f;
	m_tMonsterInfo.fHP = m_tMonsterInfo.fMaxHP;
	m_tMonsterInfo.fDownValue = 20.f;

	m_pmatFireball = m_pMeshCom->Get_FrameMatrix("ValveBiped_Bip01_Fireball");
	m_pmatAttachLH = m_pMeshCom->Get_FrameMatrix("ValveBiped_Anim_Attachment_LH");
	m_pmatAttachRH = m_pMeshCom->Get_FrameMatrix("ValveBiped_Anim_Attachment_RH");
	m_pmatSMDImport = m_pMeshCom->Get_FrameMatrix("SMDImport");

	m_pLHShortEffect = NULL;
	m_pALHShortEffect = NULL;
	m_pARHShortEffect = NULL;
	m_pExplosionEffect = NULL;
	
	m_pBossFaceUI	= CBossFaceUI::Create(m_pGraphicDev);
	m_pBossFaceUI->SetFaceNumber(1);

	m_eMonsterType = MT_Boss;
	CGlitterHP::GetInstance()->SetpMonster(this);

	return S_OK;
}

HRESULT CGlitter::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	//Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	//Mesh Component	// 플레이어를 제외한 다이나믹메쉬들을 CDynamicMesh 로 로드할 것.
	pComponent = m_pMeshCom = (Engine::CDynamicMesh*)Engine::Clone_Resource(RESOURCE_STAGEMIDDLE, L"Mesh_Glitter");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	m_vOriginalPos = m_pTransCom->m_vPosition;
	m_vAfterPos = m_pTransCom->m_vPosition;

	m_pMeshCom->SetLeftAndRihgtMatrix("ValveBiped_Bip01_L_Toe0", "ValveBiped_Bip01_R_Toe0");

	return S_OK;
}

_int CGlitter::Update(const float& fTimeDelta)
{
	m_fTimeDelta		= fTimeDelta;

	KeyInput();

	if(true == m_bAppear)
	{
		if(false == m_bDie)
			m_bMoveCollCheck = true;

		CMonster::CollisionCheckFunction();			//충돌에 대해 검사를 한다.
		Check_Monster_Satae();						//애니메이션, 시간, 특정 조건에 따라 몬스터의 상태 타입을 지속적으로 체크하여 바꾸어 주는 곳 => 이 상태에 따라 플레이어에서 충돌 검사를 건너 뛰거나 몬스터가 다운 상태일때 데미지가 더 들어가거나 다시 다운애니메이션을 Set 안하도록 한다.
		AnimationPattern_Immediately_Change();		//충돌에 따라 몬스터의 애니메이션을 즉각적으로 바꾼다. 충돌 방향은 추후 Monster class에서 공통적으로 만들던가 충돌, Math Mgr에서 만들어 활용, 몬스터의 타입을 만들어서 특정 모션이 공격중일때는 충돌 당해도 데미지만 받게 해야 한다.
		Check_PlayerAttack_AccordingToHP();			//충돌에 따른 몬스터의 HP관리및 피격 이펙트 생성
		BattleAnimationPattern();					//플레이어의 공격에 충돌도 안하고 애니메이션이 끝나면 만들어 놓은 알고리즘을 토대로 몬스터의 AI가 돌아가도록 한다.
		AnimationTimeChange();						//기본적으로 m_fAnimationSpeed의 속도는 2.f이지만 슬로우 상태나 특정모션의 속도를 늦출때 여기서 한다.
		SoundPlayer();

		Set_AnimationSyncInfo();

		m_pBossFaceUI->Update(fTimeDelta);
		CGlitterHP::GetInstance()->Update(fTimeDelta);
		CGameObject::Update(fTimeDelta);	
		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_SHADOW, this);

	}
	else
		m_bMoveCollCheck = false;
	return 0;
}

void CGlitter::Render(void)
{
	if(NULL == m_pEffect)		return;
	Set_ContantTable();	

	CMonster::Render();

	if(false == m_bDie)
	{
		m_pMeshCom->Move_Frame(m_fRenderTimeDelta);
	}
	else
	{
		if(!m_pMeshCom->Check_EndPeriod())
			m_pMeshCom->Move_Frame(m_fRenderTimeDelta);
	}
	m_pMeshCom->Render_MeshForShader(m_pEffect, false);

}
void CGlitter::Render_Shadow(void)
{
	if(NULL == m_pShadowEffect)		return;

	//광원
	const D3DLIGHT9* pLightInfo = Engine::Get_LightInfo(0);
	if(NULL == pLightInfo)	return;

	_matrix			matLightView, matLightProj;
	D3DXMatrixIdentity(&matLightView);
	D3DXMatrixIdentity(&matLightProj);
	_vec3	vAt, vUp, vEye;
	_vec3	vLightDir =  _vec3(pLightInfo->Direction);
	D3DXVec3Normalize(&vLightDir,  &vLightDir);

	//vEye = m_pTransCom->m_vPosition + ( vLightDir * -30.f);
	vEye = CPlayer::GetInstance()->Get_TransCom()->m_vPosition + (vLightDir * -100.f);
	vAt = vEye + vLightDir;
	vUp = _vec3(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(&matLightView, &vEye, &vAt, &vUp);
	D3DXMatrixPerspectiveFovLH(&matLightProj, D3DX_PI * 0.4f, float(WINSIZEX) / WINSIZEY,  0.1f, 1000.f);

	//CPlayer::GetInstance()->Get_TransCom()->m_matWorld;
	m_pShadowEffect->SetMatrix("g_matLightWV", &(m_pTransCom->m_matWorld * matLightView));
	m_pShadowEffect->SetMatrix("g_matLightProj", &matLightProj);

	//m_pMeshCom->Render_MeshForShader(m_pShadowEffect);
	m_pMeshCom->Render_MeshForShaderShadow(m_pShadowEffect);
}


CGlitter* CGlitter::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGlitter*	pMonster = new CGlitter(pGraphicDev);

	if( FAILED(pMonster->Initialize()) )
	{
		MSG_BOX("Glitter Create Failed");
		::Safe_Release(pMonster);
	}
	return pMonster;
}

void CGlitter::KeyInput()
{
	if(Engine::GetDIKeyState(DIK_1) & 0x80)
	{
		m_bAppear = true;
	}
}

void CGlitter::Set_ContantTable(void)
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

void CGlitter::Check_Monster_Satae(void)
{
	//공격 충돌스피어가 생성되어있는 상태에서 다운이 되면 충돌스피어를 삭제한다.
	if(Glitter::down_start <= m_iMonsterAnimationIndex && m_iMonsterAnimationIndex <= Glitter::down_standup)
	{
		if(NULL != m_pLHShortEffect)
			m_pLHShortEffect->Set_Delete(true);
		if(NULL != m_pALHShortEffect)
			m_pALHShortEffect->Set_Delete(true);
		if(NULL != m_pARHShortEffect)
			m_pARHShortEffect->Set_Delete(true);
		if(NULL != m_pExplosionEffect)
			m_pExplosionEffect->Set_Delete(true);
	}
}

void CGlitter::AnimationPattern_Immediately_Change(void)
{
	if(true == m_bDie)
	{
		if(m_pMeshCom->Check_EndPeriod())
		{
			printf("죽음 %d\n", m_iMonsterAnimationIndex);

			if(NULL != m_pLHShortEffect)
				m_pLHShortEffect->Set_Delete(true);
			if(NULL != m_pALHShortEffect)
				m_pALHShortEffect->Set_Delete(true);
			if(NULL != m_pARHShortEffect)
				m_pARHShortEffect->Set_Delete(true);
			if(NULL != m_pExplosionEffect)
				m_pExplosionEffect->Set_Delete(true);

			if(true == m_bMoveCollCheck)
			{
				printf("==========죽는사운드==========\n");
				CSoundMgr::GetInstance()->PlayEffect(SOUND::action_landing_higher);
				CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::vampire_hurt1 + rand()%3));
			}
			m_bMoveCollCheck = false;
			
		}
	}
	else
	{
		if(Glitter::RunN == m_iMonsterAnimationIndex)
		{
			m_fStepTime += m_fTimeDelta;
			if(m_fStepTime > 0.5f)
			{
				printf("==========뛰는사운드==========\n");
				CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::vampire_step1 + m_iSoundStep));
				++m_iSoundStep;
				if(3 < m_iSoundStep)
					m_iSoundStep = 0;
				m_fStepTime = 0.f;
			}

			Compute_AngleToPlayer();
			m_bRun = true;

			//일정 거리가 될 때 까지 달려온다.
			if( m_pMeshCom->Check_EndPeriod() )
			{
				if(m_fRunDistance >= Compute_DisToPlayer())
				{
					m_iMonsterAnimationIndex = Glitter::vampire_attack_forehand2_lightning;
					m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
					m_bRun = false;
				}
				else
				{
					m_pMeshCom->Set_AnimationSetSameFinish(Glitter::RunN);
				}

				m_bRotationOnce = true;
				CSoundMgr::GetInstance()->SetEnvironmentChannel(false);
			}
		}
		else if(Glitter::turn_Left == m_iMonsterAnimationIndex)
		{
			m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] -= D3DXToRadian(15.f) * m_fTimeDelta;
			if(m_pMeshCom->Check_EndPeriod())
			{
				m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
			}

		}
		else if(Glitter::turn_Right == m_iMonsterAnimationIndex)
		{
			m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] += D3DXToRadian(15.f) * m_fTimeDelta;
			if(m_pMeshCom->Check_EndPeriod())
			{
				m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
			}
		}
		else if(Glitter::WalkE <= m_iMonsterAnimationIndex && m_iMonsterAnimationIndex <= Glitter::WalkN)
		{
			if(m_pMeshCom->Check_EndPeriod())
			{
				m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
			}	
		}
		else if(Glitter::down_start == m_iMonsterAnimationIndex)
		{
			m_eMonsterState = CMonster::MS_Super;
			
			if(m_pMeshCom->Check_EndPeriodMulTime(0.f) && !m_pMeshCom->Check_EndPeriodMulTime(0.1f))
			{
				if(false == m_bEffectOnce)
				{
					printf("==========낮은체력1사운드==========\n");
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::vampire_hurt1 + rand()%3));
					m_bEffectOnce = true;
				}
			}
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.4f) && !m_pMeshCom->Check_EndPeriodMulTime(0.5f))
			{
				if(false == m_bEffectOnce)
				{
					printf("==========낮은체력1-1사운드==========\n");
					CSoundMgr::GetInstance()->PlayEffect(SOUND::Bear_Step);
					m_bEffectOnce = true;
				}
			}
			else
				m_bEffectOnce = false;
		}
		else if(Glitter::vampire_attack_lightning_onehand_glitter == m_iMonsterAnimationIndex)
		{
			//공격 시작할 때 방향 계산
			if(m_pMeshCom->Check_EndPeriodMulTime(0.f) && !m_pMeshCom->Check_EndPeriodMulTime(0.1f))
			{
				if(m_bRotationOnce == true)
				{
					printf("==========왼손공격사운드==========\n");
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::vampire_shout1 + rand()%3));
				}
				Compute_AngleToPlayer();
			}
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.2f) && !m_pMeshCom->Check_EndPeriodMulTime(0.3f))
			{
				//손따라다니는 이펙트 생성
				if(NULL == m_pLHShortEffect)
				{
					printf("==========왼손공격사운드2==========\n");
					m_pLHShortEffect = CLHLightning::Create(Engine::Get_GraphicDev(), &m_pTransCom->m_matWorld, m_pmatAttachLH);
					CSoundMgr::GetInstance()->PlayEffect(SOUND::effect_fireball_appear);
				}
			}
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.55f) && !m_pMeshCom->Check_EndPeriodMulTime(0.56f))
			{
				//손따라다니는 이펙트 삭제
				if(NULL != m_pLHShortEffect)
				{
					m_pLHShortEffect->Set_Delete(true);
					m_pLHShortEffect = NULL;
				}
			}
			//마법공격 날리기 직전에 방향 계산
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.56f) && !m_pMeshCom->Check_EndPeriodMulTime(0.57f))
			{
				Compute_AngleToPlayer();
				if(false == m_bEffectOnce)
				{
					printf("==========왼손공격사운드3==========\n");
					CSoundMgr::GetInstance()->PlayEffect(SOUND::effect_lightning_charge);

					//던져지는 이펙트 생성
					CLHLightning::Create(Engine::Get_GraphicDev(), &m_pTransCom->m_matWorld, m_pmatFireball, true);
					m_bEffectOnce = true;
				}
			}
			else
			{
				m_bRotationOnce = true;
				m_bEffectOnce = false;
			}
			if(m_pMeshCom->Check_EndPeriodMulTime(0.f) && !m_pMeshCom->Check_EndPeriodMulTime(0.57f))
				m_eMonsterState = CMonster::MS_Super;
			else
				m_eMonsterState = CMonster::MS_Attack;
			
		}
		else if(Glitter::vampire_attack_lightning_strike == m_iMonsterAnimationIndex) // 오른손
		{
			//공격 시작할 때 방향 계산
			if(m_pMeshCom->Check_EndPeriodMulTime(0.f) && !m_pMeshCom->Check_EndPeriodMulTime(0.1f))
			{
				if(m_bRotationOnce == true)
				{
					printf("==========오른손공격사운드==========\n");
					CSoundMgr::GetInstance()->PlayEffect(SOUND::effect_lightning_charge);
				}
				Compute_AngleToPlayer();
			}
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.2f) && !m_pMeshCom->Check_EndPeriodMulTime(0.3f))
			{
				//손따라다니는 이펙트 생성
				if(NULL == m_pLHShortEffect)
				{
					printf("==========오른손공격사운드2==========\n");
					m_pLHShortEffect = CRHLightning::Create(Engine::Get_GraphicDev(), &m_pTransCom->m_matWorld, m_pmatAttachRH);
					CSoundMgr::GetInstance()->PlayEffect(SOUND::effect_fireball_appear);
				}
			}
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.51f) && !m_pMeshCom->Check_EndPeriodMulTime(0.56f))
			{
				//손따라다니는 이펙트 삭제
				if(NULL != m_pLHShortEffect)
				{
					m_pLHShortEffect->Set_Delete(true);
					m_pLHShortEffect = NULL;
				}
			}
			//직전에 방향 계산
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.55f) && !m_pMeshCom->Check_EndPeriodMulTime(0.6f))
			{
				//m_bRotationOnce = true;
				Compute_AngleToPlayer();
				if(false == m_bEffectOnce)
				{
					//던져지는 이펙트 생성
					CRHLightning::Create(Engine::Get_GraphicDev(), &m_pTransCom->m_matWorld, m_pmatFireball, true);
					m_bEffectOnce = true;
				}
			}
			else
			{
				m_bRotationOnce = true;
				m_bEffectOnce = false;
			}
			if(m_pMeshCom->Check_EndPeriodMulTime(0.f) && !m_pMeshCom->Check_EndPeriodMulTime(0.6f))
				m_eMonsterState = CMonster::MS_Super;
			else
				m_eMonsterState = CMonster::MS_Attack;
		}
		else if(Glitter::vampire_attack_lightning_around == m_iMonsterAnimationIndex) // 폭발
		{
			//공격 시작할 때 방향 계산
			if(m_pMeshCom->Check_EndPeriodMulTime(0.f) && !m_pMeshCom->Check_EndPeriodMulTime(0.1f))
			{
				//m_bRotationOnce = true;
				Compute_AngleToPlayer();
				//양손에 이펙트 생성
				if(NULL == m_pALHShortEffect)
				{
					printf("==========폭발공격사운드==========\n");
					m_pALHShortEffect = CLHLightning::Create(Engine::Get_GraphicDev(), &m_pTransCom->m_matWorld, m_pmatAttachLH);
					CSoundMgr::GetInstance()->PlayEffect(SOUND::effect_lightning_charge);
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::vampire_laugh1 + rand()%3));
				}
				if(NULL == m_pARHShortEffect)
					m_pARHShortEffect = CLHLightning::Create(Engine::Get_GraphicDev(), &m_pTransCom->m_matWorld, m_pmatAttachRH);
			}
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.38f) && !m_pMeshCom->Check_EndPeriodMulTime(0.4f))
			{
				//m_bRotationOnce = true;
				Compute_AngleToPlayer();
				//폭발 이펙트 생성
				if(NULL == m_pExplosionEffect)
				{
					printf("==========폭발공격사운드2==========\n");
					m_pExplosionEffect = CAroundLightning::Create(Engine::Get_GraphicDev(), &m_pTransCom->m_matWorld, m_pmatSMDImport, true);
					CSoundMgr::GetInstance()->PlayEffect(SOUND::effect_lightning_explosion);
					CSoundMgr::GetInstance()->PlayEffect(SOUND::effect_lightning_shock);
				}
				
			}
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.8f))
			{
				if(NULL != m_pExplosionEffect)
				{
					m_pExplosionEffect->Set_Delete(true);
					m_pExplosionEffect = NULL;
				}
			}
			else
			{
				m_bRotationOnce = true;
			}
			if(m_pMeshCom->Check_EndPeriodMulTime(0.f) && !m_pMeshCom->Check_EndPeriodMulTime(0.6f))
				m_eMonsterState = CMonster::MS_Super;
			else
				m_eMonsterState = CMonster::MS_Attack;
		}
		else if(Glitter::vampire_attack_forehand2_lightning == m_iMonsterAnimationIndex) //양손
		{
			//공격 시작할 때 방향 계산
			if(m_pMeshCom->Check_EndPeriodMulTime(0.f) && !m_pMeshCom->Check_EndPeriodMulTime(0.1f))
			{
				//m_bRotationOnce = true;
				Compute_AngleToPlayer();
				
				//이펙트 생성
				if(NULL == m_pALHShortEffect)
				{
					printf("==========양손공격사운드==========\n");
					m_pALHShortEffect = CForeHandLightning::Create(Engine::Get_GraphicDev(), &m_pTransCom->m_matWorld, m_pmatAttachLH,L"Test_Lightning");
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::vampire_shout_long1 + rand()%3));
					CSoundMgr::GetInstance()->PlayEffect(SOUND::effect_lightning_charge);
				}
				if(NULL == m_pARHShortEffect)
					m_pARHShortEffect = CForeHandLightning::Create(Engine::Get_GraphicDev(), &m_pTransCom->m_matWorld, m_pmatAttachRH,L"Test_Lightning");

			}
			//직전에 방향 계산
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.43f) && !m_pMeshCom->Check_EndPeriodMulTime(0.44f))
			{
				//m_bRotationOnce = true;
				Compute_AngleToPlayer();
			}
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.45f) && !m_pMeshCom->Check_EndPeriodMulTime(0.5f))
			{
				//이 구간에서는 싱크값보다 조금멀리 이동시켜야해서 2.f로 줬다.
				m_fDistanceSize = 2.f;
			}
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.57f) && !m_pMeshCom->Check_EndPeriodMulTime(0.58f))
			{
				//폭발 이펙트 생성
				if(NULL == m_pExplosionEffect)
				{
					printf("==========양손공격사운드2==========\n");
					m_pExplosionEffect = CForeHandLightning::Create(Engine::Get_GraphicDev(), &m_pTransCom->m_matWorld, m_pmatSMDImport, L"Thunder_storm2");
					CSoundMgr::GetInstance()->PlayEffect(SOUND::effect_lightning_explosion);
				}
			}
			else
			{
				m_bRotationOnce = true;
				m_fDistanceSize = 1.f;
				m_bEffectOnce = false;
			}
			if(m_pMeshCom->Check_EndPeriodMulTime(0.f) && !m_pMeshCom->Check_EndPeriodMulTime(0.6f))
				m_eMonsterState = CMonster::MS_Super;
			else
				m_eMonsterState = CMonster::MS_Attack;
		}
		else if(Glitter::Spawn == m_iMonsterAnimationIndex)
		{
			if(m_pMeshCom->Check_EndPeriodMulTime(0.15f) && !m_pMeshCom->Check_EndPeriodMulTime(0.2f))
			{
				if(false == m_bEffectOnce)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::step_lv4);
					CEffectMgr::GetInstance()->Play_Effect(L"DustBig", m_pTransCom->m_vPosition + _vec3(-2.f, 0.f, 2.f));
					CEffectMgr::GetInstance()->Play_Effect(L"DustBig", m_pTransCom->m_vPosition + _vec3(0.f, 0.f, 2.f));
					CEffectMgr::GetInstance()->Play_Effect(L"DustBig", m_pTransCom->m_vPosition + _vec3(2.f, 0.f, 2.f));
					m_bEffectOnce = true;
				}
			}
			if(m_pMeshCom->Check_EndPeriod())
			{
				m_pBossFaceUI->SetRender(true);
				CGlitterHP::GetInstance()->SetRender(true);
				m_bEffectOnce = false;
			}
			
		}
	}
}

void CGlitter::Check_PlayerAttack_AccordingToHP(void)
{
	if( (m_bPlayerAttack_Body_CollisionCheck || m_bPlayerEffect_Body_CollisionCheck) && false == m_bDie )	
	{
		m_bDamage = true;
	}
	else
		m_bDamage = false;

	if( m_bDamage )
	{
		if( m_bDisHP )
			return;

		m_bDisHP = true;
		printf("충돌이다!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		m_tMonsterInfo.fHP -= CPlayer::GetInstance()->GetPlayerAttackValue();
		//오버킬 처리
		float MonsterHp = m_tMonsterInfo.fHP;	//몬스터 오버킬을 위해 임시로 선언 
		m_tMonsterInfo.fHP -= CPlayer::GetInstance()->GetPlayerAttackValue();

		if(m_tMonsterInfo.fHP <= 0.f)
		{
			m_tMonsterInfo.fHP = 0.f;

			if((CPlayer::GetInstance()->GetPlayerAttackValue() - MonsterHp) <= 100)//혹시 100보다 작을때는 문제가 생기니 강제 예외처리함
			{
				CDamageFont::Create(m_pGraphicDev, DFONT_TYPE_OVERKILL, DFONT_OBJ_MONSTER, (int)(CPlayer::GetInstance()->GetPlayerAttackValue() - MonsterHp + 100), m_vCollisionPos);	//몬스터가 받은 데미지를 띄운다.
			}
			else
				CDamageFont::Create(m_pGraphicDev, DFONT_TYPE_OVERKILL, DFONT_OBJ_MONSTER, (int)(CPlayer::GetInstance()->GetPlayerAttackValue() - MonsterHp), m_vCollisionPos);	//몬스터가 받은 데미지를 띄운다.
		}
		else
		{
			CDamageFont::Create(m_pGraphicDev, DFONT_TYPE_DAMAGE, DFONT_OBJ_MONSTER, (int)CPlayer::GetInstance()->GetPlayerAttackValue(), m_vCollisionPos);	//몬스터가 받은 데미지를 띄운다.
		}

		printf("몬스터 체력 == %f\n", m_tMonsterInfo.fHP);

		//충돌되면 중간 보스 UI의 랜더를 바꿔야한다.
		CGlitterHP::GetInstance()->SetRender(true);
		CMoonchopperHP::GetInstance()->SetRender(false);

		CPlayer::GetInstance()->Player_AttackSuccess_InfoChange();
		CEffectMgr::GetInstance()->Play_Effect(L"Player_Slash_Effect", m_pTransCom->m_vPosition + _vec3(0.f, m_vCollisionPos.y, 0.f), false, true, _vec3(3.f, 15.f, 0.f), _vec3(m_fAngleX, 0.f, m_fAngleZ), 0.f, false, 0.f, NULL);
		CPlayerBeShotUI::GetInstance()->PlayerBeShotUI();

		if(MS_Super != m_eMonsterState)
		{
			//수정해야함
			//내가 플레이어를 보는 방향벡터
			_vec3 vDirToP = m_vPlayerPos - m_pTransCom->m_vPosition;
			D3DXVec3Normalize(&vDirToP, &vDirToP);

			//마주보고있다면
			if((m_vOriginDir.x * vDirToP.x) >= 0 || (m_vOriginDir.z * vDirToP.z) >= 0 )
			{
				m_iMonsterAnimationIndex = Glitter::damage_weak_front;
				m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
			}
			else
			{
				m_iMonsterAnimationIndex = Glitter::damage_weak_back;
				m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
			}
		}
	}
	else
		m_bDisHP = false;


	if(false == m_bLowHealthOnce && (m_tMonsterInfo.fHP <= m_tMonsterInfo.fMaxHP * 0.5f) && (m_tMonsterInfo.fHP > m_tMonsterInfo.fMaxHP * 0.1f))
	{
		m_bLowHealth = true;
		m_bLowHealthOnce = true;
		m_iMonsterAnimationIndex = Glitter::down_start;
	}
	else if(true == m_bLowHealthOnce && (m_tMonsterInfo.fHP <= m_tMonsterInfo.fMaxHP * 0.1f))
	{
		m_bLowHealth = true;
		m_bLowHealthOnce = false;
		m_iMonsterAnimationIndex = Glitter::down_start;
	}
	else if(false == m_bDie && m_tMonsterInfo.fHP <= 0)
	{
		m_bDie = true;
		m_iMonsterAnimationIndex = Glitter::down_from_bottom2downfront;
		m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
	}
	if(true == m_bLowHealth)
		Pattern_LowHealth();
}

void CGlitter::BattleAnimationPattern(void)
{
	if(m_pMeshCom->Check_EndPeriod()) 
	{
		if(Glitter::vampire_attack_forehand2_lightning == m_iMonsterAnimationIndex)
		{
			if(NULL != m_pALHShortEffect)
				m_pALHShortEffect->Set_Delete(true);
			if(NULL != m_pARHShortEffect)
				m_pARHShortEffect->Set_Delete(true);
			if(NULL != m_pExplosionEffect)
				m_pExplosionEffect->Set_Delete(true);

			m_pALHShortEffect = NULL;
			m_pARHShortEffect = NULL;
			m_pExplosionEffect = NULL;
		}
		else if(Glitter::vampire_attack_lightning_onehand_glitter == m_iMonsterAnimationIndex)
		{
			if(NULL != m_pALHShortEffect)
				m_pALHShortEffect->Set_Delete(true);
			m_pLHShortEffect = NULL;
		}
		else if(Glitter::vampire_attack_lightning_strike == m_iMonsterAnimationIndex)
		{
			if(NULL != m_pALHShortEffect)
				m_pALHShortEffect->Set_Delete(true);
			m_pLHShortEffect = NULL;
		}
		else if(Glitter::vampire_attack_lightning_around == m_iMonsterAnimationIndex)
		{
			if(NULL != m_pALHShortEffect)
				m_pALHShortEffect->Set_Delete(true);
			if(NULL != m_pARHShortEffect)
				m_pARHShortEffect->Set_Delete(true);
			if(NULL != m_pExplosionEffect)
				m_pExplosionEffect->Set_Delete(true);

			m_pALHShortEffect = NULL;
			m_pARHShortEffect = NULL;
			m_pExplosionEffect = NULL;
		}

		m_bRotationOnce = true;
		m_bEffectOnce = false;

		_bool bNear = Pattern_Near_Distance();
		_bool bMid = Pattern_Mid_Distance();
		_bool bFar = Pattern_Far_Distance();
		if( false == bNear && false == bMid && false == bFar )	
		{
			Pattern_Out_Distance();
		}	
	}	
}

void CGlitter::AnimationTimeChange(void)
{
	m_fRenderTimeDelta	= m_fTimeDelta * m_fAnimationSpeed * 1.f;
}

void CGlitter::SoundPlayer(void)
{
// 	if(m_pMeshCom->Get_CurrentAniIdx() != m_iAnimationSoundCheck || m_pMeshCom->Check_EndPeriod())
// 	{
// 		m_iAnimationSoundCheck = m_pMeshCom->Get_CurrentAniIdx();
// 		ZeroMemory(m_bSoundPlay, sizeof(_bool) * 10);
// 	}
// 
// 	if(Glitter::vampire_attack_lightning_onehand_glitter == m_iMonsterAnimationIndex)
// 	{
// 		//SoundEffectPlay(SOUND::EFF(SOUND::vampire_shout1 + rand()%3), 0.f, 0, 5);
// 		SoundEffectPlay(SOUND::effect_fireball_appear, 0.f, 1, 5);
// 		SoundEffectPlay(SOUND::effect_lightning_charge, 0.56f, 2, 5);
// 	}
// 	else if(Glitter::vampire_attack_lightning_strike == m_iMonsterAnimationIndex) 
// 	{	
// 		SoundEffectPlay(SOUND::effect_lightning_charge, 0.f, 0, 5);
// 		SoundEffectPlay(SOUND::effect_fireball_appear, 0.2, 1, 5);
// 	}
// 	else if(Glitter::vampire_attack_lightning_around == m_iMonsterAnimationIndex) // 폭발
// 	{
// 
// 		SoundEffectPlay(SOUND::effect_lightning_charge, 0.f, 0, 5);
// 		SoundEffectPlay(SOUND::EFF(SOUND::vampire_laugh1 + rand()%3), 0.f, 1, 5);
// 		SoundEffectPlay(SOUND::effect_lightning_explosion, 0.38f, 2, 5);
// 		SoundEffectPlay(SOUND::effect_lightning_shock, 0.38f, 3, 5);
// 	}
// 	else if(Glitter::vampire_attack_forehand2_lightning == m_iMonsterAnimationIndex) //양손
// 	{
// 
// 		//SoundEffectPlay(SOUND::EFF(SOUND::vampire_shout_long1 + rand()%3), 0.f, 0, 5);
// 		SoundEffectPlay(SOUND::effect_lightning_charge, 0.f, 1, 5);
// 		SoundEffectPlay(SOUND::effect_lightning_explosion, 0.57, 2, 5);
// 	}
// 	else if(Glitter::Spawn == m_iMonsterAnimationIndex)
// 	{
// 		SoundEffectPlay(SOUND::step_lv4, 0.15f, 0);	
// 	}
}

void CGlitter::Compute_AngleToPlayer()
{
	if(Glitter::RunN == m_iMonsterAnimationIndex || (Glitter::vampire_attack_lightning_onehand_glitter <= m_iMonsterAnimationIndex
		&& m_iMonsterAnimationIndex <= Glitter::vampire_attack_forehand2_lightning))
	{
		if(true == m_bRotationOnce)
		{
			m_vPlayerPos = CPlayer::GetInstance()->GetPos();
			_vec3 vDir = m_vPlayerPos - m_pTransCom->m_vPosition;
			D3DXVec3Normalize(&vDir, &vDir);

			_vec3 vMonsterDir;
			memcpy( &vMonsterDir, &(m_pTransCom->m_matWorld.m[2][0]), sizeof(_vec3) );	
			D3DXVec3Normalize(&vMonsterDir, &vMonsterDir);	

			vMonsterDir *= -1.f; //x파일 자체가 0 0 -1보고있는데 실제 방향벡터는 0 0 1이라서 곱해줌

			//플레이어와의 각도를 구함.
			_float fAngle = acosf( D3DXVec3Dot(&vMonsterDir, &vDir) );

			//acos함수는 0 ~ 180라서 보정해줘야함.
			_matrix matRotationDefault;
			D3DXMatrixRotationY(&matRotationDefault, D3DXToRadian(0.f));
			_vec3 vRight;
			memcpy( &vRight, &(m_pTransCom->m_matWorld.m[0][0]), sizeof(_vec3) );	
			D3DXVec3Normalize(&vRight, &vRight);									
			D3DXVec3TransformNormal(&vRight, &vRight, &matRotationDefault);	
			_float fAngle_Right = acosf( D3DXVec3Dot(&vRight, &vDir) );

			if( D3DXToDegree(fAngle_Right) <= 90.f )
				fAngle = D3DX_PI * 2.f - fAngle;//fAngle = fAngle * -1.f;

			//소수점 이하 
			_float fResultAngle = (_int)((m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] + fAngle) * 100 ) / 100.f;

			m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = fResultAngle;
			printf("m_fAngle === %f\n", m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);

			m_fOriginAngle = fResultAngle;

			m_bRotationOnce = false;
		}
	}
	m_pTransCom->Update(m_fTimeDelta);

	//내가 플레이어를 보는 방향벡터
	_vec3 vDirToP = m_vPlayerPos - m_pTransCom->m_vPosition;
	D3DXVec3Normalize(&vDirToP, &vDirToP);

	_matrix matRotY;
	D3DXMatrixRotationY(&matRotY, m_fOriginAngle);
	D3DXVec3TransformCoord(&vDirToP, &vDirToP, &matRotY); //실제 행렬의 Y의 회전 값만큼 회전 시킨다.
	m_vOriginDir = vDirToP;
	/*printf("내가 플레이어를 보는 방향벡터\n %f  %f  %f\n", vDirToP.x, vDirToP.y, vDirToP.z);*/

}
void CGlitter::Set_AnimationSyncInfo()
{
	boost::unordered_map<int, vector<FOOTANISYNC*>>::iterator iter = m_mapAniSync.find(m_iMonsterAnimationIndex);
	if( iter != m_mapAniSync.end() )
	{
		vector<FOOTANISYNC*>::iterator iter_vec = iter->second.begin();
		vector<FOOTANISYNC*>::iterator iter_end = iter->second.end();

		for( ; iter_vec != iter_end; ++iter_vec )
		{
			if(m_pMeshCom->Check_EndPeriodMulTime((*iter_vec)->fStartTime) && !m_pMeshCom->Check_EndPeriodMulTime((*iter_vec)->fEndTime))
				CMonster::MonsterMoveFunctionTwo((*iter_vec)->bCompulsion, (*iter_vec)->bLeftFoot, (*iter_vec)->bFront);
		}
	}
}
_float CGlitter::Compute_DisToPlayer()
{
	m_vPlayerPos = CPlayer::GetInstance()->GetPos();
	_float fDisToPlayer = D3DXVec3Length(&(m_vPlayerPos - m_pTransCom->m_vPosition));
	return fDisToPlayer;
}
void CGlitter::Load_Pattern()
{

	//근거리 패턴
	m_vecInPattern.push_back(Glitter::vampire_attack_lightning_onehand_glitter); //왼손라이트닝
	//m_vecInPattern.push_back(Glitter::RunN); //달리기
	m_vecInPattern.push_back(Glitter::vampire_attack_lightning_around); //전격 폭발
	m_vecInPattern.push_back(Glitter::vampire_attack_forehand2_lightning); //덮치기

	//중거리 패턴
	//m_vecMidPattern.push_back(Glitter::RunN); //달리기
	m_vecMidPattern.push_back(Glitter::vampire_attack_lightning_strike); //오른손라이트닝
	m_vecMidPattern.push_back(Glitter::vampire_attack_forehand2_lightning); //덮치기
	m_vecMidPattern.push_back(Glitter::RunN); //달리기
	m_vecMidPattern.push_back(Glitter::vampire_attack_lightning_onehand_glitter); //왼손라이트닝
	m_vecMidPattern.push_back(Glitter::vampire_attack_lightning_around); //전격 폭발
	//장거리 패턴
	m_vecOutPattern.push_back(Glitter::RunN); //달리기
	m_vecOutPattern.push_back(Glitter::vampire_attack_lightning_strike); //오른손라이트닝

// 	m_vecInPattern.push_back(Glitter::idle);
// 	m_vecMidPattern.push_back(Glitter::RunN);
// 	m_vecOutPattern.push_back(Glitter::idle);

}
_bool CGlitter::Pattern_Near_Distance()
{
	if(true == m_bRun || true == m_bLowHealth || true == m_bDie)
		return false;

	if(Compute_DisToPlayer() <= 30.f)
	{
		if( m_iNearPattern > int(m_vecInPattern.size()) - 1 )
			m_iNearPattern = 0;

		//printf("Pattern_Near_Distance들어옴\n");
		//printf("Near ====== %d\n", m_iNearPattern);
		if(m_iMonsterAnimationIndex == m_vecInPattern[m_iNearPattern])
		{
			m_iMonsterAnimationIndex = m_vecInPattern[m_iNearPattern];
			m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
		}
		else
		{
			m_iMonsterAnimationIndex = m_vecInPattern[m_iNearPattern];
			m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
		}

		
		++m_iNearPattern;
		return true;
	}
	else 
		return false;
}

_bool CGlitter::Pattern_Mid_Distance()
{
	if(true == m_bRun || true == m_bLowHealth || true == m_bDie)
		return false;

	if(30 < Compute_DisToPlayer() && Compute_DisToPlayer() <= 70.f)
	{
		if( m_iMidPattern > int(m_vecMidPattern.size()) - 1 )
			m_iMidPattern = 0;

		//printf("Pattern_Mid_Distance들어옴\n");
		//printf("Mid ====== %d\n", m_iMidPattern);
		if(m_iMonsterAnimationIndex == m_vecMidPattern[m_iMidPattern])
		{
			m_iMonsterAnimationIndex = m_vecMidPattern[m_iMidPattern];
			m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
		}
		else
		{
			m_iMonsterAnimationIndex = m_vecMidPattern[m_iMidPattern];
			m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
		}
		
		++m_iMidPattern;
		return true;
	}
	else 
		return false;
}

_bool CGlitter::Pattern_Far_Distance()
{
	if(true == m_bRun || true == m_bLowHealth || true == m_bDie)
		return false;

	if(70 < Compute_DisToPlayer() && Compute_DisToPlayer() <= 100.f)
	{
		if( m_iFarPattern > int(m_vecOutPattern.size()) - 1 )
			m_iFarPattern = 0;

		if(m_iMonsterAnimationIndex == m_vecOutPattern[m_iFarPattern])
		{
			//printf("같은모션이다\n");
			m_iMonsterAnimationIndex = m_vecOutPattern[m_iFarPattern];
			m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
		}
		else
		{
			m_iMonsterAnimationIndex = m_vecOutPattern[m_iFarPattern];
			m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
		}
		++m_iFarPattern;
		return true;
	}
	else 
		return false;
}

_bool CGlitter::Pattern_Out_Distance()
{
	if(true == m_bRun || true == m_bLowHealth || true == m_bDie)
		return false;

	if(100 < Compute_DisToPlayer())
	{
		//printf("Pattern_Out_Distance들어옴\n");
		m_iMonsterAnimationIndex = Glitter::RunN;
		m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
		return true;
	}
	else 
		return false;
}
void CGlitter::Pattern_LowHealth()
{
	if( m_iMonsterAnimationIndex == Glitter::down_start )
		m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);

	if(m_pMeshCom->Check_EndPeriod())
	{
		if( m_iMonsterAnimationIndex != Glitter::down_start )
		{
			m_eMonsterState = CMonster::MS_Super;
			m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
		}

		if(m_iMonsterAnimationIndex < Glitter::down_standup)
		{
			++m_iMonsterAnimationIndex;
		}
		else
			m_bLowHealth = false;
	}
}
_ulong CGlitter::Release(void)
{
	_ulong dwRefCnt = CMonster::Release();

	if(0 == dwRefCnt)
	{
		if(NULL != m_pLHShortEffect)
			m_pLHShortEffect->Set_Delete(true);
		if(NULL != m_pALHShortEffect)
			m_pALHShortEffect->Set_Delete(true);
		if(NULL != m_pARHShortEffect)
			m_pARHShortEffect->Set_Delete(true);
		if(NULL != m_pExplosionEffect)
			m_pExplosionEffect->Set_Delete(true);

		m_pLHShortEffect = NULL;
		m_pALHShortEffect = NULL;
		m_pARHShortEffect = NULL;
		m_pExplosionEffect = NULL;

		m_vecInPattern.clear();
		m_vecMidPattern.clear();
		m_vecOutPattern.clear();
		CGlitterHP::GetInstance()->DestroyInstance();
		Engine::Safe_Release(m_pBossFaceUI);

		boost::unordered_map<int, vector<FOOTANISYNC*>>::iterator iter = m_mapAniSync.begin();
		boost::unordered_map<int, vector<FOOTANISYNC*>>::iterator iter_end = m_mapAniSync.end();

		for( ; iter != iter_end; )
		{
			vector<FOOTANISYNC*>::iterator iter_box = iter->second.begin();
			vector<FOOTANISYNC*>::iterator iter_box_end = iter->second.end();

			for( ; iter_box != iter_box_end; )
			{
				Engine::Safe_Delete( (*iter_box) );
				++iter_box;
			}
			iter->second.clear();
			iter = m_mapAniSync.erase(iter);

			if( iter == iter_end )
				m_mapAniSync.clear();
		}
		delete this;
	}

	return 0;
}
