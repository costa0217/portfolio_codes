#include "stdafx.h"
#include "SCHeroSel.h"
#include "ImageUI.h"
#include "ClickButton.h"

#include "LoadingBack.h"
#include "SelectObj.h"

#include "CameraMgr.h"



CSCHeroSel::CSCHeroSel(SCENE_ID eID, CDirectXFramework* pFrameWork) : CScene(eID) {
	m_pFrameWork = pFrameWork;
}
CSCHeroSel::~CSCHeroSel()
{
}

bool CSCHeroSel::Begin()
{

	RCSELLER->CreateStempRenderContainer();
	//NETWORKMGR->Connect("192.168.10.101");
	//NETWORKMGR->SendPacket();

	UPDATER->GetSkyBoxContainer()->SetActive(false);
	UPDATER->GetTerrainContainer()->SetActive(false);

	string strName = "Back";
	CUIObject* pUI;
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.5f)), XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.5f)), strName, -1.f);
	m_vecUI.push_back(pUI);

	for (int i = 0; i < 6; ++i)
	{
		strName = "CButton_" + to_string(i);
		pUI = CClickButton::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f - 400.f + i * 160.f, WINSIZEY * 0.75f)),
			XMLoadFloat2(&XMFLOAT2(70.f, 100.f)), strName);
		((CClickButton*)pUI)->SetID(0);
		m_vecButtonUI.push_back(pUI);
	}
	strName = "Button_Ready";
	pUI = CClickButton::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.92f)),
		XMLoadFloat2(&XMFLOAT2(105.f, 20.f)), strName);
	((CClickButton*)pUI)->SetID(1);
	m_vecButtonUI.push_back(pUI);

	for (int i = 0; i < 4; ++i)
	{
		strName = "Ready_0";
		pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.2f, WINSIZEY * 0.5f - 150.f + i * 70.f)),
			XMLoadFloat2(&XMFLOAT2(20.f, 20.f)), strName, 2.f);
		m_vecReadyUI.push_back(pUI);

		strName = "Char_Select_N";
		pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.15f, WINSIZEY * 0.5f - 150.f + i * 70.f)),
			XMLoadFloat2(&XMFLOAT2(27.f, 27.f)), strName, 2.f);
		m_vecSelCharUI.push_back(pUI);
	}

	m_pObject = new CSelectObj("SelectElf01F", TAG_DYNAMIC_OBJECT);
	m_pObject->Begin();
	//pObject->SetTerrainContainer(UPDATER->GetTerrainContainer());
	m_pObject->SetPosition(XMVectorSet(0, 0, 0, 0));
	m_pObject->SetScale(XMVectorSet(1, 1, 1, 1));
	UPDATER->GetSpaceContainer()->AddObject(m_pObject);
	m_pObject->GetAnimater()->SetCurAnimationIndex(0);
	m_pObject->SetActive(false);

	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	INT NETWORK_ID = NETWORKMGR->GetNETWORK_ID();

	return CScene::Begin();
}
bool CSCHeroSel::End()
{
	RENDERER->GetUIRenderer()->ClearData();
	//size_t iVecSize = m_vecUI.size();
	//for (size_t i = 0; i < iVecSize; ++i)
	//{
	//	delete m_vecUI[i];
	//}
	size_t iVecSize = m_vecButtonUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		delete m_vecButtonUI[i];
	}
	iVecSize = m_vecReadyUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		delete m_vecReadyUI[i];
	}
	iVecSize = m_vecSelCharUI.size();
	for (size_t i = 0; i < iVecSize; ++i) {
		delete m_vecSelCharUI[i];
	}

	//if (m_pObject)
	//{
	//	delete m_pObject;
	//	m_pObject = nullptr;
	//}
	return CScene::End();
}

