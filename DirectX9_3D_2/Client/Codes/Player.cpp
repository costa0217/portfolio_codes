#include "stdafx.h"
#include "Player.h"
#include "NaviMesh.h"
#include "Export_Function.h"
#include "Boss.h"

#include "Layer.h"
#include "Impact.h"
#include "GroundHit.h"
#include "BloodBlack.h"
#include "Blood.h"
#include "Wang.h"
#include "Bloody.h"

#include "SDevice.h"


#define TrailFrame 60
CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
: CLandObject(pGraphicDev)
, m_pMeshCom(NULL)
, m_pHpPtCom(NULL)
, m_pBossTransCom(NULL)
, m_pEffect(NULL)
, m_fTimeDelta(0.f)
, m_bKeyPress(false)
, m_iAniIdx(23)
, m_fJumpTime(0.f)
, m_bRunJump (false)
, m_iAttackAniIdx(0)
, m_pSwordEndPos(NULL)
, m_vSwordStartPos(0.f, 0.f, 0.f)
, m_vSwordEndPos(0.f, 0.f, 0.f)
, m_pmatFrameMat(NULL)
//////////////////////////////////////////////////
, m_iFrame(0)
, m_fFixTime(0.4f)
, m_fJumpPos(0.f)
, m_bJumpFall(false)
, m_bJumpDmg(false) // 점프도중 맞았을때
, m_DiagRun(false)
, m_fSpeed(0.f)
, m_bDMG(false)
, m_iSkillIndex(0)
//////////////////////////////////////////////////
, m_vAtkCamPos(0.f, 0.f, 0.f)
, m_vDftCamPos(0.f, 0.f, 0.f)
, m_vNuckCamPos(0.f, 0.f, 0.f)
, m_matLocalAtkBone(NULL)
, m_matLocalDftBone(NULL)
, m_matLocalNuckBone(NULL)
//////////////////////////////////////////////////
, m_bFushing(false)
, m_bFushingJumping(false)
, m_fAccelSpeed(10.f)
, m_fAccelSpeed2(2.f)
, m_fSkillStartTime(0.f)
, m_fSkillStartTime2(0.f)
, m_bSlow(false)
, m_fSlowTime(0.f)
///////////////////////////////////////////////////
, m_fSkillAccTime(0.f)
, m_bSound(false)
{
	ZeroMemory(m_bPattern, sizeof(bool) * 4);
}

CPlayer::~CPlayer(void)
{
}
_bool CPlayer::GetStateJump()
{
	if(m_eAniStat == STATE_JUMP)
		return true;
	else
		return false;
}
void CPlayer::Set_CamposToBoneMatrix(void)
{
	m_matLocalAtkBone = m_pMeshCom->Get_FrameMatrix("BN_Hair_B_4");
	m_matLocalDftBone = m_pMeshCom->Get_FrameMatrix("BN_Hair_B_2");
	//m_matLocalNuckBone = m_pMeshCom->Get_FrameMatrix("BN_Weapon_R");

	memcpy(&m_vAtkCamPos, &m_matLocalAtkBone->m[3][0], sizeof(_vec3));
	memcpy(&m_vDftCamPos, &m_matLocalDftBone->m[3][0], sizeof(_vec3));
	//memcpy(&m_vNuckCamPos, &m_matLocalNuckBone->m[3][0], sizeof(_vec3));
}
HRESULT CPlayer::Initialize(void)
{
	if(FAILED(CLandObject::Initialize()))
		return E_FAIL;

	m_pEffect = Engine::Get_EffectHandle(L"Shader_Meshes");
	if(NULL == m_pEffect)		return E_FAIL;		

	if(FAILED(Add_Component()))	return E_FAIL;

	m_eObjType = TYPE_NAVI;

	m_pMeshCom->Set_AnimationSet(m_iAniIdx);

	m_pTransCom->m_vScale		= _vec3(0.06f, 0.06f, 0.06f);
	m_pTransCom->m_vPosition	= _vec3(69.f, 0.f, 50.f);

	m_fSpeed = 5.f;

	m_dwNaviIndex = m_pNaviMesh->FindIndex(m_pTransCom->m_vPosition);
	Set_CamposToBoneMatrix();
	
	return S_OK;
}

