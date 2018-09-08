#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"

#include "DynamicCamera.h"
#include "Line2D.h"
#include "EffectMgr.h"
#include "CollisionMgr.h"
#include "StaticObject.h"
#include "Monster.h"

#include "FireBall.h"
#include "RenderTargetMgr.h"
#include "PlayerSkill.h"
#include "DamageFont.h"

#include "SoundMgr.h"
#include "PlayerHeartBeatUI.h"
#include "Sword.h"
#include "EquipmentUI.h"

IMPLEMENT_SINGLETON(CPlayer)

CPlayer::CPlayer(void)
: CGameObject(Engine::Get_GraphicDev())
, m_pMeshCom(NULL)
, m_pTransCom(NULL)
, m_pEffect(NULL)
, m_pNaviCell(NULL)
, m_fTimeDelta(0.f)
, m_fAnimationSpeed(2.0f)
, m_fRenderTimeDelta(0.f)
, m_fEffectMoveTime(0.f)
, m_bOnceSpaceKeyCheck(false)
, m_bOnceLbuttonCheck(false)
, m_bOnceRbuttonCheck(false)
, m_fIdleToRestTime(0.f)
, BasicAttackOrder(BA_1a)
, SmashAttackOrder(SA_0)
, matLFoot(NULL)
, matRFoot(NULL)
, m_iChargeSlashNumber(0)
, m_pEnvironmentTexCom(NULL)	// 환경매핑 갑옷 철느낌
, m_bMonsterAttack_Body_CollisionCheck(false)
, m_bMonsterEffect_Body_CollisionCheck(false)
, m_ePlayerState(PS_Idle)
, m_bPlayerHitCheck(false)
, m_eDamageDirectionType(DDT_FRONT)
, m_iPlayerAttackCombo(0)
, m_fPlayerAttackComboTime(0.f)
, m_fPlayerPureAttackValue(0.f)
, m_fPlayerRunFullTime(0.f)
, m_bPlayerRunFull(false)
, m_fWakeUpAttackMagnification(0.f)
, m_vCollisionPos(0.f, 0.f, 0.f)
, m_bInGame(false)
, m_bSwordAttack(false)
, m_fEffAttackValue(0.f)
, m_ePlayerDefenseState(PS_Basic), m_bBerserkerCheck(false)
, m_fBerserkerTime(0.f)
, m_fBerserkerFullTime(0.f)
, m_fBerserkerCreateTime(0.f)
, m_iColBox(-1)
, m_vEffectPos(0.f, 0.f, 0.f)
, m_vAttackStartPos(0.f, 0.f, 0.f)
, m_pmatEye(NULL)
, m_vEyePos(0.f,0.f,0.f), m_fDownValue(0.f){
	Initialize();

	D3DXMatrixIdentity(&matUpperColor);
	D3DXMatrixIdentity(&matLowerColor);
	D3DXMatrixIdentity(&matHandColor);
	D3DXMatrixIdentity(&matFootColor);

	D3DXMatrixIdentity(&m_matPreView);	
#ifdef _DEBUG
	m_bClick		= false;
	m_bShowBounding	= false;
#endif

	memset(m_bBox, true, sizeof(_bool) * 5 );
}

CPlayer::~CPlayer(void)
{

}

HRESULT CPlayer::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Color");
	if(NULL == m_pEffect)		return E_FAIL;
	m_pShadowEffect = Engine::Get_EffectHandle(L"Shader_Shadow");
	if(NULL == m_pShadowEffect)		return E_FAIL;	
	m_pVelocityEffect = Engine::Get_EffectHandle(L"Shader_Velocity");
	if(NULL == m_pVelocityEffect)	return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	Engine::CLightMgr::GetInstance()->Set_PlayerPos(&m_pTransCom->m_vPosition);

	m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);
	m_pTransCom->m_vPosition = _vec3(0.f, 0.f, 0.f);//_vec3(175.f, 0.f, 250.f);
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian(0);
	m_fOriginalAngleY = 0.f;	//플레이어 초기 각도
	
	//플레이어 방향 각도 초기화
	ZeroMemory(m_fAngleFixed, sizeof(_float) * AF_End);
	/*
	m_fAngleFixed[AF_LeftFront]		= 225.f;
	m_fAngleFixed[AF_RightFront]	= 315.f;
	m_fAngleFixed[AF_LeftBack]		= 135.f;
	m_fAngleFixed[AF_RightBack]		= 45.f;
	m_fAngleFixed[AF_Front]			= 270.f;
	m_fAngleFixed[AF_Back]			= 90.f;
	m_fAngleFixed[AF_Left]			= 180.f;
	m_fAngleFixed[AF_Right]			= 0.f;
	*/
	m_fAngleFixed[AF_LeftFront]		= 45.f;
	m_fAngleFixed[AF_RightFront]	= 135.f;
	m_fAngleFixed[AF_LeftBack]		= 315.f;
	m_fAngleFixed[AF_RightBack]		= 225.f;
	m_fAngleFixed[AF_Front]			= 90.f;
	m_fAngleFixed[AF_Back]			= 270.f;
	m_fAngleFixed[AF_Left]			= 0.f;
	m_fAngleFixed[AF_Right]			= 180.f;

	//플레이어 처음 애니메이션 설정
	m_ePlayerAnimation		= Player::battle_rest;
	m_ePlayerAnimationCheck	= Player::battle_rest;
	m_ePlayerAttackStaminaAnimationCheck = Player::battle_rest;
	m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);

	//네비게이션 포즈들 초기화
	m_vOriginalPos = m_pTransCom->m_vPosition;
	m_vAfterPos = m_pTransCom->m_vPosition;

	//플레이어 능력치 초기화
	m_fPlayerHP			= 5000.f;
	m_fPlayerMaxHP		= m_fPlayerHP;
	m_fPlayerStamina	= 150.f;
	m_fPlayerMaxStamina	= m_fPlayerStamina;
	m_fPlayerSP			= 1500.f;
	m_fPlayerMaxSP		= m_fPlayerSP;

	m_fPlayerPureAttackValue	= 10000.f;
	m_fPlayerAttackValue		= m_fPlayerPureAttackValue;

	//이펙트 생성에 필요한 매트릭스들의 주소값을 가져온다.
	m_pAttachment_LH_Matrix = m_pMeshCom->Get_FrameMatrix("ValveBiped_Anim_Attachment_LH");
	m_pAttachment_RH_Matrix = m_pMeshCom->Get_FrameMatrix("ValveBiped_Anim_Attachment_RH");
	m_pL_Foot_Matrix = m_pMeshCom->Get_FrameMatrix("ValveBiped_Bip01_L_Foot");
	m_pR_Foot_Matrix = m_pMeshCom->Get_FrameMatrix("ValveBiped_Bip01_R_Foot");

	ZeroMemory(m_bCreateEffect, sizeof(_bool) * 50);
	ZeroMemory(m_bPlayerSound, sizeof(_bool) * 5);

	//충돌관련 변수 초기화
	m_vMoveMin		= _vec3(-9.f, 0.f, -3.f);
	m_vMoveMax		= _vec3(9.f, 10.f, 12.f);
	m_fMoveRadius	= D3DXVec3Length(&(m_vMoveMax - m_vMoveMin)) * m_pTransCom->m_vScale.x;

	ZeroMemory(m_pAttackBox, sizeof(CLIENTBOXINFO*) * Player::PlayerAnimation_End);
	//// 바운딩박스 로드
	Load_AttackBox();
	Load_DamageBox();

	//// 부위파괴 관련 메쉬 이름들 초기화
	Initialize_TCHAR();

	m_fBoxHP[0] = 300;	//헬멧
	m_fBoxHP[1] = 500;	//상의 갑옷
	m_fBoxHP[2] = 200;	//장갑
	m_fBoxHP[3] = 500;	//하의 갑옷
	m_fBoxHP[4] = 100;	//신발

	return S_OK;
}

HRESULT CPlayer::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	//Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	//Mesh Component
	pComponent = m_pMeshCom = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_STATIC, L"Mesh_Player");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	//////////////////////////////////////////////////////////////////////////
	_matrix matColor;
	D3DXMatrixIdentity(&matColor);
	matColor._11 = 1.f;
	matColor._12 = 1.f;
	matColor._13 = 1.f;

	matColor._21 = 1.f;
	matColor._22 = 1.f;
	matColor._23 = 1.f;

	matColor._31 = 1.f;
	matColor._32 = 1.f;
	matColor._33 = 1.f;
	memcpy( matUpperColor, matColor, sizeof(_matrix));
	memcpy( matLowerColor, matColor, sizeof(_matrix));
	memcpy( matHandColor, matColor, sizeof(_matrix));
	memcpy( matFootColor, matColor, sizeof(_matrix));

	matColor._11 = 0.f;	matColor._12 = 0.f; matColor._13 = 0.f;
	matColor._21 = 0.f;	matColor._22 = 0.f;	matColor._23 = 0.f;
	matColor._31 = 0.f;	matColor._32 = 0.f;	matColor._33 = 0.f;
	memcpy( matHairColor, matColor, sizeof(_matrix));

	m_pMeshCom->Set_HairColor(matHairColor);
	m_pMeshCom->Set_UpperColor(matUpperColor);
	m_pMeshCom->Set_LowerColor(matUpperColor);
	m_pMeshCom->Set_HandColor(matUpperColor);
	m_pMeshCom->Set_FootColor(matUpperColor);
	//////////////////////////////////////////////////////////////////////////


	return S_OK;
}

_int CPlayer::Update(const float& fTimeDelta)
{
	//printf("%f \t %f\n", m_pTransCom->m_vPosition.x, m_pTransCom->m_vPosition.z);
	if(NULL == m_pEnvironmentTexCom)		// 금속재질 환경매핑을 위한 텍스쳐 가져오기
		m_pEnvironmentTexCom = (Engine::CTexture*)Engine::Get_Component(L"Com_Texture", L"Environment", L"Sky");

	//트레일////////////////////////////////
	if(NULL == m_pmatEye)
	{
		m_pmatEye	= m_pMeshCom->Get_FrameMatrix("jiggle_hair_SideR");
		if(NULL == m_pmatEye	)	MSG_BOX("FAILED GET m_pmatEye");
	}
	memcpy(&m_vEyePos, &m_pmatEye->m[3][0], sizeof(_vec3));
	//////////////////////////////////////////

	m_fTimeDelta		= fTimeDelta;
	m_fRenderTimeDelta	= fTimeDelta * m_fAnimationSpeed * 1.0f;

	if(m_bInGame)
	{
		PlayerStateCheck();			//플레이어의 상태를 변경해준다. (공격상태, 슈퍼아머 등등)
		CollisionCheckFunction();	//이동을 제외한 충돌 체크들을 체크한다.

		Key_Input_Change_PlayerAnimationState(fTimeDelta);	// 키입력에 따라 플레이어 애니메이션 상태를 바꾼다.

		AnimationPattern_Immediately_Change();				// 플레이어의 애니메이션 상태와 여러 조건에 따라 애니메이션을 바꾼다.

		Player_InfoChange();		//플레이어의 공격력, 다운수치를 조절 하는 곳
		Player_UnderAttack_InfoChange();	//플레이어의 HP, SP, 스태미나, 충돌에 따른 피격 모션
		CreateEffectFunction(); //이펙트를 생성하는 곳
		PlayerMoveFunction();		//플레이어 이동 관련(오브젝트, 다이나믹 메쉬 충돌, 애니메이션 싱크)
		

		
		PlayerSound();			//사운드를 재생시키는곳(맞는 사운드 는 Player_UnderAttack_InfoChange에서 재생)
	}
	else
	{
		//시작 화면에서의 플레이어 관련 함수들 호출
		if( m_pMeshCom->Check_EndPeriod() )
			m_pMeshCom->Set_AnimationSetThree(Player::battle_idle_lower);

	}

#pragma region 임시 염색, 장비 교체 키 입력
	//임시 염색 용 키 입력
	if(GetAsyncKeyState('T') & 0x0001)
	{
		_matrix matColor;
		D3DXMatrixIdentity(&matColor);
		matColor._11 = 1.f - (255.f - 0)/255.f;
		matColor._12 = 1.f - (255.f - 0)/255.f;
		matColor._13 = 1.f - (255.f - 0)/255.f;
		matColor._21 = 1.f - (255.f - 20)/255.f;
		matColor._22 = 1.f - (255.f - 20)/255.f;
		matColor._23 = 1.f - (255.f - 20)/255.f;

		matColor._31 = 1.f - (255.f - 80)/255.f;
		matColor._32 = 1.f - (255.f - 40)/255.f;
		matColor._33 = 1.f - (255.f - 40)/255.f;
		memcpy( matHairColor, matColor, sizeof(_matrix));
		m_pMeshCom->Set_HairColor(matHairColor);

		matColor._11 = 1.f - (255.f - 200)/255.f;
		matColor._12 = 1.f - (255.f - 60)/255.f;
		matColor._13 = 1.f - (255.f - 60)/255.f;

		matColor._21 = 1.f - (255.f - 200)/255.f;
		matColor._22 = 1.f - (255.f - 80)/255.f;
		matColor._23 = 1.f - (255.f - 80)/255.f;

		matColor._31 = 1.f - (255.f - 200)/255.f;
		matColor._32 = 1.f - (255.f - 100)/255.f;
		matColor._33 = 1.f - (255.f - 100)/255.f;
		memcpy( matUpperColor, matColor, sizeof(_matrix));
		m_pMeshCom->Set_UpperColor(matUpperColor);
		matColor._11 = 1.f - (255.f - 200)/255.f;
		matColor._12 = 1.f - (255.f - 80)/255.f;
		matColor._13 = 1.f - (255.f - 80)/255.f;

		matColor._21 = 1.f - (255.f - 20)/255.f;
		matColor._22 = 1.f - (255.f - 20)/255.f;
		matColor._23 = 1.f - (255.f - 20)/255.f;

		matColor._31 = 1.f - (255.f - 80)/255.f;
		matColor._32 = 1.f - (255.f - 40)/255.f;
		matColor._33 = 1.f - (255.f - 40)/255.f;
		memcpy( matLowerColor, matColor, sizeof(_matrix));
		m_pMeshCom->Set_LowerColor(matLowerColor);

		matColor._11 = 1.f - (255.f - 200)/255.f;
		matColor._12 = 1.f - (255.f - 60)/255.f;
		matColor._13 = 1.f - (255.f - 60)/255.f;

		matColor._21 = 1.f - (255.f - 200)/255.f;
		matColor._22 = 1.f - (255.f - 80)/255.f;
		matColor._23 = 1.f - (255.f - 80)/255.f;

		matColor._31 = 1.f - (255.f - 200)/255.f;
		matColor._32 = 1.f - (255.f - 100)/255.f;
		matColor._33 = 1.f - (255.f - 100)/255.f;
		memcpy( matHandColor, matColor, sizeof(_matrix));
		m_pMeshCom->Set_HandColor(matHandColor);

		matColor._11 = 1.f - (255.f - 0)/255.f;
		matColor._12 = 1.f - (255.f - 0)/255.f;
		matColor._13 = 1.f - (255.f - 0)/255.f;

		matColor._21 = 1.f - (255.f - 20)/255.f;
		matColor._22 = 1.f - (255.f - 20)/255.f;
		matColor._23 = 1.f - (255.f - 20)/255.f;

		matColor._31 = 1.f - (255.f - 80)/255.f;
		matColor._32 = 1.f - (255.f - 40)/255.f;
		matColor._33 = 1.f - (255.f - 40)/255.f;
		memcpy( matFootColor, matColor, sizeof(_matrix));
		m_pMeshCom->Set_FootColor(matFootColor);

		matColor._11 = 1.f - (255.f - 255)/255.f;
		matColor._12 = 1.f - (255.f - 94)/255.f;
		matColor._13 = 1.f - (255.f - 0)/255.f;

		matColor._21 = 1.f - (255.f - 255)/255.f;
		matColor._22 = 1.f - (255.f - 228)/255.f;
		matColor._23 = 1.f - (255.f - 20)/255.f;

		matColor._31 = 1.f - (255.f - 152)/255.f;
		matColor._32 = 1.f - (255.f - 0)/255.f;
		matColor._33 = 1.f - (255.f - 0)/255.f;
		memcpy(g_tSwordInfo.matColor, matColor, sizeof(_matrix));
	}
#pragma endregion

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_SHADOW, this);
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_VELOCITY, this);
	CGameObject::Update(fTimeDelta);


	//player 방향벡터		★ 재훈
	/*_matrix		matRotY;
	D3DXMatrixRotationY(&matRotY, D3DXToRadian(m_fOriginalAngleY));
	D3DXVec3TransformNormal(&m_pTransCom->m_vDir, &g_vLook, &matRotY);*/
	//////////////////////////////////////

	return 0;
}

void CPlayer::Render(void)
{
	if(NULL == m_pEffect)		return;

#ifdef _DEBUG
	if(Engine::GetDIKeyState(DIK_K) & 0x80)
	{
		if(!m_bClick)
			m_bShowBounding ^= true;
		m_bClick = true;
	}
	else
		m_bClick = false;

	if(m_bShowBounding)
	{
		Render_BoudingSphere();
		Render_AttackBox();	
		Render_DamageBox();	
	}
	//m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
	//	// 그림자 컨트롤
#endif
	
	Set_ContantTable();		
	m_pMeshCom->Move_Frame(m_fRenderTimeDelta);
	m_pMeshCom->Render_MeshForShader(m_pEffect, m_pEnvironmentTexCom, true);
}
void CPlayer::Render_Velocity(void)
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

	m_pMeshCom->Render_MeshForShaderVelocity(m_pVelocityEffect);
	m_pVelocityEffect->SetTexture("g_BaseTexture", NULL);
	m_matPreView = matView;
}

void CPlayer::Render_Shadow(void)
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
	vEye = m_pTransCom->m_vPosition + (vLightDir * -100.f);
	vAt = vEye + vLightDir;
	vUp = _vec3(0.f, 1.f, 0.f);

	D3DXMatrixLookAtLH(&matLightView, &vEye, &vAt, &vUp);
	D3DXMatrixPerspectiveFovLH(&matLightProj, D3DX_PI * 0.4f, float(WINSIZEX) / WINSIZEY,  0.1f, 1000.f);

	m_pShadowEffect->SetMatrix("g_matLightWV", &(m_pTransCom->m_matWorld * matLightView));
	m_pShadowEffect->SetMatrix("g_matLightProj", &matLightProj);

	//m_pMeshCom->Render_MeshForShader(m_pShadowEffect);
	m_pMeshCom->Render_MeshForShaderShadow(m_pShadowEffect);
}

void CPlayer::Set_ContantTable(void)
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

	m_pEffect->SetVector("g_LightDir", &_vec4(0.f, -0.5f, 0.5f, 0.f));
}

void CPlayer::PlayerStateCheck(void)
{
	if(m_pMeshCom->Get_CurrentAniIdx() != m_ePlayerAnimationCheck || m_pMeshCom->Check_EndPeriod())
	{
		m_ePlayerAnimationCheck = (Player::PLAYERANIMATION)m_pMeshCom->Get_CurrentAniIdx();
		ZeroMemory(m_bCreateEffect, sizeof(_bool) * 50);
		ZeroMemory(m_bPlayerSound, sizeof(_bool) * 5);
	}

	if(m_pAttackBox[m_ePlayerAnimationCheck] != NULL)	//플레이어의 공격 박스가 존재 한다면
	{
		//플레이어의 공격박스가 Hit가 가능한 상태라면 참 아니면 거짓
		if( m_pMeshCom->Check_EndPeriodMulTime(m_pAttackBox[m_ePlayerAnimationCheck]->fStartTime) == false )
		{
			m_bSwordAttack = false;
		}
		else if( m_pMeshCom->Check_EndPeriodMulTime(m_pAttackBox[m_ePlayerAnimationCheck]->fEndTime) == true )
		{
			m_bSwordAttack = false;
		}
		else
			m_bSwordAttack = true;
	}
	else
		m_bSwordAttack = false;

	//공격 상태 관련
	if(m_pMeshCom->Get_CurrentAniIdx() >= Player::battle_attack_01_a && m_pMeshCom->Get_CurrentAniIdx() <= Player::battle_attack_04)
	{
		m_ePlayerState = PS_BasicAttack;	//기본 공격
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() >= Player::battle_attack_break_l && m_pMeshCom->Get_CurrentAniIdx() <= Player::battle_attack_break_speedymove_r)
	{
		m_ePlayerState = PS_BreakAttack;	//봉쇄
	}

	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_00)
	{
		m_ePlayerState = PS_SmashAttack_0;	//밟기
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() >= Player::battle_attack_strong_01 && m_pMeshCom->Get_CurrentAniIdx() <= Player::battle_attack_strong_run_sprint)
	{
		m_ePlayerState = PS_SmashAttack;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() >= Player::battle_idle_lower && m_pMeshCom->Get_CurrentAniIdx() <= Player::battle_rest)
	{
		m_ePlayerState = PS_Idle;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_run_sprint
		|| m_pMeshCom->Get_CurrentAniIdx() == Player::speedymove_l
		|| m_pMeshCom->Get_CurrentAniIdx() == Player::speedymove_r)
	{
		m_ePlayerState = PS_Run;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() >= Player::battle_run_lower && m_pMeshCom->Get_CurrentAniIdx() <= Player::battle_run_stop)
	{
		m_ePlayerState = PS_Walk;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() >= Player::SP_attack_charge_slash_begin_l && m_pMeshCom->Get_CurrentAniIdx() <= Player::SP_cast_berserker)
	{
		m_ePlayerState = PS_SkillAttack;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() >= Player::damage_light_back && m_pMeshCom->Get_CurrentAniIdx() <= Player::damage_strong_front_begin)
	{
		m_ePlayerState = PS_Damage;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() >= Player::down_back_roll_front && m_pMeshCom->Get_CurrentAniIdx() <= Player::down_resist)
	{
		m_ePlayerState = PS_Down;
	}

	//방어 상태 관련
	if(m_pMeshCom->Get_CurrentAniIdx() >= Player::battle_attack_01_a && m_pMeshCom->Get_CurrentAniIdx() <= Player::battle_attack_04)
	{
		if(m_bSwordAttack)
			m_ePlayerDefenseState = PS_SuperArmour;
		else
			m_ePlayerDefenseState = PS_Basic;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() >= Player::battle_attack_break_l && m_pMeshCom->Get_CurrentAniIdx() <= Player::battle_attack_break_speedymove_r)
	{
		m_ePlayerDefenseState = PS_SuperArmour;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_00)
	{
		if(m_pMeshCom->Check_EndPeriodMulTime(0.85f))
			m_ePlayerDefenseState = PS_Basic;
		else
			m_ePlayerDefenseState = PS_SuperArmour;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_01)
	{
		if(m_pMeshCom->Check_EndPeriodMulTime(0.55f))
			m_ePlayerDefenseState = PS_Basic;
		else
			m_ePlayerDefenseState = PS_SuperArmour;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_02_a)
	{
		if(m_pMeshCom->Check_EndPeriodMulTime(0.5f))
			m_ePlayerDefenseState = PS_Basic;
		else
			m_ePlayerDefenseState = PS_SuperArmour;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_03)
	{
		if(m_pMeshCom->Check_EndPeriodMulTime(0.6f))
			m_ePlayerDefenseState = PS_Basic;
		else
			m_ePlayerDefenseState = PS_SuperArmour;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_a)
	{
		if(m_pMeshCom->Check_EndPeriodMulTime(0.625f))
			m_ePlayerDefenseState = PS_Basic;
		else
			m_ePlayerDefenseState = PS_SuperArmour;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_b)
	{
		if(m_pMeshCom->Check_EndPeriodMulTime(0.51f))
			m_ePlayerDefenseState = PS_Basic;
		else
			m_ePlayerDefenseState = PS_SuperArmour;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_run_sprint)
	{
		if(m_pMeshCom->Check_EndPeriodMulTime(0.6f))
			m_ePlayerDefenseState = PS_Basic;
		else
			m_ePlayerDefenseState = PS_SuperArmour;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() >= Player::battle_idle_lower && m_pMeshCom->Get_CurrentAniIdx() <= Player::SP_attack_charge_slash_during_r)
	{
		m_ePlayerDefenseState = PS_Basic;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_charge_slash_up_l)
	{
		if(m_pMeshCom->Check_EndPeriodMulTime(0.55f))
			m_ePlayerDefenseState = PS_Basic;
		else
			m_ePlayerDefenseState = PS_SuperArmour;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_charge_slash_up_r)
	{
		if(m_pMeshCom->Check_EndPeriodMulTime(0.8f))
			m_ePlayerDefenseState = PS_Basic;
		else
			m_ePlayerDefenseState = PS_SuperArmour;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_execution)
	{
		if(m_pMeshCom->Check_EndPeriodMulTime(0.75f))
			m_ePlayerDefenseState = PS_Basic;
		else
			m_ePlayerDefenseState = PS_SuperArmour;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_hell)
	{
		m_ePlayerDefenseState = PS_SuperArmour;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_judgement_l)
	{
		if(m_pMeshCom->Check_EndPeriodMulTime(0.7f))
			m_ePlayerDefenseState = PS_Basic;
		else
			m_ePlayerDefenseState = PS_SuperArmour;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_judgement_r)
	{
		if(m_pMeshCom->Check_EndPeriodMulTime(0.61f))
			m_ePlayerDefenseState = PS_Basic;
		else
			m_ePlayerDefenseState = PS_SuperArmour;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_cast_berserker)
	{
		m_ePlayerDefenseState = PS_SuperArmour;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::speedymove_l)
	{
		if(m_pMeshCom->Check_EndPeriodMulTime(0.83f))
			m_ePlayerDefenseState = PS_Basic;
		else
			m_ePlayerDefenseState = PS_SuperArmour;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::speedymove_r)
	{
		if(m_pMeshCom->Check_EndPeriodMulTime(0.72f))
			m_ePlayerDefenseState = PS_Basic;
		else
			m_ePlayerDefenseState = PS_SuperArmour;
	}
	else
	{
		m_ePlayerDefenseState = PS_Basic;
	}
}

