#include "stdafx.h"
#include "Dementor.h"
#include "DemSkillArrow.h"

bool CDementor::Begin()
{
	CGameObject::Begin();
	return false;
}

void CDementor::Animate(float fTimeElapsed)
{
	CGameObject::MappingRimLight(fTimeElapsed);

	if (true == m_bSprit) {
		if (false == m_bDamaged)
			KeyInput(fTimeElapsed); //KeyInput(fTimeElapsed);
	}
	else	GetServerData(fTimeElapsed);


	//����
	if (true == m_bJump)	Jumping(fTimeElapsed);

	// �ִϸ��̼� ������Ʈ�Լ�
	if (m_pAnimater) m_pAnimater->Update(TIMEMGR->GetTimeElapsed());

	UpdateSkill();
	ActionMoveProc();

	//��� ������Ʈ�� ���鼭 Update����
	for (auto i : m_mapComponents) {
		i.second->Update(fTimeElapsed);
	}

	DEBUGER->RegistCoordinateSys(GetWorldMtx());

	SetWeapon();
	DEBUGER->AddText(30, 100, 600, YT_Color(255.f, 255.f, 255.f), L"animation %d", m_nAnimNum);
}

bool CDementor::End()
{
	if (m_pWeapon) {
		delete m_pWeapon;
		m_pWeapon = nullptr;
	}
	return true;
}

void CDementor::UpdateSkill()
{
	if (DEMENTOR_ANIM_HIT_F == m_nAnimNum) {
		if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
			m_bDamaged = false;
			m_nAnimNum = DEMENTOR_ANIM_IDLE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			m_bSkill = false;
		}
	}
	CCamera* pCam = CCameraMgr::GetInstance()->GetCamera(CAM_FREE);
	// 1����ų
	if (DEMENTOR_ANIM_SKILL1_START == m_nAnimNum) {
		if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
			m_nAnimNum = DEMENTOR_ANIM_SKILL1_CHARGING;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
		}
		return;
	}
	if (DEMENTOR_ANIM_SKILL1_CHARGING == m_nAnimNum) {
		if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
			m_fSkill1EndTime = m_fSkillTime + 10.f;
			m_nAnimNum = DEMENTOR_ANIM_SKILL1_FIRE;
			CSoundManager::Play_3Dsound("dementor_skill1", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
		}
		return;
	}
	// 2����ų
	if (DEMENTOR_ANIM_SKILL2_START == m_nAnimNum) {
		if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
			m_nAnimNum = DEMENTOR_ANIM_SKILL2_CHARGING;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);

			CEffectMgr::GetInstance()->Play_Effect(L"hum3_sk2", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 1.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
		}
		return;
	}
	if (DEMENTOR_ANIM_SKILL2_CHARGING == m_nAnimNum) {
		if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
			m_nAnimNum = DEMENTOR_ANIM_SKILL2_FIRE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			CSoundManager::Play_3Dsound("dementor_skill2", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Dementor_sk2_Shoot", XMVectorSet(m_xmf3ClickPos.x, m_xmf3ClickPos.y , m_xmf3ClickPos.z, 1.f),
				XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY), 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
			NETWORKMGR->WritePacket(PT_RANGE_SKILL_INFO_CS, Packet, WRITE_PT_RANGE_SKILL_INFO_CS(Packet, NETWORKMGR->GetSLOT_ID(), m_xmf3ClickPos.x, m_xmf3ClickPos.y, m_xmf3ClickPos.z));
			XMVECTOR xmvClickPos;
			xmvClickPos = XMLoadFloat3(&m_xmf3ClickPos);
			XMVECTOR xmvPos = GetPosition();
			XMVECTOR xmvDir = xmvClickPos - xmvPos;
			XMFLOAT4 xmf4Length;
			XMStoreFloat4(&xmf4Length, XMVector3Length(xmvDir));
			xmvDir = XMVector3Normalize(xmvDir);

			XMFLOAT3 xmf3Offset;
			XMStoreFloat3(&xmf3Offset, xmvDir*xmf4Length.x);
			ResetCollisionValue(xmf3Offset, 1.f, 1.5f, 5.f);

			m_bSelRangeMode = false;
			pCam->SetFixCamera(true);
		}
		return;
	}
	// 4����ų
	if (DEMENTOR_ANIM_SKILL4_START == m_nAnimNum) {
		if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
			m_nAnimNum = DEMENTOR_ANIM_SKILL4_CHARGING;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
		}
		return;
	}
	if (DEMENTOR_ANIM_SKILL4_CHARGING == m_nAnimNum) {
		if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
			CSoundManager::Play_3Dsound("dementor_skill4", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			m_nAnimNum = DEMENTOR_ANIM_SKILL4_FIRE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
		}
		return;
	}


	// ���� ������ IDLE��
	if (ANIM_JUMP_END == m_nAnimNum
		|| m_bSkill == true) {
		if (true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
			if (DEMENTOR_ANIM_HIT_F == m_nAnimNum) m_bDamaged = false;
			m_nAnimNum = DEMENTOR_ANIM_IDLE;
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

			m_nAnimNum = DEMENTOR_ANIM_SKILL2_START;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);

		}
		if (true == m_bSkill && INPUTMGR->MouseRightDown())
		{
			m_nAnimNum = DEMENTOR_ANIM_IDLE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			m_bSkill = false;
			m_bSelRangeMode = false;
			pCam->SetFixCamera(true);
		}
	}
}