HRESULT CPlayer::Add_Component(void)
{
	Engine::CComponent*			pComponent = NULL;

	// For.Transform Component
	pComponent = m_pTransCom = Engine::CTransform::Create();
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Transform", pComponent));

	// For.Mesh Component
	pComponent = m_pMeshCom = (Engine::CDynamicMesh*)Engine::Clone_Resource(RESOURCE_STAGE, L"Mesh_Player");
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_Mesh", pComponent));

	pComponent = m_pHpPtCom = Engine::CHPoint::Create(200);
	if(NULL == pComponent)
		return E_FAIL;
	m_mapComponent.insert(MAPCOMPONENT::value_type(L"Com_HP", pComponent));
		
	return S_OK;
}
/***** 업데이트 *******************************************************************/
_int CPlayer::Update(const _float& fTimeDelta)
{
	m_fTimeDelta = fTimeDelta;


	StateSet_Slow();

	if(m_bPattern[m_iSkillIndex])
	{
		m_fSkillAccTime += m_fTimeDelta;
		if(m_fSkillAccTime > 0.35f)
		{
			if(m_iSkillIndex == 0)
			{
				m_dwNaviIndex = m_pNaviMesh->MovingNaviMesh(&m_pTransCom->m_vPosition, &(m_pTransCom->m_vDir), m_dwNaviIndex, m_fSpeed + 15.f - (11.f * m_fSkillAccTime), fTimeDelta);;
			}
		}
		if(m_fSkillAccTime > 0.25f)
		{
			if(!m_bSound)
			{
				CSDevice::GetInstance()->SoundPlay(SD_Player_Skill1 + m_iSkillIndex, 0);
				m_bSound = true;
			}
		}
	}
	else
	{
		m_fSkillAccTime = 0.f;
	}

	if(m_pBossTransCom == NULL)
	{
		m_pBossTransCom = (Engine::CTransform*)Engine::Get_Component(L"Com_Transform", L"GameLogic", L"Boss"); 	
	}
	memcpy(&m_vAtkCamPos, &m_matLocalAtkBone->m[3][0], sizeof(_vec3));
	memcpy(&m_vDftCamPos, &m_matLocalDftBone->m[3][0], sizeof(_vec3));
	
	float fMeshAngle = acos(D3DXVec3Dot(&m_pTransCom->m_vDir, &g_vLook));
	if(m_pTransCom->m_vDir.z > g_vLook.z)	fMeshAngle = D3DX_PI * 2 - fMeshAngle;

	m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] = fMeshAngle;

	if(m_bDMG && m_eAniStat != STATE_DMG)
	{
		if(m_eAniStat == STATE_JUMP)
		{
			m_bJumpDmg	= true;
		}
		m_eAniStat = STATE_DMG;
		m_pMeshCom->Set_AnimationSet(7);
		m_fFixTime = 0.f;
		Create_Blood(_vec3(0.f, 0.f, 0.f));
		Create_Bloody();

		CSDevice::GetInstance()->SoundPlay(SD_Player_DMG1 + rand()%2, 0);

		if(m_pHpPtCom->m_iHP > 0)
			m_pHpPtCom->m_iHP -= 10;
	}
	if(!m_bDMG && !m_bFushingJumping)
		Check_KeyState(fTimeDelta);

	if(true == m_pMeshCom->Check_EndPeriod(m_fFixTime))
	{
		if(!m_bFushingJumping)
		{
			if(!m_bKeyPress	&& !m_DiagRun)
			{
				m_pMeshCom->Set_AnimationSet(23);
				m_eAniStat = STATE_IDLE;
				m_iAttackAniIdx = 0;
			}
			if(m_bDMG)
			{
				m_bDMG		= false;
				m_pMeshCom->Set_AnimationSet(23);
				m_eAniStat = STATE_IDLE;
				m_iAttackAniIdx = 0;
				
			}

			if(m_bPattern[m_iSkillIndex])
			{
				m_bPattern[m_iSkillIndex] = false;
				//m_fSkillTime = 0.f;		
			}
		}
		if(((CBoss*)m_pBoss)->Get_State() != CBoss::Boss_SK_Firing_02 )
		{
			m_bFushing = false;
		}
	}

	Engine::Add_RenderGroup(Engine::CRenderer::RENDER_ZSORT, this);
	CLandObject::Update(fTimeDelta);

	Fire_Skill4();		// 척력 스킬
	Fire_Skill2();		// 인력 스킬

	Check_Jumping(fTimeDelta);
	Create_Trail();
	return 0;
}
/***** 렌더 *******************************************************************/
void CPlayer::Render(void)
{
	if(NULL == m_pEffect)	return;
	
	Set_ContantTable();
	m_pMeshCom->Move_Frame(m_fTimeDelta);

#ifdef _DEBUG
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_pTransCom->m_matWorld);
#endif	 

	m_pMeshCom->Render_MeshForShader(m_pEffect, true);
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer*		pGameObject = new CPlayer(pGraphicDev);

	if(FAILED(pGameObject->Initialize()))
	{
		MSG_BOX("CPlayer Create Faild");
		::Safe_Release(pGameObject);
	}
	return pGameObject;
}

