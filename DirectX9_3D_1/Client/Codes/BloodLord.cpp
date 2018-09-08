#include "stdafx.h"
#include "BloodLord.h"

#include "Engine_Defines.h"
#include "Export_Function.h"

#include "Line2D.h"
#include "CullingMgr.h"

#include "Player.h"
#include "DynamicCamera.h"

#include "DamageFont.h"

#include "SoundMgr.h"
#include "EffectMgr.h"

#include "BloodLordHP.h"

#include "BreathEffect.h"
#include "MeteoEffect.h"

#include "FadeInOut.h"

#include "PlayerBeShotUI.h"

#include "RoyalGuard.h"

CBloodLord::CBloodLord(LPDIRECT3DDEVICE9 pGraphicDev)
: CMonster(pGraphicDev)
, m_fPlaySpeed(0.f)
, m_bBlink(false)
, m_eCurState(BS_IDLE)
, m_iNearPattern(0), m_iMidPattern(0), m_iFarPattern(0)
, m_bNearPattern(false), m_bMidPattern(false), m_bFarPattern(false)
, m_fBlinkTime(0.f)
, m_fBlinkOverTime(0.f)
, m_fBlinkEffTime(0.35f)
, m_fBlinkEndTime(0.37f)
, m_bRun(false)
, m_fRunDistance(30.f)
, m_bIdle(false)	// ���� ������
, m_fNearDistance(40.f)
, m_fMidDistance(100.f)	
, m_fFarDistance(150.f)
, m_fRevoluAngle(0.f)	// attack_dancing���� ���� ������ �ʿ��� ������
, m_bStart(false)
, m_bRotation(false)
, m_bAttackDuring(false)
, m_bDamage(false)
, m_bDamageSuperArmor(false)
, m_iBlinkNot(-1)
, m_bBlinkEnd(true)
, m_bInsideNavi(true)
, m_bDownState(false)
, m_bDownTime(false)
, m_bDisDown(false)
, m_fDownTime(0.f)
, m_fDownOverTime(0.f)
, m_bCutScene1(false)
, m_bCutScene2(false)
, m_bCutSceneTerm(false)
, m_fCutSceneEnd(0.f)
, m_fCutSceneTerm(0.f)
, m_bMeteoSkill(false)
, m_bMeteo(false)
, m_vMonsterDir(0.f, 0.f, -1.f)
, m_fRotSpeed(15.f)
, m_bRot_BreathEnd(false)
, m_fLowHealth(0.f)
, m_bDie(false)
, m_bRealDie(false)
, m_bCutsceneFadeCheck(false)
, m_bCutsceneFadeCheckTwo(false), m_iBloodField(-1)
, m_fBreathSize(0.1f)
, m_fBreathTime(0.f)
, m_fBreathOverTime(0.f)
, m_bBreath(false)
// Ʈ����
, m_pmatLeftElbow(NULL)
, m_pmatRightElbow(NULL)
, m_pmatLeftOne(NULL)
, m_pmatLeftTwo(NULL)
, m_pmatLeftThree(NULL)
, m_pmatRightOne(NULL)
, m_pmatRightTwo(NULL)
, m_pmatRightThree(NULL)
, m_vLeftElbow(0.f, 0.f, 0.f)
, m_vRightElbow(0.f, 0.f, 0.f)
, m_vLeftOne(0.f, 0.f, 0.f)
, m_vLeftTwo(0.f, 0.f, 0.f)
, m_vLeftThree(0.f, 0.f, 0.f)
, m_vRightOne(0.f, 0.f, 0.f)
, m_vRightTwo(0.f, 0.f, 0.f)
, m_vRightThree(0.f, 0.f, 0.f)
{
	D3DXMatrixIdentity(&m_matBloodBreath);
	D3DXMatrixIdentity(&m_matDancing);
	D3DXMatrixRotationY(&m_matRotationDefault, D3DXToRadian(-90.f));

	ZeroMemory(m_bCutsceneSoundPlay, sizeof(_bool)*20);
}

CBloodLord::~CBloodLord(void)
{

}

HRESULT CBloodLord::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Monster");
	if(NULL == m_pEffect)
		return E_FAIL;

	m_pShadowEffect = Engine::Get_EffectHandle(L"Shader_Shadow");
	if(NULL == m_pShadowEffect)
		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	//������ �ʱ�ȭ
	m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);
	m_pTransCom->m_vPosition = _vec3(183.f, 0.f, 300.f);
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian(-90);

	m_iMonsterAnimationIndex = BloodLord::idle;
	/*m_pMeshCom->Set_AnimationSet(BloodLord::idle);*/

	//�ٿ���ڽ� �ε�
	CMonster::Load_AttackBox(L"../Bin/Resources/Data/Box/BloodLord/BloodLordAttackBox.dat");
	CMonster::Load_DamageBox(L"../Bin/Resources/Data/Box/BloodLord/BloodLordDamageBox.dat");

// 	m_tMonsterInfo.fMaxHP = 50000.f;
// 	m_tMonsterInfo.fHP = 50000.f;

	// ���� �ε�
	Load_Pattern();

	Initialize_Min_Max_Radius();

	m_fMaxDamageValue = 100.f;
	m_fDownValue = 0.f;
	m_fMonsterDownValue = 0.f;

	m_vMoveMin		= _vec3(-10.f, 0.f, -10.f);
	m_vMoveMax		= _vec3(10.f, 10.f, 10.f);
	m_fMoveRadius	= D3DXVec3Length(&(m_vMoveMax - m_vMoveMin)) * m_pTransCom->m_vScale.x;

	m_tMonsterInfo.fAttack = 500.f;
	m_tMonsterInfo.fMaxHP = 200000.f;
	m_tMonsterInfo.fHP = m_tMonsterInfo.fMaxHP;

	m_pBossFaceUI	= CBossFaceUI::Create(m_pGraphicDev);
	m_pBossFaceUI->SetFaceNumber(0);

	// Boss_HP UI => HP�ʱ�ȭ ���Ŀ� �����ؾߵ� �ֵ��
	m_eMonsterType = MT_Boss;
	CBloodLordHP::GetInstance()->SetpMonster(this);

	return S_OK;
}

HRESULT CBloodLord::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	//Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	//Mesh Component	// �÷��̾ ������ ���̳��͸޽����� CDynamicMesh �� �ε��� ��.
	pComponent = m_pMeshCom = (Engine::CDynamicMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_BloodLord");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	m_vOriginalPos = m_pTransCom->m_vPosition;
	m_vAfterPos = m_pTransCom->m_vPosition;

	return S_OK;
}

_int CBloodLord::Update(const float& fTimeDelta)
{
	Create_TrailPos();

	m_fTimeDelta		= fTimeDelta;

	if( m_bBlink )	// ��ũ ���϶� �ð� ����� ����
		m_fBlinkOverTime += fTimeDelta;

	if( m_bDamageSuperArmor )	// �ǰݵǰ� ���۾Ƹ� ���� �ð������ ����
		m_fSuperOverTime += fTimeDelta;

	if( m_bDownTime )
		m_fDownOverTime += fTimeDelta;

	if( m_bCutSceneTerm )
	{
		m_fCutSceneTerm += fTimeDelta;
	}

	if( m_bBreath )
	{
		m_fBreathOverTime += fTimeDelta;
	}

	if( m_bCutSceneTerm && m_fCutSceneEnd + 0.8f <= m_fCutSceneTerm )
	{
		Engine::CRenderer::m_bRadialBlurRender = false;
	}

	if( m_bCutSceneTerm && m_fCutSceneEnd + 1.5f <= m_fCutSceneTerm )
	{
		m_bCutScene2 = true;
		m_bCutSceneTerm = false;
		m_fCutSceneEnd = 0.f;
		m_fCutSceneTerm = 0.f;
	}

	CameraModeCheck();
	//CMonster::Update_Monster_AnimationIndex();	//�ִϸ��̼� �ٸ��� ����?
	if( m_bBlink == false )
		CMonster::CollisionCheckFunction();			//�浹�� ���� �˻縦 �Ѵ�.
	Check_Monster_Satae();						//�ִϸ��̼�, �ð�, Ư�� ���ǿ� ���� ������ ���� Ÿ���� ���������� üũ�Ͽ� �ٲپ� �ִ� �� => �� ���¿� ���� �÷��̾�� �浹 �˻縦 �ǳ� �ٰų� ���Ͱ� �ٿ� �����϶� �������� �� ���ų� �ٽ� �ٿ�ִϸ��̼��� Set ���ϵ��� �Ѵ�.
	AnimationPattern_Immediately_Change();		//�浹�� ���� ������ �ִϸ��̼��� �ﰢ������ �ٲ۴�. �浹 ������ ���� Monster class���� ���������� ������� �浹, Math Mgr���� ����� Ȱ��, ������ Ÿ���� ���� Ư�� ����� �������϶��� �浹 ���ص� �������� �ް� �ؾ� �Ѵ�.
	Check_PlayerAttack_AccordingToHP();			//�浹�� ���� ������ HP������ �ǰ� ����Ʈ ����
	BattleAnimationPattern();					//�÷��̾��� ���ݿ� �浹�� ���ϰ� �ִϸ��̼��� ������ ����� ���� �˰����� ���� ������ AI�� ���ư����� �Ѵ�.
	AnimationTimeChange();						//�⺻������ m_fAnimationSpeed�� �ӵ��� 2.f������ ���ο� ���³� Ư������� �ӵ��� ���⶧ ���⼭ �Ѵ�.

	Check_DamageAfter_SuperArmor();				// �ǰ��� �����ð� ���۾Ƹ� ����

	//// �ҿ뵹�� �����϶��� ���� ���� �̵��ϴ°� x �÷��̾� �����̵� �ٸ� �������� �̵��ϱ� ���ؼ�
	if( m_bBlink == false && m_bBlinkEnd && m_iMonsterAnimationIndex != BloodLord::vamplord_attack_dancing_during
		&& m_iMonsterAnimationIndex != BloodLord::vamplord_wrap_air_hard_end )
		CMonster::MonsterMoveFunction("ValveBiped_Bip01_");		//������ ������ AnimationTimeChange���� ������ �ӵ��� ���� �ִϸ��̼��� ���� �����δ�.
	else if( m_bBlink && m_bBlinkEnd )
	{
		if( m_pTransCom->m_vPosition.y == 45.f )
			CMonster::MonsterMoveFunction("ValveBiped_Bip01_");
		m_pTransCom->m_vPosition.y = 45.f;
	}
	else if( m_iMonsterAnimationIndex == BloodLord::vamplord_attack_dancing_begin ||
		m_iMonsterAnimationIndex == BloodLord::vamplord_attack_dancing_during||
		m_iMonsterAnimationIndex == BloodLord::vamplord_attack_dancing_end )
		Check_Skill_Dancing();					
	// �ҿ뵹�� �������϶��� �÷��̾� �ֺ����� �����̵���

#pragma region ����� �ӽ� �ڵ�

	KeyInput();

	if( m_bDisHP == true )
		m_eCurState = BS_SUPER;

	if( m_iMonsterAnimationIndex == BloodLord::vamplord_cutscene_1  )	// �ƽ�1���϶� ���̰� ���� ����
		m_pTransCom->m_vPosition.y = 45.f;

	if( (m_bPlayerAttack_Body_CollisionCheck || m_bPlayerEffect_Body_CollisionCheck) )	// ��ũ ���°� �ƴϸ鼭 �÷��̾� ���ݿ� �浹���� ��
	{
		m_bDamage = true;
	}
	else
		m_bDamage = false;

	if(m_bPlayerAttack_MyAttack_CollisionCheck)	// ���� ���� �ڽ� - �÷��̾� ���� �ڽ� �浹 ��( �ĳ��� )
		m_pMeshCom->Set_AnimationSet(BloodLord::duel_lose_damage);

#pragma endregion

	SoundPlayer();

	CGameObject::Update(fTimeDelta);
	m_pBossFaceUI->Update(fTimeDelta);
	CBloodLordHP::GetInstance()->Update(fTimeDelta);

	// ��ũ �����̰ų� ī�޶� �ø� ���̸� �����׷쿡 �������� �ʰ� �Ѵ�.
	if( !m_bRealDie && m_bStart && !m_bCutSceneTerm && !m_bBlink && CCullingMgr::GetInstance()->CullingCamera(&m_pTransCom->m_vPosition, m_pSphere->fRadius))	
	{
		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
		Engine::Add_RenderGroup(Engine::CRenderer::RENDER_SHADOW, this);
	}
	return 0;
}

