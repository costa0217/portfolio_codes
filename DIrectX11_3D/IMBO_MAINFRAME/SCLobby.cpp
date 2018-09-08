#include "stdafx.h"
#include "SCLobby.h"
#include "ImageUI.h"
#include "ClickButton.h"

CSCLobby::CSCLobby(SCENE_ID eID, CDirectXFramework * pFrameWork) : CScene(eID) {
	m_pFrameWork = pFrameWork;
}

CSCLobby::~CSCLobby()
{
}

bool CSCLobby::Begin()
{
	////test
	//{
	//	for (int i = 0; i < 7; ++i)
	//	{
	//		tRoomData* pData = new tRoomData;
	//		pData->iJoinNum = rand() % 4 + 1;
	//		pData->iRoomNum = i + 1;
	//		m_pRoomList.push_back(pData);
	//	}
	//}
	UPDATER->GetSkyBoxContainer()->SetActive(false);
	UPDATER->GetTerrainContainer()->SetActive(false);
	PrepareUIObject();
	return false;
}

bool CSCLobby::End()
{
	CScene::End();

	RENDERER->GetUIRenderer()->ClearData();
	size_t iVecSize = m_vecUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		delete m_vecUI[i];
	}
	iVecSize = m_vecButtonUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		delete m_vecButtonUI[i];
	}

	return false;
}

void CSCLobby::Animate(float fTimeElapsed)
{
	//ÀÓ½Ã
	if (INPUTMGR->KeyBoardDown(VK_T))
	{
		SCENEMGR->ChangeScene(SCN_HEROSEL);
		return;
	}
	NetworkProc();
	if (true == INPUTMGR->MouseLeftOnlyDown())
	{
		CheckCollisionButton();
		if (m_bSceneChange) return;
	}
	/////////////////////////	
	//m_pRoomListNum->SetImageName("Num_" + to_string(m_iRoomListNum + 1));
	//list<tRoomData*>::iterator iter = m_pRoomList.begin();
	//for (int i = 0; i < m_iRoomListNum * 2.f; ++i)
	//{
	//	++iter;
	//}
	//if (iter == m_pRoomList.end())
	//{
	//	m_pRoom1Capa->SetImageName("Num_0");
	//	m_pRoom1Opt->SetImageName("Lobby_RoomEmptyFont");
	//}
	/*else
	{
		int iRoomNum10 = (*iter)->iRoomNum / 10;
		int iRoomNum1 = (*iter)->iRoomNum % 10;
		m_pRoom1Num10->SetImageName("Num_" + to_string(iRoomNum10));
		m_pRoom1Num1->SetImageName("Num_" + to_string(iRoomNum1));
		m_pRoom1Capa->SetImageName("Num_" + to_string((*iter)->iJoinNum));
		int iRoom2Num = iRoomNum10 * 10 + iRoomNum1 + 1;
		m_pRoom2Num10->SetImageName("Num_" + to_string(iRoom2Num / 10));
		m_pRoom2Num1->SetImageName("Num_" + to_string(iRoom2Num % 10));
	}
	int k = m_iRoomListNum * 2.f + 1;
	int p = m_pRoomList.size() - 1;
	bool bwhat = (m_iRoomListNum * 2.f + 1) > (m_pRoomList.size() - 1);
	if (k > p)
	{
		m_pRoom2Capa->SetImageName("Num_0");
		m_pRoom2Opt->SetImageName("Lobby_RoomEmptyFont");
	}
	else
	{
		if (iter != m_pRoomList.end())
		{
			++iter;
			m_pRoom2Capa->SetImageName("Num_" + to_string((*iter)->iJoinNum));
			m_pRoom2Opt->SetImageName("Lobby_RoomGameFont");
		}
	}*/
	/////////////////////////
	size_t iVecSize = m_vecUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		m_vecUI[i]->Update(fTimeElapsed);
	}
	iVecSize = m_vecButtonUI.size();
	for (size_t i = 0; i < iVecSize; ++i) {
		m_vecButtonUI[i]->Update(fTimeElapsed);
	}
}

void CSCLobby::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CSCLobby::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CSCLobby::ProcessInput(float fTimeElapsed)
{
}