void CDementor::ShootArrow(bool bStrong, float fAngle)
{
	string strName;
	if (false == bStrong)
		strName = "Dementor_Arrow";
	else
		strName = "StrongArrow";

	size_t iArraySize = m_mapSkill[strName].size();
	for (size_t i = 0; i < iArraySize; ++i) {
		if (false == m_mapSkill[strName][i]->GetActive()) {
			m_mapSkill[strName][i]->SetPosition(XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 3.f, m_xmf3Position.z, 1.f));
			m_mapSkill[strName][i]->Rotate(XMMatrixRotationY(m_fAngleY + fAngle));
			m_mapSkill[strName][i]->SetScale(XMVectorSet(2.f, 1.f, 2.f, 1.f));
			m_mapSkill[strName][i]->SetActive(true);

			//((CElfSkillArrow*)m_mapSkill["Arrow1"][i])->GetTrail()->SetPosition(XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 3.f, m_xmf3Position.z, 1.f));
			break;
		}
	}
}

void CDementor::KeyInput(float fDeltaTime)
{
	if (m_pAnimater->GetCurAnimationIndex() == DEMENTOR_ANIM_DIE || m_pAnimater->GetCurAnimationIndex() == DEMENTOR_ANIM_DEADBODY) {
		m_nAnimNum = DEMENTOR_ANIM_DIE;
		if (m_pAnimater->GetCurAnimationInfo()->GetLoopDone()) {
			m_nAnimNum = DEMENTOR_ANIM_DEADBODY;
			m_pAnimater->SetCurAnimationIndex(DEMENTOR_ANIM_DEADBODY);
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

#ifdef NO_SERVER

#else
	
#endif
	if (GetAsyncKeyState(VK_SHIFT)) { m_fSpeed = 50.f; }
	else { m_fSpeed = 10.f; }

	// ��ų �� ����
	if (false == m_bJump && false == m_bSkill)
	{
		if (INPUTMGR->MouseLeftDown()) {					// �⺻���� ----------------------
			m_bSkill = true;
			m_nAnimNum = DEMENTOR_ANIM_ATTACK;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);

			if (m_bSelRangeMode == false) {
				CSoundManager::Play_3Dsound("dementor_attack", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
				CEffectMgr::GetInstance()->Play_Effect(L"Dementor_Shot", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 2.f, m_xmf3Position.z, 1.f),
					XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY), 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

				ShootArrow(false);
			}
		}
		else if (INPUTMGR->KeyDown(VK_1)) {				// ��ų 1 ------------------------
			m_bSkill = true;
			//m_nAnimNum = DEMENTOR_ANIM_SKILL1_FIRE;
			m_nAnimNum = DEMENTOR_ANIM_SKILL1_START;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);

			CEffectMgr::GetInstance()->Play_Effect(L"Dementor_sk1_Shield", this);
		}
		else if (INPUTMGR->KeyDown(VK_2)) {				// ��ų 2 ------------------------
			m_bSkill = true;
			m_nAnimNum = DEMENTOR_ANIM_IDLE;
			//m_nAnimNum = DEMENTOR_ANIM_SKILL2_START;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);

			m_bSelRangeMode = true;			
		}
		else if (INPUTMGR->KeyDown(VK_3)) {				// ��ų 3 ------------------------
			m_bSkill = true;
			m_nAnimNum = DEMENTOR_ANIM_SKILL3_FIRE;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			CSoundManager::Play_3Dsound("dementor_skill3", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			ResetCollisionValue(XMFLOAT3(0,0,0), 0.f, 5.0f, 5.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Dementor_sk3_con", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 1.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY), 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
		}
		else if (INPUTMGR->KeyDown(VK_4)) {				// ��ų 4 ------------------------
			m_bSkill = true;
			CSoundManager::Play_3Dsound("dementor_skill4", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			m_nAnimNum = DEMENTOR_ANIM_SKILL4_START;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);


			XMVECTOR xmEfcPos = XMLoadFloat3(&m_xmf3Position);
			XMFLOAT3 xmf3EfcPos;
//			xmEfcPos += GetLook() * 2.f;
			

			ShootArrow(false);
			ShootArrow(false, XM_PI * 0.5f);
			ShootArrow(false, XM_PI);
			ShootArrow(false, XM_PI * 1.5f);

			XMStoreFloat3(&xmf3EfcPos, xmEfcPos + XMVector4Normalize(GetLook()) * 4.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Dementor_sk4_shoot", XMVectorSet(xmf3EfcPos.x, xmf3EfcPos.y, xmf3EfcPos.z, 1.f),
				XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY), 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

			XMStoreFloat3(&xmf3EfcPos, xmEfcPos + (XMVector4Normalize(GetRight())) * 4.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Dementor_sk4_shoot", XMVectorSet(xmf3EfcPos.x, xmf3EfcPos.y , xmf3EfcPos.z, 1.f),
				XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY) + 90.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

			XMStoreFloat3(&xmf3EfcPos, xmEfcPos + ((XMVector4Normalize(GetRight()) * -1.f)) * 4.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Dementor_sk4_shoot", XMVectorSet(xmf3EfcPos.x, xmf3EfcPos.y , xmf3EfcPos.z, 1.f),
				XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY) - 90.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

			XMStoreFloat3(&xmf3EfcPos, xmEfcPos + ((XMVector4Normalize(GetLook()) * -1.f)) * 4.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Dementor_sk4_shoot", XMVectorSet(xmf3EfcPos.x, xmf3EfcPos.y, xmf3EfcPos.z, 1.f),
				XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY) - 180.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
		}
	}

	// ��ų�� �̵� ����X
	if (true == m_bSkill) {
#ifdef NO_SERVER

#else
		m_fTranslateTime += fDeltaTime;
		if (m_fTranslateTime > FREQUENCY_TRANSFER_TIME) {
			m_fTranslateTime = 0;
			PushServerData(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, m_fAngleY, m_nAnimNum);
		}
#endif
		//60fps�� ������Ʈ, ��Ʈ��ũ ����

		return;
	}

	// ���콺 ��Ŭ��ȸ��
	if (true == INPUTMGR->MouseRightUp() && abs(m_pCamera->m_cxDelta + m_pCamera->m_cyDelta) > 1.f) {
		m_bIdle = false;
	}
	if (false == m_bIdle) {
		m_fAngleY = m_pCamera->m_fCurrentAngle[ANGLE_Y] + XM_PI;
		m_fAngleX = XM_PI * 2.f - m_pCamera->m_fCurrentAngle[ANGLE_X];

		m_xmatrixRotate = XMMatrixRotationX(m_fAngleX) *  XMMatrixRotationY(m_fAngleY);
		SetRotation(XMMatrixRotationY(m_fAngleY));
	}

	// �̵�
	if (INPUTMGR->OnlyKeyBoardDown(VK_W))		dwDirection |= DIR_FORWARD;
	if (INPUTMGR->OnlyKeyBoardDown(VK_S))		dwDirection |= DIR_BACKWARD;
	if (INPUTMGR->OnlyKeyBoardDown(VK_A))		dwDirection |= DIR_LEFT;
	if (INPUTMGR->OnlyKeyBoardDown(VK_D))		dwDirection |= DIR_RIGHT;
	// ����
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
		if (dwDirection & DIR_FORWARD) { m_xmvShift += GetLook(); fSpdX = 1.f; }
		if (dwDirection & DIR_BACKWARD) { m_xmvShift -= GetLook(); fSpdX = 0.8f; }

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
			if (DEMENTOR_ANIM_JUMP_END != m_nAnimNum) {
				m_nAnimNum = DEMENTOR_ANIM_IDLE;
				m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
			}
		}
	}
	////����
	//if (true == m_bJump)	Jumping(fDeltaTime);

#ifdef NO_SERVER
	return;
#endif 
	//60fps�� ������Ʈ, ��Ʈ��ũ ����
	m_fTranslateTime += fDeltaTime;
	if (m_fTranslateTime > FREQUENCY_TRANSFER_TIME) {
		m_fTranslateTime = 0;
		PushServerData(m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, m_fAngleY, m_nAnimNum);
	}
}