void CBloodLord::Render(void)
{
	if(NULL == m_pEffect)		return;
	Set_ContantTable();	

	CMonster::Render();

	if( !m_bRealDie && m_bStart && !m_bCutSceneTerm && !m_bBlink )	// ��ũ �������϶��� �����ӵ� �������� �ʰ� �׸����� �ʴ´�.
	{
		if( !m_bRealDie )
			m_pMeshCom->Move_Frame(m_fRenderTimeDelta);
		m_pMeshCom->Render_MeshForShader(m_pEffect, false);
	}
}

void CBloodLord::Render_Shadow(void)
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


CBloodLord* CBloodLord::Create( LPDIRECT3DDEVICE9 pGraphicDev )
{
	CBloodLord*	pMonster = new CBloodLord(pGraphicDev);

	if( FAILED(pMonster->Initialize()) )
	{
		MSG_BOX("BloodLord Create Failed");
		::Safe_Release(pMonster);
	}
	return pMonster;
}

void CBloodLord::Set_ContantTable(void)
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

void CBloodLord::CameraModeCheck(void)
{
	if(!m_bCameraModeOn)
	{
		_vec3 PlayerPos = CPlayer::GetInstance()->Get_TransCom()->m_vPosition;

		if(PlayerPos.x >= 170 && PlayerPos.x <= 190 && PlayerPos.z >= 210 && PlayerPos.z <= 230)
		{
			if(!m_bCameraModeCheck)
				CFadeInOut::GetInstance()->SetFadeInOut(true, 4.0f);
			m_bCameraModeCheck = true;
		}

		if(m_bCameraModeCheck)
		{
			if(!CFadeInOut::GetInstance()->GetRender())
			{
				CFadeInOut::GetInstance()->SetFadeInOut(false, 4.0f);
				m_bCutScene1 = true;
				m_bCameraModeOn = true;
				m_bStart = true;
				m_iMonsterAnimationIndex = BloodLord::vamplord_cutscene_1; 
				m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
				m_bIdle = false;

				CDynamicCamera::GetInstance()->SetCameraTargetTransCom(m_pTransCom);
				CDynamicCamera::GetInstance()->CameraModePlayer(0);
				CSoundMgr::GetInstance()->SetEnvironmentChannel(true);
				CSoundMgr::GetInstance()->PlayBGM(SOUND::BGM_BloodLord, 1.0f);   //���� �ε� �ƾ� �Ѿ�� BGM ���
			}
		}
	}

}

void CBloodLord::Check_Monster_Satae(void)
{
	//m_eMonsterState�� ��������
}
void CBloodLord::AnimationPattern_Immediately_Change(void)
{
	if(m_bCutsceneFadeCheckTwo)
	{
		if(!CFadeInOut::GetInstance()->GetRender())
		{
			CFadeInOut::GetInstance()->SetFadeInOut(false, 4.f);
			m_bCutsceneFadeCheckTwo = false;

		}
	}

	if( m_iMonsterAnimationIndex == BloodLord::vamplord_cutscene_2 )
	{
		if( m_pMeshCom->Check_EndPeriod() )
		{
			m_bCutsceneFadeCheckTwo = true;
			m_eCurState = BS_SUPER;
			m_iMonsterAnimationIndex = BloodLord::idle;
			m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
			CSoundMgr::GetInstance()->SetEnvironmentChannel(false);
			CSoundMgr::GetInstance()->PlayVolumeBGM(0.6f);

			m_pBossFaceUI->SetRender(true);
			CBloodLordHP::GetInstance()->SetRender(true);


			Engine::CGameObject*		pGameObject	= NULL;	

			pGameObject = CRoyalGuard::Create(m_pGraphicDev, _vec3(114.f, 0.f, 264.f));
			if(NULL == pGameObject)		return;
			m_pLayer->Ready_Object(L"RoyalGuard", pGameObject);
			pGameObject = CRoyalGuard::Create(m_pGraphicDev, _vec3(116.f, 0.f, 246.f));
			if(NULL == pGameObject)		return;
			m_pLayer->Ready_Object(L"RoyalGuard", pGameObject);
			pGameObject = CRoyalGuard::Create(m_pGraphicDev, _vec3(143.f, 0.f, 265.f));
			if(NULL == pGameObject)		return;
			m_pLayer->Ready_Object(L"RoyalGuard", pGameObject);
			pGameObject = CRoyalGuard::Create(m_pGraphicDev, _vec3(216.f, 0.f, 254.f));
			if(NULL == pGameObject)		return;
			m_pLayer->Ready_Object(L"RoyalGuard", pGameObject);
			pGameObject = CRoyalGuard::Create(m_pGraphicDev, _vec3(213.f, 0.f, 236.f));
			if(NULL == pGameObject)		return;
			m_pLayer->Ready_Object(L"RoyalGuard", pGameObject);
			pGameObject = CRoyalGuard::Create(m_pGraphicDev, _vec3(226.f, 0.f, 185.f));
			if(NULL == pGameObject)		return;
			m_pLayer->Ready_Object(L"RoyalGuard", pGameObject);
			pGameObject = CRoyalGuard::Create(m_pGraphicDev, _vec3(210.f, 0.f, 180.f));
			if(NULL == pGameObject)		return;
			m_pLayer->Ready_Object(L"RoyalGuard", pGameObject);
			pGameObject = CRoyalGuard::Create(m_pGraphicDev, _vec3(157.f, 0.f, 175.f));
			if(NULL == pGameObject)		return;
			m_pLayer->Ready_Object(L"RoyalGuard", pGameObject);
			pGameObject = CRoyalGuard::Create(m_pGraphicDev, _vec3(146.f, 0.f, 173.f));
			if(NULL == pGameObject)		return;
			m_pLayer->Ready_Object(L"RoyalGuard", pGameObject);
			pGameObject = CRoyalGuard::Create(m_pGraphicDev, _vec3(141.f, 0.f, 199.f));
			if(NULL == pGameObject)		return;
			m_pLayer->Ready_Object(L"RoyalGuard", pGameObject);
		}
	}
	else if( m_pMeshCom->Check_EndPeriod() )
	{	
		//// �ƽ�1 -> �ƽ�2
		if( m_iMonsterAnimationIndex == BloodLord::vamplord_cutscene_1  )
		{
			if( m_fCutSceneEnd == 0.f )
			{
				m_fCutSceneEnd = m_fTimeDelta;
				m_bCutSceneTerm = true;
				if(!m_bCutScene2)
					Engine::CRenderer::m_bRadialBlurRender = true;
			}

			if( m_bCutScene2 )
			{
				m_bCutScene1 = false;
				m_eCurState = BS_SUPER;
				m_iMonsterAnimationIndex = BloodLord::vamplord_cutscene_2;
				m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
				m_pTransCom->m_vPosition.y = 0.f;
				m_pTransCom->m_vPosition = _vec3(183.f, 0.f, 275.f);
			}
		}
		//// ���׿� -> �� -> ��
		else if( m_iMonsterAnimationIndex == BloodLord::vamplord_attack_bloodfield_begin1 ||
			m_iMonsterAnimationIndex == BloodLord::vamplord_attack_bloodfield_during1 )
		{
			m_eCurState = BS_SUPER;
			m_iMonsterAnimationIndex += 2;
			m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
		}
		//// �޸��� -> �޸��� -> ��ũ
		else if( m_iMonsterAnimationIndex == BloodLord::run_N )
		{
			m_iMonsterAnimationIndex = BloodLord::run_N;
			m_bRun = true;

			if( !Check_Player_Distance(m_fRunDistance) )
			{
				m_eCurState = BS_RUN;
				m_pMeshCom->Set_AnimationSetSameFinish( BloodLord::run_N );
				m_pTransCom->m_vPosition = m_vAfterPos;
			}
			else
			{
				m_bRun = false;
				m_eCurState = BS_SUPER;
				m_pMeshCom->Set_AnimationSet( BloodLord::vamplord_wrap_air2_begin );
				m_iMonsterAnimationIndex = BloodLord::vamplord_wrap_air2_begin;
			}
		}
		//// ����ȸ�� -> �� -> ��
		else if( m_iMonsterAnimationIndex == BloodLord::vamplord_attack_dancing_begin ||
			m_iMonsterAnimationIndex == BloodLord::vamplord_attack_dancing_during )
		{
			if( m_iMonsterAnimationIndex == BloodLord::vamplord_attack_dancing_begin )
			{
				_vec3 vDir;
				memcpy( &vDir, &(m_pTransCom->m_matWorld.m[2][0]), sizeof(_vec3) );
				D3DXVec3Normalize(&vDir, &vDir);
				D3DXVec3TransformNormal(&vDir, &vDir, &m_matRotationDefault);

				_vec3 vPos = 20.f * vDir;
				memcpy( &m_matDancing.m[3][0], &(m_pTransCom->m_matWorld.m[3][0]), sizeof(_vec3));
				m_matDancing.m[3][0] += vPos.x;
				m_matDancing.m[3][2] += vPos.z;

				_vec3 vPos1, vPos2;
				memcpy( &vPos1, &(m_pTransCom->m_matWorld.m[3][0]), sizeof(_vec3) );
				memcpy( &vPos2, &(m_matDancing.m[3][0]), sizeof(_vec3) );

				m_fDancingDistance_x = D3DXVec3Length(&(vPos1 - vPos2));
				_vec3 vNew = m_fDancingDistance_x * -vDir;

				m_fDancingDistance_x = vNew.x;
				m_fDancingDistance_z = vNew.z;
			}

			m_eCurState = BS_SUPER;
			m_iMonsterAnimationIndex += 1;
			m_pMeshCom->Set_AnimationSet( m_iMonsterAnimationIndex );
		}
		//// �ǿ����� -> �� -> ��
		else if( m_iMonsterAnimationIndex == BloodLord::vamplord_swamp_begin || 
			m_iMonsterAnimationIndex == BloodLord::vamplord_swamp_during )
		{
			m_eCurState = BS_SUPER;
			m_iMonsterAnimationIndex += 1;
			m_pMeshCom->Set_AnimationSet( m_iMonsterAnimationIndex );
		}
	}	
}

void CBloodLord::Check_DownState()
{
	if( m_bDownState )	// �ٿ�����϶� ���� ��� ����
	{
		if( m_pMeshCom->Check_EndPeriod() )
		{
			printf("�ٿ�********\n");
			if( m_iMonsterAnimationIndex == BloodLord::down_from_back2downback )	// �ٿ��
			{
				printf("�ٿ���********\n");
				m_iMonsterAnimationIndex = BloodLord::down_idle_facedown;
				m_pMeshCom->Set_AnimationSet(BloodLord::down_idle_facedown);
			}
			else if( m_iMonsterAnimationIndex == BloodLord::down_idle_facedown )
			{
				printf("�Ͼ��********\n");
				m_bDownState = false;
				m_iMonsterAnimationIndex = BloodLord::standup_from_facedown;
				m_pMeshCom->Set_AnimationSet(BloodLord::standup_from_facedown);
			}
		}
	}
	if( m_fDownTime + 60.f <= m_fDownOverTime )	// �����ð� �ǰ��� �ȵǸ� �ٿ������ ����
	{
		if( m_fDownTime + 120.f <= m_fDownOverTime )	// printf("*** 0���� �ʱ�ȭ ***\n");
		{
			m_bDownTime = false;
			m_bDownState = false;		
			m_bDisDown = false;
			m_fDownTime = 0.f;
			m_fDownOverTime = 0.f;


			m_fDownValue = 0.f;
		}
		else if( !m_bDisDown )	// 	printf("*** 50% ���� ***\n");

		{
			m_bDisDown = true;
			m_fDownValue = m_fDownValue * 0.5f;
		}
		return;
	}
}