void CPlayer::Key_Input_Change_PlayerAnimationState(const float& fTimeDelta)
{
	//현재 실행중인 애니메이션의 인덱스 값과 키의 입력, 타이밍을 통해서 m_ePlayerAnimation의 상태를 바꿔준다.
	//바꾼 상태를 통해 AnimationPattern_Immediately_Change에서 조건에 따라 애니메이션을 Set해준다.
#pragma region 기본 공격
	if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_01_a)
	{
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::battle_attack_02, 0.0f, m_bOnceLbuttonCheck);
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_RBUTTON, Player::battle_attack_strong_01, 0.0f, m_bOnceRbuttonCheck);

		Current_Player_Animation_Type_Change_SpaceInput();
		//Current_Player_Animation_Type_Change_KeyInput(DIK_SPACE, Player::battle_attack_break_r);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_01_b)
	{
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::battle_attack_02, 0.0f, m_bOnceLbuttonCheck);
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_RBUTTON, Player::battle_attack_strong_01, 0.0f, m_bOnceRbuttonCheck);

		Current_Player_Animation_Type_Change_SpaceInput();
		//Current_Player_Animation_Type_Change_KeyInput(DIK_SPACE, Player::battle_attack_break_r);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_02)
	{
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::battle_attack_03, 0.0f, m_bOnceLbuttonCheck);
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_RBUTTON, Player::battle_attack_strong_02_a, 0.0f, m_bOnceRbuttonCheck);

		Current_Player_Animation_Type_Change_SpaceInput();
		//Current_Player_Animation_Type_Change_KeyInput(DIK_SPACE, Player::battle_attack_break_l);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_03)
	{
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::battle_attack_04, 0.2f, m_bOnceLbuttonCheck);
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_RBUTTON, Player::battle_attack_strong_03, 0.0f, m_bOnceRbuttonCheck);

		Current_Player_Animation_Type_Change_SpaceInput();
		//Current_Player_Animation_Type_Change_KeyInput(DIK_SPACE, Player::battle_attack_break_l);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_04)
	{
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::battle_attack_01_b, 0.2f, m_bOnceLbuttonCheck);
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_RBUTTON, Player::battle_attack_strong_04_a, 0.0f, m_bOnceRbuttonCheck);

		Current_Player_Animation_Type_Change_SpaceInput();
		//Current_Player_Animation_Type_Change_KeyInput(DIK_SPACE, Player::battle_attack_break_r);
	}
#pragma endregion
#pragma region 스매시 공격
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_00)
	{
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::battle_attack_01_a, 0.0f, m_bOnceLbuttonCheck);

		Current_Player_Animation_Type_Change_SpaceInput();
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_01)
	{
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::battle_attack_03, 0.1f, m_bOnceLbuttonCheck);

		Current_Player_Animation_Type_Change_SpaceInput();
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_02_a)
	{
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::battle_attack_01_b, 0.0f, m_bOnceLbuttonCheck);

		Current_Player_Animation_Type_Change_SpaceInput();
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_03)
	{
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::battle_attack_04, 0.2f, m_bOnceLbuttonCheck);
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::battle_attack_01_a, 0.5f, m_bOnceLbuttonCheck);

		Current_Player_Animation_Type_Change_SpaceInput();
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_a)
	{
		if(m_ePlayerAnimation == Player::battle_attack_strong_04_a)
			Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::battle_attack_02, 0.3f, m_bOnceLbuttonCheck);
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_RBUTTON, Player::battle_attack_strong_04_b, 0.15f, m_bOnceRbuttonCheck);

		Current_Player_Animation_Type_Change_SpaceInput();
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_b)
	{
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::battle_attack_01_a, 0.0f, m_bOnceLbuttonCheck);

		Current_Player_Animation_Type_Change_SpaceInput();
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_run_sprint)
	{
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::battle_attack_01_b, 0.0f, m_bOnceLbuttonCheck);

		Current_Player_Animation_Type_Change_SpaceInput();
	}
#pragma endregion
#pragma region 봉쇄, 달린후 봉쇄 , 돌격
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_break_l)
	{
		Current_Player_Animation_Type_Change_KeyInput(DIK_W, Player::battle_run_lower, 0.65f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_S, Player::battle_run_lower, 0.65f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_A, Player::battle_run_lower, 0.65f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_D, Player::battle_run_lower, 0.65f);

		BlockadeRush_MouseInput(); //봉쇄, 달리면서 봉쇄, 돌격후 평타 스매시 예외처리
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_break_r)
	{
		Current_Player_Animation_Type_Change_KeyInput(DIK_W, Player::battle_run_lower, 0.35f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_S, Player::battle_run_lower, 0.35f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_A, Player::battle_run_lower, 0.35f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_D, Player::battle_run_lower, 0.35f);

		BlockadeRush_MouseInput(); //봉쇄, 달리면서 봉쇄, 돌격후 평타 스매시 예외처리
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_break_speedymove_l)
	{
		Current_Player_Animation_Type_Change_KeyInput(DIK_W, Player::battle_run_lower, 0.65f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_S, Player::battle_run_lower, 0.65f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_A, Player::battle_run_lower, 0.65f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_D, Player::battle_run_lower, 0.65f);

		if(m_ePlayerAnimation == Player::battle_attack_break_speedymove_l)
			Current_Player_Animation_Type_Change_KeyInput(DIK_SPACE, Player::speedymove_l, m_bOnceSpaceKeyCheck);

		BlockadeRush_MouseInput(); //봉쇄, 달리면서 봉쇄, 돌격후 평타 스매시 예외처리
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_break_speedymove_r)
	{
		Current_Player_Animation_Type_Change_KeyInput(DIK_W, Player::battle_run_lower, 0.35f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_S, Player::battle_run_lower, 0.35f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_A, Player::battle_run_lower, 0.35f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_D, Player::battle_run_lower, 0.35f);

		if(m_ePlayerAnimation == Player::battle_attack_break_speedymove_r)
			Current_Player_Animation_Type_Change_KeyInput(DIK_SPACE, Player::speedymove_r, m_bOnceSpaceKeyCheck);

		BlockadeRush_MouseInput(); //봉쇄, 달리면서 봉쇄, 돌격후 평타 스매시 예외처리
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::speedymove_l)					
	{
		Current_Player_Animation_Type_Change_KeyInput(DIK_W, Player::battle_run_lower, 0.75f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_S, Player::battle_run_lower, 0.75f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_A, Player::battle_run_lower, 0.75f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_D, Player::battle_run_lower, 0.75f);

		BlockadeRush_MouseInput(); //봉쇄, 달리면서 봉쇄, 돌격후 평타 스매시 예외처리

		if(m_ePlayerAnimation == Player::speedymove_l)
			Current_Player_Animation_Type_Change_KeyInput(DIK_SPACE, Player::battle_attack_break_speedymove_l, m_bOnceSpaceKeyCheck);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::speedymove_r)					
	{
		Current_Player_Animation_Type_Change_KeyInput(DIK_W, Player::battle_run_lower, 0.38f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_S, Player::battle_run_lower, 0.38f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_A, Player::battle_run_lower, 0.38f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_D, Player::battle_run_lower, 0.38f);

		BlockadeRush_MouseInput(); //봉쇄, 달리면서 봉쇄, 돌격후 평타 스매시 예외처리

		if(m_ePlayerAnimation == Player::speedymove_r)
			Current_Player_Animation_Type_Change_KeyInput(DIK_SPACE, Player::battle_attack_break_speedymove_r, m_bOnceSpaceKeyCheck);
	}
#pragma endregion
#pragma region 공격대기, 공격대기 to 쉬기, 쉬기, 빨리 달리다가 멈추기, 천천히 달리기, 빨리 달리기
	else  if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_idle_lower || m_pMeshCom->Get_CurrentAniIdx() == Player::battle_idle2rest
		|| m_pMeshCom->Get_CurrentAniIdx() == Player::battle_rest || m_pMeshCom->Get_CurrentAniIdx() == Player::battle_run_stop)	//공격대기, 공격대기to쉬기, 쉬기, 빨리 달리다가 멈추기
	{
		Current_Player_Animation_Type_Change_KeyInput(DIK_W, Player::battle_run_lower);
		Current_Player_Animation_Type_Change_KeyInput(DIK_S, Player::battle_run_lower);
		Current_Player_Animation_Type_Change_KeyInput(DIK_A, Player::battle_run_lower);
		Current_Player_Animation_Type_Change_KeyInput(DIK_D, Player::battle_run_lower);

		Current_Player_Animation_Type_Change_KeyInput(DIK_SPACE, Player::battle_attack_break_l);

		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::battle_attack_01_a, 0.f, m_bOnceLbuttonCheck);
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_RBUTTON, Player::battle_attack_strong_00, 0.f, m_bOnceRbuttonCheck);

		if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_idle_lower)
		{
			if(m_fIdleToRestTime > 10.f)
			{
				m_ePlayerAnimation = Player::battle_idle2rest;
				m_fIdleToRestTime = 0.f;
			}
		}
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_run_lower)	//천천히 달리기
	{
		if( !((Engine::GetDIKeyState(DIK_W) & 0x80) || (Engine::GetDIKeyState(DIK_S) & 0x80)
			|| (Engine::GetDIKeyState(DIK_A) & 0x80) || (Engine::GetDIKeyState(DIK_D) & 0x80)) )
			m_ePlayerAnimation = Player::battle_idle_lower;

		Current_Player_Animation_Type_Change_KeyInput(DIK_LSHIFT, Player::battle_run_sprint);

		Current_Player_Animation_Type_Change_KeyInput(DIK_SPACE, Player::speedymove_l);

		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::battle_attack_01_a, 0.f, m_bOnceLbuttonCheck);
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_RBUTTON, Player::battle_attack_strong_00, 0.f, m_bOnceRbuttonCheck);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_run_sprint)	//빨리 달리기
	{
		if( !(((Engine::GetDIKeyState(DIK_W) & 0x80) || (Engine::GetDIKeyState(DIK_S) & 0x80)
			|| (Engine::GetDIKeyState(DIK_A) & 0x80) || (Engine::GetDIKeyState(DIK_D) & 0x80))
			&& Engine::GetDIKeyState(DIK_LSHIFT)) )
			m_ePlayerAnimation = Player::battle_run_stop;

		Current_Player_Animation_Type_Change_KeyInput(DIK_SPACE, Player::speedymove_l);

		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::battle_attack_01_a, 0.f, m_bOnceLbuttonCheck);
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_RBUTTON, Player::battle_attack_strong_run_sprint, 0.f, m_bOnceRbuttonCheck);
	}
#pragma endregion
#pragma region 스킬들
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_charge_slash_during_l)
	{
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::SP_attack_charge_slash_up_l, 0.f, m_bOnceLbuttonCheck);
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_RBUTTON, Player::SP_attack_charge_slash_up_l, 0.f, m_bOnceRbuttonCheck);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_charge_slash_during_r)
	{
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_LBUTTON, Player::SP_attack_charge_slash_up_r, 0.f, m_bOnceLbuttonCheck);
		Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::DIM_RBUTTON, Player::SP_attack_charge_slash_up_r, 0.f, m_bOnceRbuttonCheck);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_judgement_l || m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_judgement_r)
	{
		Judgement_MouseInput();
	}
#pragma endregion
#pragma region 피격 관련
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::damage_strong_back_begin)
	{
		Current_Player_Animation_Type_Change_KeyInput(DIK_W, Player::down_back_roll_front, 0.6f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_S, Player::down_back_roll_front, 0.6f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_A, Player::down_back_roll_front, 0.6f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_D, Player::down_back_roll_front, 0.6f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::damage_strong_front_begin)
	{
		Current_Player_Animation_Type_Change_KeyInput(DIK_W, Player::down_front_roll_front, 0.65f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_S, Player::down_front_roll_front, 0.65f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_A, Player::down_front_roll_front, 0.65f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_D, Player::down_front_roll_front, 0.65f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::down_resist)
	{
		Current_Player_Animation_Type_Change_KeyInput(DIK_W, Player::battle_run_lower, 0.55f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_S, Player::battle_run_lower, 0.55f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_A, Player::battle_run_lower, 0.55f);
		Current_Player_Animation_Type_Change_KeyInput(DIK_D, Player::battle_run_lower, 0.55f);
	}
#pragma endregion

#pragma region 키 입력 예외처리
	//키, 마우스 한번만 입력되기 위한 구문들
	if(Engine::GetDIKeyState(DIK_SPACE) & 0x80)
	{
		m_bOnceSpaceKeyCheck = true;
	}
	else
	{
		m_bOnceSpaceKeyCheck = false;
	}

	if(Engine::GetDIMouseState(Engine::CInput::DIM_LBUTTON) & 0x80)
	{
		m_bOnceLbuttonCheck = true;
	}
	else
	{
		m_bOnceLbuttonCheck = false;
	}

	if(Engine::GetDIMouseState(Engine::CInput::DIM_RBUTTON) & 0x80)
	{
		m_bOnceRbuttonCheck = true;
	}
	else
	{
		m_bOnceRbuttonCheck = false;
	}
#pragma endregion
}

void CPlayer::AnimationPattern_Immediately_Change(void)
{
#pragma region 반복되는 동작
	if(m_ePlayerAnimation == Player::battle_idle2rest)						//공격 대기 to 휴식
	{
		if(m_pMeshCom->Get_CurrentAniIdx() != m_ePlayerAnimation)
		{
			m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
		}
		else if(m_pMeshCom->Check_EndPeriod())
		{
			m_ePlayerAnimation = Player::battle_rest;
			m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
		}
	}
	else if(m_ePlayerAnimation == Player::battle_rest)						//휴식 상태
	{
		if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_rest)			//반복
			m_pMeshCom->Set_AnimationSetSameFinish(m_ePlayerAnimation);
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_idle2rest)//다른 애니메이션 재생중 일때
			m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
	}
	else if(m_ePlayerAnimation == Player::battle_run_lower)					//천천히 달리기
	{
		BasicAttackOrder = BA_End;
		SmashAttackOrder = SA_End;

		Key_Input_Rotation();

		if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_run_lower)		//반복
			m_pMeshCom->Set_AnimationSetSameFinish(m_ePlayerAnimation);
		else																//다른 애니메이션 재생중 battle_run_lower로 Set
			m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
	}
	else if(m_ePlayerAnimation == Player::battle_run_sprint)				//빨리 달리기
	{
		BasicAttackOrder = BA_End;
		SmashAttackOrder = SA_End;

		Key_Input_Rotation();
		
		if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_run_sprint)	//반복
		{
				m_pMeshCom->Set_AnimationSetSameFinish(m_ePlayerAnimation);
		}
		else																//다른 애니메이션 재생중 battle_run_sprint로 Set
			m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
	}
#pragma endregion
#pragma region 스킬, 피격, 모든 동작 끝났을때 예외처리
	else if((m_ePlayerAnimation == Player::SP_attack_charge_slash_begin_l || m_ePlayerAnimation == Player::SP_attack_charge_slash_during_l) && m_pMeshCom->Check_EndPeriod())
	{
		++m_iChargeSlashNumber;
		m_ePlayerAnimation = Player::SP_attack_charge_slash_during_l;
		m_pMeshCom->Set_AnimationSetTwo(m_ePlayerAnimation);

		if(m_iChargeSlashNumber > 3)
		{
			m_iChargeSlashNumber = 0;
			m_ePlayerAnimation = Player::battle_idle_lower;
		}
	}
	else if((m_ePlayerAnimation == Player::SP_attack_charge_slash_begin_r || m_ePlayerAnimation == Player::SP_attack_charge_slash_during_r) && m_pMeshCom->Check_EndPeriod())
	{
		++m_iChargeSlashNumber;
		m_ePlayerAnimation = Player::SP_attack_charge_slash_during_r;
		m_pMeshCom->Set_AnimationSetTwo(m_ePlayerAnimation);

		if(m_iChargeSlashNumber > 3)
		{
			m_iChargeSlashNumber = 0;
			m_ePlayerAnimation = Player::battle_idle_lower;
		}
	}
	else if(m_ePlayerAnimation == Player::damage_middle_back_begin && m_pMeshCom->Check_EndPeriod())
	{
		m_ePlayerAnimation = Player::damage_middle_back_end;
		m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
	}
	else if(m_ePlayerAnimation == Player::damage_middle_front_begin && m_pMeshCom->Check_EndPeriod())
	{
		m_ePlayerAnimation = Player::damage_middle_front_end;
		m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
	}
	else if(m_ePlayerAnimation == Player::damage_strong_back_begin && m_pMeshCom->Check_EndPeriod())
	{
		m_ePlayerAnimation = Player::down_front2idle;
		m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
	}
	else if(m_ePlayerAnimation == Player::damage_strong_front_begin && m_pMeshCom->Check_EndPeriod())
	{
		m_ePlayerAnimation = Player::down_back2idle;
		m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
	}
	else if(m_ePlayerAnimation == Player::battle_idle_lower || m_pMeshCom->Check_EndPeriod()) //천천히, 빨리 달리기 같은 애니메이션이 아닌면 동작이 끝났을때 공격 대기로 돌아가도록 한다.
	{
		BasicAttackOrder = BA_End;
		SmashAttackOrder = SA_End;

		if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_idle_lower) //동작 끝나면 공격대기로 전환
		{
			m_fIdleToRestTime += m_fRenderTimeDelta;	//공격 대기에 있던 시간을 계산한다.
			m_pMeshCom->Set_AnimationSetSameFinish(m_ePlayerAnimation);
		}
		else	//키를 안눌렸을때 바로 공격 대기로 들어가는 동작들 예외처리
		{
			m_ePlayerAnimation = Player::battle_idle_lower;
			m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
		}
	}
	else if(m_ePlayerAnimation == Player::SP_attack_charge_slash_up_l)
	{
		Current_Animation_Change_Direction(Player::SP_attack_charge_slash_during_l);
	}
	else if(m_ePlayerAnimation == Player::SP_attack_charge_slash_up_r)
	{
		Current_Animation_Change_Direction(Player::SP_attack_charge_slash_during_r);
	}
	else if(m_ePlayerAnimation == Player::down_back_roll_front)
	{
		Current_Animation_Change_Direction(Player::damage_strong_back_begin);
	}
	else if(m_ePlayerAnimation == Player::down_front_roll_front)
	{
		Current_Animation_Change_Direction(Player::damage_strong_front_begin);
	}