void CSCHeroSel::Animate(float fTimeElapsed)
{
	MouseMoveForCamera();
	UIProc();
	NetworkProc();

	if (INPUTMGR->KeyBoardDown(VK_T))
	{
		SCENEMGR->ChangeScene(SCN_ORITOWN);
	}
	if (m_bSceneChange) {
		SCENEMGR->ChangeScene(SCN_ORITOWN);
		return;
	}

	size_t iVecSize = m_vecUI.size();
	for (size_t i = 0; i < iVecSize; ++i) {
		m_vecUI[i]->Update(fTimeElapsed);
	}
	iVecSize = m_vecButtonUI.size();
	for (size_t i = 0; i < iVecSize; ++i) {
		m_vecButtonUI[i]->Update(fTimeElapsed);
	}
	iVecSize = m_vecReadyUI.size();
	for (size_t i = 0; i < iVecSize; ++i) {
		m_vecReadyUI[i]->Update(fTimeElapsed);
	}
	iVecSize = m_vecSelCharUI.size();
	for (size_t i = 0; i < iVecSize; ++i) {
		m_vecSelCharUI[i]->Update(fTimeElapsed);
	}
	KeyInput();

	//m_vecReadyUI[0]->SetImageName()
	//if (-1 != m_iHeroSelNum)
	//{
	//	m_vecCharUI[m_iHeroSelNum]->Update(fTimeElapsed);
	//}

	if (INPUTMGR->KeyDown(VK_M))
	{
//#ifdef NO_SERVER
//		SCENEMGR->ChangeScene(SCN_ORITOWN);
//		
//		return;
//#endif
//		if (0 == NETWORKMGR->GetSLOT_ID()) {
//			BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
//			INT id = NETWORKMGR->GetNETWORK_ID();
//			NETWORKMGR->WritePacket(PT_ROOM_CREATE_CS, Packet, WRITE_PT_ROOM_CREATE_CS(Packet, id));
//		}
	}
	if (INPUTMGR->KeyDown(VK_L))
	{

	/*	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
		INT id = NETWORKMGR->GetNETWORK_ID();
		INT NETWORK_ID = NETWORKMGR->GetNETWORK_ID();
		INT ROOM_ID = 0;
		NETWORKMGR->SetROOM_ID(ROOM_ID);
		NETWORKMGR->WritePacket(PT_ROOM_JOIN_CS, Packet, WRITE_PT_ROOM_JOIN_CS(Packet, NETWORK_ID, ROOM_ID));*/

	}
	if (INPUTMGR->KeyDown(VK_F5_))
	{
#ifdef NO_SERVER
		m_pHEROSEL_INFO[NETWORKMGR->GetSLOT_ID()].READY = true;
		m_bSceneChange = true;
		return;
#else
		m_pHEROSEL_INFO[NETWORKMGR->GetSLOT_ID()].READY = (1 + m_pHEROSEL_INFO[NETWORKMGR->GetSLOT_ID()].READY) % 2;
		INT ROOM_ID = NETWORKMGR->GetROOM_ID();
		INT SLOT_ID = NETWORKMGR->GetSLOT_ID();
		INT CHARACTER = m_iHeroSelNum;
		BOOL READY = m_pHEROSEL_INFO[NETWORKMGR->GetSLOT_ID()].READY;
		BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
		NETWORKMGR->WritePacket(PT_ROOM_DATA_CHANGE_CS, Packet, WRITE_PT_ROOM_DATA_CHANGE_CS(Packet,
			ROOM_ID, SLOT_ID, READY, CHARACTER));
#endif
		
	}

}

void CSCHeroSel::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CSCHeroSel::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CSCHeroSel::ProcessInput(float fTimeElapsed)
{
	if (INPUTMGR->KeyDown(VK_P)) {
		INPUTMGR->SetDebugMode(static_cast<bool>((INPUTMGR->GetDebugMode() + 1) % 2));
	}
}