void CBloodLord::Check_PlayerAttack_AccordingToHP(void)
{
	//�浹�� �Ǿ������ �Ǹ� ���⼭ �����Ѵ�.
	//���� �ǰ� ���� ����Ʈ�� ���⼭ ����� �Ѵ�.
	if( !m_bRealDie && m_bDamage && !m_bDownState )
	{
		if( m_bDisHP )
			return;

		CPlayerBeShotUI::GetInstance()->PlayerBeShotUI();
		m_bDisHP = true;
		
		if( !m_bDownState )
		{
			m_fDownValue += CPlayer::GetInstance()->GetPlayerDownValue();
			m_fDownTime = m_fTimeDelta;
		}	
		
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

		//���� ���ݿ� ���� �ٸ� ���带 �����Ű�� �Ѵ�.
		CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::Hit_Flesh_Slash + rand()%7));

		CPlayer::GetInstance()->Player_AttackSuccess_InfoChange();
		CEffectMgr::GetInstance()->Play_Effect(L"Player_Slash_Effect", m_pTransCom->m_vPosition + _vec3(0.f, m_vCollisionPos.y, 0.f), false, true, _vec3(3.f, 15.f, 0.f), _vec3(m_fAngleX, 0.f, m_fAngleZ), 0.f, false, 0.f, NULL);

		//printf("Z�� ���� : %f\n", m_fAngleZ);
		if( m_eCurState == BS_SUPER )
			return;

		m_bDamageSuperArmor = true;
		m_bDownTime = true;

		m_vPlayerPos = CPlayer::GetInstance()->GetPos();
		DamageDirectionType eType = CMathMgr::GetInstance()->DamageDirectionDecision(m_vPlayerPos, m_pTransCom);

		printf("m_tMonsterInfo.fHP: %f\n", m_tMonsterInfo.fHP );

		if( m_fDownValue >= m_fMaxDamageValue )	// �ǰ����� ���� �ٿ�������� �ٿ� ���غ��� Ŀ���� �ٿ� ���
		{
			printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n�ٿ�\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
			m_bDownState = true;
			m_fDownValue = 0.f;
			m_pMeshCom->Set_AnimationSet(BloodLord::down_from_back2downback);
			m_iMonsterAnimationIndex = BloodLord::down_from_back2downback;
		}
		else if( m_bDownState == false )
		{
			switch(eType)
			{
			case DDT_FRONT:
				m_pMeshCom->Set_AnimationSet(BloodLord::damage_weak_bottom);
				break;
			case DDT_BACK:
				m_pMeshCom->Set_AnimationSet(BloodLord::damage_weak_back);
				break;
			case DDT_FRONT_LEFT:
				m_pMeshCom->Set_AnimationSet(BloodLord::damage_weak_left);
				break;
			case DDT_BACK_LEFT:
				m_pMeshCom->Set_AnimationSet(BloodLord::damage_weak_up);
				break;
			case DDT_FRONT_RIGHT:
				m_pMeshCom->Set_AnimationSet(BloodLord::damage_weak_right);
				break;
			case DDT_BACK_RIGHT:
				m_pMeshCom->Set_AnimationSet(BloodLord::damage_weak_up);
				break;
			}
		}
	}
	else
		m_bDisHP = false;
}

void CBloodLord::BattleAnimationPattern(void)
{
	Check_DownState();
	Check_LowHealth();		// �ǰ� ������������ üũ
	Check_Skill_Blink();	// ��ũ ���� üũ �Լ�
	Check_Skill_Swamp();	// �ǿ����� ���� üũ �Լ�
	Check_Skill_BloodField();	// ���׿� 
	Check_Skill_BloodBreath();

	if( m_bDie )
	{
		if( m_iMonsterAnimationIndex == BloodLord::dying_from_front )
		{
			if( m_pMeshCom->Check_EndPeriodMulTime(0.82f) && !m_pMeshCom->Check_EndPeriodMulTime(0.83f))
			{
				CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready",m_pTransCom->m_vPosition, false, false, _vec3(1.2f, 1.2f, 1.2f), _vec3( 0.f, 30.f, 0.f));	
			}
			else if( m_pMeshCom->Check_EndPeriodMulTime(0.85f) && !m_pMeshCom->Check_EndPeriodMulTime(0.86f))
			{
				CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready",m_pTransCom->m_vPosition, false, false);
			}
			else if( m_pMeshCom->Check_EndPeriodMulTime(0.9f) && !m_pMeshCom->Check_EndPeriodMulTime(0.91f))
			{
				CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready",m_pTransCom->m_vPosition, false, true, _vec3(0.7f, 0.7f, 0.7f), _vec3( 0.f, 60.f, 0.f));		
			}
			if( m_pMeshCom->Check_EndPeriodMulTime(0.95f) && !m_pMeshCom->Check_EndPeriodMulTime(0.96f) )
			{
				CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 3.f, 0.f), false, false, _vec3(9.f, 9.f,9.f));
				CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 3.f, 0.f), false, false, _vec3(9.f, 9.f,9.f), _vec3( 0.f, 90.f, 0.f));
				CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(3.f, 5.f, 0.f), false, false, _vec3(4.f, 4.f,4.f));
				CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(3.f, 5.f, 0.f), false, false, _vec3(4.f, 4.f,4.f), _vec3( 0.f, 90.f, 0.f));
				CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(-4.f, 2.f, 0.f), false, false, _vec3(3.f, 3.f,3.f));
				CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(-4.f, 2.f, 0.f), false, false, _vec3(3.f, 3.f,3.f), _vec3( 0.f, 90.f, 0.f));
				CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 5.f, 0.f), false, false, _vec3(15.f, 15.f,15.f));
				CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 5.f, 0.f), false, false, _vec3(15.f, 15.f,15.f), _vec3( 0.f, 90.f, 0.f));

				BatEffect();
			}
			else if( m_pMeshCom->Check_EndPeriod() )
				m_bRealDie = true;
		}
	}
	else
	{

		if( m_bRot_BreathEnd )
			m_fRotSpeed = 5.f;

		if( !m_bDownState )
			Check_Player_Radian();	// �÷��̾� �ٶ󺸵���

		if( m_bIdle)	// ����������
		{
			m_eCurState = BS_IDLE;
			m_iMonsterAnimationIndex = BloodLord::idle;
			m_pMeshCom->Set_AnimationSetSameFinish(BloodLord::idle);
			return;
		}

		//// ������ �������� ��ũ ����/��ũ ��/�ƽ�1/�޸��� ���°� �ƴϸ�
		if( m_pMeshCom->Check_EndPeriod() && !m_bBlink && !m_bDownState && !m_bDie
			&& m_iMonsterAnimationIndex != BloodLord::vamplord_cutscene_1
			&& m_iMonsterAnimationIndex != BloodLord::vamplord_cutscene_2
			&& m_iMonsterAnimationIndex != BloodLord::run_N
			&& m_iMonsterAnimationIndex != BloodLord::vamplord_wrap_air2_begin ) //������ ������ ������ AI
		{
			m_bSwamp = false;	// swamp ��� ���� ������ ����� ������ ������ false ����
			m_bDownState = false;
			m_bMoveCollCheck = true;

			// �ƽſ��� �ִϸ��̼ǵ� ���� �� AI ( �������� & ���Ÿ����� �Ѵ� �ƴϸ� ��� ��� )
			if( !Pattern_Near_Distance() && !Pattern_Mid_Distance() && !Pattern_Far_Distance() )	
			{
				if( !Pattern_Out_Distance() && m_bRotation == false)
				{
					m_eCurState = BS_IDLE;
					m_iMonsterAnimationIndex = BloodLord::idle;
					m_pMeshCom->Set_AnimationSetSameFinish(BloodLord::idle);
				}
			}
		}
	}
}

void CBloodLord::AnimationTimeChange(void)
{
	if( m_bCutScene1 )
	{
		if(!m_pMeshCom->Check_EndPeriodMulTime(0.49f))
			m_fPlaySpeed = 1.27f;
		else
			m_fPlaySpeed = 0.93f;
	}
	else if( m_bCutScene2 )
		m_fPlaySpeed = 1.1f;
	else if( m_bDie )
		m_fPlaySpeed = 0.9f;
	else
		m_fPlaySpeed = 1.f;

	m_fRenderTimeDelta	= m_fTimeDelta * m_fAnimationSpeed * m_fPlaySpeed ;
}

_ulong CBloodLord::Release(void)
{
	_ulong dwRefCnt = CMonster::Release();

	if(0 == dwRefCnt)
	{
		CBloodLordHP::GetInstance()->DestroyInstance();
		Engine::Safe_Release(m_pBossFaceUI);
		delete this;
	}

	return 0;
}

void CBloodLord::KeyInput()
{
	if( GetAsyncKeyState(VK_LEFT) & 0x0001 )
	{
		_vec3 vDir;
		memcpy( &vDir, &(m_pTransCom->m_matWorld.m[2][0]), sizeof(_vec3) );
		D3DXVec3Normalize(&vDir, &vDir);
		D3DXVec3TransformNormal(&vDir, &vDir, &m_matRotationDefault);
		m_pTransCom->m_vPosition += vDir * 5.f;
	}
	if( GetAsyncKeyState(VK_UP) & 0x0001 )
		m_pTransCom->m_vPosition.z += 5.f;
	if( GetAsyncKeyState(VK_RIGHT) & 0x0001 )
		m_pTransCom->m_vPosition.x += 5.f;
	if( GetAsyncKeyState(VK_DOWN) & 0x0001 )
		m_pTransCom->m_vPosition.z -= 5.f;

 	if( GetAsyncKeyState('V') & 0x0001 )	// ����������
 	{
 		m_tMonsterInfo.fHP = 0.f;
 	}
	if( GetAsyncKeyState('G') & 0x0001 )	// ���� �׽�Ʈ
	{
		m_eCurState = BS_SUPER;
		m_iMonsterAnimationIndex = BloodLord::vamplord_swamp_ready;
		m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
	}
}

_bool CBloodLord::Check_Player_Distance(float fDist)
{
	_vec3 vPlayerPos = CPlayer::GetInstance()->GetPos();

	float fDistance = D3DXVec3Length( &(vPlayerPos - m_pTransCom->m_vPosition) );

	if( fDistance < fDist )	// fDist�ȿ� �÷��̾ ���� ���
	{
		return true;
	}
	return false;
}

void CBloodLord::Check_Player_Radian()
{
	if( m_bSwamp || m_bMeteo || m_bDie
		|| m_iMonsterAnimationIndex == BloodLord::vamplord_laugh
		|| m_iMonsterAnimationIndex == BloodLord::vamplord_cutscene_1
		|| m_iMonsterAnimationIndex == BloodLord::vamplord_cutscene_2
		|| m_iMonsterAnimationIndex == BloodLord::spawn )	// swamp ���¿����� ���� �ٶ󺸱⶧���� return
		return;

	m_vPlayerPos = CPlayer::GetInstance()->GetPos();
	_vec3 vDir = m_vPlayerPos - m_pTransCom->m_vPosition;
	D3DXVec3Normalize(&vDir, &vDir);

	memcpy( &m_vMonsterDir, &(m_pTransCom->m_matWorld.m[2][0]), sizeof(_vec3) );	
	D3DXVec3Normalize(&m_vMonsterDir, &m_vMonsterDir);									
	D3DXVec3TransformNormal(&m_vMonsterDir, &m_vMonsterDir, &m_matRotationDefault);	

	_float fAngle = acosf( D3DXVec3Dot(&m_vMonsterDir, &vDir) );
	_vec3 vRight;

	memcpy( &vRight, &(m_pTransCom->m_matWorld.m[0][0]), sizeof(_vec3) );	
	D3DXVec3Normalize(&vRight, &vRight);									
	D3DXVec3TransformNormal(&vRight, &vRight, &m_matRotationDefault);	
	_float fAngle_Right = acosf( D3DXVec3Dot(&vRight, &vDir) );

	if( D3DXToDegree(fAngle_Right) <= 90.f )
		fAngle = D3DX_PI * 2.f - fAngle;
	//printf("%f\n", D3DXToDegree(fAngle));


	if(D3DXToDegree(fAngle) - 10.f < m_fOrigAngle &&  m_fOrigAngle < D3DXToDegree(fAngle) + 10.f)
	{
		m_fOrigAngle = D3DXToDegree(fAngle);
		if( m_bRot_BreathEnd )
			m_bRot_BreathEnd = false;
	}
	else
	{
		if( D3DXToDegree(fAngle) >= 180 - 10.f)
		{
			m_fOrigAngle +=  (360 - D3DXToDegree(fAngle)) * 15.f * m_fTimeDelta;
		}
		else if( D3DXToDegree(fAngle) >= 0.f && D3DXToDegree(fAngle) < 180.f - 10.f )
		{
			m_fOrigAngle -=  (D3DXToDegree(fAngle)) * 15.f * m_fTimeDelta;
		}
	}

	if( m_fOrigAngle > (360.f) - 10.f)
		m_fOrigAngle = (0.f);
	else if( m_fOrigAngle < (0.f) + 10.f)
		m_fOrigAngle = (360.f);

	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian( m_fOrigAngle ) + D3DXToRadian( 180.f );

}

void CBloodLord::Check_Turn()
{

}

void CBloodLord::Check_Rotation()
{

}

_bool CBloodLord::Pattern_Near_Distance()	// �ٰŸ� ����
{
	if( m_bRotation )
		return false;

	if( Check_Player_Distance(50.f) && !m_bRun )
	{
		++m_fLowHealth;
		m_bRotation = false;
		m_bAttackDuring = true;
		m_bNearPattern = true;
		m_bMidPattern = false;
		m_bFarPattern = false;

		if( m_iNearPattern > int(m_vecInPattern.size()) - 1 )
			m_iNearPattern = 0;

		m_vPlayerPos = CPlayer::GetInstance()->GetPos();
		m_iMonsterAnimationIndex = m_vecInPattern[m_iNearPattern];
		m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
		m_eCurState = BS_ATTACK;
		++m_iNearPattern;

		Check_NearAttackPower();

		return true;
	}
	return false;
}