#pragma endregion
#pragma region 평타
	else if(m_ePlayerAnimation == Player::battle_attack_01_a)	//평타 1
	{
		BasicAttackOrder = BA_1a;
		SmashAttackOrder = SA_End;

		Current_Animation_Change_Direction(Player::battle_idle_lower);	//공격대기
		Current_Animation_Change_Direction(Player::battle_idle2rest);	//공격대기에서 쉬기로
		Current_Animation_Change_Direction(Player::battle_rest);		//쉬기
		Current_Animation_Change_Direction(Player::battle_run_lower);	//천천히 달리기
		Current_Animation_Change_Direction(Player::battle_run_sprint);	//빨리 달리기
		Current_Animation_Change_Direction(Player::battle_run_stop);	//빨리 달리다가 멈추기

		Current_Animation_Change_Direction(Player::battle_attack_strong_04_b, 0.51f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_03, 0.51f);

		Current_Animation_Change_Direction(Player::battle_attack_break_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_r, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_r, 0.3f);
		Current_Animation_Change_Direction(Player::speedymove_l, 0.35f);
		Current_Animation_Change_Direction(Player::speedymove_r, 0.35f);

		Current_Animation_Change_Direction(Player::SP_attack_judgement_l, 0.35f);
		Current_Animation_Change_Direction(Player::SP_attack_judgement_r, 0.35f);
	}
	else if(m_ePlayerAnimation == Player::battle_attack_01_b)	//2번째 스매시 처형 이후에 사용되는 평타 1
	{
		BasicAttackOrder = BA_1b;
		SmashAttackOrder = SA_End;

		Current_Animation_Change_Direction(Player::battle_attack_04, 0.42f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_02_a, 0.4f);

		Current_Animation_Change_Direction(Player::battle_attack_break_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_r, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_r, 0.3f);
		Current_Animation_Change_Direction(Player::speedymove_l, 0.35f);
		Current_Animation_Change_Direction(Player::speedymove_r, 0.35f);

		Current_Animation_Change_Direction(Player::SP_attack_judgement_l, 0.35f);
		Current_Animation_Change_Direction(Player::SP_attack_judgement_r, 0.35f);
	}
	else if(m_ePlayerAnimation == Player::battle_attack_02)		//평타 2
	{
		BasicAttackOrder = BA_2;
		SmashAttackOrder = SA_End;

		Current_Animation_Change_Direction(Player::battle_attack_01_a, 0.285f);	
		Current_Animation_Change_Direction(Player::battle_attack_01_b, 0.285f);	//싱크 맞쳐야함
		Current_Animation_Change_Direction(Player::battle_attack_strong_04_a, 0.4f);

		Current_Animation_Change_Direction(Player::battle_attack_break_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_r, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_r, 0.3f);
		Current_Animation_Change_Direction(Player::speedymove_l, 0.35f);
		Current_Animation_Change_Direction(Player::speedymove_r, 0.35f);

		Current_Animation_Change_Direction(Player::SP_attack_judgement_l, 0.35f);
		Current_Animation_Change_Direction(Player::SP_attack_judgement_r, 0.35f);
	}
	else if(m_ePlayerAnimation == Player::battle_attack_03)		//평타 3
	{
		BasicAttackOrder = BA_3;
		SmashAttackOrder = SA_End;

		Current_Animation_Change_Direction(Player::battle_attack_02, 0.49f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_01, 0.49f);

		Current_Animation_Change_Direction(Player::battle_attack_break_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_r, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_r, 0.3f);
		Current_Animation_Change_Direction(Player::speedymove_l, 0.35f);
		Current_Animation_Change_Direction(Player::speedymove_r, 0.35f);

		Current_Animation_Change_Direction(Player::SP_attack_judgement_l, 0.35f);
		Current_Animation_Change_Direction(Player::SP_attack_judgement_r, 0.35f);
	}
	else if(m_ePlayerAnimation == Player::battle_attack_04)		//평타 4
	{
		BasicAttackOrder = BA_4;
		SmashAttackOrder = SA_End;

		Current_Animation_Change_Direction(Player::battle_attack_03, 0.35f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_03, 0.49f);

		Current_Animation_Change_Direction(Player::battle_attack_break_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_r, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_r, 0.3f);
		Current_Animation_Change_Direction(Player::speedymove_l, 0.35f);
		Current_Animation_Change_Direction(Player::speedymove_r, 0.35f);

		Current_Animation_Change_Direction(Player::SP_attack_judgement_l, 0.35f);
		Current_Animation_Change_Direction(Player::SP_attack_judgement_r, 0.35f);
	}
#pragma endregion
#pragma region 스매시
	else if(m_ePlayerAnimation == Player::battle_attack_strong_00)
	{
		BasicAttackOrder = BA_End;
		SmashAttackOrder = SA_0;

		Current_Animation_Change_Direction(Player::battle_idle_lower);	//공격대기
		Current_Animation_Change_Direction(Player::battle_idle2rest);	//공격대기에서 쉬기로
		Current_Animation_Change_Direction(Player::battle_rest);		//쉬기
		Current_Animation_Change_Direction(Player::battle_run_lower);	//천천히 달리기
		Current_Animation_Change_Direction(Player::battle_run_stop);	//빨리 달리다가 멈추기

		Current_Animation_Change_Direction(Player::SP_attack_judgement_l, 0.35f);
		Current_Animation_Change_Direction(Player::SP_attack_judgement_r, 0.35f);
	}
	else if(m_ePlayerAnimation == Player::battle_attack_strong_01)
	{
		BasicAttackOrder = BA_End;
		SmashAttackOrder = SA_1;	

		Current_Animation_Change_Direction(Player::battle_attack_01_a, 0.285f);
		Current_Animation_Change_Direction(Player::battle_attack_01_b, 0.285f); //싱크 맞쳐야함

		Current_Animation_Change_Direction(Player::battle_attack_break_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_r, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_r, 0.3f);
		Current_Animation_Change_Direction(Player::speedymove_l, 0.35f);
		Current_Animation_Change_Direction(Player::speedymove_r, 0.35f);

		Current_Animation_Change_Direction(Player::SP_attack_judgement_l, 0.35f);
		Current_Animation_Change_Direction(Player::SP_attack_judgement_r, 0.35f);
	}
	else if(m_ePlayerAnimation == Player::battle_attack_strong_02_a)
	{
		BasicAttackOrder = BA_End;
		SmashAttackOrder = SA_2;

		Current_Animation_Change_Direction(Player::battle_attack_02, 0.47f);

		Current_Animation_Change_Direction(Player::battle_attack_break_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_r, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_r, 0.3f);
		Current_Animation_Change_Direction(Player::speedymove_l, 0.35f);
		Current_Animation_Change_Direction(Player::speedymove_r, 0.35f);

		Current_Animation_Change_Direction(Player::SP_attack_judgement_l, 0.35f);
		Current_Animation_Change_Direction(Player::SP_attack_judgement_r, 0.35f);
	}
	else if(m_ePlayerAnimation == Player::battle_attack_strong_03)
	{
		BasicAttackOrder = BA_End;
		SmashAttackOrder = SA_3;

		Current_Animation_Change_Direction(Player::battle_attack_03, 0.35f);

		Current_Animation_Change_Direction(Player::battle_attack_break_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_r, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_r, 0.3f);
		Current_Animation_Change_Direction(Player::speedymove_l, 0.35f);
		Current_Animation_Change_Direction(Player::speedymove_r, 0.35f);

		Current_Animation_Change_Direction(Player::SP_attack_judgement_l, 0.35f);
		Current_Animation_Change_Direction(Player::SP_attack_judgement_r, 0.35f);
	}
	else if(m_ePlayerAnimation == Player::battle_attack_strong_04_a)
	{
		BasicAttackOrder = BA_End;
		SmashAttackOrder = SA_4a;

		Current_Animation_Change_Direction(Player::battle_attack_04, 0.4f);

		Current_Animation_Change_Direction(Player::battle_attack_break_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_r, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_r, 0.3f);
		Current_Animation_Change_Direction(Player::speedymove_l, 0.35f);
		Current_Animation_Change_Direction(Player::speedymove_r, 0.35f);

		Current_Animation_Change_Direction(Player::SP_attack_judgement_l, 0.35f);
		Current_Animation_Change_Direction(Player::SP_attack_judgement_r, 0.35f);
	}
	else if(m_ePlayerAnimation == Player::battle_attack_strong_04_b)
	{
		BasicAttackOrder = BA_End;
		SmashAttackOrder = SA_4b;

		Current_Animation_Change_Direction(Player::battle_attack_strong_04_a, 0.345f);

		Current_Animation_Change_Direction(Player::battle_attack_break_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_r, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_l, 0.3f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_r, 0.3f);
		Current_Animation_Change_Direction(Player::speedymove_l, 0.35f);
		Current_Animation_Change_Direction(Player::speedymove_r, 0.35f);

		Current_Animation_Change_Direction(Player::SP_attack_judgement_l, 0.35f);
		Current_Animation_Change_Direction(Player::SP_attack_judgement_r, 0.35f);
	}
	else if(m_ePlayerAnimation == Player::battle_attack_strong_run_sprint)
	{
		BasicAttackOrder = BA_End;
		SmashAttackOrder = SA_2;

		Current_Animation_Change_Direction(Player::battle_run_sprint);	//빨리 달리기
	}
#pragma endregion
#pragma region 봉쇄, 달리다가 봉쇄, 돌진
	else if(m_ePlayerAnimation == Player::battle_attack_break_l || m_ePlayerAnimation == Player::battle_attack_break_r)	//봉쇄(왼쪽, 오른쪽 => 발 위치 기준으로)
	{
		Current_Animation_Change_Direction(Player::battle_idle_lower);	//공격대기
		Current_Animation_Change_Direction(Player::battle_idle2rest);	//공격대기에서 쉬기
		Current_Animation_Change_Direction(Player::battle_rest);		//쉬기
		Current_Animation_Change_Direction(Player::battle_run_stop);	//빨리 달리다가 멈추기

		Current_Animation_Change_Direction(Player::battle_attack_01_a);
		Current_Animation_Change_Direction(Player::battle_attack_01_b);
		Current_Animation_Change_Direction(Player::battle_attack_02);
		Current_Animation_Change_Direction(Player::battle_attack_03);
		Current_Animation_Change_Direction(Player::battle_attack_04);

		Current_Animation_Change_Direction(Player::battle_attack_strong_00, 0.35f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_01, 0.44f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_02_a, 0.35f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_03, 0.45f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_04_a, 0.385f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_04_b, 0.4f);
	}
	else if(m_ePlayerAnimation == Player::battle_attack_break_speedymove_l || m_ePlayerAnimation == Player::battle_attack_break_speedymove_r)	//달린후 봉쇄(왼쪽, 오른쪽 => 발 위치 기준으로)
	{
		Current_Animation_Change_Direction(Player::battle_attack_01_a);
		Current_Animation_Change_Direction(Player::battle_attack_01_b);
		Current_Animation_Change_Direction(Player::battle_attack_02);
		Current_Animation_Change_Direction(Player::battle_attack_03);
		Current_Animation_Change_Direction(Player::battle_attack_04);

		Current_Animation_Change_Direction(Player::battle_attack_strong_00, 0.35f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_01, 0.44f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_02_a, 0.35f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_03, 0.45f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_04_a, 0.385f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_04_b, 0.4f);

		Current_Animation_Change_Direction(Player::speedymove_l, 0.35f);
		Current_Animation_Change_Direction(Player::speedymove_r, 0.35f);	
	}
	else if(m_ePlayerAnimation == Player::battle_run_stop)					//달리는 도중 
	{
		Current_Animation_Change_Direction(Player::battle_run_sprint);
	}
	else if(m_ePlayerAnimation == Player::speedymove_l || m_ePlayerAnimation == Player::speedymove_r)
	{
		Current_Animation_Change_Direction(Player::battle_run_lower);	//천천히 달리기
		Current_Animation_Change_Direction(Player::battle_run_sprint);	//빨리 달리기

		Current_Animation_Change_Direction(Player::battle_attack_01_a);
		Current_Animation_Change_Direction(Player::battle_attack_01_b);
		Current_Animation_Change_Direction(Player::battle_attack_02);
		Current_Animation_Change_Direction(Player::battle_attack_03);
		Current_Animation_Change_Direction(Player::battle_attack_04);

		Current_Animation_Change_Direction(Player::battle_attack_strong_00, 0.35f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_01, 0.44f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_02_a, 0.35f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_03, 0.45f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_04_a, 0.385f);
		Current_Animation_Change_Direction(Player::battle_attack_strong_04_b, 0.4f);

		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_l, 0.4f);
		Current_Animation_Change_Direction(Player::battle_attack_break_speedymove_r, 0.4f);
	}
#pragma endregion
}

void CPlayer::Current_Animation_Change_Direction(Player::PLAYERANIMATION ePlayAnimationType, _float fCheckEndPeriodMulTime /*= 0.f*/)
{
	if(m_pMeshCom->Get_CurrentAniIdx() == ePlayAnimationType && m_pMeshCom->Check_EndPeriodMulTime(fCheckEndPeriodMulTime))
	{
		Key_Input_Immediately_Rotation();
		m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
	}
}

void CPlayer::Key_Input_Rotation()
{
	if( (Engine::GetDIKeyState(DIK_W) & 0x80) && (Engine::GetDIKeyState(DIK_A) & 0x80) )		//왼쪽 앞
	{
		Change_AngleY(m_fAngleFixed[AF_LeftFront]);
	}
	else if( (Engine::GetDIKeyState(DIK_W) & 0x80) && (Engine::GetDIKeyState(DIK_D) & 0x80) )	//오른쪽 앞
	{
		Change_AngleY(m_fAngleFixed[AF_RightFront]);	
	}
	else if( (Engine::GetDIKeyState(DIK_S) & 0x80) && (Engine::GetDIKeyState(DIK_A) & 0x80) )	//왼쪽 뒤
	{
		Change_AngleY(m_fAngleFixed[AF_LeftBack]);		
	}
	else if( (Engine::GetDIKeyState(DIK_S) & 0x80) && (Engine::GetDIKeyState(DIK_D) & 0x80) )	//오른쪽 뒤
	{
		Change_AngleY(m_fAngleFixed[AF_RightBack]);
	}
	else if(Engine::GetDIKeyState(DIK_W) & 0x80)	//앞
	{
		Change_AngleY(m_fAngleFixed[AF_Front]);
	}
	else if(Engine::GetDIKeyState(DIK_S) & 0x80)	//뒤
	{
		Change_AngleY(m_fAngleFixed[AF_Back]);
	}
	else if(Engine::GetDIKeyState(DIK_A) & 0x80)	//왼쪽
	{
		Change_AngleY(m_fAngleFixed[AF_Left]);
	}
	else if(Engine::GetDIKeyState(DIK_D) & 0x80)	//오른쪽
	{
		Change_AngleY(m_fAngleFixed[AF_Right]);
	}

	if(m_fOriginalAngleY > 360)
	{
		m_fOriginalAngleY = 0;
	}
	else if(m_fOriginalAngleY < 0)
	{
		m_fOriginalAngleY = 360;
	}

	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian(m_fOriginalAngleY);
}

void CPlayer::Change_AngleY( _float fAngleFixed)
{
#pragma region 옛날 소스
	/*
	_float fCameraAngle = CMathMgr::GetInstance()->ReturnDegreeAngle(CDynamicCamera::GetInstance()->GetCurrentDegreeAngleY()); 
	//플레이어의 목표 각도
	_float fPlayerTargetAngleY = CMathMgr::GetInstance()->ReturnDegreeAngle(fCameraAngle + fAngleFixed);

	if(fPlayerTargetAngleY - 2.5f < m_fOriginalAngleY &&  m_fOriginalAngleY < fPlayerTargetAngleY + 2.5f)
	{
		m_fOriginalAngleY = fPlayerTargetAngleY;
	}
	else
	{
		_float fDirAngle = CMathMgr::GetInstance()->ReturnDegreeAngle(m_fOriginalAngleY + 180);	
		_float fAngleSpeed = 12.f;

		if(m_fOriginalAngleY > 180) //실제 각도가 180이 넘을때와 아닐때 다른방법으로 예외처리를 한다.
		{
			if( (m_fOriginalAngleY < fPlayerTargetAngleY && fPlayerTargetAngleY < 360)
				|| ( 0 < fPlayerTargetAngleY && fPlayerTargetAngleY < fDirAngle) )
			{
				if( abs (m_fOriginalAngleY - fPlayerTargetAngleY) > 180)
				{
					m_fOriginalAngleY += max(abs(CMathMgr::GetInstance()->ReturnDegreeAngle(m_fOriginalAngleY + 180) - CMathMgr::GetInstance()->ReturnDegreeAngle(fPlayerTargetAngleY + 180)), 15) * fAngleSpeed * m_fTimeDelta;
				}
				else
				{
					m_fOriginalAngleY += max(abs(m_fOriginalAngleY - fPlayerTargetAngleY), 15) * fAngleSpeed * m_fTimeDelta;
				}
			}
			else
			{
				if( abs (m_fOriginalAngleY - fPlayerTargetAngleY) > 180)
				{
					m_fOriginalAngleY -= max(abs(CMathMgr::GetInstance()->ReturnDegreeAngle(m_fOriginalAngleY + 180) - CMathMgr::GetInstance()->ReturnDegreeAngle(fPlayerTargetAngleY + 180)), 15) * fAngleSpeed * m_fTimeDelta;
				}
				else
				{
					m_fOriginalAngleY -= max(abs(m_fOriginalAngleY - fPlayerTargetAngleY), 15) * fAngleSpeed * m_fTimeDelta;
				}
			}
		}
		else
		{
			if( (m_fOriginalAngleY < fPlayerTargetAngleY && fPlayerTargetAngleY < fDirAngle) )
			{
				if( abs (m_fOriginalAngleY - fPlayerTargetAngleY) > 180)
				{
					m_fOriginalAngleY += max(abs(CMathMgr::GetInstance()->ReturnDegreeAngle(m_fOriginalAngleY + 180) - CMathMgr::GetInstance()->ReturnDegreeAngle(fPlayerTargetAngleY + 180)), 15) * fAngleSpeed * m_fTimeDelta;
				}
				else
				{
					m_fOriginalAngleY += max(abs(m_fOriginalAngleY - fPlayerTargetAngleY), 15) * fAngleSpeed * m_fTimeDelta;
				}
			}
			else
			{
				if( abs (m_fOriginalAngleY - fPlayerTargetAngleY) > 180)
				{
					m_fOriginalAngleY -= max(abs(CMathMgr::GetInstance()->ReturnDegreeAngle(m_fOriginalAngleY + 180) - CMathMgr::GetInstance()->ReturnDegreeAngle(fPlayerTargetAngleY + 180)), 15) * fAngleSpeed * m_fTimeDelta;
				}
				else
				{
					m_fOriginalAngleY -= max(abs(m_fOriginalAngleY - fPlayerTargetAngleY), 15) * fAngleSpeed * m_fTimeDelta;
				}
			}
		}
	}
	*/
#pragma endregion

	_vec3 vDir = m_pTransCom->m_vPosition - CDynamicCamera::GetInstance()->GetEye();
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3 vLook(0.f, 0.f, 1.f);
	_vec3 vRight(1.f, 0.f, 0.f);

	float fRadian_Look		= acosf( D3DXVec3Dot(&vLook, &vDir) );
	_float fDegree_Look		= D3DXToDegree(fRadian_Look);				

	_float fRadian_Right	= acosf( D3DXVec3Dot(&vRight, &vDir) );
	_float fDegree_Right	= D3DXToDegree(fRadian_Right);				

	_float fDegree = 0.f;
	if(fDegree_Right > 90)
	{
		fDegree = 360.f - fDegree_Look;
	}
	else
	{
		fDegree = fDegree_Look;
	}

	_float fPlayerTargetAngleY = CMathMgr::GetInstance()->ReturnDegreeAngle(fDegree + fAngleFixed);	

	if(fPlayerTargetAngleY - 2.5f < m_fOriginalAngleY &&  m_fOriginalAngleY < fPlayerTargetAngleY + 2.5f)
	{
		m_fOriginalAngleY = fPlayerTargetAngleY;
	}
	else
	{
		_float fDirAngle = CMathMgr::GetInstance()->ReturnDegreeAngle(m_fOriginalAngleY + 180);	
		_float fAngleSpeed = 12.f;

		if(m_fOriginalAngleY > 180) //실제 각도가 180이 넘을때와 아닐때 다른방법으로 예외처리를 한다.
		{
			if( (m_fOriginalAngleY < fPlayerTargetAngleY && fPlayerTargetAngleY < 360)
				|| ( 0 < fPlayerTargetAngleY && fPlayerTargetAngleY < fDirAngle) )
			{
				if( abs (m_fOriginalAngleY - fPlayerTargetAngleY) > 180)
				{
					m_fOriginalAngleY += max(abs(CMathMgr::GetInstance()->ReturnDegreeAngle(m_fOriginalAngleY + 180) - CMathMgr::GetInstance()->ReturnDegreeAngle(fPlayerTargetAngleY + 180)), 15) * fAngleSpeed * m_fTimeDelta;
				}
				else
				{
					m_fOriginalAngleY += max(abs(m_fOriginalAngleY - fPlayerTargetAngleY), 15) * fAngleSpeed * m_fTimeDelta;
				}
			}
			else
			{
				if( abs (m_fOriginalAngleY - fPlayerTargetAngleY) > 180)
				{
					m_fOriginalAngleY -= max(abs(CMathMgr::GetInstance()->ReturnDegreeAngle(m_fOriginalAngleY + 180) - CMathMgr::GetInstance()->ReturnDegreeAngle(fPlayerTargetAngleY + 180)), 15) * fAngleSpeed * m_fTimeDelta;
				}
				else
				{
					m_fOriginalAngleY -= max(abs(m_fOriginalAngleY - fPlayerTargetAngleY), 15) * fAngleSpeed * m_fTimeDelta;
				}
			}
		}
		else
		{
			if( (m_fOriginalAngleY < fPlayerTargetAngleY && fPlayerTargetAngleY < fDirAngle) )
			{
				if( abs (m_fOriginalAngleY - fPlayerTargetAngleY) > 180)
				{
					m_fOriginalAngleY += max(abs(CMathMgr::GetInstance()->ReturnDegreeAngle(m_fOriginalAngleY + 180) - CMathMgr::GetInstance()->ReturnDegreeAngle(fPlayerTargetAngleY + 180)), 15) * fAngleSpeed * m_fTimeDelta;
				}
				else
				{
					m_fOriginalAngleY += max(abs(m_fOriginalAngleY - fPlayerTargetAngleY), 15) * fAngleSpeed * m_fTimeDelta;
				}
			}
			else
			{
				if( abs (m_fOriginalAngleY - fPlayerTargetAngleY) > 180)
				{
					m_fOriginalAngleY -= max(abs(CMathMgr::GetInstance()->ReturnDegreeAngle(m_fOriginalAngleY + 180) - CMathMgr::GetInstance()->ReturnDegreeAngle(fPlayerTargetAngleY + 180)), 15) * fAngleSpeed * m_fTimeDelta;
				}
				else
				{
					m_fOriginalAngleY -= max(abs(m_fOriginalAngleY - fPlayerTargetAngleY), 15) * fAngleSpeed * m_fTimeDelta;
				}
			}
		}
	}
}