void CPlayer::Set_ContantTable(void)
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

	D3DXMATRIX matCamera;
	D3DXMatrixInverse(&matCamera, NULL, &matView);
	_vec4 vCameraPos = _vec4(matCamera._41, matCamera._42, matCamera._43, 1.f);

	m_pEffect->SetVector("g_vLightDir", &_vec4(pLightInfo->Direction, 0.f));

	m_pEffect->SetVector("g_vLightDiffuse", &_vec4((_float*)&pLightInfo->Diffuse));
	m_pEffect->SetVector("g_vLightAmbient", &_vec4((_float*)&pLightInfo->Ambient));	
	m_pEffect->SetVector("g_vLightSpecular", &_vec4((_float*)&pLightInfo->Specular));	

	m_pEffect->SetVector("g_vWorldCamPos", &vCameraPos);
}

void CPlayer::Check_KeyState(const _float& fTimeDelta)
{
	m_bKeyPress = false;
	m_DiagRun	= false;
	m_fFixTime	= 0.4f;

	if(m_bPattern[m_iSkillIndex]) return;

	if(Engine::GetDIMouseState(Engine::CInput::DIM_LBUTTON))
	{
		if(true == m_pMeshCom->Check_EndPeriod()
			|| m_eAniStat == STATE_IDLE
			|| m_eAniStat == STATE_RUN)
		{
			if(m_iAttackAniIdx > 2)
			{
				m_iAttackAniIdx = 0;
				return;
			}
			CSDevice::GetInstance()->SoundPlay(SD_Player_Attack1 + m_iAttackAniIdx, 0);

			Create_Impact(m_iAttackAniIdx);
			m_pMeshCom->Set_AnimationSet(22 - m_iAttackAniIdx);
			m_eAniStat = STATE_ATTACK;
			++m_iAttackAniIdx;
			m_dwNaviIndex = m_pNaviMesh->MovingNaviMesh(&m_pTransCom->m_vPosition, &(m_pTransCom->m_vDir), m_dwNaviIndex, m_fSpeed + 20.f, fTimeDelta);;

			
		}		
	}
	if (Engine::GetDIKeyState(DIK_W) & 0x80)	//전진
	{
		if(m_eAniStat == STATE_ATTACK)	return;

		m_dwNaviIndex = m_pNaviMesh->MovingNaviMesh(&m_pTransCom->m_vPosition, &m_pTransCom->m_vDir, m_dwNaviIndex, m_fSpeed, fTimeDelta);
		m_bKeyPress = true;

		if (Engine::GetDIKeyState(DIK_A) & 0x80)
		{
			m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] += D3DXToRadian(-160.0f) * fTimeDelta;

			_matrix matRotY;
			D3DXMatrixRotationY(&matRotY, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);
			D3DXVec3TransformNormal(&m_pTransCom->m_vDir, &g_vLook, &matRotY);
		}

		if (Engine::GetDIKeyState(DIK_D) & 0x80)
		{
			m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y] += D3DXToRadian(160.0f) * fTimeDelta;

			_matrix matRotY;
			D3DXMatrixRotationY(&matRotY, m_pTransCom->m_fAngle[Engine::CTransform::ANGLE_Y]);
			D3DXVec3TransformNormal(&m_pTransCom->m_vDir, &g_vLook, &matRotY);
		}

		
		if(m_eAniStat != STATE_JUMP	&& !m_bRunJump )
		{
			if(m_eAniStat == STATE_LAND)
			{
				if(m_bRunJump)	return;

				m_pMeshCom->Set_AnimationSet(10);
				m_bRunJump = true;
			}
			m_pMeshCom->Set_AnimationSet(33);
			m_eAniStat = STATE_RUN;
		}
		else
			m_pMeshCom->Set_AnimationSet(30);

		if(!CSDevice::GetInstance()->SoundPlaying(SD_PlayerRun2) && m_eAniStat != STATE_JUMP)
		{
			CSDevice::GetInstance()->SoundPlay(SD_PlayerRun2, 0);
		}
	}
	else if (Engine::GetDIKeyState(DIK_S) & 0x80)
	{
		if(m_eAniStat == STATE_ATTACK)	return;	// 어텍도중 이동 x
		
		m_dwNaviIndex = m_pNaviMesh->MovingNaviMesh(&m_pTransCom->m_vPosition, &(-m_pTransCom->m_vDir), m_dwNaviIndex, m_fSpeed - 2, fTimeDelta);
		m_bKeyPress = true;

		if(m_eAniStat == STATE_JUMP)	return;		

		m_pMeshCom->Set_AnimationSet(32);	// 백스텝
		m_eAniStat = STATE_RUNBACK;

		if(!CSDevice::GetInstance()->SoundPlaying(SD_PlayerRun2) && m_eAniStat != STATE_JUMP)
		{
			CSDevice::GetInstance()->SoundPlay(SD_PlayerRun2, 0);
		}
	}
	else if (Engine::GetDIKeyState(DIK_A) & 0x80)
	{
		if(m_eAniStat == STATE_JUMP)	return;
		if(m_eAniStat == STATE_ATTACK)	return;
		_vec3			vRight(0.f, 0.f, 0.f);

		memcpy(&vRight, &m_pTransCom->m_matWorld.m[0][0], sizeof(_vec3));
		D3DXVec3Normalize(&vRight, &vRight);
		vRight = _vec3(-vRight.z, 0.f, vRight.x);
		m_dwNaviIndex = m_pNaviMesh->MovingNaviMesh(&m_pTransCom->m_vPosition, &vRight, m_dwNaviIndex, m_fSpeed - 2, fTimeDelta);
		m_bKeyPress = true;

		m_fFixTime = 0.f;
		m_pMeshCom->Set_AnimationSet(18);
		m_eAniStat = STATE_RUN;

		if(!CSDevice::GetInstance()->SoundPlaying(SD_PlayerRun2) && m_eAniStat != STATE_JUMP)
		{
			CSDevice::GetInstance()->SoundPlay(SD_PlayerRun2, 0);
		}
	}		
	else if (Engine::GetDIKeyState(DIK_D) & 0x80)
	{
		if(m_eAniStat == STATE_JUMP)	return;
		if(m_eAniStat == STATE_ATTACK)	return;
		_vec3			vRight(0.f, 0.f, 0.f);

		memcpy(&vRight, &m_pTransCom->m_matWorld.m[0][0], sizeof(_vec3));
		D3DXVec3Normalize(&vRight, &vRight);
		vRight = _vec3(-vRight.z, 0.f, vRight.x);
		m_dwNaviIndex = m_pNaviMesh->MovingNaviMesh(&m_pTransCom->m_vPosition, &-vRight, m_dwNaviIndex, m_fSpeed - 2, fTimeDelta);
		m_bKeyPress = true;

		m_fFixTime = 0.f;
		m_pMeshCom->Set_AnimationSet(16);
		m_eAniStat = STATE_RUN;

		if(!CSDevice::GetInstance()->SoundPlaying(SD_PlayerRun2) && m_eAniStat != STATE_JUMP)
		{
			CSDevice::GetInstance()->SoundPlay(SD_PlayerRun2, 0);
		}
	}
	else if (Engine::GetDIKeyState(DIK_Q) & 0x80)
	{
		if(m_eAniStat == STATE_JUMP)	return;
		if(m_eAniStat == STATE_ATTACK)	return;
		_vec3			vRight(0.f, 0.f, 0.f);

		memcpy(&vRight, &m_pTransCom->m_matWorld.m[0][0], sizeof(_vec3));
		D3DXVec3Normalize(&vRight, &vRight);
		vRight = _vec3(-vRight.z, 0.f, vRight.x) + m_pTransCom->m_vDir;
		D3DXVec3Normalize(&vRight, &vRight);
		m_dwNaviIndex = m_pNaviMesh->MovingNaviMesh(&m_pTransCom->m_vPosition, &vRight, m_dwNaviIndex, m_fSpeed, fTimeDelta);
		m_DiagRun = true;

		m_fFixTime = -0.2f;
		m_pMeshCom->Set_AnimationSet(17);
		m_eAniStat = STATE_RUN;

		if(!CSDevice::GetInstance()->SoundPlaying(SD_PlayerRun2) && m_eAniStat != STATE_JUMP)
		{
			CSDevice::GetInstance()->SoundPlay(SD_PlayerRun2, 0);
		}
	}		
	else if (Engine::GetDIKeyState(DIK_E) & 0x80)
	{
		if(m_eAniStat == STATE_JUMP)	return;
		if(m_eAniStat == STATE_ATTACK)	return;
		_vec3			vRight(0.f, 0.f, 0.f);

		memcpy(&vRight, &m_pTransCom->m_matWorld.m[0][0], sizeof(_vec3));
		D3DXVec3Normalize(&vRight, &vRight);
		vRight = _vec3(vRight.z, 0.f, -vRight.x) + m_pTransCom->m_vDir;
		D3DXVec3Normalize(&vRight, &vRight);
		m_dwNaviIndex = m_pNaviMesh->MovingNaviMesh(&m_pTransCom->m_vPosition, &vRight, m_dwNaviIndex, m_fSpeed, fTimeDelta);
		m_DiagRun = true;

		m_fFixTime = -0.2f;
		m_pMeshCom->Set_AnimationSet(15);
		m_eAniStat = STATE_RUN;

		if(!CSDevice::GetInstance()->SoundPlaying(SD_PlayerRun2) && m_eAniStat != STATE_JUMP)
		{
			CSDevice::GetInstance()->SoundPlay(SD_PlayerRun2, 0);
		}
	}

	if(Engine::GetDIKeyState(DIK_SPACE) & 0x80)
	{
		if(m_eAniStat == STATE_JUMP)	return;
		m_pMeshCom->Set_AnimationSet(11);
		m_eAniStat = STATE_JUMP;

		CSDevice::GetInstance()->SoundPlay(SD_Player_Jump, 0);
	}

	if(m_eAniStat != STATE_JUMP)
	{
		//스킬
		if(Engine::GetDIKeyState(DIK_1) & 0x80)
		{
			m_iSkillIndex = 0;
			if(!m_bPattern[m_iSkillIndex])
			{
				m_pMeshCom->Set_AnimationSet(4);
				m_eAniStat = STATE_SKILL;
				m_bPattern[m_iSkillIndex] = true;

				//CSDevice::GetInstance()->SoundPlay(SD_Player_Skill1, 0);

				Create_Impact(0);
				m_bSound = false;
			}		
		}
		if(Engine::GetDIKeyState(DIK_2) & 0x80)
		{
			m_iSkillIndex = 1;
			if(!m_bPattern[m_iSkillIndex])
			{
				m_pMeshCom->Set_AnimationSet(3);
				m_eAniStat = STATE_SKILL;
				m_bPattern[m_iSkillIndex] = true;

				Create_GroundHit();

				//CSDevice::GetInstance()->SoundPlay(SD_Player_Skill2, 0);
				m_bSound = false;
			}				
		}
		if(Engine::GetDIKeyState(DIK_3) & 0x80)
		{
			m_iSkillIndex = 2;
			if(!m_bPattern[m_iSkillIndex])
			{
				m_pMeshCom->Set_AnimationSet(2);
				m_eAniStat = STATE_SKILL;
				m_bPattern[m_iSkillIndex] = true;

				//CSDevice::GetInstance()->SoundPlay(SD_Player_Skill3, 0);
				m_bSound = false;
			}				
		}
		if(Engine::GetDIKeyState(DIK_4) & 0x80)
		{
			m_iSkillIndex = 3;
			if(!m_bPattern[m_iSkillIndex])
			{
				m_pMeshCom->Set_AnimationSet(1);
				m_eAniStat = STATE_SKILL;
				m_bPattern[m_iSkillIndex] = true;

				/*CSDevice::GetInstance()->SoundPlay(SD_Player_Skill4, 0);*/
				m_bSound = false;
			}				
		}
	}	
}