void CSCHeroSel::SetSelSceneInfo(int slot_id, int input_slot_id, bool is_ready, int character) {
#ifdef NO_SERVER
	NETWORKMGR->GetServerPlayerInfos()[NETWORKMGR->GetSLOT_ID()].CHARACTER = character;
#endif

	m_pHEROSEL_INFO[slot_id].SLOT_ID = input_slot_id;
	m_pHEROSEL_INFO[slot_id].READY = is_ready;
	m_pHEROSEL_INFO[slot_id].CHARACTER = character;

	if (m_pHEROSEL_INFO[slot_id].READY)
		m_vecReadyUI[slot_id]->SetImageName("Ready_1");
	else
		m_vecReadyUI[slot_id]->SetImageName("Ready_0");


	string strName;
	if (input_slot_id == -1) {
		strName = "Char_Select_N";
		m_vecSelCharUI[slot_id]->SetImageName(strName);
		return;
	}
	if (character == -1) {
		strName = "Char_Select_D";
		m_vecSelCharUI[slot_id]->SetImageName(strName);
		return;
	}

	if (m_pHEROSEL_INFO[slot_id].CHARACTER >= 0) {
		strName = "Char_Select_" + to_string(m_pHEROSEL_INFO[slot_id].CHARACTER);
		m_vecSelCharUI[slot_id]->SetImageName(strName);
	}
	if (NETWORKMGR->GetSLOT_ID() == slot_id) {
		switch (m_pHEROSEL_INFO[slot_id].CHARACTER)
		{
		case 0:
			m_pObject->SetActive(true);
			m_pObject->SetName("SelectElf01F");
			m_pObject->Begin();
			break;
		case 1:
			m_pObject->SetActive(true);
			m_pObject->SetName("SelectHum01M");
			m_pObject->Begin();
			break;
		case 2:
			m_pObject->SetActive(true);
			m_pObject->SetName("SelectHum03M");
			m_pObject->Begin();
			break;
		case 3:
			m_pObject->SetActive(true);
			m_pObject->SetName("SelectLup01M");
			m_pObject->Begin();
			break;
		case 4:
			m_pObject->SetActive(true);
			m_pObject->SetName("SelectHum02F");
			m_pObject->Begin();
			break;
		case 5:
			m_pObject->SetActive(true);
			m_pObject->SetName("SelectHum04F");
			m_pObject->Begin();
			
			break;
		}

	}

}

void CSCHeroSel::UIProc() {
	/*string strName;

	for (int i = 0; i < 6; ++i) {
	strName = "CButton_" + to_string(i);
	m_vecButtonUI[i]->SetImageName(StringToTCHAR(strName));
	}
	for (int i = 0; i < 4; ++i) {
	if (m_pHEROSEL_INFO[i].SLOT_ID == -1) continue;

	if(m_pHEROSEL_INFO[i].READY)
	m_vecReadyUI[i]->SetImageName(L"Ready_1");
	else
	m_vecReadyUI[i]->SetImageName(L"Ready_0");

	for (int j = 0; j < 6; ++j) {
	if(j == m_pHEROSEL_INFO[i].CHARACTER)
	m_vecButtonUI[j]->SetImageName(L"");
	}
	}*/
}

void CSCHeroSel::NetworkProc() {
#ifdef NO_SERVER
	return;
#endif
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	DWORD dwProtocol = 0;
	DWORD dwPacketLength = 0;

	//LATER. 0414
	//이걸 ServerController가 처리하게 바꾸자!
	if (NETWORKMGR->GetClientSession()->ReadPacket(dwProtocol, Packet, dwPacketLength)) {
		switch (dwProtocol) {
		case PT_ENTER_SERVER_SUC:
			PROC_PT_ENTER_SERVER_SUC(dwProtocol, Packet, dwPacketLength);
			break;
		case PT_ROOM_JOIN_SCC_SC:
			PROC_PT_ROOM_JOIN_SCC_SC(dwProtocol, Packet, dwPacketLength);
			break;
		case PT_ROOM_JOIN_SC:
			PROC_PT_ROOM_JOIN_SC(dwProtocol, Packet, dwPacketLength);
			break;
		case PT_ROOM_DATA_CHANGE_SC:
			PROC_PT_ROOM_DATA_CHANGE_SC(dwProtocol, Packet, dwPacketLength);
			break;
		case PT_ROOM_OUT_SC:
			PROC_PT_ROOM_OUT_SC(dwProtocol, Packet, dwPacketLength);
			break;
		case PT_CHANGE_SLOTID_SC:
			PROC_PT_CHANGE_SLOTID_SC(dwProtocol, Packet, dwPacketLength);
			break;
		case PT_FTOWN_START_SC:
			PROC_PT_FTOWN_START_SC(dwProtocol, Packet, dwPacketLength);
			break;
		case PT_ROOM_CREATE_SC:
			PROC_PT_ROOM_CREATE_SC(dwProtocol, Packet, dwPacketLength);
		default:
			break;

		}
	}
}