void CPlayer::Key_Input_Immediately_Rotation(void)
{
	if( (Engine::GetDIKeyState(DIK_W) & 0x80) && (Engine::GetDIKeyState(DIK_A) & 0x80) )		//왼쪽 앞
	{
		Change_AngleY_Immediately(m_fAngleFixed[AF_LeftFront]);
	}
	else if( (Engine::GetDIKeyState(DIK_W) & 0x80) && (Engine::GetDIKeyState(DIK_D) & 0x80) )	//오른쪽 앞
	{
		Change_AngleY_Immediately(m_fAngleFixed[AF_RightFront]);	
	}
	else if( (Engine::GetDIKeyState(DIK_S) & 0x80) && (Engine::GetDIKeyState(DIK_A) & 0x80) )	//왼쪽 뒤
	{
		Change_AngleY_Immediately(m_fAngleFixed[AF_LeftBack]);		
	}
	else if( (Engine::GetDIKeyState(DIK_S) & 0x80) && (Engine::GetDIKeyState(DIK_D) & 0x80) )	//오른쪽 뒤
	{
		Change_AngleY_Immediately(m_fAngleFixed[AF_RightBack]);
	}
	else if(Engine::GetDIKeyState(DIK_W) & 0x80)	//앞
	{
		Change_AngleY_Immediately(m_fAngleFixed[AF_Front]);
	}
	else if(Engine::GetDIKeyState(DIK_S) & 0x80)	//뒤
	{
		Change_AngleY_Immediately(m_fAngleFixed[AF_Back]);
	}
	else if(Engine::GetDIKeyState(DIK_A) & 0x80)	//왼쪽
	{
		Change_AngleY_Immediately(m_fAngleFixed[AF_Left]);
	}
	else if(Engine::GetDIKeyState(DIK_D) & 0x80)	//오른쪽
	{
		Change_AngleY_Immediately(m_fAngleFixed[AF_Right]);
	}

	if(m_fOriginalAngleY > 360)
	{
		m_fOriginalAngleY = 0;
	}
	else if(m_fOriginalAngleY < 0)
	{
		m_fOriginalAngleY = 360;
	}

	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian(m_fOriginalAngleY);
}

void CPlayer::Change_AngleY_Immediately( _float fAngleFixed)
{
	/*
	_float fCameraAngle = CMathMgr::GetInstance()->ReturnDegreeAngle(CDynamicCamera::GetInstance()->GetCurrentDegreeAngleY()); 
	//플레이어의 목표 각도
	_float fPlayerTargetAngleY = CMathMgr::GetInstance()->ReturnDegreeAngle(fCameraAngle + fAngleFixed);

	m_fOriginalAngleY = fPlayerTargetAngleY;
	*/

	_vec3 vDir = m_pTransCom->m_vPosition - CDynamicCamera::GetInstance()->GetEye();
	vDir.y = 0.f;
	D3DXVec3Normalize(&vDir, &vDir);

	_vec3 vLook(0.f, 0.f, 1.f);
	_vec3 vRight(1.f, 0.f, 0.f);

	float fRadian_Look		= acosf( D3DXVec3Dot(&vLook, &vDir) );
	_float fDegree_Look		= D3DXToDegree(fRadian_Look);				

	_float fRadian_Right	= acosf( D3DXVec3Dot(&vRight, &vDir) );
	_float fDegree_Right	= D3DXToDegree(fRadian_Right);				

	_float fDegree = 0.f;
	if(fDegree_Right > 90)
	{
		fDegree = 360.f - fDegree_Look;
	}
	else
	{
		fDegree = fDegree_Look;
	}

	_float fPlayerTargetAngleY = CMathMgr::GetInstance()->ReturnDegreeAngle(fDegree + fAngleFixed);	

	m_fOriginalAngleY = fPlayerTargetAngleY;
}

CPlayer::FootFront CPlayer::LeftAndRightCheck(void)
{
	if(matLFoot == NULL || matRFoot == NULL)
	{
		matLFoot = m_pMeshCom->Get_FrameMatrix("ValveBiped_Bip01_L_Foot");
		matRFoot = m_pMeshCom->Get_FrameMatrix("ValveBiped_Bip01_R_Foot");
	}
	_vec3 vDir(-1.f, 0.f, 0.f);
	D3DXVec3TransformCoord(&vDir, &vDir, &m_pTransCom->m_matWorld);

	_vec3 vLeftFoot(0.f, 0.f, 0.f);
	D3DXVec3TransformCoord(&vLeftFoot, &vLeftFoot, &(*matLFoot * m_pTransCom->m_matWorld));

	_vec3 vRightFoot(0.f, 0.f, 0.f);
	D3DXVec3TransformCoord(&vRightFoot, &vRightFoot, &(*matRFoot * m_pTransCom->m_matWorld));

	D3DXVECTOR3 vDirPos			= vDir - m_pTransCom->m_vPosition;
	D3DXVECTOR3 vLeftFootPos	= vLeftFoot - m_pTransCom->m_vPosition;
	D3DXVECTOR3 vRightFootPos	= vRightFoot - m_pTransCom->m_vPosition;

	_float fLeftFootAngle = CMathMgr::GetInstance()->GetAngleBetweenTwoVectors(vDirPos, vLeftFootPos);
	_float fRightFootAngle = CMathMgr::GetInstance()->GetAngleBetweenTwoVectors(vDirPos, vRightFootPos);

	if(fLeftFootAngle > 180.f)
		fLeftFootAngle = 360 - fLeftFootAngle;

	if(fRightFootAngle > 180.f)
		fRightFootAngle = 360 - fRightFootAngle;

	if(fLeftFootAngle <= fRightFootAngle)
	{
		return Left_Foot_Front;
	}
	else
	{
		return Right_Foot_Front;
	}
}

void CPlayer::Current_Player_Animation_Type_Change_KeyInput(BYTE KeyFlag, Player::PLAYERANIMATION eChangePlayerAnimationType, _float fCheckEndPeriodMulTime /*= 0.f*/, _bool bOnceBool /*= false*/)
{
	if( (Engine::GetDIKeyState(KeyFlag) & 0x80) && m_pMeshCom->Check_EndPeriodMulTime(fCheckEndPeriodMulTime) && !bOnceBool)
		m_ePlayerAnimation = eChangePlayerAnimationType;
}

void CPlayer::Current_Player_Animation_Type_Change_MouseInput(Engine::CInput::MOUSECLICK MouseFlag, Player::PLAYERANIMATION eChangePlayerAnimationType, _float fCheckEndPeriodMulTime /*= 0.f*/, _bool bOnceBool /*= false*/)
{
	if( (Engine::GetDIMouseState(MouseFlag) & 0x80) && m_pMeshCom->Check_EndPeriodMulTime(fCheckEndPeriodMulTime) && !bOnceBool)
		m_ePlayerAnimation = eChangePlayerAnimationType;
}

void CPlayer::Current_Player_Animation_Type_Change_SpaceInput(void)
{
	if((Engine::GetDIKeyState(DIK_SPACE) & 0x80) && !m_bOnceSpaceKeyCheck)
	{
		//추후 여기에 방향을 정해주는 코드를 짜서 예외처리를 한다.
		if( (Engine::GetDIKeyState(DIK_W) & 0x80) || (Engine::GetDIKeyState(DIK_S) & 0x80) || (Engine::GetDIKeyState(DIK_A) & 0x80) || (Engine::GetDIKeyState(DIK_D) & 0x80))
		{
			if(LeftAndRightCheck() == Left_Foot_Front)
				m_ePlayerAnimation = Player::speedymove_l;
			else
				m_ePlayerAnimation = Player::speedymove_r;
		}
		else
		{
			if(LeftAndRightCheck() == Left_Foot_Front)
				m_ePlayerAnimation = Player::battle_attack_break_l;
			else
				m_ePlayerAnimation = Player::battle_attack_break_r;
		}
	}
}

void CPlayer::BlockadeRush_MouseInput(void)
{
	//봉쇄, 달리면서 봉쇄, 돌진 후에 평타 스매시 예외처리
	if( (m_ePlayerAnimation == Player::battle_attack_break_l
		|| m_ePlayerAnimation == Player::battle_attack_break_r
		|| m_ePlayerAnimation == Player::battle_attack_break_speedymove_l
		|| m_ePlayerAnimation == Player::battle_attack_break_speedymove_r
		|| m_ePlayerAnimation == Player::speedymove_l
		|| m_ePlayerAnimation == Player::speedymove_r)

		&& m_pMeshCom->Check_EndPeriodMulTime(0.0f)
		&& (Engine::GetDIMouseState(Engine::CInput::DIM_LBUTTON) & 0x80)
		&& !m_bOnceLbuttonCheck) //평타 예외처리
	{
		if(BasicAttackOrder == BA_1a)
		{
			m_ePlayerAnimation = Player::battle_attack_02;
		}
		else if(BasicAttackOrder == BA_1b)
		{
			m_ePlayerAnimation = Player::battle_attack_02;
		}
		else if(BasicAttackOrder == BA_2)
		{
			m_ePlayerAnimation = Player::battle_attack_03;
		}
		else if(BasicAttackOrder == BA_3)
		{
			m_ePlayerAnimation = Player::battle_attack_04;;
		}
		else if(BasicAttackOrder == BA_4)
		{
			m_ePlayerAnimation = Player::battle_attack_01_b;
		}
		else if(SmashAttackOrder == SA_0)
		{
			m_ePlayerAnimation = Player::battle_attack_03;
		}
		else if(SmashAttackOrder == SA_1)
		{
			m_ePlayerAnimation = Player::battle_attack_03;
		}
		else if(SmashAttackOrder == SA_2)
		{
			m_ePlayerAnimation = Player::battle_attack_01_b;
		}
		else if(SmashAttackOrder == SA_3)
		{
			m_ePlayerAnimation = Player::battle_attack_04;
		}
		else if(SmashAttackOrder == SA_4a)
		{
			m_ePlayerAnimation = Player::battle_attack_02;
		}
		else if(SmashAttackOrder == SA_4b)
		{
			m_ePlayerAnimation = Player::battle_attack_03;
		}
		else if(SmashAttackOrder == SA_End)
		{
			m_ePlayerAnimation = Player::battle_attack_03;
		}
	}

	if( (m_ePlayerAnimation == Player::battle_attack_break_l
		|| m_ePlayerAnimation == Player::battle_attack_break_r
		|| m_ePlayerAnimation == Player::battle_attack_break_speedymove_l
		|| m_ePlayerAnimation == Player::battle_attack_break_speedymove_r
		|| m_ePlayerAnimation == Player::speedymove_l
		|| m_ePlayerAnimation == Player::speedymove_r)

		&& m_pMeshCom->Check_EndPeriodMulTime(0.0f)
		&& (Engine::GetDIMouseState(Engine::CInput::DIM_RBUTTON) & 0x80)
		&& !m_bOnceRbuttonCheck) //스매시 예외처리
	{
		if(BasicAttackOrder == BA_1a)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_01;
		}
		else if(BasicAttackOrder == BA_1b)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_01;
		}
		else if(BasicAttackOrder == BA_2)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_02_a;
		}
		else if(BasicAttackOrder == BA_3)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_03;
		}
		else if(BasicAttackOrder == BA_4)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_04_a;
		}
		else if(SmashAttackOrder == SA_0)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_02_a;
		}
		else if(SmashAttackOrder == SA_1)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_02_a;
		}
		else if(SmashAttackOrder == SA_2)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_04_a;
		}
		else if(SmashAttackOrder == SA_3)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_03;
		}
		else if(SmashAttackOrder == SA_4a)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_01;
		}
		else if(SmashAttackOrder == SA_4b)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_02_a;
		}
		else if(SmashAttackOrder == SA_End)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_02_a;
		}
	}
}

void CPlayer::Judgement_MouseInput(void)
{
	if( (m_ePlayerAnimation == Player::SP_attack_judgement_l
		|| m_ePlayerAnimation == Player::SP_attack_judgement_r)
		&& m_pMeshCom->Check_EndPeriodMulTime(0.0f)
		&& (Engine::GetDIMouseState(Engine::CInput::DIM_LBUTTON) & 0x80)
		&& !m_bOnceLbuttonCheck) //평타 예외처리
	{
		if(BasicAttackOrder == BA_1a)
		{
			m_ePlayerAnimation = Player::battle_attack_02;
		}
		else if(BasicAttackOrder == BA_1b)
		{
			m_ePlayerAnimation = Player::battle_attack_02;
		}
		else if(BasicAttackOrder == BA_2)
		{
			m_ePlayerAnimation = Player::battle_attack_03;
		}
		else if(BasicAttackOrder == BA_3)
		{
			m_ePlayerAnimation = Player::battle_attack_04;;
		}
		else if(BasicAttackOrder == BA_4)
		{
			m_ePlayerAnimation = Player::battle_attack_01_b;
		}
		else if(SmashAttackOrder == SA_0)
		{
			m_ePlayerAnimation = Player::battle_attack_03;
		}
		else if(SmashAttackOrder == SA_1)
		{
			m_ePlayerAnimation = Player::battle_attack_03;
		}
		else if(SmashAttackOrder == SA_2)
		{
			m_ePlayerAnimation = Player::battle_attack_01_b;
		}
		else if(SmashAttackOrder == SA_3)
		{
			m_ePlayerAnimation = Player::battle_attack_04;
		}
		else if(SmashAttackOrder == SA_4a)
		{
			m_ePlayerAnimation = Player::battle_attack_02;
		}
		else if(SmashAttackOrder == SA_4b)
		{
			m_ePlayerAnimation = Player::battle_attack_03;
		}
		else if(SmashAttackOrder == SA_End)
		{
			m_ePlayerAnimation = Player::battle_attack_03;
		}
	}

	if( (m_ePlayerAnimation == Player::SP_attack_judgement_l
		|| m_ePlayerAnimation == Player::SP_attack_judgement_r)
		&& m_pMeshCom->Check_EndPeriodMulTime(0.0f)
		&& (Engine::GetDIMouseState(Engine::CInput::DIM_RBUTTON) & 0x80)
		&& !m_bOnceRbuttonCheck) //스매시 예외처리
	{
		if(BasicAttackOrder == BA_1a)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_01;
		}
		else if(BasicAttackOrder == BA_1b)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_01;
		}
		else if(BasicAttackOrder == BA_2)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_02_a;
		}
		else if(BasicAttackOrder == BA_3)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_03;
		}
		else if(BasicAttackOrder == BA_4)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_04_a;
		}
		else if(SmashAttackOrder == SA_0)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_02_a;
		}
		else if(SmashAttackOrder == SA_1)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_02_a;
		}
		else if(SmashAttackOrder == SA_2)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_04_a;
		}
		else if(SmashAttackOrder == SA_3)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_03;
		}
		else if(SmashAttackOrder == SA_4a)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_01;
		}
		else if(SmashAttackOrder == SA_4b)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_02_a;
		}
		else if(SmashAttackOrder == SA_End)
		{
			m_ePlayerAnimation = Player::battle_attack_strong_02_a;
		}
	}
}

_bool CPlayer::PlayerSkill_Input(_int iSKillNumber)
{
	//CPlayerSkill::SkillSelect() 에서 사용함
	if(iSKillNumber == 0)
	{
		if(LeftAndRightCheck() == Left_Foot_Front)
		{
			if(m_pMeshCom->Get_CurrentAniIdx() != Player::SP_attack_judgement_l)
			{
				m_ePlayerAnimation = Player::SP_attack_judgement_l;
				
				if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_judgement_r && m_pMeshCom->Check_EndPeriodMulTime(0.35f))
				{
					Key_Input_Immediately_Rotation();
					m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
					return true;
				}
				else if(m_pMeshCom->Get_CurrentAniIdx() != Player::SP_attack_judgement_r)
				{
					Key_Input_Immediately_Rotation();
					m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
					return true;
				}				
			}
		}
		else if(LeftAndRightCheck() == Right_Foot_Front)
		{
			if(m_pMeshCom->Get_CurrentAniIdx() != Player::SP_attack_judgement_r)
			{
				m_ePlayerAnimation = Player::SP_attack_judgement_r;

				if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_judgement_l && m_pMeshCom->Check_EndPeriodMulTime(0.35f))
				{
					Key_Input_Immediately_Rotation();
					m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
					return true;
				}
				else if(m_pMeshCom->Get_CurrentAniIdx() != Player::SP_attack_judgement_l)
				{
					Key_Input_Immediately_Rotation();
					m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
					return true;
				}
			}
		}
	}
	else if(iSKillNumber == 1)
	{
		if(LeftAndRightCheck() == Left_Foot_Front)
		{

			if(m_ePlayerAnimation != Player::SP_attack_charge_slash_begin_l && m_ePlayerAnimation != Player::SP_attack_charge_slash_during_l)
			{
				m_fPlayerSP -= 250.f / CPlayerSkill::m_fSkillUseValueBalance;
				m_iChargeSlashNumber = 0;
				m_ePlayerAnimation = Player::SP_attack_charge_slash_begin_l;
				m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
				return true;
			}
		}
		else
		{

			if(m_ePlayerAnimation != Player::SP_attack_charge_slash_begin_r && m_ePlayerAnimation != Player::SP_attack_charge_slash_during_r)
			{
				m_fPlayerSP -= 250.f / CPlayerSkill::m_fSkillUseValueBalance;
				m_iChargeSlashNumber = 0;
				m_ePlayerAnimation = Player::SP_attack_charge_slash_begin_r;
				m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
				return true;
			}
		}
	}
	else if(iSKillNumber == 2)
	{
		if(m_ePlayerAnimation != Player::SP_attack_execution)
		{
			m_fPlayerSP -= 500.f / CPlayerSkill::m_fSkillUseValueBalance;
			m_ePlayerAnimation = Player::SP_attack_execution;
			m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
			return true;
		}
	}
	else if(iSKillNumber == 3)
	{
		if(m_ePlayerAnimation != Player::SP_cast_berserker)
		{
			m_fPlayerSP -= 750.f / CPlayerSkill::m_fSkillUseValueBalance;
			m_ePlayerAnimation = Player::SP_cast_berserker;
			m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
			m_bBerserkerCheck		= true;
			m_fBerserkerTime		= 0.f;
			m_fBerserkerFullTime	= 14.f;
			return true;
		}
	}
	else if(iSKillNumber == 4)
	{
		//미구현
		//무조건 false;
		return false;
		/*if(m_ePlayerAnimation != Player::SP_attack_hell)
		{
			m_fPlayerSP -= 1000.f / CPlayerSkill::m_fSkillUseValueBalance;
			m_ePlayerAnimation = Player::SP_attack_hell;
			m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
			return true;
		}*/
	}

	return false;
}

void CPlayer::CollisionCheckFunction(void)
{
	m_bMonsterAttack_Body_CollisionCheck	= false;
	m_bMonsterEffect_Body_CollisionCheck	= false;

	MonsterAttackBox_And_MyBodyBox();
	MonsterEffectBox_And_MyBodyBox();
}

void CPlayer::MonsterAttackBox_And_MyBodyBox(void)
{
	list<CMonster*>::iterator Monsteriter		= CCollisionMgr::GetInstance()->Get_MonsterList()->begin();
	list<CMonster*>::iterator MonsteriterEnd	= CCollisionMgr::GetInstance()->Get_MonsterList()->end();

	for( ; Monsteriter != MonsteriterEnd; ++Monsteriter )
	{
		if((*Monsteriter)->Get_PlayerDistanceCheck() == true)
		{
			boost::unordered_map<int, vector<CLIENTBOXINFO*>>::iterator MapiterEnd	= (*Monsteriter)->Get_MonsterMapAttackBox()->end();
			boost::unordered_map<int, vector<CLIENTBOXINFO*>>::iterator MapiterFind	= (*Monsteriter)->Get_MonsterMapAttackBox()->find((*Monsteriter)->Get_MonsterAnimationIndex());

			if(MapiterFind != MapiterEnd)
			{
				vector<CLIENTBOXINFO*>::iterator iter		= MapiterFind->second.begin();
				vector<CLIENTBOXINFO*>::iterator iterEnd	= MapiterFind->second.end();

				for(; iter != iterEnd; ++iter)
				{
					if((*Monsteriter)->Get_MeshCom()->Check_EndPeriodMulTime((*iter)->fStartTime) == false)
						continue;

					if((*Monsteriter)->Get_MeshCom()->
						Check_EndPeriodMulTime((*iter)->fEndTime) == true)
						continue;

					//몬스터의 어택 박스와 플레이어의 구가 충돌하면
					const _matrix pMonsterAttackBoxMatrix = *(*Monsteriter)->Get_MeshCom()->Get_FrameMatrix((*iter)->pFrameName) * (*Monsteriter)->Get_TransCom()->m_matWorld;

					if(Engine::CCollision::GetInstance()->Collision_AABB_Sphere((*iter)->vMin, (*iter)->vMax,
						pMonsterAttackBoxMatrix
						, m_pTransCom->m_vPosition, PlayerCollisionDistance))
					{
						for(int i = BT_Head; i < BT_End-1; ++i)
						{
							const _matrix* pPlayerDamageBoxMatrix = m_pMeshCom->Get_FrameMatrix(m_pDamageBox[i]->pFrameName);

							if(Engine::CCollision::GetInstance()->Collision_OBB_OBB((*iter)->vMin, (*iter)->vMax,
								pMonsterAttackBoxMatrix, m_pDamageBox[i]->vMin, m_pDamageBox[i]->vMax, (*pPlayerDamageBoxMatrix) * m_pTransCom->m_matWorld))
							{
								m_bMonsterAttack_Body_CollisionCheck = true;

								m_pMonster = (*Monsteriter);	//충돌후 몬스터의 공격 상태에 따라 피를 깍기 위해서 선언
								m_eDamageDirectionType = CMathMgr::GetInstance()->DamageDirectionDecision((*Monsteriter)->Get_TransCom()->m_vPosition, m_pTransCom); //몬스터의 위치와 플레이어의 상태에 따라 피격 방향을 설정

								//데미지 폰트를 좀더 랜덤하게 좌우로 띄어 자연스러운 효과를 준다.
								m_vCollisionPos = (m_pDamageBox[i]->vMin + m_pDamageBox[i]->vMax) / 2.f;//맞은 박스의 중심값을 넘겨준다.
								D3DXVec3TransformCoord(&m_vCollisionPos, &m_vCollisionPos, &((*pPlayerDamageBoxMatrix) * m_pTransCom->m_matWorld));
								float fGapX = m_vCollisionPos.x - m_pTransCom->m_vPosition.x;
								
								if(fGapX > 0.1f)
									m_vCollisionPos.x += 100.f + rand()%50;
								else if(fGapX < -0.1f)
									m_vCollisionPos.x -= 100.f + rand()%50;
								else
									m_vCollisionPos.x += -50.f + rand()%100;
								
								//추후 어떤 부위가 맞아는지 체크하는 것을 만들어서
								//플레이어의 Info를 관리하는 부분에서 HP와 연관하여 부위파괴를 만들자.
								if( m_iColBox == -1 )
								{
 									if( m_bBox[0] == true && ( i == BT_Head ) )
 										m_iColBox = 0;
 									else if( m_bBox[1] == true && (i == BT_Upper) )
										m_iColBox = 1;
									else if( m_bBox[3] == true && ( i == BT_LowerLeft || i == BT_LowerRight ) )
										m_iColBox = 3;
									else if( m_bBox[2] == true && ( i == BT_HandLeft || i == BT_HandRight ) )
										m_iColBox = 2;
									else if( m_bBox[4] == true && ( /*i == BT_FootLeft ||*/ i == BT_FootRight ) )
										m_iColBox = 4;
								}

								printf("충돌 %d\n", m_iColBox);

								return;
							}
						}
					}
				}
			}
		}
	}
}

