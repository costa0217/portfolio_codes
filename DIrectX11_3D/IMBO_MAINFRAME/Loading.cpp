#include "stdafx.h"
#include "Loading.h"
#include "SceneMgr.h"

#include "SCHeroSel.h"
#include "SCOriTown.h"
#include "SCTitle.h"
#include "SCLobby.h"
#include "SCAldenard.h"
#include "SCSarasen.h"

#include "LoadingBack.h"
#include "ImageUI.h"

#include "EffectMgr.h"

CLoading::CLoading(SCENE_ID eID, CDirectXFramework* pFrameWork)
	:CScene(eID)
{
	m_pFrameWork = pFrameWork;
}
CLoading::~CLoading()
{
	End();
}

bool CLoading::Begin()
{
	m_bIsLoading = true;
	UPDATER->GetSkyBoxContainer()->SetActive(false);
	UPDATER->GetTerrainContainer()->SetActive(false);

	RESOURCEMGR->ReleaseAllResource();

	RESOURCEMGR->CreateTexture("Logo", _T("../../Assets/Loading/Logo.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Logo2", _T("../../Assets/Loading/Logo2.png"), PS_TEXTURE, BIND_PS);

	string strName = rand() % 2 == 0 ? "Logo" : "Logo2";
	RESOURCEMGR->CreateTexture("Loading", _T("../../Assets/Loading/LoadingText.png"), PS_TEXTURE, BIND_PS);

	CUIObject* pUI = CLoadingBack::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.5f)), XMLoadFloat2(&XMFLOAT2(250.f, 250.f)), strName);
	m_vecUI.push_back(pUI);

	strName = "Loading";
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.8f, WINSIZEY * 0.9f)), XMLoadFloat2(&XMFLOAT2(120.f, 30.f)), strName, 1.f);
	m_vecUI.push_back(pUI);

	m_hLoadThread = CreateThread(NULL, 0, FuncLoadResourceThread, this, 0, NULL);

	return true;
}
bool CLoading::End()
{
	RENDERER->GetUIRenderer()->ClearData();
	size_t iVecSize = m_vecUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		m_vecUI[i]->Release();
		delete m_vecUI[i];
		m_vecUI[i] = nullptr;
	}
	m_vecUI.clear();

	return true;
}

void CLoading::Animate(float fTimeElapsed)
{
	Sleep(10);
	size_t iVecSize = m_vecUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		m_vecUI[i]->Update(fTimeElapsed);
	}

	if (true == m_bComplete)
	{
		CScene*	pScene = nullptr;
		switch (m_eSceneID) {
		case SCN_TITLE:			pScene = new CSCTitle(m_eSceneID, m_pFrameWork);			break;
		case SCN_LOBBY:			pScene = new CSCLobby(m_eSceneID, m_pFrameWork);			break;
		case SCN_HEROSEL:		pScene = new CSCHeroSel(m_eSceneID, m_pFrameWork);			break;
		case SCN_ORITOWN:		pScene = new CSCOriTown(m_eSceneID, m_pFrameWork);			break;
		case SCN_ALDENAD:		pScene = new CSCAldenard(m_eSceneID, m_pFrameWork);			break;
		case SCN_BOSS:			pScene = new CSCSarasen(m_eSceneID, m_pFrameWork);			break;
		}

		m_bIsLoading = false;
		UPDATER->GetSkyBoxContainer()->SetActive(true);
		UPDATER->GetTerrainContainer()->SetActive(true);
		CSceneMgr::GetInstance()->SetPresentScene(m_eSceneID, pScene);

		End();
		delete this;
	}
}

void CLoading::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CLoading::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CLoading::ProcessInput(float fTimeElapsed)
{

}