VOID CSCHeroSel::PROC_PT_ENTER_SERVER_SUC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength) {

	//NetworkID받기
	READ_PACKET(PT_ENTER_SERVER_SUC);

	INT NetworkID = Data.ID;
	//접속이 됨
	//여기서 뭐 로그인 버튼 활성화 같은거 해줌
	BOOL bConnect = true;

	NETWORKMGR->SetConnect(bConnect);
	NETWORKMGR->SetNETWORK_ID(NetworkID);
}

VOID CSCHeroSel::PROC_PT_ROOM_CREATE_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_ROOM_CREATE_SC);
	INT ROOM_ID = Data.ROOM_ID;

	NETWORKMGR->SetROOM_ID(ROOM_ID);
	//room 만들었으면 내 슬롯은 그냥 0이야
	INT SLOT_ID = 0;
	NETWORKMGR->SetSLOT_ID(SLOT_ID);

	SetSelSceneInfo(0, SLOT_ID, false, -1);
	return VOID();
}

VOID CSCHeroSel::PROC_PT_ROOM_JOIN_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_ROOM_JOIN_SC);
	//받기 전에 정보 초기화 지금은 ready만 초기화 나중에 character초기화 요함

	INT SLOT_ID = Data.SLOT_ID;
	BOOL READY = Data.READY;
	INT CHARCTER = Data.CHARACTER;

	SetSelSceneInfo(SLOT_ID, SLOT_ID, READY, CHARCTER);
}

VOID CSCHeroSel::PROC_PT_ROOM_JOIN_SCC_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_ROOM_JOIN_SCC_SC);
	INT SLOT_ID = Data.SLOT_ID;
	INT SLOT_COUNT = Data.SLOT_COUNT;

	SetSelSceneInfo(SLOT_ID, SLOT_ID, false, -1);

	for (int i = SLOT_COUNT; i < 4; ++i) {
		SetSelSceneInfo(i, -1, false, -1);//나머지는 전부 접속 안한애들 slot -1
	}
	NETWORKMGR->SetSLOT_ID(SLOT_ID);
}
VOID CSCHeroSel::PROC_PT_ROOM_DATA_CHANGE_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_ROOM_DATA_CHANGE_SC);

	INT SLOT_ID = Data.SLOT_ID;
	BOOL READY = Data.READY;
	INT CHARACTER = Data.CHARACTER;

	m_pHEROSEL_INFO[SLOT_ID].SLOT_ID = SLOT_ID;
	m_pHEROSEL_INFO[SLOT_ID].READY = READY;
	m_pHEROSEL_INFO[SLOT_ID].CHARACTER = CHARACTER;

	//모든 애들 정보
	SetSelSceneInfo(SLOT_ID, SLOT_ID, READY, CHARACTER);
	return VOID();
}

VOID CSCHeroSel::PROC_PT_ROOM_OUT_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_ROOM_OUT_SC);

	INT nSlot = Data.SLOT_COUNT;
	//slot 정보 초기화
	for (int i = nSlot; i < 4; ++i) {
		SetSelSceneInfo(i, -1, false, -1);//나갔으면 다 slot -1 나간애들
	}
	return VOID();
}

VOID CSCHeroSel::PROC_PT_CHANGE_SLOTID_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_CHANGE_SLOTID_SC);

	INT SLOT_ID = Data.SLOT_ID;
	NETWORKMGR->SetSLOT_ID(SLOT_ID);

	return VOID();
}

VOID CSCHeroSel::PROC_PT_FTOWN_START_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength) {
	for (auto data : m_pHEROSEL_INFO) {//0번 부터 순서대로 캐릭터 인덱스를 넣는다.
		if(data.SLOT_ID != -1 && data.SLOT_ID < 4)
			NETWORKMGR->AddServerPlayerInfo(data.CHARACTER);
	}
	m_bSceneChange = true;
	return VOID();
}