void CPlayer::MonsterEffectBox_And_MyBodyBox(void)
{
	list<CEffect*>::iterator iter		= CCollisionMgr::GetInstance()->Get_EffectList()->begin();
	list<CEffect*>::iterator iterEnd	= CCollisionMgr::GetInstance()->Get_EffectList()->end();

	for( ; iter != iterEnd; ++iter)
	{
		if(Engine::CCollision::GetInstance()->Collision_Sphere_Sphere((*iter)->GetWorldPos(), (*iter)->GetRanius(), m_pTransCom->m_vPosition, PlayerCollisionDistance))
		{
			for(int i = BT_Head; i < BT_End-1; ++i)
			{
				const _matrix* pPlayerDamageBoxMatrix = m_pMeshCom->Get_FrameMatrix(m_pDamageBox[i]->pFrameName);

				if(Engine::CCollision::GetInstance()->Collision_WorldSphere_Sphere((m_pDamageBox[i]->vMin + m_pDamageBox[i]->vMax)/2.f
					, D3DXVec3Length(&(m_pDamageBox[i]->vMax - m_pDamageBox[i]->vMin)) / 2.f * 0.1f //플레이어의 사이즈
					, *pPlayerDamageBoxMatrix * m_pTransCom->m_matWorld
					, (*iter)->GetWorldPos(), (*iter)->GetRanius()))
				{
					m_bMonsterEffect_Body_CollisionCheck = true;
					//데미지 폰트를 좀더 랜덤하게 좌우로 띄어 자연스러운 효과를 준다.
					m_vCollisionPos = (m_pDamageBox[i]->vMin + m_pDamageBox[i]->vMax) / 2.f;//맞은 박스의 중심값을 넘겨준다.
					D3DXVec3TransformCoord(&m_vCollisionPos, &m_vCollisionPos, &((*pPlayerDamageBoxMatrix) * m_pTransCom->m_matWorld));
					float fGapX = m_vCollisionPos.x - m_pTransCom->m_vPosition.x;

					if(fGapX > 0.1f)
						m_vCollisionPos.x += 100.f + rand()%50;
					else if(fGapX < -0.1f)
						m_vCollisionPos.x -= 100.f + rand()%50;
					else
						m_vCollisionPos.x += -50.f + rand()%100;

					//공격되는 이펙트가 여러개 이면 어떻게 해야 하는가? => 처음 한번 맞고 난후 맞는 동작 중일때는 데미지를 안맞는 무적 동작으로 들어가게 한다.
					m_fEffAttackValue	= (*iter)->Get_AttackValue();
					m_fEffDownValue		= (*iter)->Get_DownValue();
					(*iter)->SetCollsion(true);

					if( m_iColBox == -1 )
					{
						if( m_bBox[0] == true && ( i == BT_Head ) )
							m_iColBox = 0;
						else if( m_bBox[1] == true && (i == BT_Upper) )
							m_iColBox = 1;
						else if( m_bBox[3] == true && ( i == BT_LowerLeft || i == BT_LowerRight ) )
							m_iColBox = 3;
						else if( m_bBox[2] == true && ( i == BT_HandLeft || i == BT_HandRight ) )
							m_iColBox = 2;
						else if( m_bBox[4] == true && ( /*i == BT_FootLeft ||*/ i == BT_FootRight ) )
							m_iColBox = 4;
					}

					return;
				}
			}
		}
	}
}

bool CPlayer::ObjectBox_And_MyMoveBox(void)
{
	list<CStaticObject*>::iterator iter		= CCollisionMgr::GetInstance()->Get_StaticObjectList()->begin();
	list<CStaticObject*>::iterator iterEnd	= CCollisionMgr::GetInstance()->Get_StaticObjectList()->end();

	for( ; iter != iterEnd; ++iter )
	{
		//Sphere - Sphere 충돌이 되면 서로 가까이 있다는 말이다.
		if(Engine::CCollision::GetInstance()->Collision_Sphere_Sphere((*iter)->Get_WorldCenter(), (*iter)->Get_WorldRadius(), m_pTransCom->m_vPosition, m_fMoveRadius))
		{
			//if((*iter)->GetObjectRadianY() == D3DXToRadian(0) || (*iter)->GetObjectRadianY() == D3DXToRadian(90))
			//{
			//	if(Engine::CCollision::GetInstance()->Collision_WorldAABB_WorldSphere((*iter)->Get_WorldMin(), (*iter)->Get_WorldMax(), m_pTransCom->m_vPosition, m_fMoveRadius))
			//	{
			//		//충돌시 해당 오브벡트의 충돌 면을 알아내어 슬라이딩 벡터를 고 구현해보자.
			//		return true;
			//	}
			//}
			//else
			//{
				if(Engine::CCollision::GetInstance()->Collision_OBB_WorldSphere_SlidingVector((*iter)->Get_LocalMin(), (*iter)->Get_LocalMax(), (*iter)->Get_TransCom()->m_matWorld, (*iter)->Get_TransCom()->m_vScale, m_pTransCom->m_vPosition, m_fMoveRadius, m_vOriginalPos, &m_pTransCom->m_vPosition))
				{
					//충돌시 해당 오브벡트의 충돌 면을 알아내어 슬라이딩 벡터를 고 구현해보자.
					return true;
				}
			//}
		}
	}
	return false;
}

bool CPlayer::CDynamicMesh_And_MyMoveBox(void)
{
	list<CMonster*>::iterator iter		= CCollisionMgr::GetInstance()->Get_MonsterList()->begin();
	list<CMonster*>::iterator iterEnd	= CCollisionMgr::GetInstance()->Get_MonsterList()->end();

	for( ; iter != iterEnd; ++iter )
	{
		//충돌할건지 여부를 검사한다.
		if(false == (*iter)->Get_MoveCollCheck()) 
		{
			// 몬스터의 상태가 충돌 안함이므로 false로 넘어간다.
			continue;
		}
		else
		{
			//Sphere - Sphere 충돌이 되면 서로 가까이 있다는 말이다.
			if(Engine::CCollision::GetInstance()->Collision_Sphere_Sphere((*iter)->Get_TransCom()->m_vPosition, (*iter)->Get_MoveRadius(), m_pTransCom->m_vPosition, m_fMoveRadius))
			{
				return true;
			}
		}
		
	}

	return false;
}

void CPlayer::Player_InfoChange(void)
{
	//플레이어 풀 달리기 예외처리
	if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_run_sprint)
		m_fPlayerRunFullTime += m_fTimeDelta;
	else
		m_fPlayerRunFullTime = 0.f;
	
	if(m_fPlayerRunFullTime >= 2.f)
	{
		if(!m_bPlayerRunFull)
		{
			CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_attack_strong_05 + rand()%2));
			//CEffectMgr::GetInstance()->Play_Effect(L"Wind", m_pTransCom->m_vPosition + _vec3(0.f, 6.f, 0.f), false, true , _vec3(5.f, 5.f, 0.f), _vec3(0.f, 0.f, 0.f), m_fOriginalAngleY);
			//PlayEffect(L"Wind",0.f, NULL, m_pTransCom->m_vPosition + _vec3(0.f, 5.f, 0.f)/*_vec3(95.f, 5.f, 1.5f)*/, 2, false, _vec3(5.f, 5.f, 0.f));
			CEffectMgr::GetInstance()->Play_Effect(L"Wind", m_pTransCom->m_vPosition + _vec3(0.f, 6.f, 0.f), false, true , _vec3(5.f, 5.f, 0.f), _vec3(0.f, 0.f, 0.f), D3DXToRadian(m_fOriginalAngleY-90.f));
		}
		m_bPlayerRunFull = true;
	}
	else
		m_bPlayerRunFull = false;

	//플레이어 콤보 예외처리
	if(m_fPlayerAttackComboTime > 5.f)
	{
		m_fPlayerAttackComboTime = 0.f;
		m_iPlayerAttackCombo = 0;
	}
	else
	{
		m_fPlayerAttackComboTime += m_fTimeDelta;
	}

	//버서커 상태에서 무적 모드
	if(m_bBerserkerCheck)
	{
		m_fBerserkerTime += m_fTimeDelta;
		if(m_fBerserkerTime >= m_fBerserkerFullTime)
			m_bBerserkerCheck = false;
	}

	//플레이어 콤보에 따른 각성 수치
	if(m_iPlayerAttackCombo == 0)
	{
		m_fWakeUpAttackMagnification = 1.f;
	}
	else if(m_iPlayerAttackCombo < 5)
	{
		m_fWakeUpAttackMagnification = 1.3f;
	}
	else if(m_iPlayerAttackCombo < 10)
	{
		m_fWakeUpAttackMagnification = 1.6f;
	}
	else if(m_iPlayerAttackCombo < 15)
	{
		m_fWakeUpAttackMagnification = 1.9f;
	}
	else if(m_iPlayerAttackCombo < 20)
	{
		m_fWakeUpAttackMagnification = 2.1f;
	}
	else
	{
		m_fWakeUpAttackMagnification = 2.4f;
	}

	//스태미나 관련
	if(m_pMeshCom->Get_CurrentAniIdx() != m_ePlayerAttackStaminaAnimationCheck)
	{
		m_ePlayerAttackStaminaAnimationCheck = (Player::PLAYERANIMATION)m_pMeshCom->Get_CurrentAniIdx();
		if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_break_l
			|| m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_break_r
			|| m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_break_speedymove_l
			|| m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_break_speedymove_r)
		{
			m_fPlayerStamina -= 8.f;
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::speedymove_l
			|| m_pMeshCom->Get_CurrentAniIdx() == Player::speedymove_r)
		{
			m_fPlayerStamina -= 5.f;
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_00)
		{
			m_fPlayerStamina -= 8.f;
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_01)
		{
			m_fPlayerStamina -= 12.f;
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_02_a)
		{
			m_fPlayerStamina -= 12.f;
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_run_sprint)
		{
			m_fPlayerStamina -= 15.f;
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_03)
		{
			m_fPlayerStamina -= 22.f;
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_a)
		{
			m_fPlayerStamina -= 15.f;
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_b)
		{
			m_fPlayerStamina -= 25.f;
		}
	}

	//공격력 다운 수치 관련
	if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_01_a || m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_01_b)
	{
		m_fPlayerAttackValue = m_fPlayerPureAttackValue * (0.051f * m_fWakeUpAttackMagnification * ((100.f + rand()%10) / 100.f));
		m_fPlayerDownValue = 3.f * m_fWakeUpAttackMagnification;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_02)
	{
		m_fPlayerAttackValue = m_fPlayerPureAttackValue * (0.058f * m_fWakeUpAttackMagnification * ((100.f + rand()%10) / 100.f));
		m_fPlayerDownValue = 4.f * m_fWakeUpAttackMagnification;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_03)
	{
		m_fPlayerAttackValue = m_fPlayerPureAttackValue * (0.070f * m_fWakeUpAttackMagnification * ((100.f + rand()%10) / 100.f));
		m_fPlayerDownValue = 5.f * m_fWakeUpAttackMagnification;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_04)
	{
		m_fPlayerAttackValue = m_fPlayerPureAttackValue * (0.083f * m_fWakeUpAttackMagnification * ((100.f + rand()%10) / 100.f));
		m_fPlayerDownValue = 7.f * m_fWakeUpAttackMagnification;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_break_l
		|| m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_break_r
		|| m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_break_speedymove_l
		|| m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_break_speedymove_r)
	{
		m_fPlayerAttackValue = m_fPlayerPureAttackValue * (0.064f * m_fWakeUpAttackMagnification * ((100.f + rand()%20) / 100.f));
		m_fPlayerDownValue = 10.f * m_fWakeUpAttackMagnification;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_00)
	{
		m_fPlayerAttackValue = m_fPlayerPureAttackValue * (0.080f * m_fWakeUpAttackMagnification * ((100.f + rand()%20) / 100.f));
		m_fPlayerDownValue = 5.f * m_fWakeUpAttackMagnification;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_01)
	{
		m_fPlayerAttackValue = m_fPlayerPureAttackValue * (0.134f * m_fWakeUpAttackMagnification * ((100.f + rand()%30) / 100.f));
		m_fPlayerDownValue = 40.f * m_fWakeUpAttackMagnification;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_02_a)
	{
		m_fPlayerAttackValue = m_fPlayerPureAttackValue * (0.289f * m_fWakeUpAttackMagnification * ((100.f + rand()%30) / 100.f));
		m_fPlayerDownValue = 20.f * m_fWakeUpAttackMagnification;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_03)
	{
		m_fPlayerAttackValue = m_fPlayerPureAttackValue * (0.242f * m_fWakeUpAttackMagnification * ((100.f + rand()%30) / 100.f));
		m_fPlayerDownValue = 35.f * m_fWakeUpAttackMagnification;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_a)
	{
		m_fPlayerAttackValue = m_fPlayerPureAttackValue * (0.432f * m_fWakeUpAttackMagnification * ((100.f + rand()%30) / 100.f));
		m_fPlayerDownValue = 50.f * m_fWakeUpAttackMagnification;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_b)
	{
		m_fPlayerAttackValue = m_fPlayerPureAttackValue * (0.544f * m_fWakeUpAttackMagnification * ((100.f + rand()%30) / 100.f));
		m_fPlayerDownValue = 80.f * m_fWakeUpAttackMagnification;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_judgement_l || m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_judgement_r)
	{
		m_fPlayerAttackValue = m_fPlayerPureAttackValue * (0.25f * m_fWakeUpAttackMagnification * ((100.f + rand()%10) / 100.f));
		m_fPlayerDownValue = 30.f * m_fWakeUpAttackMagnification;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_charge_slash_up_l || m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_charge_slash_up_r)
	{
		m_fPlayerAttackValue = m_fPlayerPureAttackValue * (0.3f * m_fWakeUpAttackMagnification * ((100.f + rand()%10) / 100.f) * m_iChargeSlashNumber);
		m_fPlayerDownValue = 50.f * m_fWakeUpAttackMagnification * m_iChargeSlashNumber;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_execution)
	{
		m_fPlayerAttackValue = m_fPlayerPureAttackValue * (0.777f * m_fWakeUpAttackMagnification * ((100.f + rand()%50) / 100.f));
		m_fPlayerDownValue = 500.f;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_hell)
	{
		m_fPlayerAttackValue = m_fPlayerPureAttackValue * (0.555f * m_fWakeUpAttackMagnification * ((100.f + rand()%50) / 100.f));
		m_fPlayerDownValue = 0.f;
	}

	if(m_fPlayerAttackValue >= 9999.f)
		m_fPlayerAttackValue = 9000.f + rand()%999;

	if(m_fPlayerPureAttackValue < 300.f)
		m_fPlayerPureAttackValue += 200 + rand()%300;

	//라디안 블러 예외처리
	if(m_bPlayerRunFull)
	{
		if(!CDynamicCamera::GetInstance()->GetModeCheck())
			Engine::CRenderer::m_bRadialBlurRender = true;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::speedymove_l && (m_pMeshCom->Check_EndPeriodMulTime(0.1f) && !m_pMeshCom->Check_EndPeriodMulTime(0.45f)))
	{
		if(!CDynamicCamera::GetInstance()->GetModeCheck())
			Engine::CRenderer::m_bRadialBlurRender = true;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::speedymove_r && !m_pMeshCom->Check_EndPeriodMulTime(0.3f))
	{
		if(!CDynamicCamera::GetInstance()->GetModeCheck())
			Engine::CRenderer::m_bRadialBlurRender = true;
	}
	else
	{
		if(!CDynamicCamera::GetInstance()->GetModeCheck())
			Engine::CRenderer::m_bRadialBlurRender = false;
	}

	//모션 블러 예외처리
	if( (m_ePlayerState == CPlayer::PS_BasicAttack || m_ePlayerState == CPlayer::PS_BreakAttack || m_ePlayerState == CPlayer::PS_SmashAttack || m_ePlayerState == CPlayer::PS_SkillAttack)
		&& m_pAttackBox[m_ePlayerAnimationCheck] != NULL)	//플레이어의 공격 박스가 존재 한다면
	{
		Engine::CRenderer::m_bMotionBlurRender = false;
		if( m_pMeshCom->Check_EndPeriodMulTime(m_pAttackBox[m_ePlayerAnimationCheck]->fStartTime) == false )
			return;

		if( m_pMeshCom->Check_EndPeriodMulTime(Get_ClientBoxInfo()->fEndTime) == true )
			return;

		Engine::CRenderer::m_bMotionBlurRender = true;

		//공격 관련 라디안 블러 예외처리
		if(m_ePlayerState == CPlayer::PS_BreakAttack || m_ePlayerState == CPlayer::PS_SmashAttack || m_ePlayerState == CPlayer::PS_SkillAttack)
		{
			if(!CDynamicCamera::GetInstance()->GetModeCheck())
				Engine::CRenderer::m_bRadialBlurRender = true;
		}
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::speedymove_l && (m_pMeshCom->Check_EndPeriodMulTime(0.1f) && !m_pMeshCom->Check_EndPeriodMulTime(0.45f)))
	{
		Engine::CRenderer::m_bMotionBlurRender = true;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::speedymove_r && !m_pMeshCom->Check_EndPeriodMulTime(0.3f))
	{
		Engine::CRenderer::m_bMotionBlurRender = true;
	}
	else
	{
		Engine::CRenderer::m_bMotionBlurRender = false;
		Engine::CRenderer::m_bMotionBlurRender = false;
	}
}