DWORD WINAPI CLoading::FuncLoadResourceThread(LPVOID arg)
{
	// 계속 추가하는애들 함수로 ㄱㄱ
	RESOURCEMGR->CreateTexture("DEFAULT", _T("../../Assets/default.jpg"), PS_TEXTURE, BIND_PS);

	CLoading*	pMyThread = (CLoading*)arg;
	switch (pMyThread->GetSceneID())
	{
	case SCN_TITLE:
		pMyThread->LoadScene_TITLE();
		break;
	case SCN_LOBBY:
		pMyThread->LoadScene_LOBBY();
		break;
	case SCN_HEROSEL:
		pMyThread->LoadScene_HEROSEL();
		break;
	case SCN_ORITOWN:
		pMyThread->LoadScene_ORITOWN();
		break;
	case SCN_REPAIRTOWN:
		pMyThread->LoadScene_REPAIRTOWN();
		break;
	case SCN_ALDENAD:
		pMyThread->LoadScene_ALDENAD();
		break;
	case SCN_BOSS:
		pMyThread->LoadScene_BOSS();
		break;
	}
	pMyThread->SetLoadComplete(true);

	return 0;
}

void CLoading::LoadScene_TITLE()
{
	RESOURCEMGR->CreateTexture("Title", _T("../../Assets/Scene_Title/Title.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Press", _T("../../Assets/Scene_Title/Press.png"), PS_TEXTURE, BIND_PS);
}

void CLoading::LoadScene_LOBBY()
{
	RESOURCEMGR->CreateTexture("Lobby_back", _T("../../Assets/Scene_Lobby/Lobby_back.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Lobby_window", _T("../../Assets/Scene_Lobby/Lobby_window.png"), PS_TEXTURE, BIND_PS);


	RESOURCEMGR->CreateTexture("Lobby_CreateButt", _T("../../Assets/Scene_Lobby/Lobby_CreateButt.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Lobby_JoinButt", _T("../../Assets/Scene_Lobby/Lobby_JoinButt.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Lobby_LeftButt", _T("../../Assets/Scene_Lobby/Lobby_LeftButt.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Lobby_RightButt", _T("../../Assets/Scene_Lobby/Lobby_RightButt.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Lobby_RoomFrame", _T("../../Assets/Scene_Lobby/Lobby_RoomFrame.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Lobby_Deco1", _T("../../Assets/Scene_Lobby/Lobby_Deco1.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Lobby_Menu", _T("../../Assets/Scene_Lobby/Lobby_Menu.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Lobby_SelFrame", _T("../../Assets/Scene_Lobby/Lobby_SelFrame.png"), PS_TEXTURE, BIND_PS);

	RESOURCEMGR->CreateTexture("Num_0", _T("../../Assets/Scene_Lobby/Num_0.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Num_1", _T("../../Assets/Scene_Lobby/Num_1.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Num_2", _T("../../Assets/Scene_Lobby/Num_2.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Num_3", _T("../../Assets/Scene_Lobby/Num_3.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Num_4", _T("../../Assets/Scene_Lobby/Num_4.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Num_5", _T("../../Assets/Scene_Lobby/Num_5.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Num_6", _T("../../Assets/Scene_Lobby/Num_6.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Num_7", _T("../../Assets/Scene_Lobby/Num_7.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Num_8", _T("../../Assets/Scene_Lobby/Num_8.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Num_9", _T("../../Assets/Scene_Lobby/Num_9.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Num_slash", _T("../../Assets/Scene_Lobby/Num_slash.png"), PS_TEXTURE, BIND_PS);

	RESOURCEMGR->CreateTexture("Lobby_RoomGameFont", _T("../../Assets/Scene_Lobby/Lobby_RoomGameFont.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Lobby_RoomEmptyFont", _T("../../Assets/Scene_Lobby/Lobby_RoomEmptyFont.png"), PS_TEXTURE, BIND_PS);

}
void CLoading::LoadScene_HEROSEL()
{
	RESOURCEMGR->CreateTexture("Back", _T("../../Assets/Scene_HeroSel/background.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("CButton_0", _T("../../Assets/Scene_HeroSel/Char_Button_1.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("CButton_1", _T("../../Assets/Scene_HeroSel/Char_Button_2.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("CButton_2", _T("../../Assets/Scene_HeroSel/Char_Button_3.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("CButton_3", _T("../../Assets/Scene_HeroSel/Char_Button_4.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("CButton_4", _T("../../Assets/Scene_HeroSel/Char_Button_5.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("CButton_5", _T("../../Assets/Scene_HeroSel/Char_Button_6.jpg"), PS_TEXTURE, BIND_PS);

	RESOURCEMGR->CreateTexture("Char_Select_0", _T("../../Assets/Scene_HeroSel/Char_Select_1.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Char_Select_1", _T("../../Assets/Scene_HeroSel/Char_Select_2.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Char_Select_2", _T("../../Assets/Scene_HeroSel/Char_Select_3.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Char_Select_3", _T("../../Assets/Scene_HeroSel/Char_Select_4.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Char_Select_4", _T("../../Assets/Scene_HeroSel/Char_Select_5.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Char_Select_5", _T("../../Assets/Scene_HeroSel/Char_Select_6.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Char_Select_D", _T("../../Assets/Scene_HeroSel/Char_Select_D.jpg"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Char_Select_N", _T("../../Assets/Scene_HeroSel/Char_Select_N.jpg"), PS_TEXTURE, BIND_PS);

	RESOURCEMGR->CreateTexture("Button_Ready", _T("../../Assets/Scene_HeroSel/Button_Ready.png"), PS_TEXTURE, BIND_PS);

	RESOURCEMGR->CreateTexture("Ready_0", _T("../../Assets/Scene_HeroSel/Ready_0.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Ready_1", _T("../../Assets/Scene_HeroSel/Ready_1.png"), PS_TEXTURE, BIND_PS);

	RESOURCEMGR->CreateMultiMesh("../../Assets/Scene_HeroSel/SelectElf01F.gjm", "SelectElf01F");
	RESOURCEMGR->CreateMultiMesh("../../Assets/Scene_HeroSel/SelectHum01M.gjm", "SelectHum01M");
	RESOURCEMGR->CreateMultiMesh("../../Assets/Scene_HeroSel/SelectHum02F.gjm", "SelectHum02F");

	RESOURCEMGR->CreateMultiMesh("../../Assets/Scene_HeroSel/SelectHum03M.gjm", "SelectHum03M");
	RESOURCEMGR->CreateMultiMesh("../../Assets/Scene_HeroSel/SelectHum04F.gjm", "SelectHum04F");
	RESOURCEMGR->CreateMultiMesh("../../Assets/Scene_HeroSel/SelectLup01M.gjm", "SelectLup01M");
}

#define TEST_CHAR 6
void LoadCharUI() {
	
	//hp
	RESOURCEMGR->CreateTexture("Bar", _T("../../Assets/Game_UI/HpBar.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Bar_Fill", _T("../../Assets/Game_UI/HpPoint.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("SkillBack", _T("../../Assets/Game_UI/SkillBack.png"), PS_TEXTURE, BIND_PS);

	RESOURCEMGR->CreateTexture("Bar_tr", _T("../../Assets/Game_UI/Bar_tr.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("Bar_Fill_tr", _T("../../Assets/Game_UI/Bar_Fill_tr.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("SkillBack_tr", _T("../../Assets/Game_UI/SkillBack_tr.png"), PS_TEXTURE, BIND_PS);

	RESOURCEMGR->CreateTexture("Boss_Icon", _T("../../Assets/Game_UI/Boss_Icon.png"), PS_TEXTURE, BIND_PS);

	int nChracter = NETWORKMGR->GetServerPlayerInfo(NETWORKMGR->GetSLOT_ID()).CHARACTER;
	switch (nChracter) {
	case 0:
		//char
		RESOURCEMGR->CreateTexture("Char_Select_0", _T("../../Assets/Scene_HeroSel/Char_Select_1.jpg"), PS_TEXTURE, BIND_PS);
		//skill
		RESOURCEMGR->CreateTexture("ranger_skicon1", _T("../../Assets/Game_UI/ranger_skicon1.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("ranger_skicon2", _T("../../Assets/Game_UI/ranger_skicon2.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("ranger_skicon3", _T("../../Assets/Game_UI/ranger_skicon3.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("ranger_skicon4", _T("../../Assets/Game_UI/ranger_skicon4.tga"), PS_TEXTURE, BIND_PS);
		break;
	case 1:
		//char
		RESOURCEMGR->CreateTexture("Char_Select_1", _T("../../Assets/Scene_HeroSel/Char_Select_2.jpg"), PS_TEXTURE, BIND_PS);
		//skill
		RESOURCEMGR->CreateTexture("knight_skicon1", _T("../../Assets/Game_UI/knight_skicon1.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("knight_skicon2", _T("../../Assets/Game_UI/knight_skicon2.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("knight_skicon3", _T("../../Assets/Game_UI/knight_skicon3.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("knight_skicon4", _T("../../Assets/Game_UI/knight_skicon4.tga"), PS_TEXTURE, BIND_PS);
	case 2:
		//char
		RESOURCEMGR->CreateTexture("Char_Select_2", _T("../../Assets/Scene_HeroSel/Char_Select_3.jpg"), PS_TEXTURE, BIND_PS);
		//skill
		RESOURCEMGR->CreateTexture("dementor_skicon1", _T("../../Assets/Game_UI/dementor_skicon1.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("dementor_skicon2", _T("../../Assets/Game_UI/dementor_skicon2.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("dementor_skicon3", _T("../../Assets/Game_UI/dementor_skicon3.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("dementor_skicon4", _T("../../Assets/Game_UI/dementor_skicon4.tga"), PS_TEXTURE, BIND_PS);
	case 3:
		//char
		RESOURCEMGR->CreateTexture("Char_Select_3", _T("../../Assets/Scene_HeroSel/Char_Select_4.jpg"), PS_TEXTURE, BIND_PS);
		//skill
		RESOURCEMGR->CreateTexture("sister_skicon1", _T("../../Assets/Game_UI/sister_skicon1.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("sister_skicon2", _T("../../Assets/Game_UI/sister_skicon2.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("sister_skicon3", _T("../../Assets/Game_UI/sister_skicon3.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("sister_skicon4", _T("../../Assets/Game_UI/sister_skicon4.tga"), PS_TEXTURE, BIND_PS);
	case 4:
		//char
		RESOURCEMGR->CreateTexture("Char_Select_4", _T("../../Assets/Scene_HeroSel/Char_Select_5.jpg"), PS_TEXTURE, BIND_PS);
		//skill
		RESOURCEMGR->CreateTexture("wizard_skicon1", _T("../../Assets/Game_UI/wizard_skicon1.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("wizard_skicon2", _T("../../Assets/Game_UI/wizard_skicon2.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("wizard_skicon3", _T("../../Assets/Game_UI/wizard_skicon3.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("wizard_skicon4", _T("../../Assets/Game_UI/wizard_skicon4.tga"), PS_TEXTURE, BIND_PS);
	case 5:
		//char
		RESOURCEMGR->CreateTexture("Char_Select_5", _T("../../Assets/Scene_HeroSel/Char_Select_6.jpg"), PS_TEXTURE, BIND_PS);
		//skill
		RESOURCEMGR->CreateTexture("bard_skicon1", _T("../../Assets/Game_UI/bard_skicon1.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("bard_skicon2", _T("../../Assets/Game_UI/bard_skicon2.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("bard_skicon3", _T("../../Assets/Game_UI/bard_skicon3.tga"), PS_TEXTURE, BIND_PS);
		RESOURCEMGR->CreateTexture("bard_skicon4", _T("../../Assets/Game_UI/bard_skicon4.tga"), PS_TEXTURE, BIND_PS);
	default:
		break;
	}
}
void CLoading::LoadScene_ORITOWN()
{
	INT CHARACTER[4]{ 0,0,0,0 };
	LoadCharUI();
#ifdef NO_SERVER
	//LoadUI_Skill(TEST_CHAR, 0, 0, 0);
	//NETWORKMGR->GetServerPlayerInfos()[0].CHARACTER = TEST_CHAR - 1;
	//LoadUI_Skill(NETWORKMGR->GetServerPlayerInfos()[NETWORKMGR->GetSLOT_ID()].CHARACTER + 1, 0, 0, 0);
	LoadUI_Skill(1, 0, 0, 0);
#else
	for (int i = 0; i < NETWORKMGR->GetServerPlayerInfos().size(); ++i)
		CHARACTER[i] = NETWORKMGR->GetServerPlayerInfos()[i].CHARACTER + 1;

	LoadUI_Skill(CHARACTER[0], CHARACTER[1], CHARACTER[2], CHARACTER[3]);
#endif

	CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/TestBlood.dat", L"TestBlood");
	CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/SparkTest.dat", L"SparkTest");
	CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/blood2.dat", L"blood2");
	CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Heal.dat", L"Heal");

	// Boss
	RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Boss01L.gjm", "Boss01L");
	
	

	//LoadScene("../../Assets/SceneResource/test/test.scn");
	LoadScene("../../Assets/SceneResource/FirstTown/FirstTown.scn");
	//LoadScene("../../Assets/SceneResource/Aldenard/Aldenard.scn");	

	RESOURCEMGR->CreateTexture("UI_Game_Clear", _T("../../Assets/Game_UI/UI_Game_Clear.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("UI_Game_Over", _T("../../Assets/Game_UI/UI_Game_Over.png"), PS_TEXTURE, BIND_PS);
}

void CLoading::LoadScene_REPAIRTOWN()
{
}

void CLoading::LoadScene_ALDENAD()
{
	INT CHARACTER[4]{ 0,0,0,0 };
	LoadCharUI();

#ifdef NO_SERVER
	//LoadUI_Skill(TEST_CHAR, 0, 0, 0);
	//NETWORKMGR->GetServerPlayerInfos()[0].CHARACTER = TEST_CHAR - 1;
	LoadUI_Skill(NETWORKMGR->GetServerPlayerInfos()[NETWORKMGR->GetSLOT_ID()].CHARACTER + 1, 0, 0, 0);
#else
	for (int i = 0; i < NETWORKMGR->GetServerPlayerInfos().size(); ++i)
		CHARACTER[i] = NETWORKMGR->GetServerPlayerInfos()[i].CHARACTER + 1;

	LoadUI_Skill(CHARACTER[0], CHARACTER[1], CHARACTER[2], CHARACTER[3]);
#endif
	CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Heal.dat", L"Heal");
	CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/TestBlood.dat", L"TestBlood");
	CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/SparkTest.dat", L"SparkTest");

	LoadScene("../../Assets/SceneResource/Aldenard/Aldenard.scn");
}

void CLoading::LoadScene_BOSS()
{
	INT CHARACTER[4]{ 0,0,0,0 };
	LoadCharUI();

#ifdef NO_SERVER
	//LoadUI_Skill(TEST_CHAR, 0, 0, 0);
	//NETWORKMGR->GetServerPlayerInfos()[0].CHARACTER = TEST_CHAR - 1;
	LoadUI_Skill(NETWORKMGR->GetServerPlayerInfos()[NETWORKMGR->GetSLOT_ID()].CHARACTER + 1, 0, 0, 0);
#else
	for (int i = 0; i < NETWORKMGR->GetServerPlayerInfos().size(); ++i)
		CHARACTER[i] = NETWORKMGR->GetServerPlayerInfos()[i].CHARACTER + 1;

	LoadUI_Skill(CHARACTER[0], CHARACTER[1], CHARACTER[2], CHARACTER[3]);
#endif

	CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/TestBlood.dat", L"TestBlood");
	CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/SparkTest.dat", L"SparkTest");
	CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/blood2.dat", L"blood2");
	CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Heal.dat", L"Heal");

	RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Roisa.gjm", "Boss02R");
	//RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Roisa.gjm", "Boss02R");
	RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Wizard_Arrow.gjm", "Meteo");
	CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Wizard_shot_trail.dat", L"Wizard_shot_trail", 200);
	RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Boss_Skill/boss2_sk2.gjm", "boss2_sk2");
	RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Boss_Skill/Boss2_sk3.gjm", "Boss2_sk3");
	RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Boss_Skill/boss2_sk3_efc1.gjm", "boss2_sk3_efc1");
	RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Boss_Skill/boss2_sk3_efc2.gjm", "boss2_sk3_efc2");
	RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Boss_Skill/boss2_sk3_efc3.gjm", "boss2_sk3_efc3");
	//RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Boss_Skill/boss2_sk2_expefc.gjm", "boss2_sk2_expefc");
	CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/boss2_sk2_expefc.dat", L"boss2_sk2_expefc", 40);
	CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/boss2_sk2_efc.dat", L"boss2_sk2_efc", 40);
	CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Boss2_sk3_iceSmoke.dat", L"Boss2_sk3_iceSmoke", 40);
	
	LoadScene("../../Assets/SceneResource/Sarasen/Sarasen.scn");

	RESOURCEMGR->CreateTexture("UI_Game_Clear", _T("../../Assets/Game_UI/UI_Game_Clear.png"), PS_TEXTURE, BIND_PS);
	RESOURCEMGR->CreateTexture("UI_Game_Over", _T("../../Assets/Game_UI/UI_Game_Over.png"), PS_TEXTURE, BIND_PS);
}

void CLoading::LoadUI_Skill(int cn1, int cn2, int cn3, int cn4)
{
	int cn[4] = { cn1, cn2, cn3, cn4 };
	int nCheckRepeat[4] = { 0, 0, 0, 0 };

	enum CharNum { ranger = 1, knight, demantor, sister, wizard, bard };
	// 1 - ranger || 2 - knight  || 3 - demantor || 4 - sister || 5 - wizard  || 6 - bard
	for (int i = 0; i < 4; ++i) {
		if (i != 0 && nCheckRepeat[i - 1] == cn[i])	continue;
		nCheckRepeat[i] = cn[i];
		switch (cn[i]) {
		case ranger:

			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Ranger_sk1_efc.dat", L"Ranger_sk1_efc");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Ranger_sk3_wheelwind.dat", L"Ranger_sk3_wheelwind");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Ranger_sk2_con.dat", L"Ranger_sk2_con");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/elf_sk1.dat", L"elf_sk1");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/elf_sk2.dat", L"elf_sk2");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Ranger_sk4_con.dat", L"Ranger_sk4_con");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Ranger_sk4_Shoot_m.dat", L"Ranger_sk4_Shoot");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Arrow_Trace.dat", L"Arrow_Trace", 50);
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Arrow_Skill1Shot.dat", L"Arrow_Skill1Shot");

			RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Elf01F.gjm", "Elf01F");

			//RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Bow1.gjm", "Bow1");
			RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Arrow1.gjm", "Arrow1");
			RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Rskill/sk1.gjm", "RangerSK1");

			RESOURCEMGR->CreateTexture("Trail01", _T("../../Assets/SceneResource/Trail/Trail01.tga"), PS_TEXTURE, BIND_PS);
			break;
		case knight:
			RESOURCEMGR->CreateTexture("Char_Select_1", _T("../../Assets/Scene_HeroSel/Char_Select_2.jpg"), PS_TEXTURE, BIND_PS);

			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Knight_sk1_con.dat", L"Knight_sk1_con");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Knight_sk1_fire.dat", L"Knight_sk1_fire", 50);
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Knight_sk1_fire2.dat", L"Knight_sk1_fire2", 50);
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Knight_sk1_fire3.dat", L"Knight_sk1_fire3", 50);
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Knight_sk2_Shield.dat", L"Knight_sk2_Shield");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Knight_sk3_con.dat", L"Knight_sk3_con");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Knight_sk4_con.dat", L"Knight_sk4_con");

			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/hum2_sk1.dat", L"hum2_sk1");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/hum2_sk2.dat", L"hum2_sk2");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/hum2_sk3.dat", L"hum2_sk3");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/hum2_sk4.dat", L"hum2_sk4");

			RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Hum01M.gjm", "Hum01M");

			RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/OSW.gjm", "OSW");
			RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/SLD.gjm", "SLD");

			RESOURCEMGR->CreateTexture("Trail02", _T("../../Assets/SceneResource/Trail/Trail02.tga"), PS_TEXTURE, BIND_PS);
			RESOURCEMGR->CreateTexture("Trail04", _T("../../Assets/SceneResource/Trail/Trail04.tga"), PS_TEXTURE, BIND_PS);
			break;
		case demantor:
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Dementor_Shot.dat", L"Dementor_Shot");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Dementor_sk1_Shield.dat", L"Dementor_sk1_Shield");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Dementor_sk2_Shoot.dat", L"Dementor_sk2_Shoot");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Dementor_sk3_con.dat", L"Dementor_sk3_con");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Dementor_sk4_shoot.dat", L"Dementor_sk4_shoot");

			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/hum3_sk1.dat", L"hum3_sk1");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/hum3_sk2.dat", L"hum3_sk2");
			//CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/hum2_sk3.dat", L"hum2_sk3");
			//CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/hum2_sk4.dat", L"hum2_sk4");

			RESOURCEMGR->CreateTexture("Trail01", _T("../../Assets/SceneResource/Trail/Trail01.tga"), PS_TEXTURE, BIND_PS);
			RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Dementor_Arrow.gjm", "Dementor_Arrow");

			RESOURCEMGR->CreateTexture("Char_Select_2", _T("../../Assets/Scene_HeroSel/Char_Select_3.jpg"), PS_TEXTURE, BIND_PS);
			RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Hum03M.gjm", "Hum03M");
			break;
		case sister:
			RESOURCEMGR->CreateTexture("Char_Select_3", _T("../../Assets/Scene_HeroSel/Char_Select_4.jpg"), PS_TEXTURE, BIND_PS);
			RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Lup01M.gjm", "Lup01M");

			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/sister_skill1.dat", L"sister_skill1", 10);
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/sister_skill2.dat", L"sister_skill2", 10);
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/sister_skill3.dat", L"sister_skill3", 10);
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/sister_skill4.dat", L"sister_skill4", 10);


			RESOURCEMGR->CreateTexture("Trail03", _T("../../Assets/SceneResource/Trail/Trail03.tga"), PS_TEXTURE, BIND_PS);
			RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/THM.gjm", "THM");
			//RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/OSW.gjm", "OSW");
			break;
		case wizard:
			RESOURCEMGR->CreateTexture("Char_Select_4", _T("../../Assets/Scene_HeroSel/Char_Select_5.jpg"), PS_TEXTURE, BIND_PS);
			RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Hum02F.gjm", "Hum02F");

			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Wizard_Shot.dat", L"Wizard_Shot", 10);
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Wizard_sk1_con.dat", L"Wizard_sk1_con");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Wizard_sk3_con.dat", L"Wizard_sk3_con");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Wizard_sk24_shot.dat", L"Wizard_sk24_shot", 10);
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Wizard_shot_trail.dat", L"Wizard_shot_trail", 100);

			RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Wizard_Arrow.gjm", "Wizard_Arrow");
			break;
		case bard:
			/*
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/blood2.dat", L"hum2_sk1");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/heal.dat", L"hum2_sk2");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/portal.dat", L"hum2_sk3");
			*/
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/blood2.dat", L"bard_skill1");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/heal.dat", L"bard_skill2");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/portal.dat", L"bard_skill3");
			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/bard_skill4.dat", L"bard_skill4");

			RESOURCEMGR->CreateTexture("Char_Select_5", _T("../../Assets/Scene_HeroSel/Char_Select_6.jpg"), PS_TEXTURE, BIND_PS);
			RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Hum04F.gjm", "Hum04F");

			CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/Arrow_Trace.dat", L"Bard_Arrow_Trace", 50);
			RESOURCEMGR->CreateTexture("BardTrail01", _T("../../Assets/SceneResource/Trail/Trail01.tga"), PS_TEXTURE, BIND_PS);
			RESOURCEMGR->CreateMultiMesh("../../Assets/SceneResource/GJM/Bard_arrow.gjm", "Bard_arrow");
			break;
		}
	}

	// 공용

	CEffectMgr::GetInstance()->Load_EffectData(L"../../Assets/EffectData/walk_dust.dat", L"walk_dust", 100);
}