_bool CBloodLord::Pattern_Mid_Distance()	// �߰��Ÿ� ����
{
	if( m_bRotation )
		return false;

	if( !(Check_Player_Distance(m_fNearDistance)) && Check_Player_Distance(m_fMidDistance) && !m_bRun )
	{
		++m_fLowHealth;
		m_bRotation = false;
		m_bAttackDuring = true;
		m_bNearPattern = false;
		m_bMidPattern = true;
		m_bFarPattern = false;

		if( m_iMidPattern > int(m_vecMidPattern.size()) - 1 )
			m_iMidPattern = 0;

		m_vPlayerPos = CPlayer::GetInstance()->GetPos();
		m_iMonsterAnimationIndex = m_vecMidPattern[m_iMidPattern];
		m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
		m_eCurState = BS_ATTACK;
		++m_iMidPattern;

		Check_MidAttackPower();

		return true;
	}
	return false;
}

_bool CBloodLord::Pattern_Far_Distance()	// ���Ÿ� ����
{
	if( m_bRotation )
		return false;

	if( !(Check_Player_Distance(m_fMidDistance)) && Check_Player_Distance(m_fFarDistance) )
	{
		++m_fLowHealth;
		m_bRotation = false;
		m_bAttackDuring = true;
		m_bNearPattern = false;
		m_bMidPattern = false;
		m_bFarPattern = true;

		if( m_iFarPattern > int(m_vecOutPattern.size()) - 1 )
			m_iFarPattern = 0;

		m_vPlayerPos = CPlayer::GetInstance()->GetPos();
		m_iMonsterAnimationIndex = m_vecOutPattern[m_iFarPattern];
		m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
		m_eCurState = BS_ATTACK;
		++m_iFarPattern;

		Check_FarAttackPower();

		if( m_iMonsterAnimationIndex == BloodLord::vamplord_attack_bloodfield_begin1 )
			m_bMeteoSkill = true;

		return true;
	}
	return false;
}


_bool CBloodLord::Pattern_Out_Distance()
{
	if( m_bRotation )
		return false;

	//// ���� �ָ� ���� ��� �÷��̾� ��ó�� �̵��� �� �ְ� ��ũ ������ �Ѵ�.
	if( !(Check_Player_Distance(m_fFarDistance)) )
	{
		m_bNearPattern = false;
		m_bAttackDuring = true;
		m_bMidPattern = false;
		m_bFarPattern = false;

		m_iMonsterAnimationIndex = BloodLord::vamplord_wrap_air_hard;
		m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
		m_eCurState = BS_SUPER;

		return true;
	}
	return false;
}



void CBloodLord::Set_BlinkPosition()
{
	srand((unsigned)time(NULL));
	int iRnd = rand() % 8;
	int XPos  = 0, ZPos = 0;

	if( !m_bInsideNavi )
	{
		if( iRnd < 4 )
			iRnd +=4;
		else
			iRnd -=4;
	}

	if( iRnd == m_iBlinkNot )
	{
		if( iRnd < 6 )
			iRnd +=1;
		else
			iRnd -=1;
	}

	switch(iRnd)
	{
	case 0:
		XPos = 0; ZPos = 15; break;
	case 1:
		XPos = 10; ZPos = 10; break;
	case 2:
		XPos = 15; ZPos = 0; break;
	case 3:
		XPos = 15; ZPos = -15; break;
	case 4:
		XPos = 0; ZPos = -15; break;
	case 5:
		XPos = -10; ZPos = -10; break;
	case 6:
		XPos = -15; ZPos = 0; break;
	case 7:
		XPos = -10; ZPos = 10; break;
	}

	_vec3 vBlinkPos = CPlayer::GetInstance()->GetPos();
	vBlinkPos.x += XPos;
	vBlinkPos.z += ZPos;
	_vec3 vOrigPos = m_pTransCom->m_vPosition;
	_bool bInside;

	list<Engine::CNaviCell*>::iterator iter = Engine::Get_NaviMesh()->Get_listNaviCell()->begin();
	list<Engine::CNaviCell*>::iterator iter_end = Engine::Get_NaviMesh()->Get_listNaviCell()->end();
	for(; iter != iter_end; ++iter)
	{
		bInside = (*iter)->Inside_Point(&vBlinkPos);
		if( bInside)
			break;
	}

	if( bInside )
	{
		m_bBlinkEnd = true;
		m_pTransCom->m_vPosition.x = vBlinkPos.x;
		m_pTransCom->m_vPosition.z = vBlinkPos.z;
		m_bInsideNavi = true;
	}
	else
	{
		m_iBlinkNot = iRnd;
		m_bBlinkEnd = false;
		m_bInsideNavi = false;
	}
}

void CBloodLord::Check_Skill_Blink()
{
	// ��ũ ���� ������ ������ ��( ��ũ ���ְ� �ð� ���� )
	if( m_iMonsterAnimationIndex == BloodLord::vamplord_wrap_air2_begin )
	{
		m_bBlinkEnd = false;
		m_eCurState = BS_SUPER;

		if( !m_bBlink )	
		{
			if( m_pMeshCom->Check_EndPeriodMulTime(m_fBlinkEffTime) )
			{
				BatEffect();
			}
			if( m_pMeshCom->Check_EndPeriodMulTime(m_fBlinkEndTime) )
			{
				m_bBlink = true;
				m_fBlinkTime = m_fTimeDelta;
				m_bMoveCollCheck = false;
			}
		}
		// ��ũ �����̰� ���� �ð��� ���� ��( ���� ���� )
		else if( m_bBlink && m_fBlinkTime + 1.75f <= m_fBlinkOverTime )	
		{			
			Set_BlinkPosition();

			if( m_bBlinkEnd )
			{
				m_bBlink = false;
				m_bBlinkEnd = true;
				m_fBlinkTime = 0.f;			// ��ũ ���� �ð��� �ʱ�ȭ.
				m_fBlinkOverTime = 0.f;

				// ��Ʈ ���� ���� ����( ���� )
				int iRnd = rand() % 2;

				//if( iRnd == 0 )
				//	m_iMonsterAnimationIndex = BloodLord::vamplord_wrap_air2_end;					// ����5Ÿ
				//else
				m_iMonsterAnimationIndex = BloodLord::vamplord_wrap_air_hard_end;				// ����

				m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);

				_matrix matView;
				m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
				_vec3 vCam;
				memcpy( &vCam, &matView.m[3][0], sizeof(_vec3));

				_vec3 vDir = vCam - m_pTransCom->m_vPosition;
				D3DXVec3Normalize(&vDir, &vDir);

				memcpy( &m_vMonsterDir, &(m_pTransCom->m_matWorld.m[2][0]), sizeof(_vec3) );	
				D3DXVec3Normalize(&m_vMonsterDir, &m_vMonsterDir);									
				D3DXVec3TransformNormal(&m_vMonsterDir, &m_vMonsterDir, &m_matRotationDefault);	

				_float fAngle = acosf( D3DXVec3Dot(&m_vMonsterDir, &vDir) );
				_vec3 vRight;

				memcpy( &vRight, &(m_pTransCom->m_matWorld.m[0][0]), sizeof(_vec3) );	
				D3DXVec3Normalize(&vRight, &vRight);									
				D3DXVec3TransformNormal(&vRight, &vRight, &m_matRotationDefault);	
				_float fAngle_Right = acosf( D3DXVec3Dot(&vRight, &g_vLook) );

				if( D3DXToDegree(fAngle_Right) <= 90.f )
					fAngle = D3DX_PI * 2.f - fAngle;



			}
		}
	}
	else if( m_iMonsterAnimationIndex == BloodLord::vamplord_wrap_air_hard )
	{
		m_eCurState = BS_SUPER;

		if( !m_bBlink && m_pMeshCom->Check_EndPeriodMulTime(m_fBlinkEndTime) )	
		{
			m_bBlink = true;
			m_fBlinkTime = m_fTimeDelta;
			m_pTransCom->m_vPosition = _vec3(0.f, 0.f, 0.f);
		}
		// ��ũ �����̰� ���� �ð��� ���� ��( ���� ���� )
		else if( m_bBlink && m_fBlinkTime + 1.75f <= m_fBlinkOverTime )	
		{
			// 			printf("OutDistance Blink\n");
			m_bBlink = false;
			srand((unsigned)time(NULL));
			int XPos = rand() % 25;
			int ZPos = rand() % 25;

			_vec3 vBlinkPos = CPlayer::GetInstance()->GetPos();
			vBlinkPos.x += XPos;
			vBlinkPos.z += ZPos;

			m_pTransCom->m_vPosition = vBlinkPos;	//  ��ġ ����
			m_fBlinkTime = 0.f;						// ��ũ ���� �ð��� �ʱ�ȭ.
			m_fBlinkOverTime = 0.f;

			// ��Ʈ ���� ���� ����( ��� ������ )
			m_iMonsterAnimationIndex = BloodLord::idle;
			m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
		}
	}
}

void CBloodLord::SwampPattern_0()
{
	if( m_pMeshCom->Check_EndPeriodMulTime(0.25f) && !m_pMeshCom->Check_EndPeriodMulTime(0.26f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready",m_pTransCom->m_vPosition, false, false, _vec3(1.2f, 1.2f, 1.2f), _vec3( 0.f, 30.f, 0.f));	
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.3f) && !m_pMeshCom->Check_EndPeriodMulTime(0.31f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready",m_pTransCom->m_vPosition, false, true);	
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.35f) && !m_pMeshCom->Check_EndPeriodMulTime(0.36f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready",m_pTransCom->m_vPosition, false, true, _vec3(0.7f, 0.7f, 0.7f), _vec3( 0.f, 60.f, 0.f));		
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.4f) && !m_pMeshCom->Check_EndPeriodMulTime(0.41f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready",m_pTransCom->m_vPosition, false, false, _vec3(1.2f, 1.2f, 1.2f), _vec3( 0.f, 30.f, 0.f));	
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.45f) && !m_pMeshCom->Check_EndPeriodMulTime(0.46f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready",m_pTransCom->m_vPosition, false, true);	
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.5f) && !m_pMeshCom->Check_EndPeriodMulTime(0.51f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready",m_pTransCom->m_vPosition, false, true, _vec3(0.7f, 0.7f, 0.7f), _vec3( 0.f, 60.f, 0.f));	
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.6f) && !m_pMeshCom->Check_EndPeriodMulTime(0.61f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready",m_pTransCom->m_vPosition, false, false, _vec3(1.2f, 1.2f, 1.2f), _vec3( 0.f, 30.f, 0.f));	
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.65f) && !m_pMeshCom->Check_EndPeriodMulTime(0.66f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready",m_pTransCom->m_vPosition, false, true);	
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.7f) && !m_pMeshCom->Check_EndPeriodMulTime(0.71f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready",m_pTransCom->m_vPosition, false, true, _vec3(0.7f, 0.7f, 0.7f), _vec3( 0.f, 60.f, 0.f));	
	}


	else if( m_pMeshCom->Check_EndPeriodMulTime(0.75f) && !m_pMeshCom->Check_EndPeriodMulTime(0.76f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f), _vec3( 0.f, 60.f, 0.f));	
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.8f) && !m_pMeshCom->Check_EndPeriodMulTime(0.81f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f), _vec3( 0.f, 60.f, 0.f));	
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.85f) && !m_pMeshCom->Check_EndPeriodMulTime(0.86f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 7.f, 7.f, 7.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 7.f, 7.f, 7.f), _vec3( 0.f, 90.f, 0.f));	
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.9f) && !m_pMeshCom->Check_EndPeriodMulTime(0.91f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 7.f, 7.f, 7.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 7.f, 7.f, 7.f), _vec3( 0.f, 90.f, 0.f));	
	}
}

