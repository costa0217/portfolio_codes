#include "stdafx.h"
#include "SCTitle.h"
#include "ImageUI.h"


CSCTitle::CSCTitle(SCENE_ID eID, CDirectXFramework * pFrameWork) : CScene(eID) {
	m_pFrameWork = pFrameWork;
}
CSCTitle::~CSCTitle()
{

}

bool CSCTitle::Begin()
{
	UPDATER->GetSkyBoxContainer()->SetActive(false);
	UPDATER->GetTerrainContainer()->SetActive(false);

	string strName = "Title";
	//CUIObject* pUI;
	m_pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.5f)), XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.5f)), strName, 1.f);
	m_vecUI.push_back(m_pUI);

	strName = "Press";
	CUIObject* pUI;
	pUI = CImageUI::Create(XMLoadFloat2(&XMFLOAT2(WINSIZEX * 0.5f, WINSIZEY * 0.91f)), XMLoadFloat2(&XMFLOAT2(150.f, 27.f)), strName, 1.2f);
	m_vecUI.push_back(pUI);

	return true;
}

bool CSCTitle::End()
{
	
	return CScene::End();;
}

void CSCTitle::Animate(float fTimeElapsed)
{
	size_t iVecSize = m_vecUI.size();
	for (size_t i = 0; i < iVecSize; ++i)
	{
		m_vecUI[i]->Update(fTimeElapsed);
	}

	if (INPUTMGR->KeyBoardDown(VK_SPACE_))
	{
		SCENEMGR->ChangeScene(SCN_LOBBY);
	}
}

void CSCTitle::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CSCTitle::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
}

void CSCTitle::ProcessInput(float fTimeElapsed)
{
}