_ulong CPlayer::Release(void)
{
	_ulong dwRefCnt = CLandObject::Release();

	if(0 == dwRefCnt)
	{	
		delete this;		 
	}
	return dwRefCnt;
}

void CPlayer::SetDirection(_vec3 vDir)
{
	m_pTransCom->m_vDir = vDir;
}

void CPlayer::Jump_Player(void)
{
	m_fJumpPos			= 0.3f * m_fJumpTime * m_fJumpTime;
	float fJumpPower	= 0.1f;

	fJumpPower > m_fJumpPos ? m_bJumpFall = false : m_bJumpFall = true;
	
	m_pTransCom->m_vPosition.y += fJumpPower - m_fJumpPos;
}
void CPlayer::Check_Jumping(const _float& fTimeDelta)
{
	if(m_bJumpFall)
	{
		const _vec3 vCamparePos = m_pTransCom->m_vPosition;
		if(0 <= m_pNaviMesh->Compute_Height(vCamparePos, m_dwNaviIndex) - m_pTransCom->m_vPosition.y)
		{
			m_bJumpFall = false;
		 	m_eAniStat  = STATE_LAND;
			m_pTransCom->m_vPosition.y = m_pNaviMesh->Compute_Height(vCamparePos, m_dwNaviIndex);
			m_bJumpDmg = false;
			m_bFushingJumping = false;
		}
	}
	if(!m_bJumpFall && m_eAniStat != STATE_JUMP)
		m_pNaviMesh->Set_Jump(false);

	if(m_eAniStat == STATE_LAND
		&& !m_bRunJump)
	{
		m_pMeshCom->Set_AnimationSet(12);
		m_eAniStat = STATE_IDLE;
		m_bJumpDmg = false;

		if(m_bFushing)	//2번스킬 푸싱 끝
		{
			m_bFushingJumping =false;
			m_fAccelSpeed = 10.f;
			m_fSkillStartTime = 0.f;
		}
	}

	if(m_eAniStat == STATE_JUMP
		|| m_bJumpDmg)
	{
		m_fJumpTime += fTimeDelta;
		m_pNaviMesh->Set_Jump(true);
		Jump_Player();	
	}
	else
	{
		m_fJumpTime = 0.f;
		m_pNaviMesh->Set_Jump(false);
	}
}