void CBloodLord::SwampPattern_1()
{
	if(m_pMeshCom->Check_EndPeriodMulTime(0.0f) && !m_pMeshCom->Check_EndPeriodMulTime(0.01f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f), _vec3( 0.f, 90.f, 0.f));		

		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 22.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 35.f));	CMeteoEffect::SetSize(5.f);
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 5.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 25.f));	CMeteoEffect::SetSize(5.f);
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 30.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 30.f));	CMeteoEffect::SetSize(5.f);
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.15f) && !m_pMeshCom->Check_EndPeriodMulTime(0.16f))
	{
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 15.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 50.f));	CMeteoEffect::SetSize(5.f);
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 9.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 65.f));	CMeteoEffect::SetSize(5.f);
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 35.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 45.f));	CMeteoEffect::SetSize(5.f);
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 10.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 30.f));	CMeteoEffect::SetSize(5.f);
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.2f) && !m_pMeshCom->Check_EndPeriodMulTime(0.21f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f), _vec3( 0.f, 90.f, 0.f));		
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 15.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 60.f));	CMeteoEffect::SetSize(5.f);
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 35.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 40.f));	CMeteoEffect::SetSize(5.f);
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 30.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 55.f));	CMeteoEffect::SetSize(5.f);
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.25f) && !m_pMeshCom->Check_EndPeriodMulTime(0.26f))
	{
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 35.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 30.f));	CMeteoEffect::SetSize(5.f);
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 15.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 15.f));	CMeteoEffect::SetSize(5.f);
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 42.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 20.f));	CMeteoEffect::SetSize(5.f);
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.3f) && !m_pMeshCom->Check_EndPeriodMulTime(0.31f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f), _vec3( 0.f, 90.f, 0.f));		
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 35.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 65.f));	CMeteoEffect::SetSize(5.f);
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 42.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 40.f));	CMeteoEffect::SetSize(5.f);
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.4f) && !m_pMeshCom->Check_EndPeriodMulTime(0.41f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f), _vec3( 0.f, 90.f, 0.f));		
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 19.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 15.f));	CMeteoEffect::SetSize(5.f);
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 20.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 40.f));	CMeteoEffect::SetSize(5.f);
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.5f) && !m_pMeshCom->Check_EndPeriodMulTime(0.51f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f), _vec3( 0.f, 90.f, 0.f));		
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 38.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 50.f));	CMeteoEffect::SetSize(5.f);
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 39.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 20.f));	CMeteoEffect::SetSize(5.f);
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.6f) && !m_pMeshCom->Check_EndPeriodMulTime(0.61f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f), _vec3( 0.f, 90.f, 0.f));		
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 49.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 45.f));	CMeteoEffect::SetSize(5.f);
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 25.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 60.f));	CMeteoEffect::SetSize(5.f);
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.7f) && !m_pMeshCom->Check_EndPeriodMulTime(0.71f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f), _vec3( 0.f, 90.f, 0.f));		
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.8f) && !m_pMeshCom->Check_EndPeriodMulTime(0.81f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f), _vec3( 0.f, 90.f, 0.f));		
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 19.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 14.f));	CMeteoEffect::SetSize(5.f);
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 5.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 35.f));	CMeteoEffect::SetSize(5.f);
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.9f) && !m_pMeshCom->Check_EndPeriodMulTime(0.91f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -2.f), false, false, _vec3( 10.f, 10.f, 10.f), _vec3( 0.f, 90.f, 0.f));		
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 15.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 42.f));	CMeteoEffect::SetSize(5.f);
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 9.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 50.f));	CMeteoEffect::SetSize(5.f);
	}
}

void CBloodLord::SwampPattern_2()
{

}
void CBloodLord::Check_Skill_Swamp()
{
	if( m_iMonsterAnimationIndex == BloodLord::vamplord_swamp_ready )
	{
		m_eCurState = BS_SUPER;

		if( !m_bBlink && m_pMeshCom->Check_EndPeriodMulTime(0.36f) )	
		{
			m_bBlink = true;		// blink ���� ����
			m_fBlinkTime = m_fTimeDelta;	// blink�� ����

			BatEffect();
		}
		// ��ũ �����̰� ���� �ð��� ���� ��( �ǿ����� ���� ���� )
		else if( m_bBlink && m_fBlinkTime + 1.f <= m_fBlinkOverTime )	
		{
			m_bBlink = false;		// blink ���� ��
			m_bSwamp= true;			// swamp �������� Ȯ��
			m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian(-90.f);	// swamp ���¿����� ���� �ٶ󺸱⶧���� �������� ����
			m_pTransCom->m_vPosition = _vec3( 184.f, 0.f, 310.f );						// ��ġ ����
			m_fBlinkTime = 0.f;		// ��ũ ���� �ð��� �ʱ�ȭ.
			m_fBlinkOverTime = 0.f;

			// ��Ʈ ���� ���� ����( ���� )
			m_iMonsterAnimationIndex = BloodLord::vamplord_swamp_begin;					// ���� ���� ����( ����/ ����5Ÿ )
			m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
			BatEffect();
			++m_iFarPattern;
		}
	}
	else if( m_iMonsterAnimationIndex == BloodLord::vamplord_swamp_begin )
	{
		SwampPattern_0();
	}
	else if( m_iMonsterAnimationIndex == BloodLord::vamplord_swamp_during )
	{
		SwampPattern_1();
	}
}

void CBloodLord::Check_Skill_Dancing()
{
	//// �ҿ뵹�� ���� ���϶��� �÷��̾� �ֺ����� �����Ÿ� �������� �÷��̾ �߽����� ���鼭 �����Ѵ�.
	if( m_iMonsterAnimationIndex == BloodLord::vamplord_attack_dancing_begin )
	{
		if( m_pMeshCom->Check_EndPeriodMulTime(0.3f) && !m_pMeshCom->Check_EndPeriodMulTime(0.31f) )
		{	
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Dancing",m_pTransCom->m_vPosition, false, true, _vec3(1.4f, 1.4f,1.4f));
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 3.f, 0.f), false, true, _vec3(9.f, 9.f,9.f));
		}
		else if( m_pMeshCom->Check_EndPeriodMulTime(0.4f) && !m_pMeshCom->Check_EndPeriodMulTime(0.41f) )
		{	
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Dancing",m_pTransCom->m_vPosition, false, true, _vec3(0.7f, 0.7f,0.7f));
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 4.f, 0.f), false, true, _vec3(10.f, 10.f,10.f));
		}
		else if( m_pMeshCom->Check_EndPeriodMulTime(0.5f) && !m_pMeshCom->Check_EndPeriodMulTime(0.51f) )
		{	
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Dancing",m_pTransCom->m_vPosition, false, true, _vec3(1.4f, 1.4f,1.4f));
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 5.f, 0.f), false, true, _vec3(12.f, 12.f,12.f));
		}
		else if( m_pMeshCom->Check_EndPeriodMulTime(0.6f) && !m_pMeshCom->Check_EndPeriodMulTime(0.61f) )
		{	
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Dancing",m_pTransCom->m_vPosition, false, true);
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 4.f, 0.f), false, true, _vec3(9.f, 9.f,9.f));
		}
		else if( m_pMeshCom->Check_EndPeriodMulTime(0.7f) && !m_pMeshCom->Check_EndPeriodMulTime(0.71f) )
		{	
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Dancing",m_pTransCom->m_vPosition, false, true, _vec3(0.7f, 0.7f,0.7f));
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 3.f, 0.f), false, true, _vec3(9.f, 9.f,9.f));
		}
	}
	else if( m_iMonsterAnimationIndex == BloodLord::vamplord_attack_dancing_during )
	{
		D3DXMATRIX			matRot, matTrans, matRevol, matParent;

		D3DXMatrixTranslation(&matTrans, m_fDancingDistance_x, 0.f, m_fDancingDistance_z);
		D3DXMatrixRotationY(&matRevol, D3DXToRadian(m_fRevoluAngle));

		int i = 1;
		int j = 1;

		if( m_fDancingDistance_x > 0.f )
			i = -1;
		if( m_fDancingDistance_z > 0.f )
			j = -1;

		if( m_pMeshCom->Check_EndPeriodMulTime(0.f) && !m_pMeshCom->Check_EndPeriodMulTime(0.01f) )
		{	
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Dancing",m_pTransCom->m_vPosition, false, true, _vec3(0.7f, 0.7f,0.7f));
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 5.f, 0.f), false, true, _vec3(10.f, 10.f, 10.f));
		}
		else if( m_pMeshCom->Check_EndPeriodMulTime(0.1f) && !m_pMeshCom->Check_EndPeriodMulTime(0.11f) )
		{	
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Dancing",m_pTransCom->m_vPosition, false, true, _vec3(1.4f, 1.4f,1.4f));
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 3.f, 0.f), false, true, _vec3(9.f, 9.f,9.f));
		}
		else if( m_pMeshCom->Check_EndPeriodMulTime(0.25f) && !m_pMeshCom->Check_EndPeriodMulTime(0.26f) )
		{	
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Dancing",m_pTransCom->m_vPosition, false, true, _vec3(0.7f, 0.7f,0.7f));
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 5.f, 0.f), false, true, _vec3(12.f, 12.f,12.f));
		}
		else if( m_pMeshCom->Check_EndPeriodMulTime(0.3f) && !m_pMeshCom->Check_EndPeriodMulTime(0.31f) )
		{	
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Dancing",m_pTransCom->m_vPosition, false, true, _vec3(1.4f, 1.4f,1.4f));
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 3.f, 0.f), false, true, _vec3(9.f, 9.f,9.f));
		}
		else if( m_pMeshCom->Check_EndPeriodMulTime(0.4f) && !m_pMeshCom->Check_EndPeriodMulTime(0.41f) )
		{	
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Dancing",m_pTransCom->m_vPosition, false, true);
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 4.f, 0.f), false, true, _vec3(10.f, 10.f,10.f));
		}
		else if( m_pMeshCom->Check_EndPeriodMulTime(0.5f) && !m_pMeshCom->Check_EndPeriodMulTime(0.51f) )
		{	
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Dancing",m_pTransCom->m_vPosition, false, true, _vec3(0.7f, 0.7f,0.7f));
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 5.f, 0.f), false, true, _vec3(12.f, 12.f,12.f));
		}
		else if( m_pMeshCom->Check_EndPeriodMulTime(0.6f) && !m_pMeshCom->Check_EndPeriodMulTime(0.61f) )
		{	
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Dancing",m_pTransCom->m_vPosition, false, true);
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 4.f, 0.f), false, true, _vec3(9.f, 9.f,9.f));
		}
		else if( m_pMeshCom->Check_EndPeriodMulTime(0.7f) && !m_pMeshCom->Check_EndPeriodMulTime(0.71f) )
		{	
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Dancing",m_pTransCom->m_vPosition, false, true, _vec3(1.4f, 1.4f,1.4f));
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 3.f, 0.f), false, true, _vec3(9.f, 9.f,9.f));
		}


		m_fDancingDistance_x += i * 4.f * m_fTimeDelta;
		m_fDancingDistance_z += j * 4.f * m_fTimeDelta;

		m_fRevoluAngle -= 180.f * m_fTimeDelta;

		m_pTransCom->m_matWorld = matTrans * matRevol * m_matDancing;
		m_pTransCom->m_vPosition.x = m_pTransCom->m_matWorld._41;
		m_pTransCom->m_vPosition.z = m_pTransCom->m_matWorld._43;
	}

	else if( m_iMonsterAnimationIndex == BloodLord::vamplord_attack_dancing_during )
	{
		if( m_pMeshCom->Check_EndPeriodMulTime(0.2f) && !m_pMeshCom->Check_EndPeriodMulTime(0.21f) )
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 3.f, 0.f), false, true, _vec3(1.f, 1.f,1.f));
		else if( m_pMeshCom->Check_EndPeriodMulTime(0.25f) && !m_pMeshCom->Check_EndPeriodMulTime(0.26f) )
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(3.f, 3.f, 0.f), false, true, _vec3(6.f, 6.f,6.f));
		else if( m_pMeshCom->Check_EndPeriodMulTime(0.3f) && !m_pMeshCom->Check_EndPeriodMulTime(0.31f) )
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 3.f, 0.f), false, true, _vec3(9.f, 9.f,9.f));
		else if( m_pMeshCom->Check_EndPeriodMulTime(0.34f) && !m_pMeshCom->Check_EndPeriodMulTime(0.35f) )
			CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(-3.f, 3.f, 0.f), false, true, _vec3(4.f, 4.f,4.f));
	}

	if( m_pMeshCom->Check_EndPeriod() && m_iMonsterAnimationIndex == BloodLord::vamplord_attack_dancing_end )
		m_bMoveCollCheck = true;

}