void CSCLobby::SetSelSceneInfo(int slot_id, int input_slot_id, bool is_ready, int character)
{
}
void CSCLobby::NetworkProc() {
#ifdef NO_SERVER
	return;
#endif
	BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
	DWORD dwProtocol = 0;
	DWORD dwPacketLength = 0;

	if (NETWORKMGR->GetClientSession()->ReadPacket(dwProtocol, Packet, dwPacketLength)) {
		switch (dwProtocol) {
		case PT_ROOM_LIST_COUNT_SC:
			PROC_PT_ROOM_LIST_COUNT_SC(dwProtocol, Packet, dwPacketLength);
			break;
		case PT_ROOM_LIST_SC:
			PROC_PT_ROOM_LIST_SC(dwProtocol, Packet, dwPacketLength);
			break;
		}
	}
}
VOID CSCLobby::PROC_PT_ROOM_LIST_COUNT_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_ROOM_LIST_COUNT_SC);
	m_iRoomListNum = Data.ROOM_COUNT;
	for (auto d : m_pRoomList)
		delete d;
	
	m_pRoomList.clear();
	return VOID();
}
VOID CSCLobby::PROC_PT_ROOM_LIST_SC(DWORD dwProtocol, BYTE * Packet, DWORD dwPacketLength) {
	READ_PACKET(PT_ROOM_LIST_SC);

	tRoomData* pData = new tRoomData;
	pData->iRoomNum = Data.ROOM_ID;
	pData->iJoinNum = Data.PLAYER_NUM;
	m_pRoomList.push_back(pData);
	return VOID();
}
void CSCLobby::CheckCollisionButton()
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
			switch (((CClickButton*)m_vecButtonUI[i])->GetID())
			{
			case BUTT_ROOM1:
				m_iClickRoomNum = 0;
				m_pSelectUI->SetRender(true);
				m_pSelectUI->SetUIPos(XMFLOAT2(530.f, 320.f));
				break;
			case BUTT_ROOM2:
				m_iClickRoomNum = 1;
				m_pSelectUI->SetRender(true);
				m_pSelectUI->SetUIPos(XMFLOAT2(530.f, 550.f));
				break;
			case BUTT_JOIN:
				if (NETWORKMGR->GetSLOT_ID() == 0) {
					BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
					INT id = NETWORKMGR->GetNETWORK_ID();
					INT NETWORK_ID = NETWORKMGR->GetNETWORK_ID();
					INT ROOM_ID = 0;
					NETWORKMGR->SetROOM_ID(ROOM_ID);
					NETWORKMGR->WritePacket(PT_ROOM_JOIN_CS, Packet, WRITE_PT_ROOM_JOIN_CS(Packet, NETWORK_ID, ROOM_ID));
				}
				// ¾ÀÀüÈ¯
				SCENEMGR->ChangeScene(SCN_HEROSEL);
				m_bSceneChange = true;
				return;
				break;
			case BUTT_CREATE:
				m_iClickRoomNum = -1;
				m_pSelectUI->SetRender(false);

#ifdef NO_SERVER

#else
				if (0 == NETWORKMGR->GetSLOT_ID()) {
					BYTE Packet[MAX_BUFFER_LENGTH] = { 0, };
					INT id = NETWORKMGR->GetNETWORK_ID();
					NETWORKMGR->WritePacket(PT_ROOM_CREATE_CS, Packet, WRITE_PT_ROOM_CREATE_CS(Packet, id));
				}
#endif
				
				// ¾ÀÀüÈ¯
				SCENEMGR->ChangeScene(SCN_HEROSEL);
				m_bSceneChange = true;
				return;
				break;
			case BUTT_LEFT:
				m_iClickRoomNum = -1;
				if (m_iRoomListNum > 0) {
					--m_iRoomListNum;
				}
				m_pSelectUI->SetRender(false);
				break;
			case BUTT_RIGHT:
				m_iClickRoomNum = -1;
				int iRoomListSize = (m_pRoomList.size() + 1) * 0.5f;
				if (m_iRoomListNum < iRoomListSize - 1)
				{
					++m_iRoomListNum;
				}
				m_pSelectUI->SetRender(false);
				break;
			}
		}
	}
}