void CPlayer::Create_Trail()
{
	//// 트레일(검광)
	if(m_pmatFrameMat == NULL)
	{
		m_pmatFrameMat = m_pMeshCom->Get_FrameMatrix("BN_Weapon_R");
		memcpy(&m_vSwordStartPos, &m_pmatFrameMat->m[3][0], sizeof(_vec3));
	}
	

	D3DXVec3TransformCoord(&m_vSwordEndPos, m_pSwordEndPos,  m_pmatFrameMat);

	_vec3 vEndNStartDir = (m_vSwordEndPos - m_vSwordStartPos) * 0.25f;
	m_vSwordStartPos += vEndNStartDir;
}
void CPlayer::Set_SwordEndPos(_vec3* pvEndpos)
{
	m_pSwordEndPos = pvEndpos;
}
void CPlayer::Fire_Skill2(void)
{
	if( ((CBoss*)m_pBoss)->Get_State() == CBoss::Boss_SK_Firing_02 )
	{
		m_fSkillStartTime += m_fTimeDelta;
		if(m_fSkillStartTime > 3.5f)
		{
			if(!m_bFushing)
			{

				m_eAniStat = STATE_JUMP;
				m_pMeshCom->Set_AnimationSet(13);
				m_bFushing = true;
				m_bFushingJumping = true;

				//Create_Wang();
			}
		}		
	}	
	else
	{
		m_fSkillStartTime = 0.f;
		m_fAccelSpeed = 10.f;
	}

	if(m_bFushingJumping)
	{
		_vec3 vBToPDir = m_pTransCom->m_vPosition - m_pBossTransCom->m_vPosition ;
		D3DXVec3Normalize(&vBToPDir, &vBToPDir);

		m_fAccelSpeed += m_fTimeDelta * 16.f;
		m_dwNaviIndex = m_pNaviMesh->MovingNaviMesh(&m_pTransCom->m_vPosition, &vBToPDir, m_dwNaviIndex, m_fAccelSpeed, m_fTimeDelta);
	}
}
void CPlayer::Fire_Skill4(void)
{
	if( ((CBoss*)m_pBoss)->Get_State() == CBoss::Boss_SK_Firing_01 )
	{
		m_fSkillStartTime2 += m_fTimeDelta;

		if(m_fSkillStartTime2 > 5.f)
			m_fAccelSpeed2 = 2.f;

		else
		{
			_vec3 vBToPDir = m_pBossTransCom->m_vPosition - m_pTransCom->m_vPosition;

			if(D3DXVec3Length(&vBToPDir) > 3.f)
			{
				D3DXVec3Normalize(&vBToPDir, &vBToPDir);

				m_fAccelSpeed2 += m_fTimeDelta;
				m_dwNaviIndex = m_pNaviMesh->MovingNaviMesh(&m_pTransCom->m_vPosition, &vBToPDir, m_dwNaviIndex, m_fAccelSpeed2, m_fTimeDelta);
			}			
		}
	}		
	else
	{
		m_fSkillStartTime2 = 0.f;
		m_fAccelSpeed2 = 2.f;
	}

}