void CBloodLord::BloodFieldPattern_0()
{
	if( m_pMeshCom->Check_EndPeriodMulTime(0.1f) && !m_pMeshCom->Check_EndPeriodMulTime(0.11f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready",m_pTransCom->m_vPosition, false, false, _vec3(1.2f, 1.2f, 1.2f), _vec3( 0.f, 30.f, 0.f));	
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.15f) && !m_pMeshCom->Check_EndPeriodMulTime(0.16f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready",m_pTransCom->m_vPosition, false, true);	
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.2f) && !m_pMeshCom->Check_EndPeriodMulTime(0.21f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready",m_pTransCom->m_vPosition, false, true, _vec3(0.7f, 0.7f, 0.7f), _vec3( 0.f, 60.f, 0.f));		
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.6f) && !m_pMeshCom->Check_EndPeriodMulTime(0.61f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 2.f, 0.f), false, false, _vec3( 1.f, 1.f, 1.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 2.f, -0.f), false, false, _vec3( 1.f, 1.f, 1.f), _vec3( 0.f, 30.f, 0.f));	

		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 3.f, 0.f), false, false, _vec3( 2.5f, 2.5f, 2.5f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 3.f, -0.f), false, false, _vec3( 2.5f, 2.5f, 2.5f), _vec3( 0.f, 90.f, 0.f));	
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.65f) && !m_pMeshCom->Check_EndPeriodMulTime(0.66f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 4.f, 0.f), false, false, _vec3( 4.f, 4.f, 4.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 4.f, -0.f), false, false, _vec3( 4.f, 4.f, 4.f), _vec3( 0.f, 60.f, 0.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 5.f, 0.f), false, false, _vec3( 6.5f, 6.5f, 6.5f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 5.f, -0.f), false, false, _vec3( 6.5f, 6.5f, 6.5f), _vec3( 0.f, 30.f, 0.f));	
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.7f) && !m_pMeshCom->Check_EndPeriodMulTime(0.71f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, 0.f), false, false, _vec3( 7.f, 7.f, 7.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -0.f), false, false, _vec3( 7.f, 7.f, 7.f), _vec3( 0.f, 90.f, 0.f));	
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.75f) && !m_pMeshCom->Check_EndPeriodMulTime(0.76f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, 0.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -0.f), false, false, _vec3( 10.f, 10.f, 10.f), _vec3( 0.f, 60.f, 0.f));	
	}
	else if( m_pMeshCom->Check_EndPeriodMulTime(0.8f) && !m_pMeshCom->Check_EndPeriodMulTime(0.81f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, 0.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, -0.f), false, false, _vec3( 10.f, 10.f, 10.f), _vec3( 0.f, 60.f, 0.f));	
	}
}

void CBloodLord::BloodFieldPattern_1()
{
	if(m_pMeshCom->Check_EndPeriodMulTime(0.0f) && !m_pMeshCom->Check_EndPeriodMulTime(0.01f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 10.f, 0.f), false, false, _vec3( 10.f, 10.f, 10.f));	

		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 22.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 15.f));
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 14.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z + 25.f));
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 20.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 14.f));
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.1f) && !m_pMeshCom->Check_EndPeriodMulTime(0.11f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 10.f, 0.f), false, false, _vec3( 10.f, 10.f, 10.f));	
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.15f) && !m_pMeshCom->Check_EndPeriodMulTime(0.16f))
	{
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 15.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 20.f));
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 9.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z + 19.f));
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.2f) && !m_pMeshCom->Check_EndPeriodMulTime(0.21f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 10.f, 0.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 15.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 20.f));
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.25f) && !m_pMeshCom->Check_EndPeriodMulTime(0.26f))
	{
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 25.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z));
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 5.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z + 5.f));
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 12.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 12.f));
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.3f) && !m_pMeshCom->Check_EndPeriodMulTime(0.31f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 10.f, 0.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 5.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z + 4.f));
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 12.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 10.f));
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.4f) && !m_pMeshCom->Check_EndPeriodMulTime(0.41f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 10.f, 0.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 19.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z + 15.f));
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 20.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 10.f));
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.5f) && !m_pMeshCom->Check_EndPeriodMulTime(0.51f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 10.f, 0.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 18.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 9.f));
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 9.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z + 19.f));
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.6f) && !m_pMeshCom->Check_EndPeriodMulTime(0.61f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 10.f, 0.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 19.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z + 5.f));
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 25.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z));
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.7f) && !m_pMeshCom->Check_EndPeriodMulTime(0.71f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 10.f, 0.f), false, false, _vec3( 10.f, 10.f, 10.f));	
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.8f) && !m_pMeshCom->Check_EndPeriodMulTime(0.81f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 10.f, 0.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 19.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z + 14.f));
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 5.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 15.f));
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.9f) && !m_pMeshCom->Check_EndPeriodMulTime(0.91f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 10.f, 0.f), false, false, _vec3( 10.f, 10.f, 10.f));	
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x - 15.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z - 12.f));
		CMeteoEffect::Create(m_pGraphicDev, _vec3(m_pTransCom->m_vPosition.x + 9.f, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z + 19.f));
	}
}

void CBloodLord::BloodFieldPattern_2()
{
	if(m_pMeshCom->Check_EndPeriodMulTime(0.0f) && !m_pMeshCom->Check_EndPeriodMulTime(0.01f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 10.f, 0.f), false, false, _vec3( 10.f, 10.f, 10.f));	
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.1f) && !m_pMeshCom->Check_EndPeriodMulTime(0.11f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 10.f, 0.f), false, false, _vec3( 10.f, 10.f, 10.f));	
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.2f) && !m_pMeshCom->Check_EndPeriodMulTime(0.21f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 10.f, 0.f), false, false, _vec3( 10.f, 10.f, 10.f));	
	}
	else if(m_pMeshCom->Check_EndPeriodMulTime(0.3f) && !m_pMeshCom->Check_EndPeriodMulTime(0.31f))
	{
		CEffectMgr::GetInstance()->Play_Effect(L"Blood_Jin_Ready2",m_pTransCom->m_vPosition + _vec3(0.f, 10.f, 0.f), false, false, _vec3( 10.f, 10.f, 10.f));	
	}
}

void CBloodLord::Check_Skill_BloodField()
{
	if( m_iMonsterAnimationIndex == BloodLord::vamplord_attack_bloodfield_begin1 )
	{
		BloodFieldPattern_0();
	}
	else if( m_iMonsterAnimationIndex == BloodLord::vamplord_attack_bloodfield_during1 )
	{
		BloodFieldPattern_1();
	}
	else if( m_iMonsterAnimationIndex == BloodLord::vamplord_attack_bloodfield_end )
	{
		BloodFieldPattern_2();
	}
}

void CBloodLord::Check_Skill_BloodBreath()
{
	if(m_iMonsterAnimationIndex == BloodLord::vamplord_attack_blood_breath)
	{
		//printf("hello\n");
		m_bMeteoSkill = false;
		m_vPlayerPos = CPlayer::GetInstance()->GetPos();
		_vec3 vDir = m_vPlayerPos - m_pTransCom->m_vPosition;
		D3DXVec3Normalize(&vDir, &vDir);

		_vec3 vEffect;
		_matrix matWorld;
		const _matrix* m_pParentMatrix = m_pMeshCom->Get_FrameMatrix("mouth");

		matWorld = (*m_pParentMatrix) * m_pTransCom->m_matWorld;

		memcpy(&vEffect, &matWorld.m[3][0], sizeof(_vec3));

		if( m_pMeshCom->Check_EndPeriodMulTime(0.18f) && !m_pMeshCom->Check_EndPeriodMulTime(0.2f) )
		{
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Breath_Start", vEffect, false, false, _vec3( 10.f, 10.f, 10.f));
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Breath_Start", vEffect, false, false, _vec3( 10.f, 10.f, 10.f), _vec3(0.f, 90.f, 0.f));
		}
		else if( m_pMeshCom->Check_EndPeriodMulTime(0.12f) && !m_pMeshCom->Check_EndPeriodMulTime(0.8f) )
		{			
			if( m_fBreathTime == 0.f )
			{
				m_bBreath = true;
				m_fBreathTime = m_fTimeDelta;
			}
			if( m_fBreathOverTime > m_bBreath + 0.01f )
			{
				m_fBreathTime = m_fTimeDelta;
				m_fBreathSize += 0.4f;
				m_fBreathOverTime = 0.f;
				CBreathEffect::Create(m_pGraphicDev, _vec3(vEffect.x , vEffect.y, vEffect.z));
				CBreathEffect::SetDir(vDir);

			}
		}
		else if( m_pMeshCom->Check_EndPeriodMulTime(0.8f) && !m_pMeshCom->Check_EndPeriodMulTime(0.82f) )
		{
			m_bBreath = false;
			m_fBreathTime = 0.f;
			m_fBreathOverTime = 0.f;
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Breath_Doing", vEffect, false, false, _vec3( 10.f, 10.f, 10.f));
			CEffectMgr::GetInstance()->Play_Effect(L"Blood_Breath_Doing", vEffect, false, false, _vec3( 10.f, 10.f, 10.f), _vec3(0.f, 90.f, 0.f));
		}
	}
}


void CBloodLord::Check_LowHealth()
{
	if( m_tMonsterInfo.fHP <= 2000.f && m_fLowHealth == 0.f)
	{
		if( m_tMonsterInfo.fHP <= 0.f )
		{
			m_bDie = true;
			Check_DieMotion();
		}
		else if( m_pMeshCom->Check_EndPeriod() && m_iMonsterAnimationIndex != BloodLord::vamplord_idle_low_health )
		{
			m_iMonsterAnimationIndex = BloodLord::vamplord_idle_low_health;
			m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
		}
	}
	if( m_fLowHealth >= 4.f)
		m_fLowHealth = 0.f;
}

void CBloodLord::Check_DieMotion()
{
	m_iMonsterAnimationIndex = BloodLord::dying_from_front;
	m_pMeshCom->Set_AnimationSet(BloodLord::dying_from_front);
}

void CBloodLord::Check_DamageAfter_SuperArmor()
{
	if( m_bDamageSuperArmor )		// �ǰ��� ���۾Ƹ� ���¶��
	{
		//printf("���۾Ƹ� true\n");
		m_eCurState = BS_SUPER;

		if( m_fSuperTime + 1.3f <= m_fSuperOverTime )	// ���� �ð� ������ ���۾Ƹ� ���� ����
		{
			m_bDamageSuperArmor = false;
			m_fSuperTime = 0.f;
			m_fSuperOverTime = 0.f;
			//printf("TRUE-> false\n");
		}
	}
}

void CBloodLord::Load_Pattern()
{
	//// �ٰŸ� ����
	m_vecInPattern.push_back(BloodLord::vamplord_attack_forehand_hard);	// ��ġ��
	m_vecInPattern.push_back(BloodLord::vamplord_attack02_hard);		// �Ѽ�ġ��
	m_vecInPattern.push_back(BloodLord::vamplord_attack_hard);			// ����2Ÿ
	m_vecInPattern.push_back(BloodLord::vamplord_wrap_air2_begin);		// ��ũ ����

	m_vecInPattern.push_back(BloodLord::vamplord_attack03_hard);		// ���ڸ� 5Ÿ
	m_vecInPattern.push_back(BloodLord::vamplord_attack_blood_breath);		// �극�� ����

	m_vecInPattern.push_back(BloodLord::vamplord_laugh);				// ���� ���� ���°�
	m_vecInPattern.push_back(BloodLord::vamplord_attack_dancing_begin);	// ����ȸ�� ����
	m_vecInPattern.push_back(BloodLord::vamplord_wrap_air2_begin);		// ��ũ ����
	m_vecInPattern.push_back(BloodLord::vamplord_attack_forehand_hard);	// ��ġ��
	m_vecInPattern.push_back(BloodLord::vamplord_attack03_fast_hard);	// �ָ�5Ÿ

	m_vecInPattern.push_back(BloodLord::vamplord_laugh);				// ���� ���� ���°�
	m_vecInPattern.push_back(BloodLord::vamplord_swamp_ready);			// �ǿ�����

	//// �߰��Ÿ� ����
	m_vecMidPattern.push_back(BloodLord::run_N);							// �޷�����
	m_vecMidPattern.push_back(BloodLord::vamplord_attack_dancing_begin);	// ����ȸ�� ����
	m_vecMidPattern.push_back(BloodLord::vamplord_attack_blood_breath);		// �극�� ����
	m_vecMidPattern.push_back(BloodLord::vamplord_laugh);					// ���� ���� ���°�
	m_vecMidPattern.push_back(BloodLord::vamplord_attack_bloodfield_begin1);	// ���׿�

	m_vecMidPattern.push_back(BloodLord::run_N);							// �޷�����
	m_vecMidPattern.push_back(BloodLord::vamplord_wrap_air2_begin);			// ��ũ ����
	m_vecMidPattern.push_back(BloodLord::vamplord_attack_hello);			// �帱

	m_vecMidPattern.push_back(BloodLord::vamplord_attack_dancing_begin);	// ����ȸ�� ����
	m_vecMidPattern.push_back(BloodLord::vamplord_attack_bloodfield_begin1);	// ���׿�

	//// ���Ÿ� ����
	m_vecOutPattern.push_back(BloodLord::vamplord_attack_bloodfield_begin1);	// ���׿�
	m_vecOutPattern.push_back(BloodLord::vamplord_attack_hello);				// �帱

	m_vecOutPattern.push_back(BloodLord::vamplord_swamp_ready);					// �ǿ�����
}