void CSCHeroSel::KeyInput()
{
	if (true == INPUTMGR->MouseLeftOnlyDown())
	{
		CheckCollisionButton();
	}
}

void CSCHeroSel::CheckCollisionButton()
{
	POINT ptPos = INPUTMGR->GetMousePoint();
	size_t iVecSize = m_vecButtonUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		XMFLOAT2 xmButtonPos, xmButtonSize;
		XMStoreFloat2(&xmButtonPos, m_vecButtonUI[i]->GetUIPos());
		XMStoreFloat2(&xmButtonSize, m_vecButtonUI[i]->GetUISize());
		if ((ptPos.x > xmButtonPos.x - xmButtonSize.x&& ptPos.x < xmButtonPos.x + xmButtonSize.x)
			&& (ptPos.y > xmButtonPos.y - xmButtonSize.y&& ptPos.y < xmButtonPos.y + xmButtonSize.y))
		{
			if (((CClickButton*)m_vecButtonUI[i])->GetID() == 0)
			{
				if (m_iHeroSelNum != i) m_pHEROSEL_INFO[NETWORKMGR->GetSLOT_ID()].READY = false;
				m_iHeroSelNum = i;
				m_pHEROSEL_INFO[NETWORKMGR->GetSLOT_ID()].CHARACTER = m_iHeroSelNum;

				/*cs_packet_client_info_in_room* pPacket = new cs_packet_client_info_in_room;
				pPacket->Character = i;
				pPacket->isReady = false;
				pPacket->Size = sizeof(cs_packet_client_info_in_room);
				pPacket->Type = CS_CHARACTER_READY_CHANGE;
				NETWORKMGR->SendPacket(pPacket, pPacket->Size);*/

				//return;
			}
			else if (((CClickButton*)m_vecButtonUI[i])->GetID() == 1)
			{
				if (m_iHeroSelNum == -1)
					return;

				m_pHEROSEL_INFO[NETWORKMGR->GetSLOT_ID()].READY = (m_pHEROSEL_INFO[NETWORKMGR->GetSLOT_ID()].READY + 1) % 2;
				/*cs_packet_client_info_in_room* pPacket = new cs_packet_client_info_in_room;
				pPacket->Character = m_iHeroSelNum;
				pPacket->isReady = m_bReady;
				pPacket->Size = sizeof(cs_packet_client_info_in_room);
				pPacket->Type = CS_CHARACTER_READY_CHANGE;
				NETWORKMGR->SendPacket(pPacket, pPacket->Size);*/

				((CSelectObj*)m_pObject)->SetReady(m_pHEROSEL_INFO[NETWORKMGR->GetSLOT_ID()].READY);
				//return;
			}

			INT ROOM_ID = NETWORKMGR->GetROOM_ID();
			INT SLOT_ID = NETWORKMGR->GetSLOT_ID();
			INT CHARACTER = m_iHeroSelNum;
			BOOL READY = m_pHEROSEL_INFO[NETWORKMGR->GetSLOT_ID()].READY;
			BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };

#ifdef NO_SERVER
			SetSelSceneInfo(SLOT_ID, SLOT_ID, READY, CHARACTER);
			return;
#endif

			NETWORKMGR->WritePacket(PT_ROOM_DATA_CHANGE_CS, Packet, WRITE_PT_ROOM_DATA_CHANGE_CS(Packet,
				ROOM_ID, SLOT_ID, READY, CHARACTER));


			//내정보
			//SetSelSceneInfo(SLOT_ID, SLOT_ID, READY, CHARACTER);
		}
	}
}

void CSCHeroSel::MouseMoveForCamera()
{
	CCamera* pCam = CCameraMgr::GetInstance()->GetCamera(CAM_FREE);

	//pCam->SetPosition(XMVectorSet(0.f, 1.f, 6.f, 1.f));
	pCam->SetLookAt(XMVectorSet(0.f, 2.5f, 6.f, 1.f), XMVectorSet(0.f, 1.7f, 0.f, 1.f), XMVectorSet(0.f, 1.f, 0.f, 1.f));
	//pCam->seta

	/*if (INPUTMGR->MouseLeftDown())
	{
		int k = 0;
	}*/
}
