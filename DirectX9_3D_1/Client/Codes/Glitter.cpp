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

	//������ �ʱ�ȭ
	m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);
	m_pTransCom->m_vPosition = _vec3(180.f, 0.f, 215.f);
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian(0);

	m_iMonsterAnimationIndex = Glitter::Spawn;
	m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
	m_eMonsterState = CMonster::MS_Super;

	bInput = false;

	m_bAppear = false; // ó���� false�� �س��� ���� ���ϰ� �Ѵ�.

	//�ٿ���ڽ� �ε�
	CMonster::Load_AttackBox(L"../Bin/Resources/Data/Box/Glitter/Glitter_AttackBox.dat");
	CMonster::Load_DamageBox(L"../Bin/Resources/Data/Box/Glitter/Glitter_DamageBox.dat");
	// Boss_HP UI

	// ���� �ε�
	Load_Pattern();

	// �ִϸ��̼� ��ũ
	CMonster::Load_FootAnimationSync(L"../Bin/Resources/Data/Box/Glitter/Glitter_Sync.dat");

	Initialize_Min_Max_Radius();

	m_fMaxDamageValue = 500.f;
	m_fDownValue = 0.f;
	m_fMonsterDownValue = 0.f;

	m_vMoveMin		= _vec3(-7.f, 0.f, -7.f);
	m_vMoveMax		= _vec3(7.f, 10.f, 7.f);
	m_fMoveRadius	= D3DXVec3Length(&(m_vMoveMax - m_vMoveMin)) * m_pTransCom->m_vScale.x;

	//���� ����
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

	//Mesh Component	// �÷��̾ ������ ���̳��͸޽����� CDynamicMesh �� �ε��� ��.
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

		CMonster::CollisionCheckFunction();			//�浹�� ���� �˻縦 �Ѵ�.
		Check_Monster_Satae();						//�ִϸ��̼�, �ð�, Ư�� ���ǿ� ���� ������ ���� Ÿ���� ���������� üũ�Ͽ� �ٲپ� �ִ� �� => �� ���¿� ���� �÷��̾�� �浹 �˻縦 �ǳ� �ٰų� ���Ͱ� �ٿ� �����϶� �������� �� ���ų� �ٽ� �ٿ�ִϸ��̼��� Set ���ϵ��� �Ѵ�.
		AnimationPattern_Immediately_Change();		//�浹�� ���� ������ �ִϸ��̼��� �ﰢ������ �ٲ۴�. �浹 ������ ���� Monster class���� ���������� ������� �浹, Math Mgr���� ����� Ȱ��, ������ Ÿ���� ���� Ư�� ����� �������϶��� �浹 ���ص� �������� �ް� �ؾ� �Ѵ�.
		Check_PlayerAttack_AccordingToHP();			//�浹�� ���� ������ HP������ �ǰ� ����Ʈ ����
		BattleAnimationPattern();					//�÷��̾��� ���ݿ� �浹�� ���ϰ� �ִϸ��̼��� ������ ����� ���� �˰����� ���� ������ AI�� ���ư����� �Ѵ�.
		AnimationTimeChange();						//�⺻������ m_fAnimationSpeed�� �ӵ��� 2.f������ ���ο� ���³� Ư������� �ӵ��� ���⶧ ���⼭ �Ѵ�.
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

	//����
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
	//���� �浹���Ǿ �����Ǿ��ִ� ���¿��� �ٿ��� �Ǹ� �浹���Ǿ �����Ѵ�.
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
			printf("���� %d\n", m_iMonsterAnimationIndex);

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
				printf("==========�״»���==========\n");
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
				printf("==========�ٴ»���==========\n");
				CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::vampire_step1 + m_iSoundStep));
				++m_iSoundStep;
				if(3 < m_iSoundStep)
					m_iSoundStep = 0;
				m_fStepTime = 0.f;
			}

			Compute_AngleToPlayer();
			m_bRun = true;

			//���� �Ÿ��� �� �� ���� �޷��´�.
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
					printf("==========����ü��1����==========\n");
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::vampire_hurt1 + rand()%3));
					m_bEffectOnce = true;
				}
			}
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.4f) && !m_pMeshCom->Check_EndPeriodMulTime(0.5f))
			{
				if(false == m_bEffectOnce)
				{
					printf("==========����ü��1-1����==========\n");
					CSoundMgr::GetInstance()->PlayEffect(SOUND::Bear_Step);
					m_bEffectOnce = true;
				}
			}
			else
				m_bEffectOnce = false;
		}
		else if(Glitter::vampire_attack_lightning_onehand_glitter == m_iMonsterAnimationIndex)
		{
			//���� ������ �� ���� ���
			if(m_pMeshCom->Check_EndPeriodMulTime(0.f) && !m_pMeshCom->Check_EndPeriodMulTime(0.1f))
			{
				if(m_bRotationOnce == true)
				{
					printf("==========�޼հ��ݻ���==========\n");
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::vampire_shout1 + rand()%3));
				}
				Compute_AngleToPlayer();
			}
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.2f) && !m_pMeshCom->Check_EndPeriodMulTime(0.3f))
			{
				//�յ���ٴϴ� ����Ʈ ����
				if(NULL == m_pLHShortEffect)
				{
					printf("==========�޼հ��ݻ���2==========\n");
					m_pLHShortEffect = CLHLightning::Create(Engine::Get_GraphicDev(), &m_pTransCom->m_matWorld, m_pmatAttachLH);
					CSoundMgr::GetInstance()->PlayEffect(SOUND::effect_fireball_appear);
				}
			}
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.55f) && !m_pMeshCom->Check_EndPeriodMulTime(0.56f))
			{
				//�յ���ٴϴ� ����Ʈ ����
				if(NULL != m_pLHShortEffect)
				{
					m_pLHShortEffect->Set_Delete(true);
					m_pLHShortEffect = NULL;
				}
			}
			//�������� ������ ������ ���� ���
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.56f) && !m_pMeshCom->Check_EndPeriodMulTime(0.57f))
			{
				Compute_AngleToPlayer();
				if(false == m_bEffectOnce)
				{
					printf("==========�޼հ��ݻ���3==========\n");
					CSoundMgr::GetInstance()->PlayEffect(SOUND::effect_lightning_charge);

					//�������� ����Ʈ ����
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
		else if(Glitter::vampire_attack_lightning_strike == m_iMonsterAnimationIndex) // ������
		{
			//���� ������ �� ���� ���
			if(m_pMeshCom->Check_EndPeriodMulTime(0.f) && !m_pMeshCom->Check_EndPeriodMulTime(0.1f))
			{
				if(m_bRotationOnce == true)
				{
					printf("==========�����հ��ݻ���==========\n");
					CSoundMgr::GetInstance()->PlayEffect(SOUND::effect_lightning_charge);
				}
				Compute_AngleToPlayer();
			}
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.2f) && !m_pMeshCom->Check_EndPeriodMulTime(0.3f))
			{
				//�յ���ٴϴ� ����Ʈ ����
				if(NULL == m_pLHShortEffect)
				{
					printf("==========�����հ��ݻ���2==========\n");
					m_pLHShortEffect = CRHLightning::Create(Engine::Get_GraphicDev(), &m_pTransCom->m_matWorld, m_pmatAttachRH);
					CSoundMgr::GetInstance()->PlayEffect(SOUND::effect_fireball_appear);
				}
			}
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.51f) && !m_pMeshCom->Check_EndPeriodMulTime(0.56f))
			{
				//�յ���ٴϴ� ����Ʈ ����
				if(NULL != m_pLHShortEffect)
				{
					m_pLHShortEffect->Set_Delete(true);
					m_pLHShortEffect = NULL;
				}
			}
			//������ ���� ���
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.55f) && !m_pMeshCom->Check_EndPeriodMulTime(0.6f))
			{
				//m_bRotationOnce = true;
				Compute_AngleToPlayer();
				if(false == m_bEffectOnce)
				{
					//�������� ����Ʈ ����
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
		else if(Glitter::vampire_attack_lightning_around == m_iMonsterAnimationIndex) // ����
		{
			//���� ������ �� ���� ���
			if(m_pMeshCom->Check_EndPeriodMulTime(0.f) && !m_pMeshCom->Check_EndPeriodMulTime(0.1f))
			{
				//m_bRotationOnce = true;
				Compute_AngleToPlayer();
				//��տ� ����Ʈ ����
				if(NULL == m_pALHShortEffect)
				{
					printf("==========���߰��ݻ���==========\n");
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
				//���� ����Ʈ ����
				if(NULL == m_pExplosionEffect)
				{
					printf("==========���߰��ݻ���2==========\n");
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
		else if(Glitter::vampire_attack_forehand2_lightning == m_iMonsterAnimationIndex) //���
		{
			//���� ������ �� ���� ���
			if(m_pMeshCom->Check_EndPeriodMulTime(0.f) && !m_pMeshCom->Check_EndPeriodMulTime(0.1f))
			{
				//m_bRotationOnce = true;
				Compute_AngleToPlayer();
				
				//����Ʈ ����
				if(NULL == m_pALHShortEffect)
				{
					printf("==========��հ��ݻ���==========\n");
					m_pALHShortEffect = CForeHandLightning::Create(Engine::Get_GraphicDev(), &m_pTransCom->m_matWorld, m_pmatAttachLH,L"Test_Lightning");
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::vampire_shout_long1 + rand()%3));
					CSoundMgr::GetInstance()->PlayEffect(SOUND::effect_lightning_charge);
				}
				if(NULL == m_pARHShortEffect)
					m_pARHShortEffect = CForeHandLightning::Create(Engine::Get_GraphicDev(), &m_pTransCom->m_matWorld, m_pmatAttachRH,L"Test_Lightning");

			}
			//������ ���� ���
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.43f) && !m_pMeshCom->Check_EndPeriodMulTime(0.44f))
			{
				//m_bRotationOnce = true;
				Compute_AngleToPlayer();
			}
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.45f) && !m_pMeshCom->Check_EndPeriodMulTime(0.5f))
			{
				//�� ���������� ��ũ������ ���ݸָ� �̵����Ѿ��ؼ� 2.f�� ���.
				m_fDistanceSize = 2.f;
			}
			else if(m_pMeshCom->Check_EndPeriodMulTime(0.57f) && !m_pMeshCom->Check_EndPeriodMulTime(0.58f))
			{
				//���� ����Ʈ ����
				if(NULL == m_pExplosionEffect)
				{
					printf("==========��հ��ݻ���2==========\n");
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
		printf("�浹�̴�!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
		m_tMonsterInfo.fHP -= CPlayer::GetInstance()->GetPlayerAttackValue();
		//����ų ó��
		float MonsterHp = m_tMonsterInfo.fHP;	//���� ����ų�� ���� �ӽ÷� ���� 
		m_tMonsterInfo.fHP -= CPlayer::GetInstance()->GetPlayerAttackValue();

		if(m_tMonsterInfo.fHP <= 0.f)
		{
			m_tMonsterInfo.fHP = 0.f;

			if((CPlayer::GetInstance()->GetPlayerAttackValue() - MonsterHp) <= 100)//Ȥ�� 100���� �������� ������ ����� ���� ����ó����
			{
				CDamageFont::Create(m_pGraphicDev, DFONT_TYPE_OVERKILL, DFONT_OBJ_MONSTER, (int)(CPlayer::GetInstance()->GetPlayerAttackValue() - MonsterHp + 100), m_vCollisionPos);	//���Ͱ� ���� �������� ����.
			}
			else
				CDamageFont::Create(m_pGraphicDev, DFONT_TYPE_OVERKILL, DFONT_OBJ_MONSTER, (int)(CPlayer::GetInstance()->GetPlayerAttackValue() - MonsterHp), m_vCollisionPos);	//���Ͱ� ���� �������� ����.
		}
		else
		{
			CDamageFont::Create(m_pGraphicDev, DFONT_TYPE_DAMAGE, DFONT_OBJ_MONSTER, (int)CPlayer::GetInstance()->GetPlayerAttackValue(), m_vCollisionPos);	//���Ͱ� ���� �������� ����.
		}

		printf("���� ü�� == %f\n", m_tMonsterInfo.fHP);

		//�浹�Ǹ� �߰� ���� UI�� ������ �ٲ���Ѵ�.
		CGlitterHP::GetInstance()->SetRender(true);
		CMoonchopperHP::GetInstance()->SetRender(false);

		CPlayer::GetInstance()->Player_AttackSuccess_InfoChange();
		CEffectMgr::GetInstance()->Play_Effect(L"Player_Slash_Effect", m_pTransCom->m_vPosition + _vec3(0.f, m_vCollisionPos.y, 0.f), false, true, _vec3(3.f, 15.f, 0.f), _vec3(m_fAngleX, 0.f, m_fAngleZ), 0.f, false, 0.f, NULL);
		CPlayerBeShotUI::GetInstance()->PlayerBeShotUI();

		if(MS_Super != m_eMonsterState)
		{
			//�����ؾ���
			//���� �÷��̾ ���� ���⺤��
			_vec3 vDirToP = m_vPlayerPos - m_pTransCom->m_vPosition;
			D3DXVec3Normalize(&vDirToP, &vDirToP);

			//���ֺ����ִٸ�
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
// 	else if(Glitter::vampire_attack_lightning_around == m_iMonsterAnimationIndex) // ����
// 	{
// 
// 		SoundEffectPlay(SOUND::effect_lightning_charge, 0.f, 0, 5);
// 		SoundEffectPlay(SOUND::EFF(SOUND::vampire_laugh1 + rand()%3), 0.f, 1, 5);
// 		SoundEffectPlay(SOUND::effect_lightning_explosion, 0.38f, 2, 5);
// 		SoundEffectPlay(SOUND::effect_lightning_shock, 0.38f, 3, 5);
// 	}
// 	else if(Glitter::vampire_attack_forehand2_lightning == m_iMonsterAnimationIndex) //���
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

			vMonsterDir *= -1.f; //x���� ��ü�� 0 0 -1�����ִµ� ���� ���⺤�ʹ� 0 0 1�̶� ������

			//�÷��̾���� ������ ����.
			_float fAngle = acosf( D3DXVec3Dot(&vMonsterDir, &vDir) );

			//acos�Լ��� 0 ~ 180�� �����������.
			_matrix matRotationDefault;
			D3DXMatrixRotationY(&matRotationDefault, D3DXToRadian(0.f));
			_vec3 vRight;
			memcpy( &vRight, &(m_pTransCom->m_matWorld.m[0][0]), sizeof(_vec3) );	
			D3DXVec3Normalize(&vRight, &vRight);									
			D3DXVec3TransformNormal(&vRight, &vRight, &matRotationDefault);	
			_float fAngle_Right = acosf( D3DXVec3Dot(&vRight, &vDir) );

			if( D3DXToDegree(fAngle_Right) <= 90.f )
				fAngle = D3DX_PI * 2.f - fAngle;//fAngle = fAngle * -1.f;

			//�Ҽ��� ���� 
			_float fResultAngle = (_int)((m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] + fAngle) * 100 ) / 100.f;

			m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = fResultAngle;
			printf("m_fAngle === %f\n", m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);

			m_fOriginAngle = fResultAngle;

			m_bRotationOnce = false;
		}
	}
	m_pTransCom->Update(m_fTimeDelta);

	//���� �÷��̾ ���� ���⺤��
	_vec3 vDirToP = m_vPlayerPos - m_pTransCom->m_vPosition;
	D3DXVec3Normalize(&vDirToP, &vDirToP);

	_matrix matRotY;
	D3DXMatrixRotationY(&matRotY, m_fOriginAngle);
	D3DXVec3TransformCoord(&vDirToP, &vDirToP, &matRotY); //���� ����� Y�� ȸ�� ����ŭ ȸ�� ��Ų��.
	m_vOriginDir = vDirToP;
	/*printf("���� �÷��̾ ���� ���⺤��\n %f  %f  %f\n", vDirToP.x, vDirToP.y, vDirToP.z);*/

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

	//�ٰŸ� ����
	m_vecInPattern.push_back(Glitter::vampire_attack_lightning_onehand_glitter); //�޼ն���Ʈ��
	//m_vecInPattern.push_back(Glitter::RunN); //�޸���
	m_vecInPattern.push_back(Glitter::vampire_attack_lightning_around); //���� ����
	m_vecInPattern.push_back(Glitter::vampire_attack_forehand2_lightning); //��ġ��

	//�߰Ÿ� ����
	//m_vecMidPattern.push_back(Glitter::RunN); //�޸���
	m_vecMidPattern.push_back(Glitter::vampire_attack_lightning_strike); //�����ն���Ʈ��
	m_vecMidPattern.push_back(Glitter::vampire_attack_forehand2_lightning); //��ġ��
	m_vecMidPattern.push_back(Glitter::RunN); //�޸���
	m_vecMidPattern.push_back(Glitter::vampire_attack_lightning_onehand_glitter); //�޼ն���Ʈ��
	m_vecMidPattern.push_back(Glitter::vampire_attack_lightning_around); //���� ����
	//��Ÿ� ����
	m_vecOutPattern.push_back(Glitter::RunN); //�޸���
	m_vecOutPattern.push_back(Glitter::vampire_attack_lightning_strike); //�����ն���Ʈ��

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

		//printf("Pattern_Near_Distance����\n");
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

		//printf("Pattern_Mid_Distance����\n");
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
			//printf("��������̴�\n");
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
		//printf("Pattern_Out_Distance����\n");
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