void CBloodLord::Check_NearAttackPower()
{
	switch( m_iMonsterAnimationIndex )
	{
	case BloodLord::vamplord_attack_forehand_hard:	// ��ġ��
		m_tMonsterInfo.fAttack = 350.f + (float)(rand()%150);
		break;
	case BloodLord::vamplord_attack02_hard:			// �Ѽ�ġ��
		m_tMonsterInfo.fAttack = 800.f + (float)(rand()%150);
		break;
	case BloodLord::vamplord_attack_hard:			// ����ġ��
		m_tMonsterInfo.fAttack = 750.f + (float)(rand()%150);
		break;
	case BloodLord::vamplord_attack03_hard:			// ���ڸ� 5Ÿ
		m_tMonsterInfo.fAttack = 350.f + (float)(rand()%150);
		break;
	case BloodLord::vamplord_attack03_fast_hard:	// �ָ� 5Ÿ
		m_tMonsterInfo.fAttack = 700.f + (float)(rand()%150);
		break;
	case BloodLord::vamplord_attack_dancing_begin:	// ����ȸ��
		m_tMonsterInfo.fAttack = 450.f + (float)(rand()%150);
		break;
	case BloodLord::vamplord_attack_dancing_during:	// ����ȸ��
		m_tMonsterInfo.fAttack = 520.f + (float)(rand()%150);
		break;
	}
}

void CBloodLord::Check_MidAttackPower()
{
	switch( m_iMonsterAnimationIndex )
	{
	case BloodLord::vamplord_attack_blood_breath:	// �극��
		m_tMonsterInfo.fAttack = 800.f + (float)(rand()%100);
		break;
	case BloodLord::vamplord_attack_dancing_begin:	// ����ȸ��
		m_tMonsterInfo.fAttack = 450.f + (float)(rand()%150);
		break;
	case BloodLord::vamplord_attack_dancing_during:	// ����ȸ��
		m_tMonsterInfo.fAttack = 450.f + (float)(rand()%150);
		break;
	case BloodLord::vamplord_attack_hello:				// �帱
		m_tMonsterInfo.fAttack = 650.f + (float)(rand()%150);
		break;
	}
}

void CBloodLord::Check_FarAttackPower()
{
	//// �ٵ� ��״� ���� ����Ʈ �����̶�.... �극���� �ǿ����̴�..
	switch( m_iMonsterAnimationIndex )
	{
	case BloodLord::vamplord_attack_bloodfield_begin1:	// ���׿�
		break;
	case BloodLord::vamplord_attack_hello:				// �帱
		break;
	case BloodLord::vamplord_swamp_ready:				// �ǿ�����
		break;
	case BloodLord::vamplord_attack_dancing_during:	// ����ȸ��
		m_tMonsterInfo.fAttack = 550.f + (float)(rand()%150);
		break;
	}
}

void CBloodLord::SoundPlayer(void)
{
	if(m_pMeshCom->Get_CurrentAniIdx() != m_iAnimationSoundCheck || m_pMeshCom->Check_EndPeriod())
	{
		m_iAnimationSoundCheck = m_pMeshCom->Get_CurrentAniIdx();
		ZeroMemory(m_bSoundPlay, sizeof(_bool) * 20);
	}

	if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_cutscene_1)
	{
		//cutscene1�� �Ұ� ���Ƽ� ������
		if(!m_bCutsceneSoundPlay[0] && m_pMeshCom->Check_EndPeriodMulTime(0.05f))
		{
			m_bCutsceneSoundPlay[0] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_breath01, m_pTransCom->m_vPosition, 0.2f);
		}
		if(!m_bCutsceneSoundPlay[1] && m_pMeshCom->Check_EndPeriodMulTime(0.15f))
		{
			m_bCutsceneSoundPlay[1] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_attack01, m_pTransCom->m_vPosition, 0.8f);
		}
		if(!m_bCutsceneSoundPlay[2] && m_pMeshCom->Check_EndPeriodMulTime(0.20f))
		{
			m_bCutsceneSoundPlay[2] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_breath02, m_pTransCom->m_vPosition, 0.6f);
		}
		if(!m_bCutsceneSoundPlay[3] && m_pMeshCom->Check_EndPeriodMulTime(0.3f))
		{
			m_bCutsceneSoundPlay[3] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_attack02, m_pTransCom->m_vPosition, 0.8f);
		}
		if(!m_bCutsceneSoundPlay[4] && m_pMeshCom->Check_EndPeriodMulTime(0.35f))
		{
			m_bCutsceneSoundPlay[4] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_breath03, m_pTransCom->m_vPosition, 0.6f);
		}		
		if(!m_bCutsceneSoundPlay[5] && m_pMeshCom->Check_EndPeriodMulTime(0.45f))
		{
			m_bCutsceneSoundPlay[5] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_attack03, m_pTransCom->m_vPosition, 0.8f);
		}		
		if(!m_bCutsceneSoundPlay[6] && m_pMeshCom->Check_EndPeriodMulTime(0.49f))
		{
			m_bCutsceneSoundPlay[6] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_breath02, m_pTransCom->m_vPosition, 0.6f);
		}		
		if(!m_bCutsceneSoundPlay[7] && m_pMeshCom->Check_EndPeriodMulTime(0.55f))
		{
			m_bCutsceneSoundPlay[7] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_breath03, m_pTransCom->m_vPosition, 1.0f);			
		}
		if(!m_bCutsceneSoundPlay[8] && m_pMeshCom->Check_EndPeriodMulTime(0.56f))
		{
			m_bCutsceneSoundPlay[8] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_bone03, m_pTransCom->m_vPosition, 0.5f);			
		}
		if(!m_bCutsceneSoundPlay[9] && m_pMeshCom->Check_EndPeriodMulTime(0.6f))
		{
			m_bCutsceneSoundPlay[9] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_breath01, m_pTransCom->m_vPosition, 0.8f);
		}
		if(!m_bCutsceneSoundPlay[10] && m_pMeshCom->Check_EndPeriodMulTime(0.63f))
		{
			m_bCutsceneSoundPlay[10] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_bone01, m_pTransCom->m_vPosition, 0.7f);	
		}
		if(!m_bCutsceneSoundPlay[11] && m_pMeshCom->Check_EndPeriodMulTime(0.69f))
		{
			m_bCutsceneSoundPlay[11] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_bone02, m_pTransCom->m_vPosition, 0.7f);
		}
		if(!m_bCutsceneSoundPlay[12] && m_pMeshCom->Check_EndPeriodMulTime(0.745f))
		{
			m_bCutsceneSoundPlay[12] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_bone03, m_pTransCom->m_vPosition, 0.7f);
		}
		if(!m_bCutsceneSoundPlay[13] && m_pMeshCom->Check_EndPeriodMulTime(0.78f))
		{
			m_bCutsceneSoundPlay[13] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_bone04, m_pTransCom->m_vPosition, 0.7f);
		}
		if(!m_bCutsceneSoundPlay[14] && m_pMeshCom->Check_EndPeriodMulTime(0.84f))
		{
			m_bCutsceneSoundPlay[14] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_bone02, m_pTransCom->m_vPosition, 0.7f);	
		}		
		if(!m_bCutsceneSoundPlay[15] && m_pMeshCom->Check_EndPeriodMulTime(0.87f))
		{
			m_bCutsceneSoundPlay[15] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_attack02, m_pTransCom->m_vPosition, 1.0f);			
		}
		if(!m_bCutsceneSoundPlay[16] && m_pMeshCom->Check_EndPeriodMulTime(0.96f))
		{
			m_bCutsceneSoundPlay[16] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_warp_begin, m_pTransCom->m_vPosition, 1.0f);			
		}
		if(!m_bCutsceneSoundPlay[17] && m_pMeshCom->Check_EndPeriodMulTime(0.98f))
		{
			m_bCutsceneSoundPlay[17] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_warp_end, m_pTransCom->m_vPosition, 1.0f);
			CEffectMgr::GetInstance()->Play_Effect(L"bat",m_pTransCom->m_vPosition + _vec3(-2.f, 2.f, 0.f), false, false/*, _vec3(4.f, 4.f, 4.f)*/);	
			CEffectMgr::GetInstance()->Play_Effect(L"bat",m_pTransCom->m_vPosition + _vec3(2.f, 2.f, 0.f), false, false/*, _vec3(4.f, 4.f, 4.f)*/);			
			CEffectMgr::GetInstance()->Play_Effect(L"bat",m_pTransCom->m_vPosition + _vec3(2.f, -2.f, 0.f), false, false/*, _vec3(4.f, 4.f, 4.f)*/);
			CEffectMgr::GetInstance()->Play_Effect(L"bat",m_pTransCom->m_vPosition + _vec3(-2.f, -2.f, 0.f), false, false/*, _vec3(4.f, 4.f, 4.f)*/);
			CEffectMgr::GetInstance()->Play_Effect(L"bat",m_pTransCom->m_vPosition , false, false/*, _vec3(4.f, 4.f, 4.f)*/);
		}

		if(!m_bCutsceneSoundPlay[18] && m_pMeshCom->Check_EndPeriodMulTime(0.7f))
		{
			m_bCutsceneSoundPlay[18] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_breath01, m_pTransCom->m_vPosition, 0.5f);
		}

		if(!m_bCutsceneSoundPlay[19] && m_pMeshCom->Check_EndPeriodMulTime(0.75f))
		{
			m_bCutsceneSoundPlay[19] = true;
			CSoundMgr::GetInstance()->PlayEffect(SOUND::lord_breath03, m_pTransCom->m_vPosition, 0.5f);
		}
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_cutscene_2)
	{
		SoundEffectPlay(SOUND::lord_roar, 0.05f, 1);
		SoundEffectPlay(SOUND::lord_breath02, 0.4f, 2, 0.8f);
		SoundEffectPlay(SOUND::lord_breath03, 0.6f, 3, 0.8f);
		SoundEffectPlay(SOUND::lord_threat, 0.95f, 4);

		if(m_pMeshCom->Check_EndPeriodMulTime(0.9f))
		{
			if(!m_bCutsceneFadeCheck)
			{
				CFadeInOut::GetInstance()->SetFadeInOut(true, 3.f);
				m_bCutsceneFadeCheck = true;
			}
		}
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_attack_forehand_hard)
	{
		SoundEffectPlay(SOUND::lord_attack01, 0.0f, 1);
		SoundEffectPlay(SOUND::effect_fireball_appear, 0.3f, 2);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_attack_hard)
	{
		SoundEffectPlay(SOUND::lord_attack01, 0.0f, 1);
		SoundEffectPlay(SOUND::lord_bone01, 0.1f, 2);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.15f, 3);
		SoundEffectPlay(SOUND::lord_bone01, 0.35f, 4);
		SoundEffectPlay(SOUND::lord_attack01, 0.36f, 5);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.4f, 6);
		SoundEffectPlay(SOUND::lord_bone01, 0.9f, 7);
		SoundEffectPlay(SOUND::lord_bone01, 0.93f, 8);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_attack_hello)
	{
		SoundEffectPlay(SOUND::lord_breath01, 0.f, 0);
		SoundEffectPlay(SOUND::lord_roar, 0.35f, 1);
		SoundEffectPlay(SOUND::lord_attack01, 0.4f, 2);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.41f, 3);
		SoundEffectPlay(SOUND::lord_attack01, 0.44f, 4);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.45f, 5);
		SoundEffectPlay(SOUND::lord_attack01, 0.47f, 6);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.48f, 7);
		SoundEffectPlay(SOUND::lord_attack01, 0.6f, 8);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.62f, 9);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_attack02_hard)
	{
		SoundEffectPlay(SOUND::lord_attack01, 0.2f, 1, 0.5f);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.3f, 2);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_attack03_fast_hard)
	{
		SoundEffectPlay(SOUND::lord_attack01, 0.26f, 1, 0.5f);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.3f, 2);

		SoundEffectPlay(SOUND::lord_attack01, 0.34f, 3, 0.5f);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.38f, 4);

		SoundEffectPlay(SOUND::lord_attack01, 0.42f, 5, 0.5f);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.46f, 6);

		SoundEffectPlay(SOUND::lord_attack01, 0.5f, 7, 0.5f);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.54f, 8);

		SoundEffectPlay(SOUND::lord_attack01, 0.58f, 9, 0.5f);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.63f, 10);

		SoundEffectPlay(SOUND::lord_roar, 0.72f, 13);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_attack03_hard)
	{
		SoundEffectPlay(SOUND::lord_attack01, 0.3f, 1, 0.5f);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.33f, 2);

		SoundEffectPlay(SOUND::lord_attack01, 0.42f, 3, 0.5f);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.45f, 4);

		SoundEffectPlay(SOUND::lord_attack01, 0.55f, 5, 0.5f);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.58f, 6);

		SoundEffectPlay(SOUND::lord_attack01, 0.65f, 7, 0.5f);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.68f, 8);

		SoundEffectPlay(SOUND::lord_attack01, 0.75f, 9, 0.5f);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.78f, 10);

		SoundEffectPlay(SOUND::lord_attack01, 0.83f, 11, 0.5f);

	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_laugh)
	{
		SoundEffectPlay(SOUND::lord_threat, 0.2f, 0);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_wrap_air2_begin)
	{
		SoundEffectPlay(SOUND::lord_warp_end, 0.33f, 0);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_wrap_air2_end)
	{
		SoundEffectPlay(SOUND::lord_warp_end, 0.0f, 0);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.1f, 1);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.2f, 2);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.35f, 3);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.65f, 4);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_wrap_air_hard_end)
	{
		SoundEffectPlay(SOUND::lord_threat, 0.f, 0);
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.4f, 1);
		SoundEffectPlay(SOUND::Bear_Step, 0.7f, 2);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_attack_dancing_begin)
	{
		m_bMoveCollCheck = false;
		SoundEffectPlay(SOUND::lord_breath02, 0.0f, 0);
		SoundEffectPlay(SOUND::lord_attack01, 0.4f, 1);

		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.8f, 2);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_attack_dancing_during)
	{
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.0f, 0);
		SoundEffectPlay(SOUND::lord_attack01, 0.0f, 1);

		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.05f, 2);
		SoundEffectPlay(SOUND::lord_attack01, 0.1f, 3);

		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.1f, 4);
		SoundEffectPlay(SOUND::lord_attack01, 0.15f, 5);

		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.4f, 6);
		SoundEffectPlay(SOUND::lord_attack01, 0.45f, 7);

		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.5f, 8);
		SoundEffectPlay(SOUND::lord_attack01, 0.55f, 9);

		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.6f, 10);
		SoundEffectPlay(SOUND::lord_attack01, 0.65f, 11);

		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.7f, 12);
		SoundEffectPlay(SOUND::lord_attack01, 0.75f, 13);

		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.8f, 14);
		SoundEffectPlay(SOUND::lord_attack01, 0.85f, 15);

		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.9f, 16);
		SoundEffectPlay(SOUND::lord_attack01, 0.95f, 17);

		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.9f, 18);
		SoundEffectPlay(SOUND::lord_attack01, 0.95f, 19);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_attack_dancing_end)
	{
		SoundEffectPlay(SOUND::Hit_Flesh_Slash, 0.75f, 0);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::run_N)
	{
		SoundEffectPlay(SOUND::vampire_step1, 0.1f, 0);
		SoundEffectPlay(SOUND::vampire_step2, 0.5f, 1);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_attack_bloodfield_begin1)
	{
		SoundEffectPlay(SOUND::lord_breath02, 0.f, 0);
		SoundEffectPlay(SOUND::lord_breath01, 0.25f, 1);
		SoundEffectPlay(SOUND::lord_breath03, 0.5f, 2);
		SoundEffectPlay(SOUND::lord_breath02, 0.75f, 3);
		SoundEffectPlay(SOUND::lord_attack01, 0.9f, 4);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_attack_bloodfield_during1)
	{
		SoundEffectPlay(SOUND::lord_attack01, 0.f, 0);
		SoundEffectPlay(SOUND::lord_attack01, 0.1f, 1);

		SoundEffectPlay(SOUND::explosion_lv4, 0.2f, 2);
		SoundEffectPlay(SOUND::explosion_lv4, 0.3f, 3);
		SoundEffectPlay(SOUND::explosion_lv4, 0.4f, 4);
		SoundEffectPlay(SOUND::explosion_lv4, 0.5f, 5);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_attack_bloodfield_end)
	{
		SoundEffectPlay(SOUND::lord_breath01, 0.f, 0);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_attack_blood_breath)
	{
		SoundEffectPlay(SOUND::lord_firebreath, 0.2f, 0);
		SoundEffectPlay(SOUND::lord_breath01, 0.4f, 1);
		SoundEffectPlay(SOUND::lord_breath01, 0.6f, 2);
		SoundEffectPlay(SOUND::lord_breath01, 0.8f, 3);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_swamp_begin)
	{
		SoundEffectPlay(SOUND::lord_warp_begin, 0.f, 0);
		SoundEffectPlay(SOUND::lord_attack01, 0.1f, 1);
		SoundEffectPlay(SOUND::lord_warp_end, 0.3f, 2);
		SoundEffectPlay(SOUND::lord_threat, 0.8f, 3);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_swamp_during)
	{
		SoundEffectPlay(SOUND::lord_attack01, 0.3f, 0);
		SoundEffectPlay(SOUND::lord_attack01, 0.5f, 1);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == BloodLord::vamplord_swamp_end)
	{
		SoundEffectPlay(SOUND::lord_attack01, 0.1f, 0);
		SoundEffectPlay(SOUND::lord_attack01, 0.3f, 1);
		SoundEffectPlay(SOUND::lord_roar, 0.6f, 2);
	}
	else if( !m_bRealDie && m_pMeshCom->Get_CurrentAniIdx() == BloodLord::dying_from_front)
	{
		SoundEffectPlay(SOUND::lord_dying2, 0.f, 0);
		SoundEffectPlay(SOUND::lord_low_health, 0.3f, 1);

		SoundEffectPlay(SOUND::lord_bone01, 0.1f, 2);
		SoundEffectPlay(SOUND::lord_bone01, 0.15f, 3);
		SoundEffectPlay(SOUND::lord_bone01, 0.2f, 4);
		SoundEffectPlay(SOUND::lord_bone01, 0.25f, 5);
		SoundEffectPlay(SOUND::lord_bone01, 0.4f, 6);
		SoundEffectPlay(SOUND::lord_bone01, 0.42f, 7);
		SoundEffectPlay(SOUND::lord_bone01, 0.5f, 8);
		SoundEffectPlay(SOUND::lord_bone01, 0.53f, 9);
		SoundEffectPlay(SOUND::lord_bone01, 0.6f, 10);
		SoundEffectPlay(SOUND::lord_bone01, 0.64f, 9);
		SoundEffectPlay(SOUND::lord_bone01, 0.7f, 11);
	}
}

