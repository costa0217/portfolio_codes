#include "stdafx.h"
#include "RoyalGuard.h"

#include "Engine_Defines.h"
#include "Export_Function.h"

#include "Line2D.h"
#include "CullingMgr.h"

#include "Player.h"

#include "Pierceaura.h"

#include "SoundMgr.h"
#include "DamageFont.h"

#include "EffectMgr.h"
#include "PlayerBeShotUI.h"

#include "CollisionMgr.h"

_vec3	CRoyalGuard::m_vPos;

CRoyalGuard::CRoyalGuard(LPDIRECT3DDEVICE9 pGraphicDev)
: CMonster(pGraphicDev)
, m_bDamage(false)
, m_bDisHP(false)
, m_bAttack(false)
, m_bDie(false)
, m_iAttackPattern(0)
, m_bEffectOnce(false)
, m_iWalkPattern(0)
, m_fPatternTime(0.f)
, m_fDieTime(0.f)
, m_fDieOverTime(0.f)
{

}
CRoyalGuard::~CRoyalGuard(void)
{

}
HRESULT CRoyalGuard::Initialize(void)
{
	m_pEffect = Engine::Get_EffectHandle(L"Shader_Monster");
	if(NULL == m_pEffect)		return E_FAIL;

	m_pShadowEffect = Engine::Get_EffectHandle(L"Shader_Shadow");
	if(NULL == m_pShadowEffect)		return E_FAIL;

	if(FAILED(Add_Component()))
		return E_FAIL;

	//변수들 초기화
	m_pTransCom->m_vScale = _vec3(0.08f, 0.08f, 0.08f);
	m_pTransCom->m_vPosition = m_vPos;
	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = D3DXToRadian(90);

	m_iMonsterAnimationIndex = RoyalGuard::spawn;
	m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);

	// 	bInput = false;

	//바운딩박스 로드
	CMonster::Load_AttackBox(L"../Bin/Resources/Data/Box/Royal/Royal_AttackBox.dat");
	CMonster::Load_DamageBox(L"../Bin/Resources/Data/Box/Royal/Royal_DamageBox.dat");
	// Boss_HP UI

	// 패턴 로드
	Attack_Pattern();
	Walk_Pattern();

	// 애니메이션 싱크
	CMonster::Load_FootAnimationSync(L"../Bin/Resources/Data/Box/Royal/Royal_Sync.dat");

	Initialize_Min_Max_Radius();

	m_fMaxDamageValue = 500.f;
	m_fDownValue = 0.f;
	m_fMonsterDownValue = 0.f;

	m_vMoveMin		= _vec3(-7.f, 0.f, -7.f);
	m_vMoveMax		= _vec3(7.f, 10.f, 7.f);
	m_fMoveRadius	= D3DXVec3Length(&(m_vMoveMax - m_vMoveMin)) * m_pTransCom->m_vScale.x;

	m_tMonsterInfo.fAttack = 100.f;
	m_tMonsterInfo.fMaxHP = 20000.f;
	m_tMonsterInfo.fHP = m_tMonsterInfo.fMaxHP;
	m_tMonsterInfo.fDownValue = 5.f; 

	//싱크만큼 움직일거면 1 
	m_fDistanceSize = 1.f;

	m_fAttackDistance = 20.f;
	m_fFollowDistance = 35.f;

	m_eMonsterType = MT_Monster;
	return S_OK;
}
HRESULT CRoyalGuard::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	//Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	//Mesh Component	// 플레이어를 제외한 다이나믹메쉬들을 CDynamicMesh 로 로드할 것.
	pComponent = m_pMeshCom = (Engine::CDynamicMesh*)Engine::Clone_Resource(RESOURCE_STATIC, L"Mesh_Royal");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	m_vOriginalPos = m_pTransCom->m_vPosition;
	m_vAfterPos = m_pTransCom->m_vPosition;

	m_pMeshCom->SetLeftAndRihgtMatrix("ValveBiped_Bip01_L_Toe0", "ValveBiped_Bip01_R_Toe0");

	return S_OK;
}
_int CRoyalGuard::Update(const float& fTimeDelta)
{
	m_fTimeDelta		= fTimeDelta;
	m_fPatternTime += m_fTimeDelta;

	if( m_bMoveCollCheck == false)
		return -1;

	KeyInput();

	CMonster::CollisionCheckFunction();			//충돌에 대해 검사를 한다.
	Check_Monster_Satae();						//애니메이션, 시간, 특정 조건에 따라 몬스터의 상태 타입을 지속적으로 체크하여 바꾸어 주는 곳 => 이 상태에 따라 플레이어에서 충돌 검사를 건너 뛰거나 몬스터가 다운 상태일때 데미지가 더 들어가거나 다시 다운애니메이션을 Set 안하도록 한다.
	AnimationPattern_Immediately_Change();		//충돌에 따라 몬스터의 애니메이션을 즉각적으로 바꾼다. 충돌 방향은 추후 Monster class에서 공통적으로 만들던가 충돌, Math Mgr에서 만들어 활용, 몬스터의 타입을 만들어서 특정 모션이 공격중일때는 충돌 당해도 데미지만 받게 해야 한다.
	Check_PlayerAttack_AccordingToHP();			//충돌에 따른 몬스터의 HP관리및 피격 이펙트 생성
	BattleAnimationPattern();					//플레이어의 공격에 충돌도 안하고 애니메이션이 끝나면 만들어 놓은 알고리즘을 토대로 몬스터의 AI가 돌아가도록 한다.
	AnimationTimeChange();						//기본적으로 m_fAnimationSpeed의 속도는 2.f이지만 슬로우 상태나 특정모션의 속도를 늦출때 여기서 한다.

	//Check_Sync(); // 이거를 Render로 옮겨서 싱크 안맞는거 해결한거같음.
	SoundPlayer();

	CGameObject::Update(fTimeDelta);	
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_SHADOW, this);

	return 0;
}
void CRoyalGuard::Render(void)
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

	Check_Sync();
}
void CRoyalGuard::Render_Shadow(void)
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
CRoyalGuard* CRoyalGuard::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 vPos)
{
	CRoyalGuard*	pMonster = new CRoyalGuard(pGraphicDev);

	m_vPos = vPos;

	if( FAILED(pMonster->Initialize()) )
	{
		MSG_BOX("CRoyalGuard Create Failed");
		::Safe_Release(pMonster);
	}
	return pMonster;
}
void CRoyalGuard::KeyInput()
{
}
void CRoyalGuard::Set_ContantTable(void)
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
void CRoyalGuard::Check_Monster_Satae(void)
{

}
void CRoyalGuard::AnimationPattern_Immediately_Change(void)
{
	if(true == m_bDie)
	{
		if(m_pMeshCom->Check_EndPeriod())
		{
			printf("죽음\n");

			m_bMoveCollCheck = false;
		}
	}
	else
	{
		if( m_pMeshCom->Check_EndPeriod())
		{
			m_bEffectOnce = false;

			Check_Rotation();
			if(RoyalGuard::spawn== m_iMonsterAnimationIndex)
			{
				m_iMonsterAnimationIndex = RoyalGuard::idle;
				m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
			}
			else if(RoyalGuard::down == m_iMonsterAnimationIndex)
			{
				//m_bDie = true;
			}
			else if(RoyalGuard::runN == m_iMonsterAnimationIndex)
			{
				_vec3 vPlayerPos = CPlayer::GetInstance()->GetPos();
				float fDistance = D3DXVec3Length( &(vPlayerPos - m_pTransCom->m_vPosition) );
				if(fDistance >= 10.f)
				{
					m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
				}
			}
		}
		if(RoyalGuard::spawn == m_iMonsterAnimationIndex)
		{
			if(m_pMeshCom->Check_EndPeriodMulTime(0.3f) && !m_pMeshCom->Check_EndPeriodMulTime(0.4f))
			{
				if(false == m_bEffectOnce)
				{
					//CSoundMgr::GetInstance()->PlayEffect(SOUND::step_lv3); 소리 이상해서 일단 뺏어 보윤아
					CEffectMgr::GetInstance()->Play_Effect(L"DustBig", m_pTransCom->m_vPosition + _vec3(-2.f, 0.f, 2.f));
					CEffectMgr::GetInstance()->Play_Effect(L"DustBig", m_pTransCom->m_vPosition + _vec3(0.f, 0.f, 2.f));
					CEffectMgr::GetInstance()->Play_Effect(L"DustBig", m_pTransCom->m_vPosition + _vec3(2.f, 0.f, 2.f));
					m_bEffectOnce = true;
				}
			}
			else
				m_bEffectOnce = false;
		}
	}
	
}
void CRoyalGuard::Check_PlayerAttack_AccordingToHP(void)
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

		_float	m_fPlayerAttack = CPlayer::GetInstance()->GetPlayerAttackValue() + rand()%100;
		m_tMonsterInfo.fHP -= m_fPlayerAttack;
		//오버킬 처리
		float MonsterHp = m_tMonsterInfo.fHP;	//몬스터 오버킬을 위해 임시로 선언 
		m_tMonsterInfo.fHP -= CPlayer::GetInstance()->GetPlayerAttackValue();
	
		if(m_tMonsterInfo.fHP <= 0.f)
		{
			m_tMonsterInfo.fHP = 0.f;

			if((CPlayer::GetInstance()->GetPlayerAttackValue() - MonsterHp) <= 100)//혹시 100보다 작을때는 문제가 생기니 강제 예외처리함
			{
				CDamageFont::Create(m_pGraphicDev, DFONT_TYPE_OVERKILL, DFONT_OBJ_MONSTER, (int)(m_fPlayerAttack - MonsterHp + 100), m_vCollisionPos);	//몬스터가 받은 데미지를 띄운다.
			}
			else
				CDamageFont::Create(m_pGraphicDev, DFONT_TYPE_OVERKILL, DFONT_OBJ_MONSTER, (int)(m_fPlayerAttack - MonsterHp), m_vCollisionPos);	//몬스터가 받은 데미지를 띄운다.
		}
		else
		{
			CDamageFont::Create(m_pGraphicDev, DFONT_TYPE_DAMAGE, DFONT_OBJ_MONSTER, (int)m_fPlayerAttack, m_vCollisionPos);	//몬스터가 받은 데미지를 띄운다.
		}
		printf("몬스터 체력 == %f\n", m_tMonsterInfo.fHP);

	
		CPlayer::GetInstance()->Player_AttackSuccess_InfoChange();
		CEffectMgr::GetInstance()->Play_Effect(L"Player_Slash_Effect", m_pTransCom->m_vPosition + _vec3(0.f, m_vCollisionPos.y, 0.f), false, true, _vec3(3.f, 15.f, 0.f), _vec3(m_fAngleX, 0.f, m_fAngleZ), 0.f, false, 0.f, NULL);

		CPlayerBeShotUI::GetInstance()->PlayerBeShotUI();

		if(MS_Super != m_eMonsterState)
		{
			CEffectMgr::GetInstance()->Play_Effect(L"Player_Slash_Effect", m_pTransCom->m_vPosition + _vec3(0.f, m_vCollisionPos.y, 0.f), false, true, _vec3(3.f, 15.f, 0.f), _vec3(m_fAngleX, 0.f, m_fAngleZ), 0.f, false, 0.f, NULL);

			_vec3 m_vPlayerPos = CPlayer::GetInstance()->GetPos();
			DamageDirectionType eType = CMathMgr::GetInstance()->DamageDirectionDecision(m_vPlayerPos, m_pTransCom);


			switch(eType)
			{
			case DDT_FRONT:  
				printf("DDT_FRONT\n");
				m_pMeshCom->Set_AnimationSet(RoyalGuard::vampire_damage_weak_back);
				break;
			case DDT_BACK:
				printf("DDT_BACK\n");
				m_pMeshCom->Set_AnimationSet(RoyalGuard::vampire_damage_weak_bottom);
				break;
			case DDT_FRONT_LEFT:
				printf("DDT_FRONT_LEFT\n");
				m_pMeshCom->Set_AnimationSet(RoyalGuard::vampire_damage_weak_back);
				break;
			case DDT_BACK_LEFT:
				printf("DDT_BACK_LEFT\n");
				m_pMeshCom->Set_AnimationSet(RoyalGuard::vampire_damage_weak_bottom);
				break;
			case DDT_FRONT_RIGHT:
				printf("DDT_FRONT_RIGHT\n");
				m_pMeshCom->Set_AnimationSet(RoyalGuard::vampire_damage_weak_back);
				break;
			case DDT_BACK_RIGHT:
				printf("DDT_BACK_RIGHT\n");
				m_pMeshCom->Set_AnimationSet(RoyalGuard::vampire_damage_weak_bottom);
				break;
			}
		}
	}
	else
		m_bDisHP = false;

	if(false == m_bDie && m_tMonsterInfo.fHP <= 0)
	{
		m_iMonsterAnimationIndex = RoyalGuard::down;
		m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
		SoundEffectPlay(SOUND::vampire_dying, 0.0f, 0, 2.f);
		m_bDie = true;
	}
}
void CRoyalGuard::BattleAnimationPattern(void)
{
	if(false == m_bDie && m_pMeshCom->Check_EndPeriod())
	{
		Check_AttackRange();
	}
}
void CRoyalGuard::AnimationTimeChange(void)
{
	m_fRenderTimeDelta	= m_fTimeDelta * m_fAnimationSpeed * 1.f;
}
void CRoyalGuard::Check_Sync()
{
	boost::unordered_map<int, vector<FOOTANISYNC*>>::iterator iter = m_mapAniSync.find(m_iMonsterAnimationIndex);
	if( iter != m_mapAniSync.end() )
	{
		vector<FOOTANISYNC*>::iterator iter_vec = iter->second.begin();
		vector<FOOTANISYNC*>::iterator iter_end = iter->second.end();

		for( ; iter_vec != iter_end; ++iter_vec )
		{
			if(m_pMeshCom->Check_EndPeriodMulTime((*iter_vec)->fStartTime) && !m_pMeshCom->Check_EndPeriodMulTime((*iter_vec)->fEndTime))
			{
				CMonster::MonsterMoveFunctionTwo((*iter_vec)->bCompulsion, (*iter_vec)->bLeftFoot, (*iter_vec)->bFront);
			}
		}
	}
}
void CRoyalGuard::SoundPlayer(void)
{
	if(m_pMeshCom->Get_CurrentAniIdx() != m_iAnimationSoundCheck || m_pMeshCom->Check_EndPeriod())
	{
		m_iAnimationSoundCheck = m_pMeshCom->Get_CurrentAniIdx();
		ZeroMemory(m_bSoundPlay, sizeof(_bool) * 10);
	}

	if(RoyalGuard::vampire_attack_double == m_iMonsterAnimationIndex)
	{
		SoundEffectPlay(SOUND::Swing_MetalStrong, 0.45f, 0, 2);
		SoundEffectPlay(SOUND::Swing_MetalStrong, 0.57f, 1, 2);
	}
	else if(RoyalGuard::vampire_attack_pierce == m_iMonsterAnimationIndex)
	{
		SoundEffectPlay(SOUND::vampire_rapier_pierce, 0.54f, 0, 2);
	}
}

