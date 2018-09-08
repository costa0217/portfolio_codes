#include "stdafx.h"
#include "Ranger.h"
#include "EffectMgr.h"
#include "ElfSkillArrow.h"
#include "ElfSkill1Efc.h"

bool CRanger::Begin()
{
	CGameObject::Begin();
	return false;
}

void CRanger::Animate(float fTimeElapsed)
{
	CGameObject::MappingRimLight(fTimeElapsed);

	if (true == m_bSprit) {
		if (false == m_bDamaged)
			KeyInput(fTimeElapsed); //KeyInput(fTimeElapsed);
	}
	else	GetServerData(fTimeElapsed);

	//점프
	if (true == m_bJump)	Jumping(fTimeElapsed);
	
	// 애니메이션 업데이트함수
	if (m_pAnimater) m_pAnimater->Update(TIMEMGR->GetTimeElapsed());


	UpdateSkill();
	ActionMoveProc();

	//모든 컴포넌트를 돌면서 Update실행
	for (auto i : m_mapComponents) {
		i.second->Update(fTimeElapsed);
	}

	DEBUGER->RegistCoordinateSys(GetWorldMtx());

	SetWeapon();
	DEBUGER->AddText(30, 100, 600, YT_Color(255.f, 255.f, 255.f), L"animation %d", m_nAnimNum);
}

bool CRanger::End()
{
	if (m_pWeapon){
		delete m_pWeapon;
		m_pWeapon = nullptr;
	}
	return true;
}


void CRanger::UpdateSkill()
{
	if (ANIM_HIT_F == m_nAnimNum) {
		if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
			m_bDamaged = false;
			m_nAnimNum = ANIM_IDLE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			m_bSkill = false;
		}
	}
	CCamera* pCam = CCameraMgr::GetInstance()->GetCamera(CAM_FREE);
	if (ANIM_SKILL2_START == m_nAnimNum)
	{
		if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone())
		{
			m_nAnimNum = ANIM_SKILL2_CHARGING;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
		}
		return;
	}
	if (ANIM_SKILL2_CHARGING == m_nAnimNum)
	{
		if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone())
		{
			m_nAnimNum = ANIM_SKILL2_FIRE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			ShootArrow(true);
		}
		return;
	}


	// 점프 끝나면 IDLE로
	if (ANIM_JUMP_END == m_nAnimNum
		|| m_bSkill == true) {
		if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
			if (ANIM_HIT_F == m_nAnimNum) m_bDamaged = false;
			m_nAnimNum = ANIM_IDLE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			m_bSkill = false;
		}
	}

	if (true == m_bSelRangeMode)
	{
		pCam->SetFixCamera(false);
		if (true == m_bSkill && INPUTMGR->MouseLeftDown())
		{
			CSceneMgr::GetInstance()->GetPresentScene()->GetPickPositionByCursor(INPUTMGR->GetMousePoint().x, INPUTMGR->GetMousePoint().y, m_xmf3ClickPos);

			m_nAnimNum = ANIM_SKILL4_FIRE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);

			CEffectMgr::GetInstance()->Play_Effect(L"Ranger_sk4_con", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 1.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY), 0.f, 0.f), XMVectorSet(2.f, 2.f, 0.f, 1.f));

			CEffectMgr::GetInstance()->Play_Effect(L"Ranger_sk4_Shoot", XMVectorSet(m_xmf3ClickPos.x, m_xmf3ClickPos.y, m_xmf3ClickPos.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));

			BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
			NETWORKMGR->WritePacket(PT_RANGE_SKILL_INFO_CS, Packet, WRITE_PT_RANGE_SKILL_INFO_CS(Packet, NETWORKMGR->GetSLOT_ID(), m_xmf3ClickPos.x, m_xmf3ClickPos.y, m_xmf3ClickPos.z));
			CSoundManager::Play_3Dsound("ranger_skill4", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			m_bSelRangeMode = false;
			pCam->SetFixCamera(true);

			XMVECTOR xmvClickPos;
			xmvClickPos = XMLoadFloat3(&m_xmf3ClickPos);
			XMVECTOR xmvPos = GetPosition();
			XMVECTOR xmvDir = xmvClickPos - xmvPos;
			XMFLOAT4 xmf4Length;
			XMStoreFloat4(&xmf4Length, XMVector3Length(xmvDir));
			xmvDir = XMVector3Normalize(xmvDir);

			XMFLOAT3 xmf3Offset;
			XMStoreFloat3(&xmf3Offset, xmvDir*xmf4Length.x);
			ResetCollisionValue(xmf3Offset, 0.f, 10.f, 10.f);
		}
		if (true == m_bSkill && INPUTMGR->MouseRightDown())
		{
			m_nAnimNum = ANIM_IDLE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			m_bSkill = false;
			m_bSelRangeMode = false;
			pCam->SetFixCamera(true);
		}
	}
}