void CBloodLord::BatEffect()
{
	_matrix matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	_vec3 vCam;
	memcpy( &vCam, &matView.m[3][0], sizeof(_vec3));

	_vec3 vDir = vCam - m_pTransCom->m_vPosition;
	D3DXVec3Normalize(&vDir, &vDir);

	memcpy( &m_vMonsterDir, &(m_pTransCom->m_matWorld.m[2][0]), sizeof(_vec3) );	
	D3DXVec3Normalize(&m_vMonsterDir, &m_vMonsterDir);									
	D3DXVec3TransformNormal(&m_vMonsterDir, &m_vMonsterDir, &m_matRotationDefault);	

	_float fAngle = acosf( D3DXVec3Dot(&m_vMonsterDir, &vDir) );
	_vec3 vRight;

	memcpy( &vRight, &(m_pTransCom->m_matWorld.m[0][0]), sizeof(_vec3) );	
	D3DXVec3Normalize(&vRight, &vRight);									
	D3DXVec3TransformNormal(&vRight, &vRight, &m_matRotationDefault);	
	_float fAngle_Right = acosf( D3DXVec3Dot(&vRight, &g_vLook) );

	if( D3DXToDegree(fAngle_Right) <= 90.f )
		fAngle = D3DX_PI * 2.f - fAngle;

	CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, 0.f), false, false, _vec3(9.f, 9.f,9.f));
	CEffectMgr::GetInstance()->Play_Effect(L"DustBig",m_pTransCom->m_vPosition + _vec3(0.f, 6.f, 0.f), false, false, _vec3(9.f, 9.f,9.f), _vec3(0.f, 90.f, 0.f));

	CEffectMgr::GetInstance()->Play_Effect(L"bat",m_pTransCom->m_vPosition + _vec3(7.f, 8.f, 0.f), false, false, _vec3( 0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), fAngle);				
	CEffectMgr::GetInstance()->Play_Effect(L"bat",m_pTransCom->m_vPosition + _vec3(-6.f, 6.f, 0.f), false, false, _vec3( 0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), fAngle);	
	CEffectMgr::GetInstance()->Play_Effect(L"bat",m_pTransCom->m_vPosition + _vec3(6.f, 6.f, 4.f), false, false, _vec3( 0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), fAngle + D3DXToRadian(90));				

	CEffectMgr::GetInstance()->Play_Effect(L"bat",m_pTransCom->m_vPosition + _vec3(-3.f, 3.f, 0.f), false, false, _vec3( 0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), fAngle);	
	CEffectMgr::GetInstance()->Play_Effect(L"bat",m_pTransCom->m_vPosition + _vec3(3.f, 8.f, -2.f), false, false, _vec3( 0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), fAngle + D3DXToRadian(90));	

	CEffectMgr::GetInstance()->Play_Effect(L"bat",m_pTransCom->m_vPosition + _vec3(0.f, 8.f, 0.f), false, false, _vec3( 1.5f, 1.5f, 1.5f), _vec3(0.f, 0.f, 0.f), fAngle);	
	CEffectMgr::GetInstance()->Play_Effect(L"bat",m_pTransCom->m_vPosition + _vec3(0.f, 3.f, 0.f), false, false, _vec3( 0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), fAngle);	

}
void CBloodLord::Create_TrailPos()
{
	if(m_pmatLeftElbow == NULL)
	{
		m_pmatLeftElbow		=	m_pMeshCom->Get_FrameMatrix("ValveBiped_Bip01_L_Forearm");
		m_pmatRightElbow	=	m_pMeshCom->Get_FrameMatrix("ValveBiped_Bip01_R_Forearm");
		m_pmatLeftOne		=	m_pMeshCom->Get_FrameMatrix("rakeL1");
		m_pmatLeftTwo		=  m_pMeshCom->Get_FrameMatrix("rakeL2");
		m_pmatLeftThree		=	m_pMeshCom->Get_FrameMatrix("rakeL3");
		m_pmatRightOne		=	m_pMeshCom->Get_FrameMatrix("rakeR1");
		m_pmatRightTwo		=	m_pMeshCom->Get_FrameMatrix("rakeR2");
		m_pmatRightThree	=	m_pMeshCom->Get_FrameMatrix("rakeR3");

		if(NULL == m_pmatLeftElbow	)	MSG_BOX("FAILED GET m_pmatLeftElbow		");
		if(NULL == m_pmatRightElbow	)	MSG_BOX("FAILED GET m_pmatRightElbow	");

		if(NULL == m_pmatLeftOne		)	MSG_BOX("FAILED GET m_pmatLeftOne		");
		if(NULL == m_pmatLeftTwo	)	MSG_BOX("FAILED GET m_pmatLeftTwo		");
		if(NULL == m_pmatLeftThree	)	MSG_BOX("FAILED GET m_pmatLeftThree		");
		if(NULL == m_pmatRightOne	)	MSG_BOX("FAILED GET m_pmatRightOne		");
		if(NULL == m_pmatRightTwo	)	MSG_BOX("FAILED GET m_pmatRightTwo		");
		if(NULL == m_pmatRightThree	)	MSG_BOX("FAILED GET m_pmatRightThree	");
	}
	memcpy(&m_vLeftElbow,	&m_pmatLeftElbow->m[3][0],		sizeof(_vec3));
	memcpy(&m_vRightElbow,	&m_pmatRightElbow->m[3][0],	sizeof(_vec3));
	memcpy(&m_vLeftOne,		&m_pmatLeftOne->m[3][0],		sizeof(_vec3));
	memcpy(&m_vLeftTwo,		&m_pmatLeftTwo->m[3][0],		sizeof(_vec3));
	memcpy(&m_vLeftThree,	&m_pmatLeftThree->m[3][0],		sizeof(_vec3));
	memcpy(&m_vRightOne,	&m_pmatRightOne->m[3][0],		sizeof(_vec3));
	memcpy(&m_vRightTwo,	&m_pmatRightTwo->m[3][0],		sizeof(_vec3));
	memcpy(&m_vRightThree,	&m_pmatRightThree->m[3][0],	sizeof(_vec3));

}