void CPlayer::Player_UnderAttack_InfoChange(void)
{
	//걷기 또는 쉴때만 스태미나를 차게 한다.
	if(m_ePlayerState == PS_Walk || m_ePlayerState == PS_Idle)
		m_fPlayerStamina += 10.f * m_fTimeDelta;

	//플레이어 정보 예외처리
	if(m_fPlayerHP > m_fPlayerMaxHP)
		m_fPlayerHP = m_fPlayerMaxHP;

	if(m_fPlayerStamina > m_fPlayerMaxStamina)
		m_fPlayerStamina = m_fPlayerMaxStamina;
	
	if(m_fPlayerSP > m_fPlayerMaxSP)
		m_fPlayerSP = m_fPlayerMaxSP;

	if(m_fPlayerHP < 0.f)
		m_fPlayerHP = 0.f;

	if(m_fPlayerStamina < 0.f)
		m_fPlayerStamina = 0.f;

	if(m_fPlayerSP < 0.f)
		m_fPlayerSP = 0.f;

	if(m_fDownValue > 0.f)
		m_fDownValue -= m_fTimeDelta/3.f;

	if(m_fDownValue < 0.f)
		m_fDownValue = 0.f;

	//피의 비율이 40% 이하로 가면 심장박동 UI를 그리면서 작동시킨다.
	if((m_fPlayerHP / m_fPlayerMaxHP) <= 0.4f)
	{
		CPlayerHeartBeatUI::GetInstance()->SetRender(true);
	}
	else
		CPlayerHeartBeatUI::GetInstance()->SetRender(false);

	//플레이어 피격 관련
	if(m_bMonsterAttack_Body_CollisionCheck || m_bMonsterEffect_Body_CollisionCheck)
	{
		if(m_bPlayerHitCheck) //공격을 1번만 체크하여 모션을 재생시킨다.
			return;

		m_bPlayerHitCheck = true;

		//피격 동작일때는 무적 판정으로 한다.
		if(m_pMeshCom->Get_CurrentAniIdx() == Player::damage_light_back
			|| m_pMeshCom->Get_CurrentAniIdx() == Player::damage_light_front
			|| m_pMeshCom->Get_CurrentAniIdx() == Player::damage_light_left
			|| m_pMeshCom->Get_CurrentAniIdx() == Player::damage_light_right
			|| m_pMeshCom->Get_CurrentAniIdx() == Player::damage_light_up
			
			|| m_pMeshCom->Get_CurrentAniIdx() == Player::damage_middle_back_begin
			|| m_pMeshCom->Get_CurrentAniIdx() == Player::damage_middle_back_end
			
			|| m_pMeshCom->Get_CurrentAniIdx() == Player::damage_middle_front_begin
			|| m_pMeshCom->Get_CurrentAniIdx() == Player::damage_middle_front_end
			
			|| m_pMeshCom->Get_CurrentAniIdx() == Player::damage_strong_back_begin
			|| m_pMeshCom->Get_CurrentAniIdx() == Player::damage_strong_front_begin

			|| m_pMeshCom->Get_CurrentAniIdx() == Player::down_back2idle
			|| m_pMeshCom->Get_CurrentAniIdx() == Player::down_front2idle)
		{
			return;
		}

		_float fAttackValue = 0.f;
		_float fDownValue	= 0.f;
		if(true == m_bMonsterEffect_Body_CollisionCheck)
		{
			//printf("m_bMonsterEffect_Body_CollisionCheck\n");
			fAttackValue	= m_fEffAttackValue;
			fDownValue		= m_fEffDownValue;
		}
		else if(true == m_bMonsterAttack_Body_CollisionCheck)
		{
			fAttackValue	= m_pMonster->Get_MonsterAttackValue();
			fDownValue		= m_pMonster->Get_MonsterInfo().fDownValue;
		}

		if(m_ePlayerState == PS_BreakAttack) //플레이어가 봉쇄 상태이고
		{
			if(m_bSwordAttack) //공격 박스가 존재하고 공격 중일때
			{
				//철벽 효과를 발생한다.
				if(m_fBerserkerFullTime < 35.f)
					m_fBerserkerFullTime += 2.f;

				//철벽 효과로 인해 데미지를 적게 받는다.
				fAttackValue = fAttackValue * 0.45f;
				fDownValue		= fDownValue * 0.45f;
				//여기서 봉쇄성공한 사운드를 재생한다.
				CSoundMgr::GetInstance()->PlayEffect(SOUND::Hurk_Swing_GreatSword_KnockBack_01, _vec3(0.f, 0.f, 0.f), 0.6f);
				m_pPlayerSkill->Init_JudgementCoolTime();
			}
		}

		m_fPlayerHP		-= fAttackValue;
		m_fDownValue	+= fDownValue;
		CDamageFont::Create(m_pGraphicDev, DFONT_TYPE_DAMAGE, DFONT_OBJ_PLAYER, (int)fAttackValue, m_vCollisionPos);   //플레이어가 받을 데미지
		
		//슈퍼 아머 상태일때는 피격당해 데미지 폰트와 피만 깍이고 원래 동작을 그대로 수행한다.
		if(m_ePlayerDefenseState == PS_SuperArmour || m_bBerserkerCheck)
		{
			m_fDownValue -= fDownValue / 2.f;
			CDynamicCamera::GetInstance()->CameraStartVibration(1.5f, 10.f);
			return;
		}

		////버서커 상대이면 슈퍼 아머이다.
		//if(m_bBerserkerCheck)
		//{
		//	m_fDownValue -= fDownValue/2.f;
		//	CDynamicCamera::GetInstance()->CameraStartVibration(1.5f, 10.f);
		//	return;
		//}

		//피격 당했을때 해당하는 애니메이션의 이펙트를 끈다.
		if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_02)
		{
			CEffectMgr::GetInstance()->Stop_Effect(L"Attack2");
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_03)
		{
			CEffectMgr::GetInstance()->Stop_Effect(L"Attack3");
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_04)
		{
			CEffectMgr::GetInstance()->Stop_Effect(L"4th_Smoke11");
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_01)
		{
			CEffectMgr::GetInstance()->Stop_Effect(L"AttackStrong2Dis");
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_02_a)
		{
			CEffectMgr::GetInstance()->Stop_Effect(L"AttackStrong2");
			//CEffectMgr::GetInstance()->Stop_Effect(L"AttackStrong2_2");
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_03)
		{
			CEffectMgr::GetInstance()->Stop_Effect(L"AttackStrong3Dis");
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_a)
		{
			CEffectMgr::GetInstance()->Stop_Effect(L"4th_Smash");
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_b)
		{
			CEffectMgr::GetInstance()->Stop_Effect(L"4th_Smash2");
		}

		/*if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_02)
		{
			CEffectMgr::GetInstance()->Stop_Effect(L"Attack2");
		}*/

		//피격 당하면 콤보 관련 수치 초기화
		m_fPlayerAttackComboTime = 0.f;
		m_iPlayerAttackCombo = 0;
		if( m_fBoxHP[m_iColBox] > 0 )
			m_fBoxHP[m_iColBox] -= fAttackValue * 0.5f;
		else if( m_fBoxHP[m_iColBox] <= 0 )
			Change_BrokenMesh();

		printf("충돌 %f\n", m_fBoxHP[m_iColBox]);

		m_iColBox = -1;

		_float	fSelectValue = fAttackValue * (m_fDownValue / 100.f);

		if(fSelectValue <= 400.f)
		{
			if(m_eDamageDirectionType == DDT_FRONT)
			{
				m_ePlayerAnimation = Player::damage_light_front;
				m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);
				if(m_bMonsterEffect_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_weak_01 + rand()%6));
				}
				else if(m_bMonsterAttack_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_weak_01 + rand()%6));
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::Hit_Metal_Slash + rand()%2));
				}

				CDynamicCamera::GetInstance()->CameraStartVibration(1.5f, 10.f);
			}
			else if(m_eDamageDirectionType == DDT_BACK)
			{
				m_ePlayerAnimation = Player::damage_light_back;
				m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);

				if(m_bMonsterEffect_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_weak_01 + rand()%6));
				}
				else if(m_bMonsterAttack_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_weak_01 + rand()%6));
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::Hit_Metal_Slash + rand()%2));
				}
				
				CDynamicCamera::GetInstance()->CameraStartVibration(1.5f, 10.f);
			}
			else if(m_eDamageDirectionType == DDT_FRONT_LEFT)
			{
				m_ePlayerAnimation = Player::damage_light_left;
				m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);

				if(m_bMonsterEffect_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_weak_01 + rand()%6));
				}
				else if(m_bMonsterAttack_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_weak_01 + rand()%6));
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::Hit_Metal_Slash + rand()%2));
				}
				
				CDynamicCamera::GetInstance()->CameraStartVibration(1.5f, 10.f);
			}
			else if(m_eDamageDirectionType == DDT_BACK_LEFT)
			{
				m_ePlayerAnimation = Player::damage_light_left;
				m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);

				if(m_bMonsterEffect_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_weak_01 + rand()%6));
				}
				else if(m_bMonsterAttack_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_weak_01 + rand()%6));
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::Hit_Metal_Slash + rand()%2));
				}
				
				CDynamicCamera::GetInstance()->CameraStartVibration(1.5f, 10.f);
			}
			else if(m_eDamageDirectionType == DDT_FRONT_RIGHT)
			{
				m_ePlayerAnimation = Player::damage_light_right;
				m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);

				if(m_bMonsterEffect_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_weak_01 + rand()%6));
				}
				else if(m_bMonsterAttack_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_weak_01 + rand()%6));
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::Hit_Metal_Slash + rand()%2));
				}
				
				CDynamicCamera::GetInstance()->CameraStartVibration(1.5f, 10.f);
			}
			else if(m_eDamageDirectionType == DDT_BACK_RIGHT)
			{
				m_ePlayerAnimation = Player::damage_light_right;
				m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);

				if(m_bMonsterEffect_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_weak_01 + rand()%6));
				}
				else if(m_bMonsterAttack_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_weak_01 + rand()%6));
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::Hit_Metal_Slash + rand()%2));
				}
				
				CDynamicCamera::GetInstance()->CameraStartVibration(1.5f, 10.f);
			}
		}
		else if(fSelectValue <= 800.f)
		{
			if(m_eDamageDirectionType == DDT_FRONT || m_eDamageDirectionType == DDT_FRONT_LEFT || m_eDamageDirectionType == DDT_FRONT_RIGHT)
			{
				m_fDownValue -= 5.f;
				m_ePlayerAnimation = Player::damage_middle_front_begin;
				m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);

				if(m_bMonsterEffect_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_medium_1 + rand()%7));
				}
				else if(m_bMonsterAttack_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_medium_1 + rand()%7));
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::Hit_Metal_Stab + rand()%2));
				}
				
				CDynamicCamera::GetInstance()->CameraStartVibration(2.0f, 7.5f);
			}
			else if(m_eDamageDirectionType == DDT_BACK || m_eDamageDirectionType == DDT_BACK_LEFT || m_eDamageDirectionType == DDT_BACK_RIGHT)
			{
				m_fDownValue -= 5.f;
				m_ePlayerAnimation = Player::damage_middle_back_begin;
				m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);

				if(m_bMonsterEffect_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_medium_1 + rand()%7));
				}
				else if(m_bMonsterAttack_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_medium_1 + rand()%7));
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::Hit_Metal_Stab + rand()%2));
				}
				
				CDynamicCamera::GetInstance()->CameraStartVibration(2.0f, 7.5f);
			}
		}
		else
		{
			if(m_eDamageDirectionType == DDT_FRONT || m_eDamageDirectionType == DDT_FRONT_LEFT || m_eDamageDirectionType == DDT_FRONT_RIGHT)
			{
				_int iRandom = rand()%10;

				if(iRandom < 4)
				{
					m_fDownValue = m_fDownValue / 2.f;
					m_ePlayerAnimation = Player::down_resist;
					m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);

					if(m_bMonsterEffect_Body_CollisionCheck)
					{
						CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_weak_02));
					}
					else if(m_bMonsterAttack_Body_CollisionCheck)
					{
						CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_weak_02));
					}
					CDynamicCamera::GetInstance()->CameraStartVibration(1.5f, 10.f);
				}
				else
				{
					m_fDownValue = 0.f;
					m_ePlayerAnimation = Player::damage_strong_front_begin;
					m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);

					if(m_bMonsterEffect_Body_CollisionCheck)
					{
						CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_strong_01 + rand()%8));
					}
					else if(m_bMonsterAttack_Body_CollisionCheck)
					{
						CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_strong_01 + rand()%8));
						CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::Hit_Metal_StrongSlash + rand()%2));
					}
					
					CDynamicCamera::GetInstance()->CameraStartVibration(2.5f, 5.f);
				}
			}
			else if(m_eDamageDirectionType == DDT_BACK || m_eDamageDirectionType == DDT_BACK_LEFT || m_eDamageDirectionType == DDT_BACK_RIGHT)
			{
				m_fDownValue = 0.f;
				m_ePlayerAnimation = Player::damage_strong_back_begin;
				m_pMeshCom->Set_AnimationSet(m_ePlayerAnimation);

				if(m_bMonsterEffect_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_strong_01 + rand()%8));
				}
				else if(m_bMonsterAttack_Body_CollisionCheck)
				{
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::hurk_hurt_strong_01 + rand()%8));
					CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::Hit_Metal_StrongSlash + rand()%2));
				}
				
				CDynamicCamera::GetInstance()->CameraStartVibration(2.5f, 5.f);
			}
		}
	}
	else
	{
		m_bPlayerHitCheck = false;
	}
}

void CPlayer::Player_AttackSuccess_InfoChange(void)
{
	m_iPlayerAttackCombo += 1;
	m_fPlayerAttackComboTime = 0.f;

	//버서커 상태에서 적을 공격하면 스태미나가 찬다.
	if(m_bBerserkerCheck)
	{
		if(m_ePlayerState == PS_BasicAttack)
			m_fPlayerStamina += 3.f + rand()%5;
		else if(m_ePlayerState == PS_BreakAttack)
			m_fPlayerStamina += 5.f + rand()%5;
		else if(m_ePlayerState == PS_SmashAttack_0)
			m_fPlayerStamina += 7.f + rand()%7;
		else if(m_ePlayerState == PS_SmashAttack)
			m_fPlayerStamina += 7.f + rand()%7;
		else if(m_ePlayerState == PS_SkillAttack)
			m_fPlayerStamina += 10.f + rand()%10;
	}

	if(m_ePlayerState == PS_SmashAttack)
	{
		if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_01)
		{
			m_fPlayerSP += 20.f;
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_02_a || m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_run_sprint)
		{
			m_fPlayerSP += 50.f;
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_03)
		{
			m_fPlayerSP += 45.f;
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_a)
		{
			m_fPlayerSP += 85.f;
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_b)
		{
			m_fPlayerSP += 85.f;
		}
	}

	//봉쇄로 적이 공격 받을때 사운드 재생
	if(m_ePlayerState == PS_BreakAttack)
	{
		CSoundMgr::GetInstance()->PlayEffect(SOUND::EFF(SOUND::Hit_Flesh_Blunt + rand()%2));
	}


}

void CPlayer::PlayerMoveFunction(void)
{
	m_vOriginalPos = m_pTransCom->m_vPosition;

	if(m_pMeshCom->Get_CurrentAniIdx() != Player::SP_attack_charge_slash_begin_l) //카메라 본의 이동이 이상하여 예외처리함
		m_pMeshCom->AnimationMove("ValveBiped_Anim_Attachment_CAM_Origin", m_pTransCom);

	//이펙트 거리 차이를 위해 선언
	_vec3	vEffectPosGap = m_pTransCom->m_vPosition;

	//m_pMeshCom->AnimationMoveTwo("ValveBiped_Bip01_L_Toe0", "ValveBiped_Bip01_R_Toe0", m_pTransCom);
	
	ObjectBox_And_MyMoveBox();

	if(!CDynamicMesh_And_MyMoveBox())
		m_vAfterPos = m_pTransCom->m_vPosition;
	else
	{
		m_vAfterPos = m_vOriginalPos;
	}

	Check_Navigation();

	m_vEffectPos	= vEffectPosGap - m_pTransCom->m_vPosition; //이펙트 거리 차이를 위해 선언
	m_vEffectPos	= _vec3(m_vEffectPos.x, 0.f, m_vEffectPos.z);

	if(D3DXVec3Length(&m_vEffectPos) != 0)			// 부딪혀서 차이가 있으면 이펙트 이동
	{
		if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_02)
		{
			CEffectMgr::GetInstance()->Move_Effect(L"Attack2",vEffectPosGap, m_vEffectPos);
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_03)
		{
			CEffectMgr::GetInstance()->Move_Effect(L"Attack3",vEffectPosGap, m_vEffectPos);
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_01)
		{
			CEffectMgr::GetInstance()->Move_Effect(L"AttackStrong2Dis",vEffectPosGap, m_vEffectPos);
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_02_a)
		{
			CEffectMgr::GetInstance()->Move_Effect(L"AttackStrong2",vEffectPosGap, m_vEffectPos);
			//CEffectMgr::GetInstance()->Move_Effect(L"AttackStrong2_2",vEffectPosGap, m_vEffectPos);
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_03)
		{
			CEffectMgr::GetInstance()->Move_Effect(L"AttackStrong3Dis",vEffectPosGap, m_vEffectPos);
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_04)
		{
			CEffectMgr::GetInstance()->Move_Effect(L"4th_Smoke11",vEffectPosGap, m_vEffectPos);
		}
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_a)
		{
			CEffectMgr::GetInstance()->Move_Effect(L"4th_Smash",vEffectPosGap, m_vEffectPos);
		}	
		else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_b)
		{
			CEffectMgr::GetInstance()->Move_Effect(L"4th_Smash2",vEffectPosGap, m_vEffectPos);
		}	
	}
}

void CPlayer::Check_Navigation(void)
{
	//처음에 있는 곳에 네비셀이 존재하면 받아온다//ㅊㅂㅇ
	if(NULL == m_pNaviCell)
	{
		list<Engine::CNaviCell*>::iterator iter = Engine::Get_NaviMesh()->Get_listNaviCell()->begin();
		list<Engine::CNaviCell*>::iterator iter_end = Engine::Get_NaviMesh()->Get_listNaviCell()->end();
		for(; iter != iter_end; ++iter)
		{
			_bool bInside = (*iter)->Inside_Point(&m_vAfterPos);
			if(true == bInside)
			{
				m_pNaviCell = (*iter);
				break;
			}
		}
	}
	else
	{
		_bool bInside = m_pNaviCell->CheckInFindNeighbor(&m_vAfterPos, &m_pNaviCell);

		if(true == bInside)
		{

			m_pTransCom->m_vPosition = m_vAfterPos;

			D3DXPLANE	CellPlane;
			D3DXPlaneFromPoints(&CellPlane, m_pNaviCell->Get_Point(Engine::CNaviCell::POINT_A), m_pNaviCell->Get_Point(Engine::CNaviCell::POINT_B), m_pNaviCell->Get_Point(Engine::CNaviCell::POINT_C));

			m_pTransCom->m_vPosition.y = (-(CellPlane.a * m_pTransCom->m_vPosition.x) - (CellPlane.c * m_pTransCom->m_vPosition.z) - CellPlane.d) / CellPlane.b;
		}
		else // 슬라이딩벡터
		{
			m_pTransCom->m_vPosition = m_vOriginalPos; // 움직이기 전 위치로 돌려놓고

			Engine::CLine2D* pLine = m_pNaviCell->Get_Line(&m_vAfterPos); // 움직인 후의 위치에 대해 판단한다.

			_vec3	vNormal = _vec3(-pLine->Get_VecNormal()->x, 0.f, -pLine->Get_VecNormal()->y);
			D3DXVec3Normalize(&vNormal, &vNormal);

			_vec3	vSlidingVector = (m_vAfterPos - m_vOriginalPos) - D3DXVec3Dot(&(m_vAfterPos - m_vOriginalPos), &vNormal) * vNormal;
			//D3DXVec3Normalize(&vSlidingVector, &vSlidingVector);

			//여기수정해야함.
			m_pTransCom->m_vPosition += vSlidingVector;
			//* (abs(D3DXVec3Length(&(m_vAfterPos - m_vOriginalPos)))) * m_fTimeDelta * 100; // 움직이기 전 위치에 슬라이딩벡터를 적용한다.

			//슬라이딩으로 이동한 후의 위치로 다시 검사.
			_bool bSlidingCheck = false;
			list<Engine::CNaviCell*>::iterator iter = Engine::Get_NaviMesh()->Get_listNaviCell()->begin();
			list<Engine::CNaviCell*>::iterator iter_end = Engine::Get_NaviMesh()->Get_listNaviCell()->end();
			for(; iter != iter_end; ++iter)
			{
				bSlidingCheck = (*iter)->Inside_Point(&m_pTransCom->m_vPosition);
				if(true == bSlidingCheck)
				{
					m_pNaviCell = (*iter);
					break;
				}
			}

			if(true == bSlidingCheck)
			{
				D3DXPLANE	CellPlane;
				D3DXPlaneFromPoints(&CellPlane, m_pNaviCell->Get_Point(Engine::CNaviCell::POINT_A), m_pNaviCell->Get_Point(Engine::CNaviCell::POINT_B), m_pNaviCell->Get_Point(Engine::CNaviCell::POINT_C));

				m_pTransCom->m_vPosition.y = (-(CellPlane.a * m_pTransCom->m_vPosition.x) - (CellPlane.c * m_pTransCom->m_vPosition.z) - CellPlane.d) / CellPlane.b;
			}
			else
			{
				m_pTransCom->m_vPosition -= vSlidingVector;
				//* (abs(D3DXVec3Length(&(m_vAfterPos - m_vOriginalPos)))) * m_fTimeDelta * 100;
			}
		}
	}
}