void CSCLobby::PrepareUIObject()
{
	///////////////////////////////////////////////
	string strName = "Lobby_back";
	CUIObject* pUI;
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.5f)), XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.5f)), strName, 1.f);
	m_vecUI.push_back(pUI);

	///////////////////////////////////////////////
	strName = "Lobby_window";
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.5f)), XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.44f, WINSIZEY * 0.44f)), strName, 1.2f);
	m_vecUI.push_back(pUI);

	///////////////////////////////////////////////
	strName = "Lobby_Deco1";
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(1020.f, WINSIZEY * 0.54f)), XMLoadFloat2(&XMFLOAT2(100.f, 250.f)), strName, 1.3f);
	m_vecUI.push_back(pUI);

	///////////////////////////////////////////////
	strName = "Lobby_RoomFrame";
	pUI = CClickButton::Create(XMLoadFloat2(&XMFLOAT2(530.f, 320.f)), XMLoadFloat2(&XMFLOAT2(380.f, 100.f)), strName);
	((CClickButton*)pUI)->SetID(BUTT_ROOM1);
	m_vecButtonUI.push_back(pUI);

	pUI = CClickButton::Create(XMLoadFloat2(&XMFLOAT2(530.f, 550.f)), XMLoadFloat2(&XMFLOAT2(380.f, 100.f)), strName);
	((CClickButton*)pUI)->SetID(BUTT_ROOM2);
	m_vecButtonUI.push_back(pUI);

	///////////////////////////////////////////////
	strName = "Lobby_JoinButt";
	pUI = CClickButton::Create(XMLoadFloat2(&XMFLOAT2(260.f, 720.f)), XMLoadFloat2(&XMFLOAT2(100.f, 45.f)), strName);
	((CClickButton*)pUI)->SetID(BUTT_JOIN);
	m_vecButtonUI.push_back(pUI);

	strName = "Lobby_CreateButt";
	pUI = CClickButton::Create(XMLoadFloat2(&XMFLOAT2(800.f, 720.f)), XMLoadFloat2(&XMFLOAT2(100.f, 45.f)), strName);
	((CClickButton*)pUI)->SetID(BUTT_CREATE);
	m_vecButtonUI.push_back(pUI);

	strName = "Lobby_LeftButt";
	pUI = CClickButton::Create(XMLoadFloat2(&XMFLOAT2(440.f, 720.f)), XMLoadFloat2(&XMFLOAT2(30.f, 45.f)), strName);
	((CClickButton*)pUI)->SetID(BUTT_LEFT);
	m_vecButtonUI.push_back(pUI);

	strName = "Lobby_RightButt";
	pUI = CClickButton::Create(XMLoadFloat2(&XMFLOAT2(620.f, 720.f)), XMLoadFloat2(&XMFLOAT2(30.f, 45.f)), strName);
	((CClickButton*)pUI)->SetID(BUTT_RIGHT);
	m_vecButtonUI.push_back(pUI);

	strName = "Lobby_RoomFrame";
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(530.f, 720.f)), XMLoadFloat2(&XMFLOAT2(45.f, 45.f)), strName, 1.4f);
	m_vecUI.push_back(pUI);

	///////////////////////////////////////////////
	strName = "Lobby_Menu";
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(530.f, 180.f)), XMLoadFloat2(&XMFLOAT2(370.f, 30.f)), strName, 1.4f);
	m_vecUI.push_back(pUI);
	///////////////////////////////////////////////
	strName = "Lobby_SelFrame";
	m_pSelectUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(530.f, 320.f)), XMLoadFloat2(&XMFLOAT2(380.f, 100.f)), strName, 4.f);
	m_pSelectUI->SetRender(false);
	m_vecUI.push_back(m_pSelectUI);

	///////////////////////////////////////////////
	strName = "Num_0";
	m_pRoom1Num10 = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(210.f, 320.f)), XMLoadFloat2(&XMFLOAT2(30.f, 30.f)), strName, 4.5f);
	m_vecUI.push_back(m_pRoom1Num10);
	m_pRoom1Num1 = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(240.f, 320.f)), XMLoadFloat2(&XMFLOAT2(30.f, 30.f)), strName, 4.5f);
	m_vecUI.push_back(m_pRoom1Num1);
	m_pRoom2Num10 = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(210.f, 550.f)), XMLoadFloat2(&XMFLOAT2(30.f, 30.f)), strName, 4.5f);
	m_vecUI.push_back(m_pRoom2Num10);
	m_pRoom2Num1 = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(240.f, 550.f)), XMLoadFloat2(&XMFLOAT2(30.f, 30.f)), strName, 4.5f);
	m_vecUI.push_back(m_pRoom2Num1);

	strName = "Num_1";
	m_pRoomListNum = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(530.f, 720.f)), XMLoadFloat2(&XMFLOAT2(30.f, 30.f)), strName, 4.5f);
	m_vecUI.push_back(m_pRoomListNum);

	strName = "Lobby_RoomGameFont";
	m_pRoom1Opt = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(520.f, 320.f)), XMLoadFloat2(&XMFLOAT2(360.f, 90.f)), strName, 4.5f);
	m_vecUI.push_back(m_pRoom1Opt);

	strName = "Lobby_RoomGameFont";
	m_pRoom2Opt = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(520.f, 550.f)), XMLoadFloat2(&XMFLOAT2(360.f, 90.f)), strName, 4.5f);
	m_vecUI.push_back(m_pRoom2Opt);
	////////////////////////////////

	strName = "Num_0";
	m_pRoom1Capa = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(810.f, 320.f)), XMLoadFloat2(&XMFLOAT2(25.f, 30.f)), strName, 4.6f);
	m_vecUI.push_back(m_pRoom1Capa);

	strName = "Num_0";
	m_pRoom2Capa = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(810.f, 550.f)), XMLoadFloat2(&XMFLOAT2(25.f, 30.f)), strName, 4.6f);
	m_vecUI.push_back(m_pRoom2Capa);

	strName = "Num_slash";
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(840.f, 320.f)), XMLoadFloat2(&XMFLOAT2(25.f, 40.f)), strName, 4.5f);
	m_vecUI.push_back(pUI);
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(840.f, 550.f)), XMLoadFloat2(&XMFLOAT2(25.f, 40.f)), strName, 4.5f);
	m_vecUI.push_back(pUI);
	strName = "Num_4";
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(870.f, 320.f)), XMLoadFloat2(&XMFLOAT2(25.f, 30.f)), strName, 4.5f);
	m_vecUI.push_back(pUI);
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(870.f, 550.f)), XMLoadFloat2(&XMFLOAT2(25.f, 30.f)), strName, 4.5f);
	m_vecUI.push_back(pUI);

}