void CPlayer::StateSet_Slow(void)
{
	if(m_bSlow)
	{
		m_fSlowTime += m_fTimeDelta;
		m_fSpeed = 3.f;
		if(m_fSlowTime > 1.f)
		{
			m_bSlow = false;
			m_fSlowTime = 0.f;
		}
	}
	else
	{
		m_fSpeed = 5.f;
		m_fSlowTime = 0.f;
	}
}

void CPlayer::Create_Impact(_int iIndex)
{
	list<Engine::CGameObject*>* pEffect = m_pGameLogicLayer->Find_ObjectList(L"Impact");
	map<const TCHAR*, list<Engine::CGameObject*>>* m_pmapObjList = m_pGameLogicLayer->Get_pmapObjList();

	Engine::CGameObject* pGameObject = CImpact::Create(m_pGraphicDev, iIndex);

	if(NULL == pGameObject)	return;	
	if(NULL == pEffect)
	{
		list<CGameObject*>		NewObjectList;		
		NewObjectList.push_back(pGameObject);
		m_pmapObjList->insert(map<const TCHAR*, list<CGameObject*>>::value_type(L"Impact", NewObjectList));
	}
	else
	{
		m_pGameLogicLayer->Ready_Object(L"Impact", pGameObject);
	}
}

void CPlayer::Create_Blood(_vec3 vPos)
{
	list<Engine::CGameObject*>* m_pBloodList = m_pGameLogicLayer->Find_ObjectList(L"Blood");
	map<const TCHAR*, list<Engine::CGameObject*>>* m_pmapObjList = m_pGameLogicLayer->Get_pmapObjList();

	//Engine::CGameObject* pGameObject = CBloodBlack::Create(m_pGraphicDev, vPos);
	Engine::CGameObject* pGameObject = CBlood::CreateScale(m_pGraphicDev, m_pTransCom->m_vPosition + _vec3(0.f, 1.f, 0.f), 1.f);

	if(NULL == pGameObject)	return;	
	if(NULL == m_pBloodList)
	{
		list<CGameObject*>		NewObjectList;		
		//NewObjectList.push_back(pGameObject);
		NewObjectList.push_back(pGameObject);
		m_pmapObjList->insert(map<const TCHAR*, list<CGameObject*>>::value_type(L"Blood", NewObjectList));
	}
	else
	{
		m_pGameLogicLayer->Ready_Object(L"Blood", pGameObject);
		//m_pGameLogicLayer->Ready_Object(L"Blood", pGameObject2);
	}
}