void CPlayer::CreateEffectFunction(void)
{
	//버서커 상태일때 이펙트
	if(m_bBerserkerCheck)
	{
		m_fBerserkerCreateTime += m_fTimeDelta;
		if(m_fBerserkerCreateTime >= 0.01f)
		{
			m_fBerserkerCreateTime = 0.0f;
			const _matrix* pMatrix = m_pMeshCom->Get_FrameMatrix("ValveBiped_Bip01_Spine4");
			_matrix vCreateMatrix = *pMatrix * m_pTransCom->m_matWorld;
			_vec3 vCreateEffect(0.f, 0.f, 0.f);

			_int i = rand()%4;
			if(i == 0)
			{
				D3DXVec3TransformCoord(&vCreateEffect, &_vec3(10.f + rand()%5, 10.f + rand()%5, 0.f), &vCreateMatrix);
			}
			else if(i == 1)
			{
				D3DXVec3TransformCoord(&vCreateEffect, &_vec3(-10.f - rand()%5, 10.f + rand()%5, 0.f), &vCreateMatrix);
			}
			else if(i == 2)
			{
				D3DXVec3TransformCoord(&vCreateEffect, &_vec3(10.f + rand()%5, -10.f - rand()%5, 0.f), &vCreateMatrix);
			}
			else if(i == 3)
			{
				D3DXVec3TransformCoord(&vCreateEffect, &_vec3(-10.f - rand()%5, -10.f - rand()%5, 0.f), &vCreateMatrix);				
			}

			i = rand()%4;

			if(i == 0)
			{
				CEffectMgr::GetInstance()->Play_Effect(L"Berserk_Dust1", vCreateEffect, false, true,
					_vec3(2.f, 2.f, 0.f), _vec3(0.f, 0.f, 0.f), D3DXToRadian(m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]));
			}
			else if(i == 1)
			{
				CEffectMgr::GetInstance()->Play_Effect(L"Berserk_Dust2", vCreateEffect, false, true,
					_vec3(2.f, 2.f, 0.f), _vec3(0.f, 0.f, 0.f), D3DXToRadian(m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]));
			}
			else if(i == 2)
			{
				CEffectMgr::GetInstance()->Play_Effect(L"Berserk_Dust3", vCreateEffect, false, true,
					_vec3(2.f, 2.f, 0.f), _vec3(0.f, 0.f, 0.f), D3DXToRadian(m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]));
			}
			else if(i == 3)
			{
				CEffectMgr::GetInstance()->Play_Effect(L"Berserk_Dust4", vCreateEffect, false, true,
					_vec3(2.f, 2.f, 0.f), _vec3(0.f, 0.f, 0.f), D3DXToRadian(m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]));
			}
		}		
	}

	//애니메이션의 상태에 따라 스킬을 생성 해주는 부분
	if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_01_a)
	{
		// 여기에 이제 충돌 조건도 달아야댐 
	}
	else if(m_bPlayerRunFull)
	{
		//성원아 플레이어 런 풀타임 달리는 시간 계산해서 bool 참일때로 했어 여기 좀 수정해야 자연스러울꺼야
		//PlayShortEffects(L"Sprint_Spark", m_pAttachment_RH_Matrix, 2, _vec3(90.f, 2.f, 2.f));	}
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_00)
	{
		_matrix		matRotY2;
		_vec3		vPlayerDir(0.f, 0.f, -1.f);

		m_fEffectMoveTime += m_fTimeDelta;

		D3DXMatrixRotationY(&matRotY2, D3DXToRadian(m_fOriginalAngleY));

		D3DXVec3TransformNormal(&vPlayerDir, &g_vLook, &matRotY2);
			
		int iNum = 16;
		for(int i = 0; i < iNum; ++i)
		{
			_matrix		matRotY;
			_vec3		vEffectDir(0.f, 0.f, 0.f);
			float		fAngle = (360.f / iNum) * (float)i;
		
			D3DXMatrixIdentity(&matRotY);
			D3DXMatrixRotationY(&matRotY, D3DXToRadian(fAngle));

			D3DXVec3TransformNormal(&vEffectDir, &vPlayerDir, &matRotY);
			D3DXVec3Normalize(&vEffectDir, &vEffectDir);

			
			PlayEffect(L"GroundAtt_Smoke",0.28f, m_pL_Foot_Matrix, _vec3(0.f, vEffectDir.x * 40.f , vEffectDir.z * 40.f), i, false, vEffectDir, _vec3(0.f, 0.f, 0.f), true);
			PlayEffect(L"GroundAtt_Smoke",0.28f, m_pL_Foot_Matrix, _vec3(0.f, vEffectDir.x * 40.f , vEffectDir.z * 40.f), i, false, vEffectDir, _vec3(0.f, 0.f, 0.f), true);
		}
	}

	/*else if(m_pMeshCom->Get_CurrentAniIdx() == 	Player::battle_run_sprint)
	{
		PlayEffect(L"Attack2", 0.f, NULL, m_pTransCom->m_vPosition, 2, false);
	}*/
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_01)
	{
		PlayEffect(L"AttackStrong2Dis", 0.f, NULL, m_pTransCom->m_vPosition, 2, false, _vec3(0.f,0.f,0.f), _vec3(0.f,0.f,0.f), false, true);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_02)
	{
		PlayEffect(L"Attack2", 0.f, NULL, m_pTransCom->m_vPosition, 2, false);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_02_a)
	{
		PlayEffect(L"AttackStrong2", 0.f, NULL, m_pTransCom->m_vPosition, 2, false, _vec3(0.f,0.f,0.f), _vec3(0.f,0.f,0.f), false, true);
		//PlayEffect(L"AttackStrong2_2", 0.f, NULL, m_pTransCom->m_vPosition, 2, false, _vec3(5.f,5.f,0.f));
	}
	
	else  if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_03)
	{
		PlayEffect(L"Attack3", 0.f, NULL, m_pTransCom->m_vPosition, 2, false);
		//m_vAttackStartPos =  m_pTransCom->m_vPosition;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_03)
	{
		PlayEffect(L"AttackStrong3Dis", 0.f, NULL, m_pTransCom->m_vPosition, 2, false, _vec3(0.f,0.f,0.f), _vec3(0.f,0.f,0.f), false, true);
	}
	else  if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_04)
	{
		PlayEffect(L"4th_Smoke11",0.f, NULL, m_pTransCom->m_vPosition/*_vec3(95.f, 5.f, 1.5f)*/, 2, false);
		//m_vAttackStartPos =  m_pTransCom->m_vPosition;
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_a)
	{
		/*if(m_pMeshCom->Check_EndPeriodMulTime(0.1f) && !m_pMeshCom->Check_EndPeriodMulTime(0.27f))
			PlayShortEffects(L"Small_spark",  m_pAttachment_RH_Matrix, 20, _vec3(95.f, 10.f, 1.5f), _vec3(0.f, 0.f, 0.f), _vec3(2.f, 2.f, 2.f));

		PlayEffect(L"Blunt_Basic",0.26f, m_pAttachment_RH_Matrix, _vec3(95.f, 5.f, 1.5f), 2, false);*/
		PlayEffect(L"4th_Smash", 0.f, NULL, m_pTransCom->m_vPosition, 0, false);
		//m_vAttackStartPos =  m_pTransCom->m_vPosition;
	}

	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_b)
	{
		PlayEffect(L"4th_Smash2", 0.f, NULL, m_pTransCom->m_vPosition, 0, false);
	}

	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_cast_berserker)
	{
		PlayEffect(L"Berserk", 0.2415f, m_pL_Foot_Matrix, _vec3(0.f, 0.f, 0.f), 0, false);
		PlayEffect(L"Berserk", 0.9f, m_pL_Foot_Matrix, _vec3(0.f, 0.f, 0.f), 0, false, _vec3(0.5f, 0.5f, 0.5f));
	}
	else
	{
		m_fEffectMoveTime = 0.f;
		m_vAttackStartPos =  _vec3(0.f,0.f,0.f);
	}
	//플레이어의 상태에 따라 스킬을 생성 해주는 부분
}

void CPlayer::PlayEffect(TCHAR* pName, _float fCheckEndPeriodMulTime, const _matrix* pMatrix, _vec3 vLocalPos, _int iboolNumber, _bool bReStart, _vec3 vSize, _vec3 vAngle, _bool bNoTesting, _bool bDistort)
{
	if(!m_bCreateEffect[iboolNumber] && m_pMeshCom->Check_EndPeriodMulTime(fCheckEndPeriodMulTime))
	{
		_vec3 vCreateEffect(0.f, 0.f, 0.f);
		if(NULL != pMatrix)
		{
			_matrix vCreateMatrix = *pMatrix * m_pTransCom->m_matWorld;
			D3DXVec3TransformCoord(&vCreateEffect, &vLocalPos, &vCreateMatrix);
		}
		else
		{
			vCreateEffect = vLocalPos;
		}
		CEffectMgr::GetInstance()->Play_Effect(pName, vCreateEffect, bReStart, true , vSize, vAngle, D3DXToRadian(m_fOriginalAngleY - 90), false, 0.f, NULL, bNoTesting, 0.f, 0.f ,bDistort);
		m_bCreateEffect[iboolNumber] = true;
		m_vAttackStartPos =  m_pTransCom->m_vPosition;
	}
}

void CPlayer::PlayShortEffects(TCHAR* pName, const _matrix* pPlayerMatrix, int iEffectCnt, _vec3 vPos, _vec3 vAngle, _vec3 vSize)
{
	_vec3 vCreateEffect(0.f, 0.f, 0.f);
	_matrix vCreateMatrix = *pPlayerMatrix * m_pTransCom->m_matWorld;

	for(int i = 0; i < iEffectCnt; ++i)
	{
		D3DXVec3TransformCoord(&vCreateEffect, &_vec3(vPos.x, vPos.y , vPos.z - (vPos.z / i) * 2.f), &vCreateMatrix);
		CEffectMgr::GetInstance()->Play_Effect(pName, vCreateEffect, false, true, vSize, _vec3(m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_X] + vAngle.x ,m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] + vAngle.y,m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Z] + vAngle.z));
	}
}

void CPlayer::PlayerSound(void)
{
	//PlayerStateCheck 에서 m_bSwordAttack 체크함
	if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_01_a)
	{
		if(m_bSwordAttack)
			SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_01), 0.0f, 0, 1.f);

		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_01 + rand()%2), 0.0f, 1, 1.f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_01_b)
	{
		if(m_bSwordAttack)
			SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_01), 0.0f, 0, 1.f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_02)
	{
		if(m_bSwordAttack)
			SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Strong_01), 0.0f, 0, 1.f);

		SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Friction_01), 0.0f, 1, 0.7f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_03)
	{
		if(m_bSwordAttack)
			SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_01), 0.0f, 0, 1.f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_04)
	{
		if(m_bSwordAttack)
			SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Strong_02), 0.0f, 0, 1.f);
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_07), 0.0f, 1, 1.f);
		SoundEffectPlay(SOUND::EFF(SOUND::step_lv4), 0.25f, 2, 1.f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_break_l)
	{
		if(m_bSwordAttack)
			SoundEffectPlay(SOUND::EFF(SOUND::step_lv3), 0.0f, 0, 0.3f);
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_skill_parrying_cast_01 + rand()%5), 0.0f, 1, 0.7f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_break_r)
	{
		if(m_bSwordAttack)
			SoundEffectPlay(SOUND::EFF(SOUND::step_lv3), 0.0f, 0, 0.3f);
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_skill_parrying_cast_01 + rand()%5), 0.0f, 1, 0.7f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_break_speedymove_l)
	{
		if(m_bSwordAttack)
			SoundEffectPlay(SOUND::EFF(SOUND::step_lv3), 0.0f, 0, 0.3f);
		//SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_skill_parrying_cast_01 + rand()%5), 0.0f, 1, 0.7f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_break_speedymove_r)
	{
		if(m_bSwordAttack)
			SoundEffectPlay(SOUND::EFF(SOUND::step_lv3), 0.0f, 0, 0.3f);
		//SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_skill_parrying_cast_01 + rand()%5), 0.0f, 1, 0.7f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_00)
	{
		SoundEffectPlay(SOUND::EFF(SOUND::step_lv4), 0.2f, 0, 1.f);
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_long_01 + rand()%3), 0.0f, 1, 0.8f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_01)
	{
		if(m_bSwordAttack)
			SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Blow_01), 0.0f, 0, 1.0f);
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_long_01 + rand()%3), 0.0f, 1, 1.0f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_02_a)
	{
		if(m_bSwordAttack)
			SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Strong_02), 0.0f, 0, 1.0f);
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_long_01 + rand()%4), 0.0f, 1, 1.0f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_run_sprint)
	{
		if(m_bSwordAttack)
			SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Strong_02), 0.0f, 0, 1.0f);
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_long_01 + rand()%4), 0.0f, 1, 1.0f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_03)
	{
		SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Strong_02), 0.15f, 0, 1.0f);
		SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Strong_02), 0.35f, 1, 1.0f);
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_strong_01 + rand()%10), 0.15f, 2, 1.0f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_a)
	{
		SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Friction_01), 0.15f, 0, 1.0f);
		SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Strong_01), 0.15f, 1, 1.0f);
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_strong_01 + rand()%10), 0.15f, 2, 1.0f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_attack_strong_04_b)
	{
		SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Friction_01), 0.1f, 0, 1.0f);
		SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Strong_01), 0.15f, 1, 1.0f);
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_strong_01 + rand()%10), 0.15f, 2, 1.0f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_run_lower)
	{
		SoundEffectPlay(SOUND::EFF(SOUND::material_dirt_step1 + rand()%2), 0.37f, 0, 0.5f);
		//SoundEffectPlay(SOUND::EFF(SOUND::material_dirt_step2), 0.81f, 1, 0.5f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_run_sprint)
	{
		SoundEffectPlay(SOUND::EFF(SOUND::material_dirt_step3 + rand()%2), 0.23f, 0, 0.5f);
		//SoundEffectPlay(SOUND::EFF(SOUND::material_dirt_step4), 0.69f, 1, 0.5f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::battle_run_stop)
	{

	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_charge_slash_begin_l)
	{
		/*SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_long_01 + rand()%4), 0.0f, 0, 1.0f);*/
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_charge_slash_begin_r)
	{
		/*SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_long_01 + rand()%4), 0.0f, 0, 1.0f);*/
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_charge_slash_during_l)
	{
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_strong_01 + rand()%10), 0.0f, 0, 1.0f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_charge_slash_during_r)
	{
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_strong_01 + rand()%10), 0.0f, 0, 1.0f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_charge_slash_up_l)
	{
		if(m_bSwordAttack)
			SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Strong_02), 0.0f, 0, 1.0f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_charge_slash_up_r)
	{
		if(m_bSwordAttack)
			SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Strong_02), 0.0f, 0, 1.0f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_execution)
	{
		/*if(m_bSwordAttack)*/
		SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Blow_01), 0.16f, 0, 1.0f);
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_skill_execution_02), 0.0f, 1, 1.0f);
		SoundEffectPlay(SOUND::EFF(SOUND::step_lv4), 0.375f, 2, 1.0f);
		SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Blow_01), 0.32f, 3, 1.0f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_hell)
	{
	
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_judgement_l)
	{
		if(m_bSwordAttack)
			SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Blow_01), 0.0f, 0, 1.0f);
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_strong_01 + rand()%10), 0.0f, 1, 1.0f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_attack_judgement_r)
	{
		if(m_bSwordAttack)
			SoundEffectPlay(SOUND::EFF(SOUND::Hurk_Swing_GreatSword_Blow_01), 0.0f, 0, 1.0f);
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_strong_01 + rand()%10), 0.0f, 1, 1.0f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::SP_cast_berserker)
	{
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_skill_piercing_last_02), 0.0f, 1, 1.0f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::speedymove_l)
	{
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_skill_parrying_cast_01 + rand()%5), 0.0f, 1, 1.0f);
	}
	else if(m_pMeshCom->Get_CurrentAniIdx() == Player::speedymove_r)
	{
		SoundEffectPlay(SOUND::EFF(SOUND::hurk_attack_skill_parrying_cast_01 + rand()%5), 0.0f, 1, 1.0f);
	}
}

void CPlayer::SoundEffectPlay(SOUND::EFF eSoundType, _float fCheckEndPeriodMulTime, _int iboolNumber, _float fVolume /*= 1.f*/)
{
	if(!m_bPlayerSound[iboolNumber] && m_pMeshCom->Check_EndPeriodMulTime(fCheckEndPeriodMulTime))
	{
		m_bPlayerSound[iboolNumber] = true;
		CSoundMgr::GetInstance()->PlayEffect(eSoundType, m_pTransCom->m_vPosition, fVolume);

		//카메라 진동
		if(eSoundType == SOUND::step_lv3)
		{
			CDynamicCamera::GetInstance()->CameraStartVibration(1.5f, 10.f);
		}
		else if(eSoundType == SOUND::step_lv4)
		{
			CDynamicCamera::GetInstance()->CameraStartVibration(2.3f, 7.f);
		}
		else if(eSoundType == SOUND::Hurk_Swing_GreatSword_Friction_01)
		{
			CDynamicCamera::GetInstance()->CameraStartVibration(1.3f, 15.f);
		}
	}
}

void CPlayer::Load_AttackBox()
{
	// Load Bounding Sphere & Box
	m_pSphere = new SPHEREINFO;

	HANDLE	hFile = CreateFile(L"../Bin/Resources/Data/Box/Player/PlayerAttackBox.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;
	int iAnimationCnt = 0;	int iAniIndex = 0;	int iBoxCnt = 0;

	ZeroMemory(m_pSphere, sizeof(SPHEREINFO));

	ReadFile( hFile, m_pSphere, sizeof(SPHEREINFO), &dwByte, NULL );
	ReadFile( hFile, &iAnimationCnt, sizeof(int), &dwByte, NULL );

	for( int i = 0; i < iAnimationCnt; ++i )
	{
		ReadFile( hFile, &iAniIndex, sizeof(int), &dwByte, NULL );
		ReadFile( hFile, &iBoxCnt, sizeof(int), &dwByte, NULL );

		for( int j = 0; j < iBoxCnt; ++j )
		{ 
			BOXINFO*	pBoxInfo = new BOXINFO;
			CLIENTBOXINFO*	pBox = new CLIENTBOXINFO;
			ReadFile( hFile, (pBoxInfo), sizeof(BOXINFO), &dwByte, NULL );

			pBox->eBoxTag = pBoxInfo->eBoxTag;
			pBox->iAniIdx = pBoxInfo->iAniIdx;

			WideCharToMultiByte(CP_ACP, 0, pBoxInfo->pFrameName, 128, pBox->pFrameName, 128, NULL, NULL);

			pBox->vMin = pBoxInfo->vMin;
			pBox->vMax = pBoxInfo->vMax;

			pBox->fStartTime = pBoxInfo->fStartTime;
			pBox->fEndTime = pBoxInfo->fEndTime;

			//// 박스의 버퍼 생성
			if( FAILED( (*m_pGraphicDev).CreateVertexBuffer(sizeof(Engine::VTXCUBE) * 8, 0, Engine::VTXFVF_CUBE, D3DPOOL_MANAGED, &pBox->pVB, 0 ) ) )
			{
				MSG_BOX("LoadBox pVB Create Failed");
				return;
			}
			if( FAILED( (*m_pGraphicDev).CreateIndexBuffer(sizeof(Engine::INDEX16) * 12, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pBox->pIB, NULL)))
			{
				MSG_BOX("LoadBox pVB Create Failed");
				return;
			}

			//// 버텍스 버퍼 설정
			Engine::VTXCUBE*			pVtxCube = NULL; 
			pBox->pVB->Lock(0, 0, (void**)&pVtxCube, 0);

			_vec3 vMin = pBox->vMin;
			_vec3 vMax = pBox->vMax;

			pVtxCube[0].vPos = D3DXVECTOR3(vMin.x, vMax.y, vMin.z);
			pVtxCube[0].vTexUV = pVtxCube[0].vPos;		

			pVtxCube[1].vPos = D3DXVECTOR3(vMax.x, vMax.y, vMin.z);
			pVtxCube[1].vTexUV = pVtxCube[1].vPos;	 

			pVtxCube[2].vPos = D3DXVECTOR3(vMax.x, vMin.y, vMin.z);
			pVtxCube[2].vTexUV = pVtxCube[2].vPos;	

			pVtxCube[3].vPos = D3DXVECTOR3(vMin.x, vMin.y, vMin.z);
			pVtxCube[3].vTexUV = pVtxCube[3].vPos;	

			pVtxCube[4].vPos = D3DXVECTOR3(vMin.x, vMax.y, vMax.z);
			pVtxCube[4].vTexUV = pVtxCube[4].vPos;

			pVtxCube[5].vPos = D3DXVECTOR3(vMax.x, vMax.y, vMax.z);
			pVtxCube[5].vTexUV = pVtxCube[5].vPos;	

			pVtxCube[6].vPos = D3DXVECTOR3(vMax.x, vMin.y, vMax.z);
			pVtxCube[6].vTexUV = pVtxCube[6].vPos;	

			pVtxCube[7].vPos = D3DXVECTOR3(vMin.x, vMin.y, vMax.z);
			pVtxCube[7].vTexUV = pVtxCube[7].vPos;

			pBox->pVB->Unlock();

			//// 인덱스 버퍼 설정
			Engine::INDEX16*			pIndex = NULL;
			pBox->pIB->Lock(0, 0, (void**)&pIndex, 0);

			//+x
			pIndex[0]._1 = 1;	pIndex[0]._2 = 5;	pIndex[0]._3 = 6;
			pIndex[1]._1 = 1;	pIndex[1]._2 = 6;	pIndex[1]._3 = 2;
			//-x
			pIndex[2]._1 = 4;	pIndex[2]._2 = 0;	pIndex[2]._3 = 3;
			pIndex[3]._1 = 4;	pIndex[3]._2 = 3;	pIndex[3]._3 = 7;
			//+y
			pIndex[4]._1 = 4;	pIndex[4]._2 = 5;	pIndex[4]._3 = 1;
			pIndex[5]._1 = 4;	pIndex[5]._2 = 1;	pIndex[5]._3 = 0;
			//-y
			pIndex[6]._1 = 3;	pIndex[6]._2 = 2;	pIndex[6]._3 = 6;
			pIndex[7]._1 = 3;	pIndex[7]._2 = 6;	pIndex[7]._3 = 7;
			//+z
			pIndex[8]._1 = 7;	pIndex[8]._2 = 6;	pIndex[8]._3 = 5;
			pIndex[9]._1 = 7;	pIndex[9]._2 = 5;	pIndex[9]._3 = 4;
			//-z
			pIndex[10]._1 = 0;	pIndex[10]._2 = 1;	pIndex[10]._3 = 2;
			pIndex[11]._1 = 0;	pIndex[11]._2 = 2;	pIndex[11]._3 = 3;

			pBox->pIB->Unlock();

			if( dwByte == 0 )
			{
				Engine::Safe_Delete(pBox);
				break;
			}
			Engine::Safe_Delete(pBoxInfo);
			Engine::Safe_Delete(m_pSphere);

			m_pAttackBox[iAniIndex] = pBox;
		}
	}
	CloseHandle(hFile);
}

void CPlayer::Load_DamageBox()
{
	// Load Bounding Sphere & Box
	m_pSphere = new SPHEREINFO;

	HANDLE	hFile = CreateFile(L"../Bin/Resources/Data/Box/Player/PlayerDamageBox.dat", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	DWORD	dwByte;
	int iAnimationCnt = 0;	int iAniIndex = 0;	int iBoxCnt = 0;

	ZeroMemory(m_pSphere, sizeof(SPHEREINFO));

	ReadFile( hFile, m_pSphere, sizeof(SPHEREINFO), &dwByte, NULL );
	ReadFile( hFile, &iAnimationCnt, sizeof(int), &dwByte, NULL );

	for( int i = 0; i < iAnimationCnt; ++i )
	{
		ReadFile( hFile, &iAniIndex, sizeof(int), &dwByte, NULL );
		ReadFile( hFile, &iBoxCnt, sizeof(int), &dwByte, NULL );

		for( int j = 0; j < iBoxCnt-1; ++j )
		{ 
			BOXINFO*		pBoxInfo = new BOXINFO;
			CLIENTBOXINFO*	pBox = new CLIENTBOXINFO;
			ReadFile( hFile, (pBoxInfo), sizeof(BOXINFO), &dwByte, NULL );

			pBox->eBoxTag = pBoxInfo->eBoxTag;
			pBox->iAniIdx = pBoxInfo->iAniIdx;

			WideCharToMultiByte(CP_ACP, 0, pBoxInfo->pFrameName, 128, pBox->pFrameName, 128, NULL, NULL);

			pBox->vMin = pBoxInfo->vMin;
			pBox->vMax = pBoxInfo->vMax;

			pBox->fStartTime = pBoxInfo->fStartTime;
			pBox->fEndTime = pBoxInfo->fEndTime;

			//// 박스의 버퍼 생성
			if( FAILED( (*m_pGraphicDev).CreateVertexBuffer(sizeof(Engine::VTXCUBE) * 8, 0, Engine::VTXFVF_CUBE, D3DPOOL_MANAGED, &pBox->pVB, 0 ) ) )
			{
				MSG_BOX("LoadBox pVB Create Failed");
				return;
			}
			if( FAILED( (*m_pGraphicDev).CreateIndexBuffer(sizeof(Engine::INDEX16) * 12, 0, D3DFMT_INDEX16, D3DPOOL_MANAGED, &pBox->pIB, NULL)))
			{
				MSG_BOX("LoadBox pVB Create Failed");
				return;
			}

			//// 버텍스 버퍼 설정
			Engine::VTXCUBE*			pVtxCube = NULL; 
			pBox->pVB->Lock(0, 0, (void**)&pVtxCube, 0);

			_vec3 vMin = pBox->vMin;
			_vec3 vMax = pBox->vMax;

			pVtxCube[0].vPos = D3DXVECTOR3(vMin.x, vMax.y, vMin.z);
			pVtxCube[0].vTexUV = pVtxCube[0].vPos;		

			pVtxCube[1].vPos = D3DXVECTOR3(vMax.x, vMax.y, vMin.z);
			pVtxCube[1].vTexUV = pVtxCube[1].vPos;	 

			pVtxCube[2].vPos = D3DXVECTOR3(vMax.x, vMin.y, vMin.z);
			pVtxCube[2].vTexUV = pVtxCube[2].vPos;	

			pVtxCube[3].vPos = D3DXVECTOR3(vMin.x, vMin.y, vMin.z);
			pVtxCube[3].vTexUV = pVtxCube[3].vPos;	

			pVtxCube[4].vPos = D3DXVECTOR3(vMin.x, vMax.y, vMax.z);
			pVtxCube[4].vTexUV = pVtxCube[4].vPos;

			pVtxCube[5].vPos = D3DXVECTOR3(vMax.x, vMax.y, vMax.z);
			pVtxCube[5].vTexUV = pVtxCube[5].vPos;	

			pVtxCube[6].vPos = D3DXVECTOR3(vMax.x, vMin.y, vMax.z);
			pVtxCube[6].vTexUV = pVtxCube[6].vPos;	

			pVtxCube[7].vPos = D3DXVECTOR3(vMin.x, vMin.y, vMax.z);
			pVtxCube[7].vTexUV = pVtxCube[7].vPos;

			pBox->pVB->Unlock();

			//// 인덱스 버퍼 설정
			Engine::INDEX16*			pIndex = NULL;
			pBox->pIB->Lock(0, 0, (void**)&pIndex, 0);

			//+x
			pIndex[0]._1 = 1;	pIndex[0]._2 = 5;	pIndex[0]._3 = 6;
			pIndex[1]._1 = 1;	pIndex[1]._2 = 6;	pIndex[1]._3 = 2;
			//-x
			pIndex[2]._1 = 4;	pIndex[2]._2 = 0;	pIndex[2]._3 = 3;
			pIndex[3]._1 = 4;	pIndex[3]._2 = 3;	pIndex[3]._3 = 7;
			//+y
			pIndex[4]._1 = 4;	pIndex[4]._2 = 5;	pIndex[4]._3 = 1;
			pIndex[5]._1 = 4;	pIndex[5]._2 = 1;	pIndex[5]._3 = 0;
			//-y
			pIndex[6]._1 = 3;	pIndex[6]._2 = 2;	pIndex[6]._3 = 6;
			pIndex[7]._1 = 3;	pIndex[7]._2 = 6;	pIndex[7]._3 = 7;
			//+z
			pIndex[8]._1 = 7;	pIndex[8]._2 = 6;	pIndex[8]._3 = 5;
			pIndex[9]._1 = 7;	pIndex[9]._2 = 5;	pIndex[9]._3 = 4;
			//-z
			pIndex[10]._1 = 0;	pIndex[10]._2 = 1;	pIndex[10]._3 = 2;
			pIndex[11]._1 = 0;	pIndex[11]._2 = 2;	pIndex[11]._3 = 3;

			pBox->pIB->Unlock();

			if( dwByte == 0 )
			{
				Engine::Safe_Delete(pBox);
				break;
			}
			Engine::Safe_Delete(pBoxInfo);
			m_pDamageBox[j] = pBox;
		}
	}
	CloseHandle(hFile);
}

void CPlayer::Initialize_TCHAR()
{
	for( int i = 0; i < 6; ++i )
	{
		lstrcpy( m_pPath[i], L"" );
		lstrcpy( m_pOirgMeshName[i], L"" );
		lstrcpy( m_pMeshName[i], L"" );
		lstrcpy( m_pOrigFileName[i], L"" );
		lstrcpy( m_pFileName[i], L"" );
	}
	m_pUpper_Broken_MeshCom = NULL;
	m_pHand_Broken_MeshCom = NULL;
	m_pLower_Broken_MeshCom = NULL;
	m_pFoot_Broken_MeshCom = NULL;

	m_pHairOrigMeshCom = NULL;
	m_pUpperOrigMeshCom = NULL;
	m_pLowerOrigMeshCom = NULL;
	m_pHandOrigMeshCom = NULL;
	m_pFootOrigMeshCom = NULL;

}

void CPlayer::Ready_ClothMesh(void)
{
	for( int i = 0; i <5; ++i )
	{
		if( i == 0 && lstrcmp( m_pOirgMeshName[0], L"") != 0)
		{
			if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STATIC, Engine::CMesh::TYPE_PLAYERDYNAMIC, m_pOirgMeshName[0], m_pPath[0], m_pOrigFileName[0])))
			{
				MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
				return;
			}
		}
		else if( i != 0 ) 
		{
			if ( lstrcmp( m_pOirgMeshName[i], L"") != 0 )
			{
				if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STATIC, Engine::CMesh::TYPE_PLAYERDYNAMIC, m_pOirgMeshName[i], m_pPath[i], m_pOrigFileName[i])))
				{
					MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
					return;
				}
			}
			if( lstrcmp( m_pMeshName[i], L"") != 0 )
			{
				if(FAILED(Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STATIC, Engine::CMesh::TYPE_PLAYERDYNAMIC, m_pMeshName[i], m_pPath[i], m_pFileName[i])))
				{
					MSG_BOX("Ready_Mesh Ready Failed(Hurk)");
					return;
				}
			}
		}
	}
	Engine::Ready_Mesh(m_pGraphicDev, RESOURCE_STATIC, Engine::CMesh::TYPE_PLAYERDYNAMIC, L"Mesh_Hair", L"../Bin/Resources/Mesh/DynamicMesh/Look/Hair/", L"Hair.x");


	Set_BrokenMesh();
}