void CDementor::PushServerData(float x, float y, float z, float fAngleY, int nAnimNum)
{
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };

	NETWORKMGR->WritePacket(PT_FREQUENCY_MOVE_CS, Packet, WRITE_PT_FREQUENCY_MOVE_CS(Packet, x, y, z, fAngleY, m_pAnimater->GetCurAnimationIndex()));
}

void CDementor::GetServerData(float fTimeElapsed)
{

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
		XMVECTOR xmEfcPos = XMLoadFloat3(&m_xmf3Position);
		XMFLOAT3 xmf3EfcPos;

		switch (m_nAnimNum) {
		case DEMENTOR_ANIM_SKILL2_CHARGING:
			CEffectMgr::GetInstance()->Play_Effect(L"hum3_sk2", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 1.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			break;
		case DEMENTOR_ANIM_SKILL2_FIRE:
			CSoundManager::Play_3Dsound("dementor_skill2", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Dementor_sk2_Shoot", XMVectorSet(NETWORKMGR->GetServerPlayerInfo(GetSlotID()).CLICK_DATA.fX, NETWORKMGR->GetServerPlayerInfo(GetSlotID()).CLICK_DATA.fY, NETWORKMGR->GetServerPlayerInfo(GetSlotID()).CLICK_DATA.fZ, 1.f),
				XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY), 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			break;
		case DEMENTOR_ANIM_ATTACK:
			ShootArrow(false);
			CEffectMgr::GetInstance()->Play_Effect(L"Dementor_Shot", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 2.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY), 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			break;
		case DEMENTOR_ANIM_SKILL1_FIRE:
			CSoundManager::Play_3Dsound("dementor_skill1", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			break;
		case DEMENTOR_ANIM_SKILL1_START:
			CEffectMgr::GetInstance()->Play_Effect(L"Dementor_sk1_Shield", this);
			break;
		case DEMENTOR_ANIM_SKILL3_FIRE:
			CSoundManager::Play_3Dsound("dementor_skill3", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Dementor_sk3_con", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 1.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY), 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			break;
		case DEMENTOR_ANIM_SKILL4_START:
			CSoundManager::Play_3Dsound("dementor_skill4", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			ShootArrow(false);
			ShootArrow(false, XM_PI * 0.5f);
			ShootArrow(false, XM_PI);
			ShootArrow(false, XM_PI * 1.5f);

			XMStoreFloat3(&xmf3EfcPos, xmEfcPos + XMVector4Normalize(GetLook()) * 4.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Dementor_sk4_shoot", XMVectorSet(xmf3EfcPos.x, xmf3EfcPos.y, xmf3EfcPos.z, 1.f),
				XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY), 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

			XMStoreFloat3(&xmf3EfcPos, xmEfcPos + (XMVector4Normalize(GetRight())) * 4.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Dementor_sk4_shoot", XMVectorSet(xmf3EfcPos.x, xmf3EfcPos.y, xmf3EfcPos.z, 1.f),
				XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY) + 90.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

			XMStoreFloat3(&xmf3EfcPos, xmEfcPos + ((XMVector4Normalize(GetRight()) * -1.f)) * 4.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Dementor_sk4_shoot", XMVectorSet(xmf3EfcPos.x, xmf3EfcPos.y, xmf3EfcPos.z, 1.f),
				XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY) - 90.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

			XMStoreFloat3(&xmf3EfcPos, xmEfcPos + ((XMVector4Normalize(GetLook()) * -1.f)) * 4.f);
			CEffectMgr::GetInstance()->Play_Effect(L"Dementor_sk4_shoot", XMVectorSet(xmf3EfcPos.x, xmf3EfcPos.y, xmf3EfcPos.z, 1.f),
				XMVectorSet(0.f, XMConvertToDegrees(m_fAngleY) - 180.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			break;
		case DEMENTOR_ANIM_HIT_F:
			CSoundManager::Play_3Dsound("dementor_hurt", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			CEffectMgr::GetInstance()->Play_Effect(L"TestBlood", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 2.f, m_xmf3Position.z, 1.f),
				XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));
			break;
		case DEMENTOR_ANIM_DIE:
			CSoundManager::Play_3Dsound("dementor_die", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
			break;
		default:
			break;
		}
	}
	// ����
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

void CDementor::SetupAnimation(DWORD dwDirection)
{
	if (false == m_bJump)
	{
		if (dwDirection & DIR_FORWARD)		if (m_nAnimNum != DEMENTOR_ANIM_RUN_F) m_nAnimNum = DEMENTOR_ANIM_RUN_F;
		if (dwDirection & DIR_BACKWARD)		if (m_nAnimNum != DEMENTOR_ANIM_RUN_B) m_nAnimNum = DEMENTOR_ANIM_RUN_B;
		if (dwDirection & DIR_LEFT)			if (m_nAnimNum != DEMENTOR_ANIM_RUN_L) m_nAnimNum = DEMENTOR_ANIM_RUN_L;
		if (dwDirection & DIR_RIGHT)		if (m_nAnimNum != DEMENTOR_ANIM_RUN_R) m_nAnimNum = DEMENTOR_ANIM_RUN_R;

		if (dwDirection & DIR_FORWARD && dwDirection & DIR_LEFT) {
			if (m_nAnimNum != DEMENTOR_ANIM_RUN_FL) m_nAnimNum = DEMENTOR_ANIM_RUN_FL;
		}
		else if (dwDirection & DIR_FORWARD && dwDirection & DIR_RIGHT) {
			if (m_nAnimNum != DEMENTOR_ANIM_RUN_FR) m_nAnimNum = DEMENTOR_ANIM_RUN_FR;
		}
		else if (dwDirection & DIR_BACKWARD && dwDirection & DIR_LEFT) {
			if (m_nAnimNum != DEMENTOR_ANIM_RUN_BL) m_nAnimNum = DEMENTOR_ANIM_RUN_BL;
		}
		else if (dwDirection & DIR_BACKWARD && dwDirection & DIR_RIGHT) {
			if (m_nAnimNum != DEMENTOR_ANIM_RUN_BR) m_nAnimNum = DEMENTOR_ANIM_RUN_BR;
		}

		if (0 != dwDirection)
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
	}
	else {
		if (m_nAnimNum != DEMENTOR_ANIM_JUMP_START
			&& m_nAnimNum != DEMENTOR_ANIM_JUMP_LOOP)
		{
			m_nAnimNum = DEMENTOR_ANIM_JUMP_START;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
		}

		if (m_nAnimNum == DEMENTOR_ANIM_JUMP_START
			&& true == m_pAnimater->GetCurAnimationInfo()->GetLoopDone())
		{
			m_nAnimNum = DEMENTOR_ANIM_JUMP_LOOP;
			m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
		}
	}
}

void CDementor::Jumping(float fDeltaTime)
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

		m_nAnimNum = DEMENTOR_ANIM_JUMP_END;
		m_pAnimater->SetCurAnimationIndex(m_nAnimNum);
	}
}

void CDementor::SetWeapon()
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

CDementor::CDementor(string name, tag t, bool bSprit, CGameObject * pWeapon, INT slot_id)
	: CGameObject(name, t)
	, m_bSprit(bSprit)
	, m_pWeapon(pWeapon)
	, m_SLOT_ID(slot_id)
{
	ResetHPValues(1000, 1000);
	m_fSpeed = 14.f;

	utag ut = UTAG_OTHERPLAYER_ARROW;
	if (bSprit) ut = UTAG_ARROW;

	vector<CGameObject*> vecSkill;
	for (int i = 0; i < 12; ++i)
	{
		CGameObject* pObject = new CDemSkillArrow("Dementor_Arrow", TAG_DYNAMIC_OBJECT);
		pObject->SetActive(false);
		pObject->SetUTag(ut);
		pObject->Begin();
		pObject->SetPosition(XMVectorSet(0, 0, 0, 1));
		UPDATER->GetSpaceContainer()->AddObject(pObject);
		vecSkill.push_back(pObject);
	}
	m_mapSkill["Dementor_Arrow"] = vecSkill;
}

CDementor::~CDementor()
{
}

void CDementor::RegistToContainer()
{
	CGameObject::RegistToContainer();
	if (m_pWeapon) m_pWeapon->RegistToContainer();
}


void CDementor::TransferCollisioinData(int target_slot_id, int skillnum) {
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	NETWORKMGR->WritePacket(PT_SKILL_COLLISION_TO_TARGET_CS, Packet, WRITE_PT_SKILL_COLLISION_TO_TARGET_CS(Packet, NETWORKMGR->GetROOM_ID(), NETWORKMGR->GetSLOT_ID(), target_slot_id, 2, skillnum));

}

void CDementor::PhisicsLogic(map<utag, list<CGameObject*>>& mlpObject, float fDeltaTime)
{
	//��ų ����
	m_fSkillTime += fDeltaTime;

	m_fCollisionTime += fDeltaTime;
	m_fAnimTime += fDeltaTime;
	if (m_fCollisionTime > 2.f) {
		m_fCollisionTime = 0.f;
		m_bCollision = false;//2�ʿ� �ѹ��� �ٽ� �°� �Ѵ�.
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
		//������ ȭ�츸 �������� ����
		if (false == pArrow->GetActive()) continue;
		if (m_bCollision) break;//���� ���� ���¸�
		if (true == IsCollision(pArrow))
		{
#ifdef NO_SERVER
			GetDemaged(100.f);
			SetRimLight();
#else
			BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
			NETWORKMGR->WritePacket(PT_SKILL_COLLISION_TO_TARGET_CS, Packet, WRITE_PT_SKILL_COLLISION_TO_TARGET_CS(Packet, NETWORKMGR->GetROOM_ID(), 5, NETWORKMGR->GetSLOT_ID(), 6, 9));
			SetRimLight();
			pArrow->DisappearSkill();
#endif
			m_bCollision = true;

			break;
		}
	}
	for (auto pBoss : mlpObject[utag::UTAG_BOSS1]) {
		switch (m_nAnimNum) {
		
		case DEMENTOR_ANIM_SKILL3_FIRE:
			if (SkillCollision(pBoss)) {//skill3 boss���� �����
#ifdef NO_SERVER
				pBoss->GetDemaged(m_iCurAttack);
#else
				TransferCollisioinData(5, 3);
				pBoss->SetRimLight();
#endif
				m_bCollision = true;
			}
			break;

		case DEMENTOR_ANIM_SKILL2_FIRE:
			if (SkillCollision(pBoss, false)) {//skill2 ����ü boss���� �����
#ifdef NO_SERVER
				pBoss->GetDemaged(m_iCurAttack);
#else
				TransferCollisioinData(5, 2);
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

bool CDementor::GetDemaged(int iDemage) {
	if (m_pAnimater->GetCurAnimationIndex() == DEMENTOR_ANIM_DIE || m_pAnimater->GetCurAnimationIndex() == DEMENTOR_ANIM_DEADBODY) {
		m_nAnimNum = m_pAnimater->GetCurAnimationIndex();
		m_bDamaged = false;
		return false;//�װ������� �浹ó�� ���� ����
	}

	if (m_fSkill1EndTime > m_fSkillTime) {
		iDemage *= 0.75f;//skill 1�̸� ������ ���� 10�ʰ�
	}
	m_bDamaged = true;
	CEffectMgr::GetInstance()->Play_Effect(L"TestBlood", XMVectorSet(m_xmf3Position.x, m_xmf3Position.y + 2.f, m_xmf3Position.z, 1.f),
		XMVectorSet(0.f, 0.f, 0.f, 0.f), XMVectorSet(1.f, 1.f, 0.f, 1.f));

	CSoundManager::Play_3Dsound("dementor_hurt", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
	m_nAnimNum = DEMENTOR_ANIM_HIT_F;
	m_pAnimater->SetCurAnimationIndex(m_nAnimNum);

#ifdef NO_SERVER
	CGameObject::GetDemaged(iDemage);//�� hp ������!
#else

#endif

	if (m_iCurHP <= 0) {
		CSoundManager::Play_3Dsound("dementor_die", 1, &m_xmf3Position, 5.f, 5.f, 500.f);
		m_nAnimNum = DEMENTOR_ANIM_DIE;
		m_pAnimater->SetCurAnimationIndex(DEMENTOR_ANIM_DIE);
	}

#ifdef NO_SERVER
#else

	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };

	NETWORKMGR->WritePacket(PT_FREQUENCY_MOVE_CS, Packet, WRITE_PT_FREQUENCY_MOVE_CS(Packet, m_xmf3Position.x, m_xmf3Position.y, m_xmf3Position.z, m_fAngleY, m_pAnimater->GetCurAnimationIndex()));
#endif


	return true;
}

void CDementor::GetSkilled(int nSkill)
{
	int slot_id = m_SLOT_ID;
}