void CRoyalGuard::Check_AttackRange()
{
	_vec3 vPlayerPos = CPlayer::GetInstance()->GetPos();

	float fDistance = D3DXVec3Length( &(vPlayerPos - m_pTransCom->m_vPosition) );

	if( fDistance < m_fFollowDistance )				// 따라가는 범위 안
	{
		if( fDistance <= m_fAttackDistance )			// 공격범위 안
		{
			m_tMonsterInfo.fAttack = 100 + rand() % 200;
			if(m_iAttackPattern >= (_int)m_vecAttackPattern.size())
				m_iAttackPattern = 0;
			if(m_iMonsterAnimationIndex == m_vecAttackPattern[m_iAttackPattern])
			{
				m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
			}
			else
			{
				m_iMonsterAnimationIndex = m_vecAttackPattern[m_iAttackPattern];
				m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
			}
			++m_iAttackPattern;
		}
		else// 공격범위 밖 & 따라가는 범위 안
		{
			if(m_iMonsterAnimationIndex == m_vecWalkPattern[m_iWalkPattern])
			{
				m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
			}
			else
			{
				m_iMonsterAnimationIndex = m_vecWalkPattern[m_iWalkPattern];
				m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
			}
			++m_iWalkPattern;
			if(m_iWalkPattern >= (_int)m_vecWalkPattern.size())
			{
				_int i = ((_int)m_fPatternTime) % 3;
				m_iWalkPattern = i * 3;
			}
		}
	}
	else
	{
		if(m_iMonsterAnimationIndex == RoyalGuard::runN)
		{
			m_pMeshCom->Set_AnimationSetSameFinish(m_iMonsterAnimationIndex);
		}
		else
		{
			m_iMonsterAnimationIndex = RoyalGuard::runN;
			m_pMeshCom->Set_AnimationSet(m_iMonsterAnimationIndex);
		}
	}

}
void CRoyalGuard::Check_Rotation()
{
	_vec3 vDir = CPlayer::GetInstance()->GetPos() - m_pTransCom->m_vPosition;
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

}
void CRoyalGuard::Attack_Pattern()
{
	m_vecAttackPattern.push_back(RoyalGuard::vampire_attack_pierce);
	m_vecAttackPattern.push_back(RoyalGuard::vampire_attack_double);
	m_vecAttackPattern.push_back(RoyalGuard::vampire_attack_double);
	m_vecAttackPattern.push_back(RoyalGuard::vampire_attack_pierce);
	m_vecAttackPattern.push_back(RoyalGuard::vampire_attack_double);
	m_vecAttackPattern.push_back(RoyalGuard::vampire_attack_pierce);
	m_vecAttackPattern.push_back(RoyalGuard::vampire_attack_pierce);
	m_vecAttackPattern.push_back(RoyalGuard::vampire_attack_double);
}
void CRoyalGuard::Walk_Pattern()
{
	m_vecWalkPattern.push_back(RoyalGuard::walkE);	//0
	m_vecWalkPattern.push_back(RoyalGuard::walkE);
	m_vecWalkPattern.push_back(RoyalGuard::idle);

	m_vecWalkPattern.push_back(RoyalGuard::walkW);	//3
	m_vecWalkPattern.push_back(RoyalGuard::walkW);
	m_vecWalkPattern.push_back(RoyalGuard::idle);
	
	m_vecWalkPattern.push_back(RoyalGuard::walkN);	//6
	m_vecWalkPattern.push_back(RoyalGuard::walkN);
	m_vecWalkPattern.push_back(RoyalGuard::idle);
}
_ulong CRoyalGuard::Release(void)
{
	_ulong dwRefCnt = CMonster::Release();

	if(0 == dwRefCnt)
	{
		m_vecAttackPattern.clear();
		{
			list<CMonster*>::iterator iter = CCollisionMgr::GetInstance()->Get_MonsterList()->begin();
			list<CMonster*>::iterator iterEnd = CCollisionMgr::GetInstance()->Get_MonsterList()->end();

			for(; iter != iterEnd; )
			{
				if((*iter) == this)
					iter = CCollisionMgr::GetInstance()->Get_MonsterList()->erase(iter);
				else
					++iter;
			}
		}

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