void CPlayer::Set_BrokenMesh()
{
	if( lstrcmp( m_pOirgMeshName[0], L"") != 0 )
	{
		m_pHairOrigMeshCom = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_STATIC, m_pOirgMeshName[0]);
	}
	if( lstrcmp( m_pMeshName[1], L"") != 0 )
		m_pUpper_Broken_MeshCom = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_STATIC, m_pMeshName[1]);
	if( lstrcmp( m_pOirgMeshName[1], L"") != 0 )
		m_pUpperOrigMeshCom = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_STATIC, m_pOirgMeshName[1]);

	if( lstrcmp( m_pMeshName[2], L"") != 0 )
		m_pHand_Broken_MeshCom = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_STATIC, m_pMeshName[2]);
	if( lstrcmp( m_pOirgMeshName[2], L"") != 0 )
		m_pHandOrigMeshCom = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_STATIC, m_pOirgMeshName[2]);

	if( lstrcmp( m_pMeshName[3], L"") != 0 )
		m_pLower_Broken_MeshCom = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_STATIC, m_pMeshName[3]);
	if( lstrcmp( m_pOirgMeshName[3], L"") != 0 )
		m_pLowerOrigMeshCom = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_STATIC, m_pOirgMeshName[3]);

	if( lstrcmp( m_pMeshName[4], L"") != 0 )
		m_pFoot_Broken_MeshCom = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_STATIC, m_pMeshName[4]);
	if( lstrcmp( m_pOirgMeshName[4], L"") != 0 )
		m_pFootOrigMeshCom = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_STATIC, m_pOirgMeshName[4]);


	m_pHair_Broken_MeshCom = (Engine::CPlayerDynamicMesh*)Engine::Clone_Resource(RESOURCE_STATIC, L"Mesh_Hair");
}

void CPlayer::Cloth_Reset()
{
	m_pMeshCom->Set_HairColor(matHairColor);

	if( m_pHairOrigMeshCom != NULL )
		m_pMeshCom->Change_ClothMesh(0, m_pHairOrigMeshCom->GetMeshCon());
	if( m_pUpperOrigMeshCom != NULL )
		m_pMeshCom->Change_ClothMesh(1, m_pUpperOrigMeshCom->GetMeshCon());
	if( m_pHandOrigMeshCom != NULL )
		m_pMeshCom->Change_ClothMesh(2, m_pHandOrigMeshCom->GetMeshCon());
	if( m_pLowerOrigMeshCom != NULL )
		m_pMeshCom->Change_ClothMesh(3, m_pLowerOrigMeshCom->GetMeshCon());
	if( m_pFootOrigMeshCom != NULL )
		m_pMeshCom->Change_ClothMesh(4, m_pFootOrigMeshCom->GetMeshCon());

	for( int i = 0; i < 5; ++i )
	{
		m_bBox[i] = true;
		m_fBoxHP[i] = 100.f;
	}

}

void CPlayer::Change_BrokenMesh()
{
	if( m_bBox[0] && m_fBoxHP[0] <= 0.f && m_pUpper_Broken_MeshCom != NULL)	// 상의 =>여기가 머리지 아영아??
	{
		m_bBox[0] = false;
		m_pMeshCom->Change_ClothMesh(0, m_pHair_Broken_MeshCom->GetMeshCon());
		_matrix mat;
		D3DXMatrixIdentity(&mat);
		mat._11 = 0.f; mat._22 = 0.f; mat._33 = 0.f;
		m_pMeshCom->Set_HairColor(mat);

		CSoundMgr::GetInstance()->PlayEffect(SOUND::Hurk_Swing_GreatSword_AttackBreak_01, _vec3(0.f, 0.f, 0.f), 3.5f);
		CEquipmentUI::GetInstance()->RenderEquipmentUI(0);
		return;
	}

	if( m_bBox[1] && m_fBoxHP[1] <= 0.f && m_pUpper_Broken_MeshCom != NULL)	// 상의
	{
		m_bBox[1] = false;
		printf("상체 \n");
		m_pMeshCom->Change_ClothMesh(1, m_pUpper_Broken_MeshCom->GetMeshCon());
		m_pMeshCom->Set_DrawBody(0);
		m_pMeshCom->Draw();
		m_pMeshCom->Draw_Hand2();
		CSoundMgr::GetInstance()->PlayEffect(SOUND::Hurk_Swing_GreatSword_AttackBreak_01, _vec3(0.f, 0.f, 0.f), 3.5f);
		CEquipmentUI::GetInstance()->RenderEquipmentUI(1);
		return;
	}

	if( m_bBox[2] && m_fBoxHP[2] <= 0.f && m_pHand_Broken_MeshCom != NULL) // 손
	{
		printf("손 \n");
		m_bBox[2] = false;
		m_pMeshCom->Change_ClothMesh(2, m_pHand_Broken_MeshCom->GetMeshCon());
		m_pMeshCom->Draw_Hand2();
		m_pMeshCom->Set_DrawBody(1);
		CSoundMgr::GetInstance()->PlayEffect(SOUND::Hurk_Swing_GreatSword_AttackBreak_01, _vec3(0.f, 0.f, 0.f), 3.5f);
		CEquipmentUI::GetInstance()->RenderEquipmentUI(2);
		return;
	}

	if( m_bBox[3] && m_fBoxHP[3] <= 0.f && m_pLower_Broken_MeshCom != NULL) // 하의
	{
		m_bBox[3] = false;
		printf("하체 \n");
		m_pMeshCom->Change_ClothMesh(3, m_pLower_Broken_MeshCom->GetMeshCon());
		m_pMeshCom->Set_DrawBody(2);
		m_pMeshCom->Draw();
		CSoundMgr::GetInstance()->PlayEffect(SOUND::Hurk_Swing_GreatSword_AttackBreak_01, _vec3(0.f, 0.f, 0.f), 3.5f);
		CEquipmentUI::GetInstance()->RenderEquipmentUI(3);
		return;
	}

	if( m_bBox[4] && m_fBoxHP[4] <= 0.f && m_pFoot_Broken_MeshCom != NULL) // 신발
	{
		printf("신발 \n");
		m_bBox[4] = false;
		m_pMeshCom->Change_ClothMesh(4, m_pFoot_Broken_MeshCom->GetMeshCon());
		m_pMeshCom->Set_DrawBody(3);
		CSoundMgr::GetInstance()->PlayEffect(SOUND::Hurk_Swing_GreatSword_AttackBreak_01, _vec3(0.f, 0.f, 0.f), 3.5f);
		CEquipmentUI::GetInstance()->RenderEquipmentUI(4);
		return;
	}
}

_ulong CPlayer::Release(void)
{
	_ulong dwRefCnt = CGameObject::Release();

	if(0 == dwRefCnt)
	{	
		///// 공격박스 삭제
		Engine::Safe_Delete(m_pSphere);

		Engine::Safe_Release(m_pHair_Broken_MeshCom);
		if( m_pUpper_Broken_MeshCom != NULL )
			Engine::Safe_Release(m_pUpper_Broken_MeshCom);
		if( m_pHand_Broken_MeshCom != NULL )
			Engine::Safe_Release(m_pHand_Broken_MeshCom);
		if( m_pLower_Broken_MeshCom != NULL )
			Engine::Safe_Release(m_pLower_Broken_MeshCom);
		if( m_pFoot_Broken_MeshCom != NULL )
			Engine::Safe_Release(m_pFoot_Broken_MeshCom);
		if( m_pUpperOrigMeshCom != NULL )
			Engine::Safe_Release(m_pUpperOrigMeshCom);
		if( m_pHandOrigMeshCom != NULL )
			Engine::Safe_Release(m_pHandOrigMeshCom);
		if( m_pLowerOrigMeshCom != NULL )
			Engine::Safe_Release(m_pLowerOrigMeshCom);
		if( m_pFootOrigMeshCom != NULL )
			Engine::Safe_Release(m_pFootOrigMeshCom);
		if( m_pHairOrigMeshCom != NULL )
			Engine::Safe_Release(m_pHairOrigMeshCom);

		for(int i = 0; i < Player::PlayerAnimation_End; ++i)
		{
			if(m_pAttackBox[i] != NULL)
			{
				Engine::Safe_Release( m_pAttackBox[i]->pVB );
				Engine::Safe_Release( m_pAttackBox[i]->pIB );
				Engine::Safe_Delete( m_pAttackBox[i] );
			}
		}

		///// 피격박스 삭제

		for( _int i = 0; i < BT_End; ++i )
		{
			Engine::Safe_Release( m_pDamageBox[i]->pVB );
			Engine::Safe_Release( m_pDamageBox[i]->pIB );
			Engine::Safe_Delete( m_pDamageBox[i] );
		}
		////////////////////////////////////////////////////////////////////
		delete this;
	}
	return 0;
}

#ifdef _DEBUG
void CPlayer::Render_BoudingSphere()
{
	//CStageScene::Render 임시 대체
	CCollisionMgr::GetInstance()->Render();
	Engine::CNaviMesh::GetInstance()->Render_NaviMesh(); //ㅊㅂㅇNaviMesh Render
	Engine::CCameraNaviMesh::GetInstance()->Render_NaviMesh();
	//피격 스피어
	D3DXCreateSphere(m_pGraphicDev, m_pSphere->fRadius, 20, 10, &m_pSphereMesh, &m_pSphereBuffer );

	_matrix World;
	D3DXMatrixIdentity(&World);
	D3DXMatrixTranslation(&World, 0.f, m_pSphere->vPos.y, 0.f);

	World = World * m_pTransCom->m_matWorld;

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &( World ) );
	m_pSphereMesh->DrawSubset(0);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	Engine::Safe_Release(m_pSphereMesh);
	Engine::Safe_Release(m_pSphereBuffer);

	//무브 스피어
	D3DXCreateSphere(m_pGraphicDev, m_fMoveRadius, 30, 15, &m_pSphereMesh, &m_pSphereBuffer );

	_matrix MoveWorld;
	D3DXMatrixIdentity(&MoveWorld);

	D3DXMatrixTranslation(&MoveWorld, m_pTransCom->m_vPosition.x, m_pTransCom->m_vPosition.y, m_pTransCom->m_vPosition.z);

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &( MoveWorld ) );
	m_pSphereMesh->DrawSubset(0);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	Engine::Safe_Release(m_pSphereMesh);
	Engine::Safe_Release(m_pSphereBuffer);
}

void CPlayer::Render_AttackBox()
{
	CLIENTBOXINFO* pClientBoxInfo = m_pAttackBox[m_ePlayerAnimation];

	// 항등행렬로 월드행렬을 설정해준다.
	_matrix matDefaultWorld;
	D3DXMatrixIdentity(&matDefaultWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matDefaultWorld);


	if( NULL == pClientBoxInfo )
		return; 


	if( m_pMeshCom->Check_EndPeriodMulTime(pClientBoxInfo->fStartTime) == false )
		return; //continue;

	
	if( m_pMeshCom->Check_EndPeriodMulTime(pClientBoxInfo->fEndTime) == true )
		return; //continue;

	const _matrix* m_pMatrix;

	m_pMatrix = m_pMeshCom->Get_FrameMatrix(pClientBoxInfo->pFrameName);	// 선택한 뼈의 행렬을 얻어온다.

	// 바운딩 박스의 행렬 = 뼈의 행렬 * 메쉬의 행렬 
	_matrix matWorld;
	matWorld = (*m_pMatrix) * m_pTransCom->m_matWorld ;

	Engine::VTXCUBE*			pVtxCube = NULL; 

	pClientBoxInfo->pVB->Lock(0, 0, (void**)&pVtxCube, 0);


	_vec3 _vMin = pClientBoxInfo->vMin;
	_vec3 _vMax = pClientBoxInfo->vMax;

	pVtxCube[0].vPos = D3DXVECTOR3(_vMin.x, _vMax.y, _vMin.z);
	pVtxCube[0].vTexUV = pVtxCube[0].vPos;		

	pVtxCube[1].vPos = D3DXVECTOR3(_vMax.x, _vMax.y, _vMin.z);
	pVtxCube[1].vTexUV = pVtxCube[1].vPos;		

	pVtxCube[2].vPos = D3DXVECTOR3(_vMax.x, _vMin.y, _vMin.z);
	pVtxCube[2].vTexUV = pVtxCube[2].vPos;	

	pVtxCube[3].vPos = D3DXVECTOR3(_vMin.x, _vMin.y, _vMin.z);
	pVtxCube[3].vTexUV = pVtxCube[3].vPos;	

	pVtxCube[4].vPos = D3DXVECTOR3(_vMin.x, _vMax.y, _vMax.z);
	pVtxCube[4].vTexUV = pVtxCube[4].vPos;

	pVtxCube[5].vPos = D3DXVECTOR3(_vMax.x, _vMax.y, _vMax.z);
	pVtxCube[5].vTexUV = pVtxCube[5].vPos;	

	pVtxCube[6].vPos = D3DXVECTOR3(_vMax.x, _vMin.y, _vMax.z);
	pVtxCube[6].vTexUV = pVtxCube[6].vPos;	

	pVtxCube[7].vPos = D3DXVECTOR3(_vMin.x, _vMin.y, _vMax.z);
	pVtxCube[7].vTexUV = pVtxCube[7].vPos;

	// 큐브의 8개의 정점을 뼈의 로컬행렬과 메쉬의 월드행렬을 곱해준 행렬로 이동 시켜준다.(월드영역으로 변환)
	for(int i = 0; i < 8; ++i)
	{
		D3DXVec3TransformCoord(&pVtxCube[i].vPos, &pVtxCube[i].vPos, &matWorld);
	}


	pClientBoxInfo->pVB->Unlock();


	m_pGraphicDev->SetStreamSource(0, pClientBoxInfo->pVB, 0, sizeof(Engine::VTXCUBE));
	m_pGraphicDev->SetFVF(Engine::VTXFVF_CUBE);

	m_pGraphicDev->SetIndices(pClientBoxInfo->pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
}

void CPlayer::Render_DamageBox()
{
	// 항등행렬로 월드행렬을 설정해준다.
	_matrix matDefaultWorld;
	D3DXMatrixIdentity(&matDefaultWorld);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matDefaultWorld);

	for(_int i = 0; i < BT_End-1; ++i)
	{
		const _matrix* m_pMatrix;
		m_pMatrix = m_pMeshCom->Get_FrameMatrix(m_pDamageBox[i]->pFrameName);	// 선택한 뼈의 행렬을 얻어온다.

		// 바운딩 박스의 행렬 = 뼈의 행렬 * 메쉬의 행렬 
		_matrix matWorld;
		matWorld = (*m_pMatrix) * m_pTransCom->m_matWorld ;

		Engine::VTXCUBE*			pVtxCube = NULL; 
		m_pDamageBox[i]->pVB->Lock(0, 0, (void**)&pVtxCube, 0);

		_vec3 _vMin = m_pDamageBox[i]->vMin;
		_vec3 _vMax = m_pDamageBox[i]->vMax;

		pVtxCube[0].vPos = D3DXVECTOR3(_vMin.x, _vMax.y, _vMin.z);
		pVtxCube[0].vTexUV = pVtxCube[0].vPos;		

		pVtxCube[1].vPos = D3DXVECTOR3(_vMax.x, _vMax.y, _vMin.z);
		pVtxCube[1].vTexUV = pVtxCube[1].vPos;		

		pVtxCube[2].vPos = D3DXVECTOR3(_vMax.x, _vMin.y, _vMin.z);
		pVtxCube[2].vTexUV = pVtxCube[2].vPos;	

		pVtxCube[3].vPos = D3DXVECTOR3(_vMin.x, _vMin.y, _vMin.z);
		pVtxCube[3].vTexUV = pVtxCube[3].vPos;	

		pVtxCube[4].vPos = D3DXVECTOR3(_vMin.x, _vMax.y, _vMax.z);
		pVtxCube[4].vTexUV = pVtxCube[4].vPos;

		pVtxCube[5].vPos = D3DXVECTOR3(_vMax.x, _vMax.y, _vMax.z);
		pVtxCube[5].vTexUV = pVtxCube[5].vPos;	

		pVtxCube[6].vPos = D3DXVECTOR3(_vMax.x, _vMin.y, _vMax.z);
		pVtxCube[6].vTexUV = pVtxCube[6].vPos;	

		pVtxCube[7].vPos = D3DXVECTOR3(_vMin.x, _vMin.y, _vMax.z);
		pVtxCube[7].vTexUV = pVtxCube[7].vPos;

		// 큐브의 8개의 정점을 뼈의 로컬행렬과 메쉬의 월드행렬을 곱해준 행렬로 이동 시켜준다.(월드영역으로 변환)
		for(int j = 0; j < 8; ++j)
		{
			D3DXVec3TransformCoord(&pVtxCube[j].vPos, &pVtxCube[j].vPos, &matWorld);
		}

		m_pDamageBox[i]->pVB->Unlock();

		m_pGraphicDev->SetStreamSource(0, m_pDamageBox[i]->pVB, 0, sizeof(Engine::VTXCUBE));
		m_pGraphicDev->SetFVF(Engine::VTXFVF_CUBE);
		m_pGraphicDev->SetIndices(m_pDamageBox[i]->pIB);
		m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
	}
}

#endif