void CPlayer::Create_GroundHit()
{
	((CGroundHit*)m_pGroundHit)->Set_DataInit();
	((CGroundHit*)m_pGroundHit)->Set_Pos(m_pTransCom->m_vPosition + m_pTransCom->m_vDir * 2.f);
	((CGroundHit*)m_pGroundHit)->Set_Switch(true);
}

void CPlayer::Create_Wang()
{
	list<Engine::CGameObject*>* m_pBloodList = m_pGameLogicLayer->Find_ObjectList(L"Wang");
	map<const TCHAR*, list<Engine::CGameObject*>>* m_pmapObjList = m_pGameLogicLayer->Get_pmapObjList();

	Engine::CGameObject* pGameObject = CWang::Create(m_pGraphicDev, ((CBoss*)m_pBoss)->Get_Pos());
	//Engine::CGameObject* pGameObject = CBlood::CreateScale(m_pGraphicDev, m_pTransCom->m_vPosition, 1.f);

	if(NULL == pGameObject)	return;	
	if(NULL == m_pBloodList)
	{
		list<CGameObject*>		NewObjectList;		
		//NewObjectList.push_back(pGameObject);
		NewObjectList.push_back(pGameObject);
		m_pmapObjList->insert(map<const TCHAR*, list<CGameObject*>>::value_type(L"Wang", NewObjectList));
	}
	else
	{
		m_pGameLogicLayer->Ready_Object(L"Wang", pGameObject);
		//m_pGameLogicLayer->Ready_Object(L"Blood", pGameObject2);
	}
}

void CPlayer::Create_Bloody()
{
	list<Engine::CGameObject*>* pEffect = m_pGameLogicLayer->Find_ObjectList(L"Bloody");
	map<const TCHAR*, list<Engine::CGameObject*>>* m_pmapObjList = m_pGameLogicLayer->Get_pmapObjList();

	Engine::CGameObject* pGameObject = CBloody::Create(m_pGraphicDev);

	if(NULL == pGameObject)	return;	
	if(NULL == pEffect)
	{
		list<CGameObject*>		NewObjectList;		
		NewObjectList.push_back(pGameObject);
		m_pmapObjList->insert(map<const TCHAR*, list<CGameObject*>>::value_type(L"Bloody", NewObjectList));
	}
	else
	{
		//if(pEffect->size() > 5.f)	return;
		m_pGameLogicLayer->Ready_Object(L"Bloody", pGameObject);
	}
}