void CRanger::KeyInput(float fDeltaTime)
{
	if (m_pAnimater->GetCurAnimationIndex() == ANIM_DIE || m_pAnimater->GetCurAnimationIndex() == ANIM_DEADBODY) {
		m_nAnimNum = ANIM_DIE;
		if (m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
			m_nAnimNum = ANIM_DEADBODY;
			m_pAnimater->SetCurAnimationIndex(ANIM_DEADBODY);
		}
#ifdef NO_SERVER

#else
		m_fTranslateTime += fDeltaTime;
		if (m_fTranslateTime > FREQUENCY_TRANSFER_TIME) {
			m_fTranslateTime = 0;
			PushServerData(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, m_fAngleY, m_pAnimater->GetCurAnimationIndex());
		}
#endif
		m_bCollision = true;
		return;
	}

	if (m_bStay) {
		m_fTranslateTime += fDeltaTime;
		if (m_fTranslateTime > FREQUENCY_TRANSFER_TIME) {
			m_fTranslateTime = 0;
			PushServerData(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, m_fAngleY, m_pAnimater->GetCurAnimationIndex());
		}
		return;
	}

	DWORD dwDirection = 0;
	m_xmvShift = XMVectorSet(0.0f, 0.0f, 0.0f, 0.f);


	if (GetAsyncKeyState(VK_SHIFT))
	{
		m_fSpeed = 50.f;
	}
	else
	{
		m_fSpeed = 10.f;
	}

	if (m_bSkill)	m_pCamera->AttackStartZoomInOut(true);
	else			m_pCamera->AttackStartZoomInOut(false);
	// 스킬 및 공격
	if (false == m_bJump && false == m_bSkill)
	{
		if (INPUTMGR->MouseLeftDown()){					// 기본공격 ----------------------
			
			//if (m_bSelRangeMode == true) return;

			m_bSkill = true;
			m_nAnimNum = ANIM_ATTACK;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);

			if (m_bSelRangeMode == false)
			{
				CEffectMgr::GetInstance()->Play_Effect(L"Arrow_Skill1Shot", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 2.f, m_xmf3Position.z, 1.f),
						XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY), 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

				CSoundManager::Play_3Dsound("ranger_attack", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
				RENDERER->SetRadialBlurTime(true, 0.5f);
				ShootArrow(false);
			}
		}
		if (INPUTMGR->KeyDown(VK_1)){				// 스킬 1 ------------------------
			m_bSkill = true;
			//m_nAnimNum = ANIM_SKILL1_CHARGING;
			m_nAnimNum = ANIM_SKILL1_FIRE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);

			
			string strName = "RangerSK1";
			size_t iArraySize = m_mapSkill[strName].size();
			for (size_t i = 0; i < iArraySize; ++i) {
				if (false == m_mapSkill[strName][i]->GetActive()) 
				{
					m_mapSkill[strName][i]->SetPosition(XMVectorSet(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, 1.f));
					//m_mapSkill[strName][i]->Rotate(XMMatrixRotationY(m_fAngleY));
					m_mapSkill[strName][i]->SetActive(true);
					break;
				}
			}
			CEffectMgr::GetInstance()->Play_Effect(L"Ranger_sk1_efc", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			CSoundManager::Play_3Dsound("ranger_skill1", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			ResetCollisionValue(XMFLOAT3(0,0,0), 0.8f, 1.f, 10.f);
		}
		else if (INPUTMGR->KeyDown(VK_2)){				// 스킬 2 ------------------------
			m_bSkill = true;
			m_nAnimNum = ANIM_SKILL2_START;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);


			/*CEffectMgr::GetInstance()->Play_Effect(L"elf_sk2", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 1.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));*/
			CEffectMgr::GetInstance()->Play_Effect(L"Ranger_sk2_con", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 2.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			//m_pCamera->CameraStartVibration(1.5f, 10.f);
			CSoundManager::Play_3Dsound("ranger_skill2", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
		}
		else if (INPUTMGR->KeyDown(VK_3)){				// 스킬 3 ------------------------
			m_bSkill = true;
			m_nAnimNum = ANIM_SKILL3_FIRE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);

			
			CEffectMgr::GetInstance()->Play_Effect(L"Ranger_sk3_wheelwind", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 1.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			ResetCollisionValue(XMFLOAT3(0, 0, 0), 0.f, 0.5f, 10.f);
			CSoundManager::Play_3Dsound("ranger_skill3", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
		}
		else if (INPUTMGR->KeyDown(VK_4)){				// 스킬 3 ------------------------
			m_bSkill = true;
			m_bSelRangeMode = true;
			m_nAnimNum = /*ANIM_SKILL2_FIRE*/ANIM_IDLE;
			//m_nAnimNum = ANIM_SKILL4_FIRE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
		
		}
	}

	// 스킬시 이동 점프X
	if (true == m_bSkill) {
#ifdef NO_SERVER

#else
		m_fTranslateTime += fDeltaTime;
		if (m_fTranslateTime > FREQUENCY_TRANSFER_TIME) {
			m_fTranslateTime = 0;
			PushServerData(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, m_fAngleY, m_nAnimNum);
		}
#endif
		//60fps로 업데이트, 네트워크 갱신
		
		return;
	}
	// 마우스 우클릭회전
	if (true == INPUTMGR->MouseRightUp() && abs(m_pCamera->m_cxDelta + m_pCamera->m_cyDelta) > 1.f) {
		m_bIdle = false;
	}
	if (false == m_bIdle) {
		m_fAngleY = m_pCamera->m_fCurrentAngle[ANGLE_Y] + XM_PI;
		m_fAngleX = XM_PI * 2.f - m_pCamera->m_fCurrentAngle[ANGLE_X];

		m_xmatrixRotate = XMMatrixRotationX(m_fAngleX) *  XMMatrixRotationY(m_fAngleY);
		SetRotation(XMMatrixRotationY(m_fAngleY));
	}

	// 이동
	if (INPUTMGR->OnlyKeyBoardDown(VK_W))		dwDirection |= DIR_FORWARD;
	if (INPUTMGR->OnlyKeyBoardDown(VK_S))		dwDirection |= DIR_BACKWARD;
	if (INPUTMGR->OnlyKeyBoardDown(VK_A))		dwDirection |= DIR_LEFT;
	if (INPUTMGR->OnlyKeyBoardDown(VK_D))		dwDirection |= DIR_RIGHT;
	// 점프
	if (INPUTMGR->KeyBoardDown(VK_SPACE_))		m_bJump = true;

	if (true == INPUTMGR->MouseRightDown()) {
		m_bIdle = true;
	}

	SetupAnimation(dwDirection);
	//float fSpeed = 20.f;
	if (dwDirection) {
		float fSpdX = 1.f;
		if (dwDirection & DIR_RIGHT)		m_xmvShift += GetRight();
		if (dwDirection & DIR_LEFT)			m_xmvShift -= GetRight();
		if (dwDirection & DIR_FORWARD)		{ m_xmvShift += GetLook(); fSpdX = 1.f; }
		if (dwDirection & DIR_BACKWARD)		{ m_xmvShift -= GetLook(); fSpdX = 0.7f; }

		Move(XMVector3Normalize(m_xmvShift), (m_fSpeed * fSpdX) * fDeltaTime);

		m_bIdle = false;

		//walk effect
		if (!m_bJump) {
			m_fWalkEffectTime += fDeltaTime;
			if (m_fWalkEffectTime > 0.15f) {
				CEffectMgr::GetInstance()->Play_Effect(L"walk_dust", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 0.5f, m_xmf3Position.z, 1.f),
					XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
				m_fWalkEffectTime = 0.f;
			}
		}		
	}
	else {
		if (false == m_bJump) {
			if (ANIM_JUMP_END != m_nAnimNum) {
				m_nAnimNum = ANIM_IDLE;
				m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			}
		}
	}
	

#ifdef NO_SERVER
	return;
#endif 
	DEBUGER->AddGameText(50, 200, 300, YT_Color(0, 0, 255), L"%d" ,m_nAnimNum);
	//60fps로 업데이트, 네트워크 갱신
	m_fTranslateTime += fDeltaTime;
	if (m_fTranslateTime > FREQUENCY_TRANSFER_TIME) {
		m_fTranslateTime = 0;
		PushServerData(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, m_fAngleY, m_nAnimNum );
	}
}

void CRanger::PushServerData(float x, float y, float z, float fAngleY, int nAnimNum)
{ 
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };

	NETWORKMGR->WritePacket(PT_FREQUENCY_MOVE_CS, Packet, WRITE_PT_FREQUENCY_MOVE_CS(Packet, x, y, z, fAngleY, m_pAnimater->GetCurAnimationIndex()));
}

void CRanger::GetServerData(float fTimeElapsed) {
#ifdef NO_SERVER
	return;
#endif
	//////
	PLAYR_FREQUENCY_DATA data = NETWORKMGR->GetPlayerFrequencyData(m_SLOT_ID);
	m_xmf3Position.x = data.fPosX;
	m_xmf3Position.y = data.fPosY;
	m_xmf3Position.z = data.fPosZ;

	m_fAngleY = data.fAngleY;
	//DWORD dwDirection = data.dwDirection;
	if (m_nAnimNum != data.iAnimNum)
		m_nAnimNum = data.iAnimNum;

	//////

	//if (m_bJump == true && data.bJump == false) {
	//	m_nAnimNum = ANIM_JUMP_END;
	//	m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
	//}
	//m_bJump = data.bJump;

	SetPositionServer(XMVectorSet(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, 1.0f));
	SetRotation(XMMatrixRotationY(m_fAngleY));

	if (m_pAnimater->SetCurAnimationIndex(m_nAnimNum)) {
		switch (m_nAnimNum) {
		case ANIM_ATTACK:
			CSoundManager::Play_3Dsound("ranger_attack", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Arrow_Skill1Shot", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 2.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY), 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			ShootArrow(true);
			break;
		case ANIM_SKILL1_FIRE:
			CSoundManager::Play_3Dsound("ranger_skill1", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Ranger_sk1_efc", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			break;
		case ANIM_SKILL2_START:
			CSoundManager::Play_3Dsound("ranger_skill2", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Ranger_sk2_con", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 2.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			break;
		case ANIM_SKILL2_FIRE:
			ShootArrow(true);
			break;
		case ANIM_SKILL3_FIRE:
			CSoundManager::Play_3Dsound("ranger_skill3", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Ranger_sk3_wheelwind", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 1.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			break;
		case ANIM_HIT_F:
			CEffectMgr::GetInstance()->Play_Effect(L"blood2", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 2.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			break;
		case ANIM_SKILL4_FIRE:
			CSoundManager::Play_3Dsound("ranger_skill4", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Ranger_sk4_con", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 1.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY), 0.f, 0.f), XMVectorSet(2.f, 2.f, 0.f, 1.f));
			CEffectMgr::GetInstance()->Play_Effect(L"Ranger_sk4_Shoot", XMVectorSet(NETWORKMGR->GetServerPlayerInfo(GetSlotID()).CLICK_DATA.fX, NETWORKMGR->GetServerPlayerInfo(GetSlotID()).CLICK_DATA.fY, NETWORKMGR->GetServerPlayerInfo(GetSlotID()).CLICK_DATA.fZ, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 1.f, 1.f));
			break;
		case ANIM_DIE:
			CSoundManager::Play_3Dsound("ranger_die", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			break;
		default:
			break;
		}
	}
	// 공격
	//if (m_bSkill == false && m_bJump == false && bAttack == true && m_nAnimNum != ANIM_ATTACK) {
	//	CEffectMgr::GetInstance()->Play_Effect(L"Test2", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 2.f, m_xmf3Position.z, 1.f),
	//		XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY), 0.f, 0.f));
	//	m_nAnimNum = ANIM_ATTACK;
	//	m_pAnimater->SetCurAnimationIndex(m_nAnimNum);

	//	size_t iArraySize = m_mapSkill["Arrow"].size();
	//	for (size_t i = 0; i < iArraySize; ++i) {
	//		if (false == m_mapSkill["Arrow"][i]->GetActive()) {
	//			m_mapSkill["Arrow"][i]->SetActive(true);
	//			m_mapSkill["Arrow"][i]->SetPosition(XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 3.f, m_xmf3Position.z, 1.f));
	//			m_mapSkill["Arrow"][i]->Rotate(m_xmatrixRotate);
	//			break;
	//		}
	//	}
	//}

	//SetupAnimation(dwDirection);
	//if (dwDirection) {
	//	m_bIdle = false;
	//}
	//else {
	//	if (false == m_bJump) {
	//		if (ANIM_JUMP_END != m_nAnimNum) {
	//			m_nAnimNum = ANIM_IDLE;
	//			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
	//		}
	//	}
	//}
}

void CRanger::SetupAnimation(DWORD dwDirection)
{
	if (false == m_bJump)
	{
		if (dwDirection & DIR_FORWARD)		if (m_nAnimNum != ANIM_RUN_F) m_nAnimNum = ANIM_RUN_F;
		if (dwDirection & DIR_BACKWARD)		if (m_nAnimNum != ANIM_RUN_B) m_nAnimNum = ANIM_RUN_B;
		if (dwDirection & DIR_LEFT)			if (m_nAnimNum != ANIM_RUN_L) m_nAnimNum = ANIM_RUN_L;
		if (dwDirection & DIR_RIGHT)		if (m_nAnimNum != ANIM_RUN_R) m_nAnimNum = ANIM_RUN_R;

		if (dwDirection & DIR_FORWARD && dwDirection & DIR_LEFT)
			if (m_nAnimNum != ANIM_RUN_FL) m_nAnimNum = ANIM_RUN_FL;
		else if (dwDirection & DIR_FORWARD && dwDirection & DIR_RIGHT)
			if (m_nAnimNum != ANIM_RUN_FR) m_nAnimNum = ANIM_RUN_FR;
		else if (dwDirection & DIR_BACKWARD && dwDirection & DIR_LEFT)
			if (m_nAnimNum != ANIM_RUN_BL) m_nAnimNum = ANIM_RUN_BL;
		else if (dwDirection & DIR_BACKWARD && dwDirection & DIR_RIGHT)
			if (m_nAnimNum != ANIM_RUN_BR) m_nAnimNum = ANIM_RUN_BR;

		if (0 != dwDirection)
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
	}else{
		if (m_nAnimNum != ANIM_JUMP_START
			&& m_nAnimNum != ANIM_JUMP_LOOP)
		{
			m_nAnimNum = ANIM_JUMP_START;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
		}

		if (m_nAnimNum == ANIM_JUMP_START
			&& true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone())
		{
			m_nAnimNum = ANIM_JUMP_LOOP;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
		}
	}
	
}

void CRanger::Jumping(float fDeltaTime)
{
	m_fJumpTime += fDeltaTime;
	float fJumpValue = 1.3f * m_fJumpTime;
	float fJumpPower = 0.6f;

	m_xmf4x4World._42 += fJumpPower - fJumpValue;
	m_xmf3Position.y += fJumpPower - fJumpValue;

	if (m_xmf4x4World._42 < GetTerrainHeight())
	{
		m_fJumpTime = 0.f;
		m_bJump = false;

		m_xmf4x4World._42 = GetTerrainHeight();
		m_xmf3Position.y = GetTerrainHeight();

		m_nAnimNum = ANIM_JUMP_END;
		m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
	}
}

void CRanger::SetWeapon()
{
	if (m_pWeapon)
	{
		XMMATRIX xmmtxFrame = this->GetAnimater()->GetCurAnimationInfo()->GetCurFrameMtx(this->GetAnimater()->GetRHand());
		XMMATRIX xmmtxFinal = xmmtxFrame * this->GetAnimater()->GetMeshOffsetMtx() * this->GetWorldMtx();

		XMMATRIX xmmtxRotX = XMMatrixRotationX(90.f);
		XMMATRIX xmmtxRotZ = XMMatrixRotationZ(90.f);
		XMMATRIX xmmtxScale = XMMatrixScaling(10.f, 10.f, 10.f);

		m_pWeapon->SetWorldMtx(xmmtxScale* xmmtxRotX * xmmtxRotZ * xmmtxFinal);
	}
}
void CRanger::ShootArrow(bool bStrong)
{
	string strName;
	if(false == bStrong)
		strName = "Arrow1";
	else
		strName = "StrongArrow";

	size_t iArraySize = m_mapSkill[strName].size();
	for (size_t i = 0; i < iArraySize; ++i) {
		if (false == m_mapSkill[strName][i]->GetActive()) {
			m_mapSkill[strName][i]->SetPosition(XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 3.f, m_xmf3Position.z, 1.f));
			m_mapSkill[strName][i]->Rotate(XMMatrixRotationY(m_fAngleY));
			m_mapSkill[strName][i]->SetScale(XMVectorSet(2.f, 2.f, 2.f, 1.f));
			m_mapSkill[strName][i]->SetActive(true);

			//((CElfSkillArrow*)m_mapSkill["Arrow1"][i])->GetTrail()->SetPosition(XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 3.f, m_xmf3Position.z, 1.f));
			break;
		}
	}
}

void CRanger::RegistToContainer() {
	CGameObject::RegistToContainer();
	if (m_pWeapon) m_pWeapon->RegistToContainer();
}

void CRanger::TransferCollisioinData(int target_slot_id, int skillnum) {
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	NETWORKMGR->WritePacket(PT_SKILL_COLLISION_TO_TARGET_CS, Packet, WRITE_PT_SKILL_COLLISION_TO_TARGET_CS(Packet, NETWORKMGR->GetROOM_ID(), NETWORKMGR->GetSLOT_ID(), target_slot_id, 0, skillnum));

}

void CRanger::PhisicsLogic(map<utag, list<CGameObject*>>& mlpObject, float fDeltaTime)
{
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	m_fCollisionTime += fDeltaTime;
	m_fAnimTime += fDeltaTime;
	if (m_fCollisionTime > 2.f) {
		m_fCollisionTime = 0.f;
		m_bCollision = false;//2초에 한번씩 다시 맞게 한다.
	}

	for (auto pObj : mlpObject[UTAG_NPC]) {
		if (true == IsCollision(pObj))
		{
			if (INPUTMGR->KeyDown(VK_F)) {
				m_pAnimater->SetCurAnimationIndex(ANIM_IDLE);
				m_bStay = true;
			}
			break;
		}
	}
	
	for (auto pArrow : mlpObject[utag::UTAG_BOSS2]) {
		//내가쏜 화살만 데미지를 입음
		if (false == pArrow->GetActive()) continue;
		if (m_bCollision) break;//내가 맞은 상태면
		if (true == IsCollision(pArrow))
		{
#ifdef NO_SERVER
			GetDemaged(100.f);
			SetRimLight();
#else
			BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
			NETWORKMGR->WritePacket(PT_SKILL_COLLISION_TO_TARGET_CS, Packet, WRITE_PT_SKILL_COLLISION_TO_TARGET_CS(Packet, NETWORKMGR->GetROOM_ID(),5, NETWORKMGR->GetSLOT_ID(), 6, 9));
			SetRimLight();
			pArrow->DisappearSkill();
#endif
			m_bCollision = true;


			break;
		}
	}
	//skill collision proc
	for (auto pPlayer : mlpObject[utag::UTAG_OTHERPLAYER]) {
		switch (m_nAnimNum) {
		case ANIM_SKILL1_FIRE:
			if (SkillCollision(pPlayer)) {
#ifdef NO_SERVER
				pPlayer->GetHeal(m_iAttack);
#else
				TransferCollisioinData(pPlayer->GetSlotID(), 1);
#endif
				//pPlayer->GetHeal(m_iAttack);
				//m_bCollision = true;
				XMFLOAT3 xmf3pPos;
				XMStoreFloat3(&xmf3pPos, pPlayer->GetPosition());
				CEffectMgr::GetInstance()->Play_Effect(L"Heal", XMVectorSet(xmf3pPos.x, xmf3pPos.y + 2.f, xmf3pPos.z, 1.f),
					XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			}
			break;
		default:
			break;
		}
	}
	for (auto pPlayer : mlpObject[utag::UTAG_PLAYER]) {
		switch (m_nAnimNum) {
		case ANIM_SKILL1_FIRE:
			if (SkillCollision(pPlayer)) {
	#ifdef NO_SERVER
				pPlayer->GetHeal(m_iAttack);
	#else
				TransferCollisioinData(pPlayer->GetSlotID(), 1);
				XMFLOAT3 xmf3pPos;
				XMStoreFloat3(&xmf3pPos, pPlayer->GetPosition());
				CEffectMgr::GetInstance()->Play_Effect(L"Heal", XMVectorSet(xmf3pPos.x, xmf3pPos.y + 2.f, xmf3pPos.z, 1.f),
					XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
	#endif
				m_bCollision = true;
			}
			break;
		default:
			break;
		}
	}
	for (auto pBoss : mlpObject[utag::UTAG_BOSS1]) {
		switch (m_nAnimNum) {
		case ANIM_SKILL3_FIRE:
			if (SkillCollision(pBoss)) {//boss
#ifdef NO_SERVER
				pBoss->GetDemaged(m_iAttack);
#else
				TransferCollisioinData(5, 3);
				pBoss->SetRimLight();
#endif
				
				
				m_bCollision = true;
			}
			break;
		case ANIM_SKILL4_FIRE:
			if (SkillCollision(pBoss, false)) {
#ifdef NO_SERVER
				pBoss->GetDemaged(m_iAttack);
#else
				TransferCollisioinData(5, 4);
				pBoss->SetRimLight();
#endif
				m_bCollision = true;
			}
			break;
		default:
			break;
		}
	}
}

bool CRanger::GetDemaged(int iDemage) {

		if (m_pAnimater->GetCurAnimationIndex() == ANIM_DIE || m_pAnimater->GetCurAnimationIndex() == ANIM_DEADBODY) {
			m_nAnimNum = m_pAnimater->GetCurAnimationIndex();
			m_bDamaged = false;
			return false;//죽고있으면 충돌처리 하지 않음
		}
		m_bDamaged = true;
		CEffectMgr::GetInstance()->Play_Effect(L"blood2", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 2.f, m_xmf3Position.z, 1.f),
			XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

		m_nAnimNum = ANIM_HIT_F;
		CSoundManager::Play_3Dsound("ranger_hurt", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
		m_pAnimater->SetCurAnimationIndex(m_nAnimNum);

#ifdef NO_SERVER
		CGameObject::GetDemaged(iDemage);//내 hp 날리고!
#else

#endif

		if (m_iCurHP <= 0) {
			m_nAnimNum = ANIM_DIE;
			if (m_pAnimater->SetCurAnimationIndex(ANIM_DIE)) {
				CSoundManager::Play_3Dsound("ranger_die", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			}
		}

#ifdef NO_SERVER
#else

		BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };

		NETWORKMGR->WritePacket(PT_FREQUENCY_MOVE_CS, Packet, WRITE_PT_FREQUENCY_MOVE_CS(Packet, m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, m_fAngleY, m_pAnimater->GetCurAnimationIndex()));
#endif


	return true;
}

void CRanger::GetSkilled(int nSkill)
{
	int slot_id = m_SLOT_ID;
}

CRanger::CRanger(string name, tag t, bool bSprit, CGameObject* pWeapon, INT slot_id)
	: CGameObject(name, t)
	, m_bSprit(bSprit)
	, m_pWeapon(pWeapon)
	, m_SLOT_ID(slot_id)
{
	m_fSpeed = 10.f;

	ResetHPValues(1000, 1000);

	utag ut = UTAG_OTHERPLAYER_ARROW;
	if (bSprit) ut = UTAG_ARROW;
	//attack

	vector<CGameObject*> vecSkill;
	for (int i = 0; i < 5; ++i)
	{
		CGameObject* pObject = new CElfSkillArrow("Arrow1", TAG_DYNAMIC_OBJECT);
		pObject->SetActive(false);
		pObject->SetUTag(ut);
		pObject->Begin();
		pObject->SetPosition(XMVectorSet(0, 0, 0, 1));
		pObject->SetScale(XMVectorSet(5, 5, 5, 1));
		UPDATER->GetSpaceContainer()->AddObject(pObject);
		vecSkill.push_back(pObject);
	}
	m_mapSkill["Arrow1"] = vecSkill;

	vector<CGameObject*> vecSkill2;
	for (int i = 0; i < 5; ++i)
	{
		CGameObject* pObject = new CElfSkillArrow("Arrow1", TAG_DYNAMIC_OBJECT , true);
		pObject->SetActive(false);
		pObject->SetUTag(utag::UTAG_ARROW);
		pObject->Begin();
		pObject->SetPosition(XMVectorSet(0, 0, 0, 1));
		pObject->SetScale(XMVectorSet(5, 5, 5, 1));
		UPDATER->GetSpaceContainer()->AddObject(pObject);

		vecSkill2.push_back(pObject);
	}
	m_mapSkill["StrongArrow"] = vecSkill2;


	vector<CGameObject*> vecSkill3;
	CGameObject* pObject = new CElfSkill1Efc("RangerSK1", TAG_STATIC_OBJECT);
	pObject->SetActive(false);
	pObject->SetUTag(utag::UTAG_DEFAULT);
	pObject->Begin();
	pObject->SetPosition(XMVectorSet(0, 0, 0, 1));
	pObject->SetScale(XMVectorSet(1, 1, 1, 1));
	UPDATER->GetSpaceContainer()->AddObject(pObject);
	vecSkill3.push_back(pObject);
	m_mapSkill["RangerSK1"] = vecSkill3;
}

CRanger::~CRanger()
{
}
